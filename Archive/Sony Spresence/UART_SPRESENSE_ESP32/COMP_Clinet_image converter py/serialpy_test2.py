################################
#SERIAL LEAF IMAGE TRANSMITTER #
#Author: Artur Cruz            #
#Date: 17/11/2022              #
################################

##TODO:
#checksum
#comments
#error messages

##Instructions:
#Run this first and wait until you get a hello message then run SERIAL LEAF IMAGE RECEIVER

##Import libraries
import serial
import time

serialPort1 = serial.Serial(
    port="COM8", baudrate=115200, bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE
)

##Clear data from previous tests
serialPort1.flushInput()
serialPort1.flushOutput()


datain=open("leaf2.png",'rb')
raw=datain.read()
datain.close

raw_array=[]
print("1")
blocksize=128
while raw!=b'':
    raw_array.append(raw[:blocksize])
    raw=raw[blocksize:]
print("2")



if 1==1:
    while 1:
        # Wait until there is data waiting in the serial buffer
        if serialPort1.in_waiting > 0:
            # Read data out of the buffer until a carraige return / new line is found
            serialString = serialPort1.readline()
            #await for 's' start signal to avoid lossing the first message due to timeout
            if serialString==b's':
                break
            else:
                #debugging print
                print('No start signal: '+serialString)
    
try:
    size=len(raw_array)
    print("0%")
    for x in range(size):
        serialPort1.write(raw_array[x])
        #time.sleep(.01)
        print(str(round(x/size*100,2))+"%")
    print("100%")
    serialPort1.write(b'\ne')
except Exception as e: print(e)


serialPort1.close()
print("Serial Port1 closed")



#### Example of base readding loop

##try:
##    while 1:
##        # Wait until there is data waiting in the serial buffer
##        if serialPort.in_waiting > 0:
##
##            # Read data out of the buffer until a carraige return / new line is found
##            serialString = serialPort.readline()
##
##            # Print the contents of the serial data
##            try:
##                print(serialString.decode("Ascii"))
##                #serialPort.write(b'Hello\n')
##            except:
##                pass
##except:
##    pass
