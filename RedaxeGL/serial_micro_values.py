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
    def __init__(self, baudrate=9600, timeout=5):
        self.port = serial.Serial()
        self.port.baudrate = baudrate

    def open(self):
        ''' Open the serial port.'''
        self.port.open()

    def close(self):
        ''' Close the serial port.'''
        self.port.close()

    def send(self, msg):
        self.port.write(msg)

    def recv(self):
        return self.port.readline()

    def getName(self):
        return str(self.port.name)

    def connectionAvailable(self):
        return self.port.isOpen()

    def SetPort(self, value):
        self.port.port = value


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

    f_port = []

    for port in checkSystemPorts():
        try:
            s = serial.Serial(port)
            s.close()
            f_port.append(port)
        except (OSError, serial.SerialException):
            pass

    try:
        print(f_port[0])
        print("connected to: " + ser.getName())

    except IndexError:
        print("no ports available")
        pass

    ser.SetPort(f_port[0])

    ser.open()



    return "ALL GOOD"

def Release():
    ser = MySerializer()
    ser.close()
    print("Serial closed")

def GetVars():
    ser = MySerializer()

    checking = 1
    line = []

    if ser.connectionAvailable():

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


    #print(line)

    return str(line).strip('[]')

if __name__ == '__main__':
    Serial_connect(9600)
