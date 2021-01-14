import plc,time
import config

plc.set_rbuf(2,config.Parameters["Zero"])
#plc.set_rbuf(3,config.Parameters["Distance"])
plc.set_rbuf(3,0)
plc.plcstart()
time.sleep(2)

while 1:
    print("sleep\n")
    time.sleep(0.3)
    plc.set_rbuf(3,0)
    print("15\n")
    print("sleep\n")
    time.sleep(0.3)
    plc.set_rbuf(3,15)
    print("0\n")
    