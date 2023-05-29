
import sensor, image, time, pyb
from pyb import UART

sensor.reset()                      # Reset and initialize the sensor.
sensor.set_pixformat(sensor.RGB565) # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QVGA)   # Set frame size to QVGA (320x240)
sensor.skip_frames(time = 2000)     # Wait for settings take effect.

# UART 3, and baudrate.
uart = UART(3, 19200)
uart.init(19200, bits = 8, parity = 0, stop = 1)
imageCount = 0

while(True):

    # Take image and save it
    imageCount = imageCount + 1
    sensor.snapshot().save("%d.jpg"%(imageCount), quality = 100)

    # Retrieve image from SD card
    img = image.Image("%d.jpg"%(imageCount), copy_to_fb = True)

    # Send image over UART to ESP 32
    uart.write(img)
    if (uart.any()):
        print(uart.read())
    time.sleep_ms(1000)
