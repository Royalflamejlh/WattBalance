import sys
import serial
from PyQt5.QtWidgets import QApplication, QMainWindow, QPushButton, QVBoxLayout, QWidget, QTextEdit, QLineEdit, QLabel, QGridLayout, QComboBox
import pyqtgraph as pg
from uart import SerialCommunicator

class SerialApp(QMainWindow):
    def __init__(self):
        super().__init__()
        self.communicator = SerialCommunicator()
        self.initUI()

    def initUI(self):
        self.setWindowTitle('Watt Balance')
        self.setGeometry(100, 100, 800, 600)

        # Main widget and layout
        widget = QWidget()
        layout = QVBoxLayout()

        # Text edit for showing received data or status
        self.text_edit = QTextEdit()
        self.text_edit.setReadOnly(True)
        layout.addWidget(self.text_edit)

        # COM Port Dropdown
        self.port_selector = QComboBox(self)
        self.port_selector.addItems(self.communicator.list_ports())
        layout.addWidget(self.port_selector)

        # Connect Button
        self.connect_button = QPushButton('Connect', self)
        self.connect_button.clicked.connect(self.connect_serial)
        layout.addWidget(self.connect_button)

        # Parameters input and send button
        self.param_input = QLineEdit(self)
        self.send_button = QPushButton('Send New Parameters', self)
        self.send_button.clicked.connect(self.send_data)
        param_layout = QVBoxLayout()
        param_layout.addWidget(self.param_input)
        param_layout.addWidget(self.send_button)
        layout.addLayout(param_layout)

        # Sensor display layout
        sensor_layout = QGridLayout()

        # Create labels and line edits for each sensor value
        self.labels = {}
        self.values = {}
        sensors = ['Coil A Voltage', 'Coil A Current', 'Coil B Voltage', 'HE Sensor Position', 'Current Mass']
        for i, sensor in enumerate(sensors):
            self.labels[sensor] = QLabel(f'{sensor}:', self)
            self.values[sensor] = QLineEdit(self)
            self.values[sensor].setReadOnly(True)
            sensor_layout.addWidget(self.labels[sensor], i, 0)
            sensor_layout.addWidget(self.values[sensor], i, 1)

        layout.addLayout(sensor_layout)
        
        self.graphWidget = pg.PlotWidget()
        self.graphWidget.setBackground('w')
        layout.addWidget(self.graphWidget)

        # Data and time for plotting
        self.time = list(range(60))  # 60 seconds of data
        self.data = [0] * 60  # Initial data

        # Plot settings
        self.plot = self.graphWidget.plot(self.time, self.data, pen=pg.mkPen(color=(255, 0, 0), width=2))
        
        widget.setLayout(layout)
        self.setCentralWidget(widget)
        
    def connect_serial(self):
        selected_port = self.port_selector.currentText()
        if self.communicator.connect(selected_port):
            self.text_edit.append(f"Connected to {selected_port}")
        else:
            self.text_edit.append(f"Failed to connect to {selected_port}")

    def send_data(self):
        message = self.param_input.text()
        result = self.communicator.send_data(message)
        self.text_edit.append(result)
    

if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = SerialApp()
    ex.show()
    sys.exit(app.exec_())