from os import wait, write
import time
import board
import adafruit_bno055

i2c = board.I2C()
sensor = adafruit_bno055.BNO055_I2C(i2c)

while(1):
    #sensor.mode = adafruit_bno055.NDOF_MODE
    time.sleep(1)
    print("Calibration Status:")
    sys, gyro, acc ,mag = sensor.calibration_status
    print(sensor.calibrated)
    
    print("Mag offset: ", sensor.offsets_magnetometer)
    print("Gyro offset: ", sensor.offsets_gyroscope)
    print("ACC offset: ", sensor.offsets_accelerometer)

    print("Calibations-> ","Sys: ", sys," Gyro: ", gyro," Acc: ", acc, " Mag: ",mag)