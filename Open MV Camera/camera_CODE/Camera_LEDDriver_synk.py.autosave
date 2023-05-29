# Untitled - By: AC - Wed Feb 22 2023


import sensor, image, time, pyb
from pyb import Pin, ExtInt

sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.QVGA)   # Set frame size to QVGA (320x240)
sensor.set_auto_gain(False)
sensor.set_auto_exposure(False,1000) # 1000
sensor.skip_frames(time = 2000)

# sensor acquisition trigger when calling snapshot()
sensor.ioctl(sensor.IOCTL_SET_TRIGGERED_MODE, True)

# Create a clock object to track the FPS.
clock = time.clock()

# set ext. trigger input pulse
sync = 0
def syncline (line):
    global sync
    sync = 1
ext = ExtInt(Pin('P7'), ExtInt.IRQ_RISING, Pin.PULL_DOWN, syncline)
ext.enable()

# set illumination source pin objects
pin0 = Pin('P0', Pin.OUT_PP, Pin.PULL_NONE)
pin7 = Pin('P7', Pin.IN)

# Acquire

while(True):
    if (sync == 1):
        clock.tick()                	# Update the FPS clock.
        pin0.value(pin7.value())    # turn on illumination source
        img = sensor.snapshot()
        pin0.low()                  	# turn off illumination source
        print(clock.fps())
        sync = 0
