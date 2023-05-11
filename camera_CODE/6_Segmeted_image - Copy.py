# Button Example

## Library/Module imports
import sensor, image, pyb, os, time, math
from pyb import UART
from array import array
from pyb import DAC

##User defined parametters
DAC_startvalue=4095  #4095 - 0
Lookuptable_active=1 #0 or 1
DC_offset = 3500-800 #4095 - 0      ###3500-600
Amplitude = 4095-DC_offset-900
Period= 25 #works within 1 to 10
buffersize=1024*2

# Lookuptable
dac = DAC(2,bits=12)
dac.write(DAC_startvalue) #0

sensor.reset()                      # Reset and initialize the sensor.
sensor.set_pixformat(sensor.GRAYSCALE) # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QVGA)   # Set frame size to QVGA (320x240)
sensor.set_windowing((80, 80))    ##!!!Make sure to include this line!!! (windowing is not optional but might be scalable)#120
sensor.set_auto_gain(False,2)#15
sensor.set_auto_exposure(False, exposure_us=2000) # make smaller to go faster  #20000

red_led = pyb.LED(1)    #Setup Red LED
green_led = pyb.LED(2)  #Setup Green LED
red_led.on()

sensor.skip_frames(time = 2000)     # Wait for settings take effect.

p = pyb.Pin("P2", pyb.Pin.IN, pyb.Pin.PULL_DOWN) #Setup botton
#button = pyb.Pin("P2", pyb.Pin.IN, pyb.Pin.PULL_DOWN)

red_led.off()
green_led.on()
time.sleep(0.2)
green_led.off()

#N_FRAMES=1
clock = time.clock()                # Create a clock object to track the FPS.


print("Ready to capture")


while(True):
    img = sensor.snapshot()
    #print(p.value())
    if (p.value()==1):              # p.value=0 when button pressed
        print("Capture Started")
        clock.tick()
        img = sensor.snapshot()     # capture image
        print(clock.fps())
        print("Capture Set")
        img.save("0.jpg",quality=100) # Store image under the name "0.jpg"
        print("Capture Sucess")



        #im = image.Image("0.jpg", copy_to_fb = True) #output image (Should be piped to segmentation)

        print("Disp")
        im_h=img.get_histogram()
        th=im_h.get_threshold()

        th_int= th.value()

        thresholdNG = [(0, th_int)]

        print(thresholdNG)
        b=img.binary(thresholdNG,to_bitmap=True, copy=True)
        img.clear(b).to_grayscale()
        img.save("0.jpg",quality=100)
        print("Done")

        while (p.value()==0): #Check if button was released to avoid deathloops
            green_led.on()
            time.sleep(0.2)
            green_led.off()

