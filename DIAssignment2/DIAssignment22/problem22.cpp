// Sherd White Digital Image Processing Assignment #2.2

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<string>

using namespace std;
using namespace cv;

Mat convert2grayscale(Mat colorimg)
{
	Mat grayimg(colorimg.rows, colorimg.cols, CV_8UC1);
	for (int r = 0; r < colorimg.rows; r++)
	{
		for (int c = 0; c < colorimg.cols; c++) {
			int B = (int)round(0.11*colorimg.at<Vec3b>(r, c)[0]);  //B
			int G = (int)round(0.59*colorimg.at<Vec3b>(r, c)[1]);  //G
			int R = (int)round(0.30*colorimg.at<Vec3b>(r, c)[2]);  //R
			int grey = (B + G + R);
			grayimg.at<uchar>(r, c) = grey;
		}
	}
	return grayimg;
}

int main(int argc, char * argv[])
{
	if (argc <= 1)
	{
		std::cerr << "Picture 1: " << argv[1] << std::endl;
		return (1);
	}

	Mat img1 = cv::imread(argv[1]);
	Mat img2 = cv::imread(argv[1]);

	if (img1.empty())
	{
		std::cerr << "Unable to open the image " << argv[1] << "." << std::endl;
		return (1);
	}

	if (img2.empty())
	{
		std::cerr << "Unable to open the img again " << argv[1] << "." << std::endl;
		return (1);
	}

	cvtColor(img1, img1, CV_BGR2GRAY);  //(input, output, Color Depth)
	img2 = convert2grayscale(img2);
	// Save image pixel values

	int intensity1 = 0, intensity2 = 0;
	for (int r = 0; r < img1.rows; r++)
	{
		for (int c = 0; c < img1.cols; c++)
			intensity1 += img1.at<uchar>(r, c);
	}

	for (int r = 0; r < img2.rows; r++)
	{
		for (int c = 0; c < img2.cols; c++)
			intensity2 += img2.at<uchar>(r, c);
	}

	//Calculate average pixel intensity of image1
	intensity1 = round(intensity1 / (img1.rows*img1.cols));
	intensity2 = round(intensity2 / (img2.rows*img2.cols));
	std::cerr << "The Average Pixel Intensity of the cvtColor image is " << intensity1 << std::endl;
	std::cerr << "The Average Pixel Intensity of the manual grayscale conversion function image is " << intensity2 << std::endl;

	//Calculate the distance between the two images
	float distance = 0;
	if (intensity1 >= intensity2) {
		distance = intensity1 - intensity2;
	}
	else
		distance = intensity2 - intensity1;

	std::cerr << "The Average Distance between corresponding Pixel Intensity Levels is " << distance << "." << std::endl;

	namedWindow("cvtColor", WINDOW_AUTOSIZE);
	imshow("cvtColor", img1);

	namedWindow("Manual Grayscale", WINDOW_AUTOSIZE);
	imshow("Manual Grayscale", img2);

	waitKey(0);

	//write results to file
	char fn[80];
	sprintf(fn, "%s.asc", "results");
	FILE * fd = fopen(fn, "w");
	if (!fd)
	{
		fprintf(stderr, "Could not open file %s for writing\n", fn);
		return (1);
	}

	fprintf(fd, "cvtColor Ave Pixel Intensity: %d.\n", intensity1);
	fprintf(fd, "Manual Grayscale Fuction Ave Pixel Intensity: %d.\n", intensity2);
	fprintf(fd, "The Average Distance between corresponding Pixel Intensity Levels is %d\n", distance);

	return (0);
}