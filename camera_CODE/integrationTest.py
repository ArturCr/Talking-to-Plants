
import sensor, image, time, pyb
from pyb import UART
#import serial

#ser = serial.Serial("usbserial-02652DF6", timeout=1, dsrdtr=False)
#   Change port name to MAC port
# at /dev/



sensor.reset()                      # Reset and initialize the sensor.
sensor.set_pixformat(sensor.GRAYSCALE) # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QVGA)   # Set frame size to QVGA (320x240)
sensor.skip_frames(time = 2000)     # Wait for settings take effect.

# UART 3, and baudrate.
uart = UART(3, 9600)
uart.init(9600, bits = 8, parity = 0, stop = 1, timeout_char=1000)
imageCount = 0

# delay start
time.sleep_ms(3000)
print("starting soon")
#time.sleep_ms(5000)

while(True):
    print("hello World")

    # Take image and save it

    imageCount = imageCount + 1
    sensor.snapshot().save("%d.jpg"%(imageCount), quality = 100)

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

    print("abcdefg")
    """
    if (uart.any()):
        print(uart.read())
    """
    # end
    time.sleep_ms(1000000)
