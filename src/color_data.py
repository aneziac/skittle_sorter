import serial
import time


ser = serial.Serial('/dev/cu.usbmodem101', 9800, timeout=1)
skittle_color = 'red'
time.sleep(4)

outfile = open(f'calibration_data/{skittle_color}.txt', 'w')
i = 0

while i < 60:
    line = ser.readline().decode().rstrip()
    outfile.write(line + '\n')
    print(line)
    time.sleep(1)
    i += 1

outfile.close()
