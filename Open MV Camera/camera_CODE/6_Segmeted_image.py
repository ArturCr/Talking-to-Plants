# Button Example

import sensor, image, pyb, os, time

sensor.reset()                      # Reset and initialize the sensor.
sensor.set_pixformat(sensor.GRAYSCALE) # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QVGA)   # Set frame size to QVGA (320x240)
sensor.set_windowing((120, 120))    ##!!!Make sure to include this line!!! (windowing is not optional but might be scalable)

red_led = pyb.LED(1)    #Setup Red LED
green_led = pyb.LED(2)  #Setup Green LED
red_led.on()

sensor.skip_frames(time = 2000)     # Wait for settings take effect.

p = pyb.Pin("P2", pyb.Pin.IN, pyb.Pin.PULL_UP) #Setup botton

red_led.off()
green_led.on()
time.sleep(0.2)
green_led.off()

#N_FRAMES=1
clock = time.clock()                # Create a clock object to track the FPS.


print("Ready to capture")


while(True):
    #print(p.value())
    if (p.value()==0):              # p.value=0 when button pressed
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

        thresholdNG = [(th_int, 255)]

        print(thresholdNG)
        b=img.binary(thresholdNG,to_bitmap=True, copy=True)
        img.clear(b).to_grayscale()
        img.save("0.jpg",quality=100)
        print("Done")

        while (p.value()==0): #Check if button was released to avoid deathloops
            green_led.on()
            time.sleep(0.2)
            green_led.off()

