"""
Author: Andrew Belter:
Creation Date: Feb. 13, 2023
This file allows for manual triggering and testing of image processing.
"""

import colour_detection as cd

while True:
    cam = cd.get_cam(1)
    img = cd.manual_get_img(cam)
    img_pixels = cd.convert_and_scan(img)
    categories = cd.categorize_pixels(img_pixels)
    colour = cd.analyse_categories(categories)
    print(f"The chip is {colour}")
