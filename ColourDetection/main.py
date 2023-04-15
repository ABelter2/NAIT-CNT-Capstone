"""
Author: Andrew Belter
Creation Date: Feb. 8, 2023
Rev. History on GitHub
This program will take a picture of a poker and analyse its outer ring.
Its colour will then be determined using defined colour ranges and saturation values
"""
import time
import colour_detection as cd
import serial

if __name__ == "__main__":
    # connect to the serial port
    ser = serial.Serial(port="COM3", baudrate=9600, timeout=1)

    # gets the camera to use for capture
    cam = cd.get_cam(1)

    # for this testing version, loop until the user manually stops the program
    while True:
        # gets the current image from the camera every loop so that if the image is processed, it is current
        # and not old due to the buffer being populated
        # https://stackoverflow.com/questions/43665208/how-to-get-the-latest-frame-from-capture-device-camera-in-opencv/63057626#63057626
        # user: imtherf
        img = cd.get_img(cam)

        # if there is data waiting to be read, read 1 byte and check if it is an analysis request
        if ser.in_waiting > 0:
            ser_in = ser.read(1)  # read 1 byte
            # print(f"ser_in : {ser_in}")  # debug line - displays the read data

            # if the byte that was read is 'a' for analyse, process the image
            if ser_in.decode() == 'a':
                # print("Processing...")  # debug line

                img_pixels = cd.convert_and_scan(img)

                categories = cd.categorize_pixels(img_pixels)
                print("Results:")
                print(categories)

                colour = cd.analyse_categories(categories)
                print(f"The chip is {colour}")

                # send a byte back through serial to indicate to the other device what colour the chip is
                # r=Red, g=Green, b=Blue, w=White, k=Black, o=Other
                # k is used for black because b is used for blue and simplifying to a single byte is simpler to process
                match colour:
                    case 'red':
                        ser_out = 'r'
                    case 'green':
                        ser_out = 'g'
                    case 'blue':
                        ser_out = 'b'
                    case 'white':
                        ser_out = 'w'
                    case 'black':
                        ser_out = 'k'
                    case other:
                        ser_out = 'o'

                # write the encoded colour byte to the other device using serial
                ser.write(ser_out.encode())
