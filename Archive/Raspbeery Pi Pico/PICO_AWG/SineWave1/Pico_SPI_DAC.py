import machine
import utime
import ustruct
import sys
import math


# Settings

# Assign chip select (CS) pin (and start it high)
cs = machine.Pin(17, machine.Pin.OUT)
ldac = machine.Pin(12, machine.Pin.OUT)
sck=machine.Pin(18, machine.Pin.OUT)
mosi=machine.Pin(19, machine.Pin.OUT)

#Initialize SPI
# spi = machine.SPI(0,
#                   baudrate=1000000,
#                   polarity=0,
#                   phase=0,
#                   bits=16,
#                   firstbit=machine.SPI.MSB,
#                   sck=machine.Pin(18),
#                   mosi=machine.Pin(19),
#                   miso=machine.Pin(16))

###############################################################################
# Functions

def reg_write(spi, cs, reg, data):
    """
    Write 1 byte to the specified register.
    """
    
    # Construct message (set ~W bit low, MB bit low)
    msg = bytearray()
    #msg.append(reg)
    #msg.append(data)
    #msg.append(0xAFFF)
    #msg.append(0xFF)
    #print(msg)
    msg=b'3FFF'
    #msg=bytearray()
    
    # Send out SPI message
    cs.value(0)
    spi.write(msg)
    cs.value(1)
    
def reg_read(spi, cs, reg, nbytes=1):
    """
    Read byte(s) from specified register. If nbytes > 1, read from consecutive
    registers.
    """
    
    # Determine if multiple byte (MB) bit should be set
    if nbytes < 1:
        return bytearray()
    elif nbytes == 1:
        mb = 0
    else:
        mb = 1
    
    # Construct message (set ~W bit high)
    msg = bytearray()
    msg.append(0x80 | (mb << 6) | reg)
    
    # Send out SPI message and read
    cs.value(0)
    spi.write(msg)
    data = spi.read(nbytes)
    cs.value(1)
    
    return data

###############################################################################
# Main

# # Start CS pin high
# cs.value(1)
# ldac.value(0)
# utime.sleep(.5)
# #data=[0,0,1,1]+[1]*6+[1]*6
# for y in range(4095):
#     data='0011'+'0'*(12-len(bin(y)[2:]))+bin(y)[2:]
#     #data='0011'+bin(4095)[2:]
#     #print(data)
#     cs.value(0)
#     for x in data:
#         mosi.value(int(x))
#         #utime.sleep(0.001)
#         sck.value(1)
#         #utime.sleep(0.001)
#         sck.value(0)
#     cs.value(1)
#     #utime.sleep(0.01)
# cs.value(1)


# Start CS pin high
cs.value(1)
ldac.value(0)
utime.sleep(.5)
#data=[0,0,1,1]+[1]*6+[1]*6 #max:4095

c=0
while (1==1):
    #1/2 1/2
    #y=int(3*4*4095/8+(4095/8)*math.sin(c))
    y=int(3000+(1095/2)+(1095/2)*math.sin(c))
    c+=.01
    if (c>=(31.4*2)):
        c=0
    #y=4095
    #y=0
    data='0011'+'0'*(12-len(bin(y)[2:]))+bin(y)[2:]
    cs.value(0)
    for x in data:
        mosi.value(int(x))
        sck.value(1)
        sck.value(0)
    cs.value(1)
