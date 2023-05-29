################################
#SERIAL LEAF IMAGE RECEIVER    #
#Author: Artur Cruz            #
#Date: 17/11/2022              #
################################

##TODO:
#checksum
#comments
#error messages

##Instructions:
#Run SERIAL LEAF IMAGE TRANSMITTER first

import serial
import time

serialPort = serial.Serial(
    port="COM7", baudrate=115200, bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE
)

serialPort.flushInput()
serialPort.flushOutput()

serialString = ''  # Used to hold data coming over UART
content=[]

try:
    serialPort.write(b's')
    
    while 1:
        # Wait until there is data waiting in the serial buffer
        if serialPort.in_waiting > 0:

            # Read data out of the buffer until a carraige return / new line is found
            serialString = serialPort.readline()
            if serialString==b'e':
                break
            content.append(serialString)
             
            # Print the contents of the serial data
            try:
                #print(serialString.decode("Ascii"))
                print(serialString)
                #serialPort.write(b'Hello\n')
            except:
                pass
except:
    pass
serialPort.close()
print("Serial Port closed")

a=open("leaf4.png",'wb')
for x in content:
    a.write(x)
a.close()
print("saved image")
