import sys
import glob
import serial
import time

def singleton(cls, *args, **kw):
    instances = {}
    def _singleton():
       if cls not in instances:
            instances[cls] = cls(*args, **kw)
       return instances[cls]
    return _singleton

@singleton
class MySerializer:
    def __init__(self, _baudrate=9600, _timeout=100):
        self._ser = serial.Serial()
        self._ser.baudrate = _baudrate
        self._ser.timeout = _timeout

    def open(self):
        ''' Open the serial port.'''
        self._ser.open()

    def close(self):
        ''' Close the serial port.'''
        self._ser.close()

    def send(self, msg):
        self._ser.write(msg)

    def recv(self):
        return self._ser.readline()

    def getName(self):
        return str(self._ser.name)

    def connectionAvailable(self):
        return self._ser.isOpen()

    def SetPort(self, value):
        self._ser.port = value


def checkSystemPorts():
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this excludes your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')
    return ports


def Serial_connect(_br):

    ser = MySerializer()

    for port in checkSystemPorts():
        try:
            ser.SetPort(port)
            ser.open()
            break
        except (OSError, serial.SerialException):
            pass
    
           
    if not ser.connectionAvailable():   
        print("No ports available, retrying...")
        return False

def Release():
    ser = MySerializer()
    ser.close()
    print("Serial closed")

def GetVars():
    ser = MySerializer()
    checking = 1
    line = []

    while checking:
        line.append(ser.recv())
        if line[0] == "START \n":
            checking = 0
        line = []


    for value in range(0, 3):
        line.append(ser.recv())

        if line[0] == "SHUTDOWN\n":
            ProgramOn = 0

        if line[0] == "STOP\n":
            ProgramOn = 0


    return str(line).strip('[]')

if __name__ == '__main__':
    Serial_connect(9600)
