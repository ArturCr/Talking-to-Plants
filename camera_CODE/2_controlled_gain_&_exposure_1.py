#User defined parametters

Amplitude= 595-400 #1000 # 4095-DC_offset while DC_offset>=4095//2
DC_offset= 3500-600
Period= 25 #works within 1 to 10
buffersize=1024




import sensor, image, time,math
from array import array
from pyb import DAC

sensor.reset()                      # Reset and initialize the sensor.
sensor.set_pixformat(sensor.GRAYSCALE) # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QVGA)   # Set frame size to QVGA (320x240)
sensor.set_auto_gain(False)
sensor.set_auto_exposure(False,5000) # 1000
sensor.skip_frames(time = 2000)     # Wait for settings take effect.
clock = time.clock()                # Create a clock object to track the FPS.
buf = bytearray(buffersize)
for i in range(len(buf)):
    #buf[i] = 3250 + int((4095-3400-300) * math.sin(2 * math.pi * i / len(buf)))
    buf = array('H', DC_offset + int(Amplitude * math.sin(2 * math.pi * i / buffersize)) for i in range(buffersize))
print(buf)
dac = DAC(2,bits=12)
dac.write_timed(buf, len(buf)//Period, mode=DAC.CIRCULAR)




while(True):
    clock.tick()                    # Update the FPS clock.
    img = sensor.snapshot()         # Take a picture and return the image.
    #print(clock.fps())              # Note: OpenMV Cam runs about half as fast when connected
                                    # to the IDE. The FPS should increase once disconnected.




# create a buffer containing a sine-wave


# output the sine-wave at 400Hz
