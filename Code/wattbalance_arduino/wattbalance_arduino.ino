

#include <SPI.h>
#include <TM1637.h>
#include <SoftPWM.h>
#include <avr/pgmspace.h>

#define DEBUG

//Position
#define PI 3.14159265358979323846
#define ARM_LENGTH .19017       // 190.17 mm
#define VELOCITY_SAMPLE_DELAY 10       // Velocity sampling delay
#define MIN_VEL          0      // Minimum Velocity to sample
#define MIDDLE_POSITION 4730    // Position when instrument is balanced
#define POSITION_ERROR   0.005    // Error allowed for balanced position

#define BL_ARRAY_SIZE 100       // Size of array to hold measured bl
#define MIN_CALIBRATE_SAMPLE 20 // How many samples until calibrated

#define KP 500 // The feedback coefficient for force mode, the position is multiplied by this coefficient
#define FORCE_START_DELAY 100 // Delay before starting force mode
#define GRAVITY 9.81

#define BOOT_DELAY     1000
#define VELOCITY_DELAY 25
#define FORCE_DELAY    5

// Mode switching
#define NO_MODE       0
#define VELOCITY_MODE 1
#define FORCE_MODE    2

//Pins
#define DISPLAYCLK 2
#define DISPLAYIO 3
#define BOOTLED 4
#define CALIBLED 5
#define LEDGND 6
#define SWITCH 7
#define COILBN 8
#define COILBP 9

//Analog Pins
#define COILAP A0
#define COILAN A1
#define COILAGND A2
#define SWITCHGND A4

//SPI defines
#define CHIPSELECT 10           // SS for 
#define DATAOUT 11              //
#define DATAIN 12               //
#define SPICLK 13               //
#define SPI_DELAY 6             //
#define SCK_SPD 14000000        //
#define coilBswitchInterval 1000 //

TM1637 tm(DISPLAYCLK,DISPLAYIO);

int coilBNstate = LOW;
int coilBPstate = HIGH;


int mode = NO_MODE;

double bl[BL_ARRAY_SIZE];
int bl_index = 0;

uint8_t duty_cycle = 0;

double mass = 0;
double avg_bl = 4.25;

unsigned long prevCoilBSwitch;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);
  Serial.println("Starting Up... ");
  tm.begin();
  SPI.begin();
  SPI.beginTransaction(SPISettings(250000, MSBFIRST, SPI_MODE0));
  SoftPWMBegin();

  pinMode(LEDGND, OUTPUT);
  pinMode(CALIBLED, OUTPUT);
  pinMode(BOOTLED, OUTPUT);
  pinMode(SWITCH, INPUT_PULLUP);
  pinMode(CHIPSELECT, OUTPUT);
  pinMode(DATAIN, INPUT);
  pinMode(SWITCHGND, OUTPUT);
                  
  digitalWrite(LEDGND, LOW);  
  digitalWrite(BOOTLED, HIGH);
  digitalWrite(CALIBLED, LOW);
  digitalWrite(CHIPSELECT, HIGH);
  digitalWrite(SWITCHGND, LOW);

  pinMode(COILBN, OUTPUT);
  pinMode(COILBP, OUTPUT); 
  
  prevCoilBSwitch = millis();
  tm.display("HOLA");
  delay(BOOT_DELAY);
  Serial.println("Finished setup");
}

void loop() {
  //switch coilB current every 1 sec
  velocity_mode();
  
}

void velocity_mode(){
  int t1, t2;
  double p1, p2, v1, v2;

  if(mode != VELOCITY_MODE){ //Logic to set up velocity mode if not already in it
    Serial.println("Switching to velocity mode");
    
    coilBPstate = HIGH; // Set up initial drive state
    coilBNstate = LOW;
    if(mode == FORCE_MODE){  //Make sure to clear Coil A duty cycle if it was in force_mode
      SoftPWMSet(COILAP, 0);
      SoftPWMEnd(COILAP);
    }
    avg_bl = 0; // Reset the BL information
    bl_index = 0;
    digitalWrite(CALIBLED, LOW);
    pinMode(COILAP, INPUT);
    pinMode(COILAN, INPUT);
    pinMode(COILAGND, OUTPUT);
    digitalWrite(COILAGND, LOW);
    mode = VELOCITY_MODE;
  }

  drive_coil_b();
  t1 = millis();
  v1 = get_voltage();
  p1 = get_pos();
  delay(VELOCITY_SAMPLE_DELAY);
  t2 = millis();
  v2 = get_voltage();
  p2 = get_pos();
  double vel = (p2 - p1) * 1000 / (t2 - t1);  //Meters per second
  if(vel <= MIN_VEL) return;
  double vol = abs((v2 + v1) / 2);
  double coil_constant = vol / vel;
  if(coil_constant == 0) return;

  //Time, vel, vol, bl
  Serial.print(millis());
  Serial.print(", ");
  Serial.print(vel, 7);
  Serial.print(", ");
  Serial.print(vol, 7);
  Serial.print(", ");
  Serial.println(coil_constant, 7);

  bl[bl_index] = coil_constant;
  bl_index = (bl_index + 1) % BL_ARRAY_SIZE;


  if(bl_index > MIN_CALIBRATE_SAMPLE){
    digitalWrite(CALIBLED, HIGH);
  }
  else{
    digitalWrite(CALIBLED, LOW);
  }
  tm.display(bl_index);
  delay(VELOCITY_DELAY);
}

