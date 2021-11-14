import csv
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

    with open(filename, 'w' ) as csvfile:
        csvwriter = csv.writer(csvfile)
        csvwriter.writerow(row)
        print(sensor.euler)