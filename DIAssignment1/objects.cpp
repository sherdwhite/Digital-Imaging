// Sherd White Digital Image Processing Assignment #1

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<time.h>       /* time */

using namespace cv;

///////////////////////////////////////////////////////////////////////////////////////////////////
int main() {

	//cv::Mat newImage(480, 640, CV_8UC1); // declare new image //if using namespace cv, no need to type cv::

	//newImage.setTo(cv::Scalar(0));  // set image background as black

	int width = 640, height = 480;
	Mat newImage = Mat::zeros(height, width, CV_8UC1); // declare new black grayscale image

	srand((unsigned int)time(NULL)); /* initialize random seed: */
	int randX = rand() % width + 1;  //random X
	int randY = rand() % height + 1; //random Y

	for (int i = 50; i > 0; i--) {
		rectangle(newImage, cvPoint(randX - i, randY + i), cvPoint(randX + i, randY - i), cvScalar(255 - i), CV_FILLED);  //CV_FILLED to fill square with single color
	}

	randX = rand() % width + 1;  //random X
	randY = rand() % height + 1; //random Y

	int k = 80;
	for (int j = 100; j > 0; j--) {
		rectangle(newImage, cvPoint(randX - j, randY + k), cvPoint(randX + j, randY - k), cvScalar(255 - j), CV_FILLED);  //CV_FILLED to fill square with single color
		if (k > 0)
			k--;
	}

	randX = rand() % width + 1;  //random X
	randY = rand() % height + 1; //random Y

	for (int l = 50; l > 0; l--) {
		circle(newImage, cvPoint(randX, randY), l , cvScalar(255 - l), CV_FILLED);
	}


	imshow("newImage", newImage);		// show windows

	waitKey(0);					// hold windows open until user presses a key

	return(0);
}