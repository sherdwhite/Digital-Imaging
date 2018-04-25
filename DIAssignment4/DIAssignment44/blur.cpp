// Sherd White Digital Image Processing Assignment #4.4

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<string>

using namespace cv;
void blur(Mat);

int main(int argc, char * argv[])
{
	if (argc <= 1)
	{
		std::cerr << "Not enough arguements." << std::endl;
		return (1);
	}

	Mat img = cv::imread(argv[1], CV_8UC1);

	if (img.empty())
	{
		std::cerr << "Unable to open the image " << argv[1] << "." << std::endl;
		return (1);
	}

	Mat laplace, result;

	namedWindow("Original Image", WINDOW_AUTOSIZE);
	imshow("Original Image", img);

	namedWindow("Blurred Image", WINDOW_AUTOSIZE);

	blur(img);

	waitKey(0);

	return (0);
}

void blur(Mat img)
{
	Mat filterimg = Mat::zeros(img.rows, img.cols, CV_8UC1);
	Mat kernel = Mat::zeros(img.rows, img.cols, CV_8UC1);

	int resolution;
	if (img.rows > img.cols)
		resolution = img.rows;
	else
		resolution = img.cols;

	double meanval = (resolution / (img.rows*img.cols)) / 2;

	cv::Point center(img.rows / 2, img.cols / 2);
	cv::Point pixel;

	int distance = 0;

	// Apply spacial filter (average)
	for (int r = 0; r < img.rows; r++)
	{
		for (int c = 0; c < img.cols; c++)
		{
			pixel.x = c;
			pixel.y = r;
			distance = cv::norm(center - pixel);
			kernel.at<uchar>(r, c) = (int)(meanval * distance);
		}
	}

	imshow("Blurred Image", filterimg);
}