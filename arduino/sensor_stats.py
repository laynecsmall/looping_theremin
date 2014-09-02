import serial
import math
# requires pyserial


class sensor:
    """
    Sensor, represents a collection of values from a sensor.
    Used to calculate a running average and std deviation for measuring sensors
    """
    count = 0.00001
    total = 0.00001
    total_square = 0.00001

    def __init__(self):
        pass

    def add_value(self, value):
        "Add a value to the totals"
        self.total += value
        self.total_square += value**2
        self.count += 1

    def get_average(self):
        "get the average - sum/count"
        return self.total/self.count

    def get_variance(self):
        "get the variance"
        return (self.total_square-(self.total**2)/self.count)/(self.count-1)

    def get_stddev(self):
        "get the standard deviation"
        return math.sqrt(self.get_variance())

# set up the serial link
ser = serial.Serial()
ser.baudrate = 115200
ser.port = 5
ser.open()

# define sensors we're watching
x_sensor = sensor()
y_sensor = sensor()

# begin the main loop
while True:
    line = ser.readline().decode("utf-8")
    x, y, button = line.strip().split(",")
    x_sensor.add_value(float(x))
    y_sensor.add_value(float(y))
    
    print("%.1f %.1f, %d" % (x_sensor.get_average(),
                             x_sensor.get_stddev(),
                             x_sensor.count))
    print("%.1f %.1f, %d" % (y_sensor.get_average(),
                             y_sensor.get_stddev(),
                             y_sensor.count))
    print("=========================")
