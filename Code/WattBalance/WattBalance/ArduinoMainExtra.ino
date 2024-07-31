#define SWITCH_PIN /*Switch pin #*/
#define POS_AVG_SIZE //Number of measurements to include in position average for velocity calculation
#define CALIBRATED_LED_PIN /*Calibrated led pin #*/
#define BALANCED_POSITION /*Hall effect sensor position when balanced */
#define BALANCED_TOLERANCE /*Tolerance of hall effect sensor position in order to be considered balanced*/
#define SETTLE_TIME /*Time in ms required to be settled to be considered balanced*/
#define LOOP_DELAY /*Time delay per loop*/
#define BL_MEASUREMENTS_REQUIRED

int positionMeasurements[POS_AVG_SIZE * 2];
int position_index;
int settle_counter;
double bl_sum;
int bl_measurements;
double calibrated_bl;
bool is_calibrated;

void init(){
    position_index = 0;
    bl_measurements = 0;
    bl_sum = 0;
    calibrated = false;
    digitalWrite(CALIBRATED_LED_PIN, LOW);
    settle_counter = SETTLE_TIME / LOOP_DELAY;
}

void loop{
    if (digitalRead(SWITCH_PIN) == LOW){ //Velocity mode
        if (!calibrated){
            positionMeasurements[position_index] = readPosition();
            currentVelocity = calculate_velocity(positionMeasurements, POS_AVG_SIZE * 2, position_index, LOOP_DELAY)
            
            /*CALCULATE BL FROM VELOCITY*/
            bl_sum += /*calculated BL*/;

            position_index = (position_index + 1) / (POS_AVG_SIZE * 2);
            bl_measurements++;
            if (bl_measurements == BL_MEASUREMENTS_REQUIRED){
                calibrated = true;
                digitalWrite(CALIBRATED_LED_PIN, HIGH);
                calibrated_bl = bl_sum / bl_measurements;
            }
        }
    }
    else{ //Force mode
        if (abs(readPosition() - BALANCED_POSITION) <= BALANCED_TOLERANCE){
            if (settle_counter > 0){
                settle_counter--;
            }
            else{
                //Calculate mass
                //Display calculated mass on 7 segment display
            }
        }
        else{
            settle_counter = SETTLE_TIME / LOOP_DELAY;
            if (readPosition() < BALANCED_POSITION){
                //Drive coils forward
            }
            else{
                //Drive coils backward
            }
        }
    }
    delay(LOOP_DELAY);
}


//buffer contains n position measurements
//size is n
//position is the index within the buffer of the most recent position measurement (second most recent should be position - 1)
//time_step is the time between position measurements in ms
double calculateVelocity(int buffer[], int size, int position, int time_step){
    int avg_size = size/2;
    int oldSum = 0;
    int newSum = 0;
    for (int i = 0; i < avg_size; i++){
        newSum += buffer[(((position - i) % avg_size) + avg_size) % avg_size];
        oldSum += buffer[(((position + avg_size - i) % avg_size) + avg_size) % avg_size];
    }
    return (newSum - oldSum) / (1.0 * size * time_step);
}

int readPosition(){
    //Read position sensor
}