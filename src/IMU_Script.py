import csv
from os import write
import time
import board
import adafruit_bno055


i2c = board.I2C()
sensor = adafruit_bno055.BNO055_I2C(i2c)



print("Writing Data...")

def writeData():
    filename = "IMU_Data.csv"
    row = []

    row.append(sensor.euler)

    row.pop(15)
    row.pop(14)
    row.pop(1)
    row.pop(0)

    print(len(row))
    print(type(row))

    with open(filename, 'w' ) as csvfile:
        csvwriter = csv.writer(csvfile)
        csvwriter.writerow(row)
        print(sensor.euler)

writeData()