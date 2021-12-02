import csv
from os import write
import time
import board
import adafruit_bno055


i2c = board.I2C()
sensor = adafruit_bno055.BNO055_I2C(i2c)

sensor.mode = adafruit_bno055.CONFIG_MODE

sensor.offsets_gyroscope = -1,-3,-1
sensor.offsets_accelerometer = -23,-25,-16
sensor.offsets_magnetometer = 49,-86,-481

sensor.mode = adafruit_bno055.NDOF_MODE
print("Writing Data...")

def writeData():
    filename = "IMU_Data.csv"
    with open(filename, 'w' ) as csvfile:
        csvwriter = csv.writer(csvfile)
        time.sleep(1)
        xx,yy,zz = sensor.euler
        xx = xx+90
        if xx > 360:
            xx = xx-360
        euler = (xx,yy,zz)
        csvwriter.writerow(euler)
        print(euler)

writeData()

