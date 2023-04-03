"""
Author: Andrew Belter:
Creation Date: Feb. 13, 2023
This file allows for manual triggering and testing of image processing.
"""

import colour_detection as cd

while True:
    cam = cd.get_cam(1)
    img = cd.manual_get_img(cam)
    pixel_values = cd.analyse_pixels(img)
    cat_pixels = cd.categorize_pixels(pixel_values)
    colour = cd.determine_colour(cat_pixels)
    print(f"The chip is {colour}")
