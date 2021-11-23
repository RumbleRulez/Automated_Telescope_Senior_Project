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
    #print("is calibrated: ", sensor.calibrated())
    cal = sensor.calibration_status()
    print("Calibration status: {0}".format(cal))
    with open(filename, 'w' ) as csvfile:
        csvwriter = csv.writer(csvfile)
        time.sleep(1)
        csvwriter.writerow(sensor.euler)
       # print(sensor.euler)

writeData()
