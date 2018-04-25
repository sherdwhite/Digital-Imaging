INTRODUCTION
------------

Written By: Sherd White for CS6420 Assignment # 3

The goal is to perform morpholical operations Erotion, Dilation, Opening, Closing, Geodesic Dilation, and Geodesic Erotion.

REQUIRES
------------

OPENCV 3.x and a compatible C++ compiler.  


RUNNING THE PROGRAM
----------------

The program contains a function for each morpholical operation.  The program takes one input image and performs all the required operations and displays 
the output of each.

For example.
morph file.jpg # results in output of the 6 morpholical operations, the original image, a thresholded image (100 used for the pixel limit), 
					#and the adaptive thresholded image.


DESIGN DECISIONS
----------------
I decided early on to take a single input image and output each morpholical operations result.  For the Geodesic functions, I utilized the sample code provided
by Prof. Bhatia for creating an adaptive thresholded image.  I perform this operation on the original and pass the new AT image to the last two functions.