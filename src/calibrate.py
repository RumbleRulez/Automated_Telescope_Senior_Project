from os import wait, write
import time
import board
import adafruit_bno055

i2c = board.I2C()
sensor = adafruit_bno055.BNO055_I2C(i2c)
sensor.mode = adafruit_bno055.NDOF_MODE
time.sleep(1)
print("Calibration Status:")
sys, gyro, acc ,mag = sensor.calibration_status
print(sensor.calibrated)
print(sensor.euler)
print("Sys: ", sys," Gyro: ", gyro," Acc: ", acc, " Mag: ",mag)