import serial
import math
import matplotlib.pyplot as plt
import numpy as np
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
    running_ave = [0]*10

    def __init__(self):
        pass

    def add_value(self, value):
        "Add a value to the totals"
        self.running_ave[self.count % 10] = value
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

    def get_running_average(self):
        return sum(self.running_ave)/10

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

sensors = []
is_setup = False


def setup(in_line):
        values = in_line.split(",")
        return [sensor() for x in values[0:-2]]


def plot():
        plot = plt.bar(np.arrange(len(sensors)),
                       [x.get_running_average() for x in sensors],
                       color='r')
        plot.ylabel('Sensor reading')
        plot.title('Sensor intensity')
        plot.yticks(np.arrange(0, max([x.max for x in sensors], 20)))
        plt.show()


# begin the main loop
while True:
    # get the data, enter it into sensor objects
    line = ser.readline().decode("utf-8")
    if "," not in line:
            continue
    elif not is_setup:
        sensors = setup(line)
        plot()

    values = line.split(",")
    for x in range(len(values)-1):
        sensors[x].add_value(values[x])

    plot()



    """
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
    """
