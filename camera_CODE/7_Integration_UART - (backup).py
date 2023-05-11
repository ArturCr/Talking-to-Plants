## Code Integration

## Library/Module imports
import sensor, image, pyb, os, time, math
from pyb import UART
from array import array
from pyb import DAC

##User defined parametters
DAC_startvalue=4095  #4095 - 0
Lookuptable_active=0 #0 or 1
DC_offset = 3500-600 #4095 - 0
Amplitude = 4095-DC_offset
Period= 5 #works within 1 to 10
buffersize=1024


## functions
def GreenBlink(duration):
    green_led.on()
    time.sleep(duration)
    green_led.off()



## -----------SETUP -----------
# Status LED Setup
red_led = pyb.LED(1)    #Setup Red LED
green_led = pyb.LED(2)  #Setup Green LED

red_led.on()            #LED RED durring setup

# UART Setup
uart = UART(3, 9600) #Bus ,  baudrate
uart.init(9600, bits = 8, parity = 0, stop = 1, timeout=1000)
#time.sleep_ms(3000) #Moredelay
imageCount = 0

# Botton Setup
button = pyb.Pin("P2", pyb.Pin.IN, pyb.Pin.PULL_DOWN)
pressed = 1 #if PULL_Down=1 if PULL_UP=0

# Lookuptable
dac = DAC(2,bits=12)
dac.write(DAC_startvalue)

if Lookuptable_active==1:
    buf = bytearray(buffersize)
    for i in range(len(buf)):
        #buf[i] = 3250 + int((4095-3400-300) * math.sin(2 * math.pi * i / len(buf)))
        buf = array('H', DC_offset + int(Amplitude * math.sin(2 * math.pi * i / buffersize)) for i in range(buffersize))
    #print(buf)
    dac.write_timed(buf, len(buf)//Period, mode=DAC.CIRCULAR)

# Global shutter camera setup and confugurations
sensor.reset()                      # Reset and initialize the sensor.
sensor.set_pixformat(sensor.GRAYSCALE) # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QVGA)   # Set frame size to QVGA (320x240)
sensor.set_auto_gain(False)
sensor.set_auto_exposure(False, exposure_us=10000) # make smaller to go faster
sensor.set_windowing((120, 120))    ##!!!Make sure to include this line!!! (windowing is not optional but might be scalable)
sensor.skip_frames(time = 2000)     # Wait for settings take effect.

red_led.off()
GreenBlink(0.2)

print("Ready to capture")

clock = time.clock()                # Create a clock object to track the FPS.


## -----------Main loop-----------
while(True):
    #print(button.value())
    if (button.value()== pressed):              # button.value=1 when button pressed
        print("Capture Started")
        clock.tick()
        img = sensor.snapshot()     # capture image
        print(clock.fps())
        print("Capture Set")
        img.save("%d.jpg"%(imageCount), quality = 100) # Store image under the name "0.jpg" #-----------------fix me!!!!
        print("Capture Sucess")



        #im = image.Image("0.jpg", copy_to_fb = True) #output image (Should be piped to segmentation)

        print("Disp")
        im_h=img.get_histogram()
        th=im_h.get_threshold()

        th_int= th.value()

        thresholdNG = [(th_int, 255)]

        print(thresholdNG)
        b=img.binary(thresholdNG,to_bitmap=True, copy=True)
        img.clear(b).to_grayscale()
        img.save("0.jpg",quality=100)
        print("Done")

        ##------------

        # Retrieve image from SD card
        img = image.Image("%d.jpg"%(imageCount), copy_to_fb = True)

        #img = image.Image("trial.jpg", copy_to_fb = True)
        time.sleep_ms(1000)
        #uart.write(img)
        x = img.size()
        count = 0
        segmentCount = 0;
        # sends img in segments of
        # works with (16 segs, 1s delay)
        print(x)
        for i in range (x):
            """
            elif count > 64:
                print("Done Packet ",(segmentCount-1))
                count = 0
                time.sleep_ms(1000)
            """
            if count == 0:
                # send packet header
                uart.write('\r'.encode()) #packet start
                uart.writechar(imageCount) #image number
                uart.writechar(segmentCount) # segment count
                print(img[i])
                uart.writechar(img[i])
                time.sleep_ms(500)

            elif count%128 == 0:
                # send 64th byte and stop
                print(img[i])
                uart.writechar(img[i])
                print("Done Packet ",(segmentCount))
                time.sleep_ms(500) # pause a bit


            elif i == x-1 or count==2999:
                print("done")
                print(img[i])
                uart.writechar(img[i])
                #art.write('\0'.encode()) # string end null character
                print("Done")
                uart.write('\r'.encode()) # string end null character
                uart.write('\n'.encode()) # string end null character
                uart.write('\r'.encode()) # string end null character
                uart.write('\n'.encode()) # string end null character
                # wait for next uart signal (notified by '\r')
                count = -1
                time.sleep_ms(30000) # pause a bit
                # TODO => waiting for response from ESP32 by reading UART
                # is currently buggy // so we just sleep for 30sec
                segmentCount +=1
                '''
                while True:
                    if (uart.any()):
                        if (uart.read()=='\r'):
                            time.sleep_ms(10000) # pause a bit
                            break
                '''
            else:
                print(img[i])

                uart.writechar(img[i])
            count +=1

        #end of for

        print("End of UART Transmission")
        """
        if (uart.any()):
            print(uart.read())
        """
        # end


        ##___________

        while (button.value()== pressed): #Check if button was released to avoid deathloops
            green_led.on()
            time.sleep(0.2)
            green_led.off()

