import serial
import time


ser = serial.Serial('', 9800, timeout=1)
skittle_color = 'red'

outfile = open(f'calibration_data/{skittle_color}.txt')
i = 0

while i < 10:
    line = ser.readline().decode().rstrip()
    outfile.write(line)
    print(line)
    time.sleep(1)
    i += 1

outfile.close()
