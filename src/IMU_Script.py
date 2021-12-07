import csv
from os import write
import time
import board
import adafruit_bno055


i2c = board.I2C()
sensor = adafruit_bno055.BNO055_I2C(i2c)

sensor.mode = adafruit_bno055.CONFIG_MODE

sensor.offsets_gyroscope = 0,-2, 0
sensor.offsets_accelerometer = 21, -21, -16
sensor.offsets_magnetometer = 8, 40, -434

sensor.mode = adafruit_bno055.NDOF_MODE
print("Writing Data...")

def writeData():
    filename = "IMU_Data.csv"
    with open(filename, 'w' ) as csvfile:
        csvwriter = csv.writer(csvfile)
        time.sleep(1)
        xx,yy,zz = sensor.euler
        xx = xx+90.0
        if xx > 360.0:
            xx = xx-360.0
        euler = (xx,yy,zz)
        csvwriter.writerow(euler)
        print(euler)

writeData()

