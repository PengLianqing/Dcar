import serial
import time
import re

last = 0 

ser = serial.Serial("/dev/ttyUSB0", 115200, timeout=1)
print (ser.name)
print (ser.port)
#ser.open() 

while 1:
    try:
        receive_data = ser.readline()
        # for i in receive_data:
        #     print(i)
        #print(receive_data)
        #receive_data = 'Server:115.00,105.00,966,359'
        #receive_data = 'D:48.60,100.50,0,629\n'
        (a, b, c, d ,e) = [t(s) for t,s in zip((str,float,float,int,int),re.search('^(\w+):([\d.]+),([\d.]+),(\d+),(\d+)',receive_data).groups())]
        #print (a, b, c, d ,e)
    except Exception as ret:
        # print(ret)
        pass
    #ser.write("hello")
    if d!= last:
        if d==1:
            print("start")
        elif d==2:
            print("stop")
    last = d
ser.close()
