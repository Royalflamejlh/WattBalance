import serial
import serial.tools.list_ports

class SerialCommunicator:
    def __init__(self):
        self.serial_port = None
    
    def list_ports(self):
        return [port.device for port in serial.tools.list_ports.comports()]

    def connect(self, port, baud_rate=9600):
        try:
            self.serial_port = serial.Serial(port, baud_rate, timeout=1)
            return True
        except serial.SerialException as e:
            print(f"Error connecting to serial port: {e}")
            return False

    def disconnect(self):
        if self.serial_port and self.serial_port.isOpen():
            self.serial_port.close()

    def send_data(self, data):
        if self.serial_port and self.serial_port.isOpen():
            try:
                self.serial_port.write(data.encode()) 
                return "Data sent successfully."
            except serial.SerialException as e:
                return f"Failed to send data: {e}"
        else:
            return "Serial port not connected."

    def receive_data(self):
        if self.serial_port and self.serial_port.isOpen():
            try:
                return self.serial_port.readline().decode().strip()
            except serial.SerialException as e:
                return f"Error receiving data: {e}"
        else:
            return "Serial port not connected."