void force_mode(){
  if(mode != FORCE_MODE){
    Serial.println("Switching to force mode");
    
    pinMode(COILAP, OUTPUT);
    pinMode(COILAN, OUTPUT);
    pinMode(COILAGND, INPUT);
    digitalWrite(COILAN, LOW);
    SoftPWMSet(COILAP, 0);
    SoftPWMSetFadeTime(COILAP, 10, 10);
    coilBPstate = LOW;
    coilBNstate = LOW;
    digitalWrite(COILBP, coilBPstate);
    digitalWrite(COILBN, coilBNstate);
    mode = FORCE_MODE;
    digitalWrite(CALIBLED, LOW);
    delay(FORCE_START_DELAY); //Delay to let balance go to held position
  }
  double pos = get_pos();

  // Time, position, current, duty_cycle, calculated_mass
  Serial.print(millis());
  Serial.print(", ");
  Serial.print(pos, 7);
  Serial.print(", ");
  Serial.print(get_current(), 7);
  Serial.print(", ");
  Serial.print(duty_cycle);
  Serial.print(", ");
  Serial.print(avg_bl, 7);
  Serial.print(", ");
  Serial.println(mass, 7);

  if(fabs(pos) <= POSITION_ERROR){ // Position is balanced
    update_mass();
    // Serial.print("Measured mass: ");
    // Serial.println(mass);
    tm.display(mass);
    digitalWrite(CALIBLED, HIGH);
  }
  else{
    int16_t new_duty_cycle = (duty_cycle + (-pos * KP));
    if (new_duty_cycle > 255){
      new_duty_cycle = 255;
    }
    else if (new_duty_cycle < 0){
      new_duty_cycle = 0;
    }
    duty_cycle = new_duty_cycle;
    // Serial.print("Duty Cycle: ");
    // Serial.println(duty_cycle);
    SoftPWMSet(COILAP, duty_cycle);
    tm.display(duty_cycle);
    digitalWrite(CALIBLED, LOW);
  }
  //Serial.println("");
  delay(FORCE_DELAY);
}

void update_mass(){
  double bl = get_avg_bl();
  double i = get_current();
  mass = bl * (i/GRAVITY);
}

double get_avg_bl(){
  return 4.25;
  if(bl_index == 0) return 0;
  if(avg_bl != 0) return avg_bl;
  double avg = 0;
  for(int i = 0; i < bl_index; i++){
    avg += bl[i];
  }
  avg_bl = avg / (double)bl_index;
  Serial.print("Using BL: ");
  Serial.println(avg_bl, 5);
  return avg_bl;
}

void drive_coil_b () {
  if (millis() - prevCoilBSwitch < coilBswitchInterval) { // If not time to switch exit
    return;
  }

  if (coilBPstate == HIGH) {
    coilBPstate = LOW;
    coilBNstate = HIGH;
  }else {
    coilBPstate = HIGH;
    coilBNstate = LOW;
  }
  digitalWrite(COILBP, coilBPstate);
  digitalWrite(COILBN, coilBNstate);
  prevCoilBSwitch = millis();
}


double get_pos(){
  int16_t sensor_position = (int16_t)he_read() - MIDDLE_POSITION;
  double rot_pos =  ((double)sensor_position * 2.0 * PI) / 16384.0;  // val * 2pi / 2^14
  return rot_pos * ARM_LENGTH;
}

double get_current(){
  double current = (((duty_cycle / 255.0) * 100.0) * 5.0)/83.1;
  return current;
}

double get_voltage(){
  double adc_val = (double)analogRead(COILAP) - (double)analogRead(COILAN);
  return (adc_val * 5.0)/ 1024.0;
}

uint16_t he_read(){
  uint8_t data[10] = {0};
  uint16_t val = 0;
  uint16_t inv_val = 0;


  while(val != ((~inv_val) & 0x3FFF)){
    digitalWrite(CHIPSELECT, LOW);
    data[0] = SPI.transfer(0xAA);
    for(int i = 1; i < 10; i++){
      delay(SPI_DELAY);
      data[i] = SPI.transfer(0xFF);
    }
    digitalWrite(CHIPSELECT, HIGH);
    
    val =     ( (data[2] << 8) | data[3]) & 0x3FFF;
    inv_val = ( (data[4] << 8) | data[5]) & 0x3FFF;
  }
  return val;
}


