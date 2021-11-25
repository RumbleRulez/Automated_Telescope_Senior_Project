import csv
from os import write
import time
import board
import adafruit_bno055


i2c = board.I2C()
sensor = adafruit_bno055.BNO055_I2C(i2c)

sensor.mode = adafruit_bno055.CONFIG_MODE

#sensor.offsets_gyroscope = 
#sensor.offsets_accelerometer = 
#sensor.offsets_magnetometer = 

print("Writing Data...")

def writeData():
    filename = "IMU_Data.csv"
    with open(filename, 'w' ) as csvfile:
        csvwriter = csv.writer(csvfile)
        time.sleep(1)
        csvwriter.writerow(sensor.euler)
        print(sensor.euler)

writeData()
