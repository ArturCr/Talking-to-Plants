# Button Example

import sensor, image, pyb, os, time

sensor.reset()                      # Reset and initialize the sensor.
sensor.set_pixformat(sensor.GRAYSCALE) # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QVGA)   # Set frame size to QVGA (320x240)
sensor.set_windowing((120, 120))

red_led = pyb.LED(1)
green_led = pyb.LED(2)
red_led.on()

sensor.skip_frames(time = 2000)     # Wait for settings take effect.

p = pyb.Pin("P0", pyb.Pin.IN, pyb.Pin.PULL_UP)

red_led.off()
green_led.on()
time.sleep(0.2)
green_led.off()

N_FRAMES=1
clock = time.clock()                # Create a clock object to track the FPS.


print("Ready to capture")


while(True):
    #print(p.value())
    if (p.value()==0):
        print("Capture Started")
        clock.tick()
        img = sensor.snapshot()
        print(clock.fps())
        print("Capture Set")

                #sensor.snapshot().save("0.jpg",(0,0,20,20), quality=100)     # Take a picture and return the image.
                                    # Note: OpenMV Cam runs about half as fast when connected
                                    # to the IDE. The FPS should increase once disconnected.
        img.save("0.jpg",quality=100)
        print("Capture Sucess")

        img = image.Image("0.jpg", copy_to_fb = True)
        print("Disp")

        while (p.value()==0):
            green_led.on()
            time.sleep(0.2)
            green_led.off()


