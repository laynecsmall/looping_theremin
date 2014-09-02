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
    max = 0
    min = 9999999999999

    def __init__(self):
        pass

    def add_value(self, value):
        "Add a value to the totals"
        self.total += value
        self.total_square += value**2
        self.count += 1

        if value < self.min:
                self.min = value
        if value > self.max:
                self.max = value

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
    try:
        x, y, button = line.strip().split(",")
    except:
        continue
    x_sensor.add_value(float(x))
    y_sensor.add_value(float(y))

    print("Average: %.1f Std-dev: %.1f Max: %d Min: %d Samples:%d" % (
        x_sensor.get_average(),
        x_sensor.get_stddev(),
        x_sensor.max,
        x_sensor.min,
        x_sensor.count))
    print("Average: %.1f Std-dev: %.1f Max: %d Min: %d Samples:%d" % (
        y_sensor.get_average(),
        y_sensor.get_stddev(),
        y_sensor.max,
        y_sensor.min,
        y_sensor.count))
    print("=========================")
