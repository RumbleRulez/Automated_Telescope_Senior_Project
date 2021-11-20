from os import write
import time
import board
import adafruit_bno055

i2c = board.I2C()
sensor = adafruit_bno055.BNO055_I2C(i2c)

print("Calibrating")

