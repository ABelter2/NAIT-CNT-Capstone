"""
Author: Andrew Belter:
Creation Date: Feb. 13, 2023
This module contains the image processing and colour detection code required to process
poker chips and determine their colour.
"""
import time

import cv2
import math

# manually define the hue colour ranges as a dictionary
# keyed by colour name, valued by low/high range tuple
# range to be used inclusively
# OpenCV hue ranges from 0-179 -
# source: https://docs.opencv.org/4.x/df/d9d/tutorial_py_colorspaces.html#:~:text=For%20HSV%2C%20hue%20range%20is,need%20to%20normalize%20these%20ranges.
""" original
colour_ranges = {
    "red_low": (0, 7),
    "orange": (8, 22),
    "yellow": (23, 37),
    "lime": (38, 52),
    "green": (53, 67),
    "turquoise": (68, 82),
    "cyan": (83, 97),
    "lightblue": (98, 112),
    "darkblue": (113, 127),
    "violet": (128, 142),
    "magenta": (143, 157),
    "pink": (158, 172),
    "red_high": (173, 179)
}
"""

# reg range has missing values because cv to reg doubles, meaning there is a gap in regular, but not in openCV range
colour_ranges = {
    "red_low": (0, 20),     # reg range: 0, 40
    "orange": (21, 25),     # reg range: 42, 50
    "yellow": (26, 34),     # reg range: 52, 68
    "green": (35, 89),      # reg range: 70, 178
    "blue": (90, 135),      # reg range: 180, 270
    "purple": (136, 150),   # reg range: 272, 300
    "pink": (151, 164),     # reg range: 302, 328
    "red_high": (165, 179)  # reg range: 330, 358
}


def get_cam(cam_num=0):
    """
    Gets an OpenCV VideoCapture to use for image capture and processing
    :param cam_num: selects which camera to get, if there are multiple cameras to use.
    If there is only 1 camera, cam0 is needed, so 0 is the default
    :return: VideoCapture camera
    """
    # get the camera, reduce the buffer size to 1
    return cv2.VideoCapture(cam_num, cv2.CAP_DSHOW)


def manual_get_img(cam) -> 'image':
    """
    Uses OpenCV to display a camera feed.
    When the user presses any key on the image window, the camera is released and the image is saved and returned.
    :param cam: the camera to read from
    :return: image from camera
    """
    while True:
        # read and show the image from the camera
        result, img = cam.read()
        result, img = cam.read()

        # get the image dimensions (height and width)
        dim = img.shape  # returns: height, width, num channels
        height = dim[0]
        width = dim[1]

        # display the edges of the analysis area to line up the read (display on a copy to not influence the image)
        copy_img = img.copy()
        for hyp in [160, 230]:
            for angle_deg in range(360):
                angle_rad = angle_deg * math.pi / 180  # convert angle to radians

                # get the offset x and y positions relative to the center of the image
                off_x, off_y = hyp * math.cos(angle_rad), hyp * math.sin(angle_rad)

                # get the absolute x and y positions of the pixel to analyse
                pos_x, pos_y = int(width / 2 + off_x), int(height / 2 + off_y)

                # draw the edges of the analysis section in yellow
                copy_img[pos_y, pos_x] = [0, 255, 255]  # openCv uses coordinate indexes as [y,x], not (x,y)

        cv2.imshow("Camera", copy_img)

        # input a keypress from the user
        key = cv2.waitKey(1)

        # if spaces (ascii 32) was pressed, continue with image processing
        if key == 32:
            break

    # save the image as a file for review
    cv2.imwrite("ImCap.jpg", img)

    cam.release()

    return img


def get_img(cam):
    """
    Accesses the passed camera, takes a picture, saves it, releases the camera, and returns the image for processing.
    :param cam: the camera to read from
    :return: image from camera
    """
    result, img = cam.read()  # get the image from the camera
    #cv2.imshow("ImCap", img)  # is crashing before it can load
    #cv2.waitKey(0)
    #cv2.destroyAllWindows()
    cv2.imwrite("ImCap.jpg", img)  # save the image as a file for review
    return img  # return the image for processing


