INTRODUCTION
------------

Written By: Sherd White for CS6420 Assignment # 2

The goal is to perform image encoding using variable length codes (Huffman code) and to recover the original image
from the encoded image. The encoded image is saved as a string of bits using a binary file. The binary file contains image
height, width, and Huffman code keys. The number of channels is not recorded as the program will convert any input image into 
8 bit, one channel image.

REQUIRES
------------

OPENCV 3.x and a compatible C++ compiler.  


RUNNING THE PROGRAM
----------------

There two functions: an encoder and a decoder, both called from the same program. The program determines if the file is a JPEG image or encoded sequence based on the file extension. 
If the file ends in .huf, the program will decode it and save it as a JPEG file. If the file extension is .jpg, the prgram will read the file through OpenCV functions
and encode it, saving the encoded file named "file" as a binary file with the .huf extension.
For example.
huffman file.jpg # results in file.huf
huffman file.huf # results in decodedfile.jpg


DESIGN DECISIONS
----------------
I originally implimented a priority queue to generate the Huffman codes.  This ended up being to complicated.  Per Prof. Sanjiv recommendations, simply worked with vectors and arrays and performed 
copy and sorts starting from the image histogram sorted by frequency to generate huffman codes starting from the binary value 0 and ending at 255, or 1111 1111.  I then resorted the encoded array 
from 0 to 255 and used this to encode the image.  

To decode I simply read in the values from the file, which were stored with spaces to indicate the next value with the width, height, and encoded values being read from the beginning of the 
file and the image values followed.  This successfully enabled me to encode the image and decode it back with the encoded image size being smaller than it would be if the raw image data was stored.  
