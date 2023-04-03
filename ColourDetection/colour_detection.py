"""
Author: Andrew Belter:
Creation Date: Feb. 13, 2023
This module contains the image processing and colour detection code required to process
poker chips and determine their colour.
"""
import time

import cv2
import math
import numpy as np

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
    cam = cv2.VideoCapture(cam_num, cv2.CAP_DSHOW)
    return cam


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


def analyse_pixels(img):
    """
    Analyses the pixels in a ring pattern around the center of the image.
    Pixels will be analysed using the HSV (hue, saturation, value) format.
    Pixels with a saturation above a set value are considered colourful, below the value are dull.
    Populates a return dictionary with the hue values of colourful pixels and value values of dull pixels.
    :param img: image to be analysed
    :return: dictionary containing lists of dull and colourful pixel values
    """
    # create the dictionary for the pixel values, keyed by colourful or dull
    pixel_values = {"Colourful": [], "Dull": []}

    # convert the image from BGR format to HSV format
    img_hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

    # get the image dimensions (height and width)
    dim = img.shape  # returns: height, width, num channels
    height = dim[0]
    width = dim[1]

    # analyse the image in a ring pattern
    # hypotenuse will range from 180px to 230px - determined from manual analysis of image capture to find hyp range
    # the pixels will be analysed in a full 360 degree ring
    # the pixel will be hyp distance from the center of the image
    for hyp in range(170, 230):
        for angle_deg in range(360):
            angle_rad = angle_deg * math.pi / 180  # convert angle to radians

            # get the offset x and y positions relative to the center of the image
            off_x, off_y = hyp * math.cos(angle_rad), hyp * math.sin(angle_rad)

            # get the absolute x and y positions of the pixel to analyse
            pos_x, pos_y = int(width / 2 + off_x), int(height / 2 + off_y)

            # get the HSV value of the target pixel - returns (hue, saturation, value)
            px = img_hsv[pos_y, pos_x]  # openCv uses coordinate indexes as [y,x], not (x,y)

            # check the saturation value to see if px is colourful or dull
            # saturation range: 0-255
            # colourful threshold is at 100 to account for poor lighting affecting the saturation
            if px[1] > 120:
                pixel_values["Colourful"].append(px[0])  # append the pixel's hue to the colourful pixels
            else:
                pixel_values["Dull"].append(px[2])  # append the pixel's value to the colourful pixels
    # print(f"analyse_pixel pixel_values: {pixel_values}")
    return pixel_values


def categorize_pixels(pixel_values):
    """
    Categorizes the majority type (colourful or dull) of pixels to use to determine the majority colour.
    Categorizes the pixels by colour based on colour range for colourful pixels and value range for dull pixels.
    :param pixel_values: dictionary of colourful and dull pixels hues and values (respectively)
    :return: A categorized dictionary of pixel values - categorizes pixels by colour.
        Keyed by colour, valued by count of that colour
    """
    cat_pixels = {}  # to store the results of categorizing the pixels

    # debugging lines
    # print(f"Num colourful px: {len(pixel_values['Colourful'])}")
    # print(f"Num dull px: {len(pixel_values['Dull'])}")

    # determine which to categorize between colourful and dull pixels based on the amount of each
    # bias towards colourful pixels due to lighting influencing the values
    if len(pixel_values["Colourful"]) * 1.5 > len(pixel_values["Dull"]):
        # for each colourful pixel, check each colour range until the range that contains the pixel value
        # is found, then add the pixel to the categorized pixels with the colour name as the key
        # if the key (colour name) is in the dict, increase the count value - otherwise init to 1
        for px_hue in pixel_values["Colourful"]:
            for colour_range in colour_ranges:
                low = colour_ranges[colour_range][0]
                high = colour_ranges[colour_range][1]
                if low <= px_hue <= high:
                    # store low and high red as just red in the categorized pixels dict
                    # store all other colours based off their colour range key
                    if "red" in colour_range:
                        if "red" in cat_pixels:
                            cat_pixels["red"] += 1
                        else:
                            cat_pixels["red"] = 1
                    else:
                        if colour_range in cat_pixels:
                            cat_pixels[colour_range] += 1
                        else:
                            cat_pixels[colour_range] = 1
                    continue  # stop processing the pixel when its range is found
    else:
        for px_val in pixel_values["Dull"]:
            # if the pixel value is greater than 255/2 (half of the value range),
            # the pixel is white - if it is less, the pixel is black
            # if the key (colour name) is in the dict, increase the count value - otherwise init to 1
            if px_val > 150:
                # print(f"white : {px_val}")
                if "white" in cat_pixels:
                    cat_pixels["white"] += 1
                else:
                    cat_pixels["white"] = 1
            else:
                # print(f"black : {px_val}")
                if "black" in cat_pixels:
                    cat_pixels["black"] += 1
                else:
                    cat_pixels["black"] = 1
    print(f"categorize_pixel cat_pixels: {cat_pixels}")
    return cat_pixels


def determine_colour(categorized_pixels):
    """
    Determines the colour name of the majority colour in the categorized pixels dictionary
    :param categorized_pixels: a dictionary containing pixels categorized by colour
    :return: the majority colour name contained within the passed dictionary
    """
    max_count = max(categorized_pixels.values())
    for colour in categorized_pixels:
        if categorized_pixels[colour] == max_count:
            return colour