def convert_and_scan(img):
    """
    Converts the passed image from BGR to HSV colour space.
    After being converted, the HSV pixel values are scanned according a ring scanning pattern.
    :param img: The image to convert to HSV and scan.
    :return: The pixels to categorize and analyse.
    """
    # convert the image from BGR format to HSV format
    img_hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

    # get the image dimensions
    height, width, channels = img_hsv.shape  # returns: (height, width, num channels)

    # analyse the image in a ring pattern
    # hypotenuse will range from 180px to 230px - determined from manual analysis of image capture to find hyp range
    # the pixels will be analysed in a full 360 degree ring
    # the pixel will be hyp distance from the center of the image
    pixels = []
    for hyp in range(170, 230):
        for angle_deg in range(360):
            angle_rad = angle_deg * math.pi / 180  # convert angle to radians

            # get the offset x and y positions relative to the center of the image
            off_x, off_y = hyp * math.cos(angle_rad), hyp * math.sin(angle_rad)

            # get the absolute x and y positions of the pixel to analyse
            pos_x, pos_y = int(width / 2 + off_x), int(height / 2 + off_y)

            # get the HSV value of the target pixel - returns (hue, saturation, value)
            pixels.append(img_hsv[pos_y, pos_x])  # openCv uses coordinate indexes as [y,x], not (x,y)
    return pixels


def categorize_pixels(pixels):
    """
    Categorizes the supplied pixels into colourful and dull.
    Then, the colourful pixels are categorized by colour using the hue and defined colour ranges
    and the dull pixels are categorized using a biased analysis of the value component.
    :param pixels: The HSV pixels to categorize.
    :return: Tuple containing the categorized pixels, the number of colourful pixels, and the number of dull pixels
    """
    # create the dictionary for the pixel values, keyed by colourful or dull
    pixel_values = {"colourful": [], "dull": []}

    # to store the results of categorizing the pixels
    colour_cat = {"white": 0, "black": 0}
    for range_key in colour_ranges:
        # concat low and high red ranges into just red
        if "red" in range_key:
            colour_cat["red"] = 0
        else:
            colour_cat[range_key] = 0

    for px in pixels:
        # sat to determine colourful/dull, hue to determine colourful category, value to determine dull category
        hue, sat, val = px

        # may need to bias saturation check for distinguishing colour/dull based on lighting
        if sat > 255//2:
            # highly saturated pixels are colourful and can later be categorized by hue
            for range_key, range_val in colour_ranges.items():
                low, high = range_val
                if low <= hue <= high:
                    # concat low and high red ranges into just red
                    if "red" in range_key:
                        colour_cat["red"] += 1
                    else:
                        colour_cat[range_key] += 1
                    continue  # stop processing the pixel when its range is found
        else:
            # lowly saturated pixels are dull and can be later categorized by value
            # may need to toward black depending on room lighting
            # glare and reflections may bias the pixel value, so a bias in this step can counteract the lighting bias
            if val > 255//2:
                colour_cat["white"] += 1
            else:
                colour_cat["black"] += 1

    # find what range the colourful pixels fall into and add it to cat_pixels
    for px_hue in pixel_values["colourful"]:
        for range_key, range_val in colour_ranges.items():
            low, high = range_val
            if low <= px_hue <= high:
                # concat low and high red ranges into just red
                if "red" in range_key:
                    colour_cat["red"] += 1
                else:
                    colour_cat[range_key] += 1
                continue  # stop processing the pixel when its range is found

    for px_val in pixel_values["dull"]:
        # may need to toward black depending on room lighting because glare and reflections may bias the pixel value,
        # so a bias in this step can reverse the bias due to lighting
        if px_val > 255//2:
            colour_cat["white"] += 1
        else:
            colour_cat["black"] += 1
    return colour_cat


def analyse_categories(categorized_results):
    max_count = max(categorized_results.values())
    for colour, count in categorized_results.items():
        if count == max_count:
            return colour
