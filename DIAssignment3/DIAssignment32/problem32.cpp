// Sherd White Digital Image Processing Assignment #3.2

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<string>

using namespace cv;

int getGrayMean(Mat);
Vec3b getColorMean(Mat);
bool getChannels(Mat);

int main(int argc, char * argv[])
{
	if (argc <= 1)
	{
		std::cerr << "Picture 1: " << argv[1] << std::endl;
		return (1);
	}

	Mat img = cv::imread(argv[1]);

	if (img.empty())
	{
		std::cerr << "Unable to open the image " << argv[1] << "." << std::endl;
		return (1);
	}

	namedWindow("Original Image", WINDOW_AUTOSIZE);
	imshow("Original Image", img);

	bool grayscale = getChannels(img);
	
	Mat img1;

	if (!grayscale)
	{
		std::cout << "This is a color image." << std::endl;
		Vec3b mean = getColorMean(img);
		std::cout << "\nThe Average Pixel Intensity (Mean) of " << argv[1] << " is " << mean << ".\n" << std::endl;
		for (int r = 0; r < img.rows; r++)
		{
			for (int c = 0; c < img.cols; c++)
			{

				if (img.at<Vec3b>(r, c)[0] < mean[0])
					img.at<Vec3b>(r, c)[0] = 0;
				else if (img.at<Vec3b>(r, c)[0] >= mean[0])
					img.at<Vec3b>(r, c)[0] = 255;
				if (img.at<Vec3b>(r, c)[1] < mean[1])
					img.at<Vec3b>(r, c)[1] = 0;
				else if (img.at<Vec3b>(r, c)[1] >= mean[1])
					img.at<Vec3b>(r, c)[1] = 255;
				if (img.at<Vec3b>(r, c)[2] < mean[2])
					img.at<Vec3b>(r, c)[2] = 0;
				else if (img.at<Vec3b>(r, c)[2] >= mean[2])
					img.at<Vec3b>(r, c)[2] = 255;
			}
		}
		namedWindow("Threshold Image", WINDOW_AUTOSIZE);
		imshow("Threshold Image", img);

	}
	else if (grayscale)
	{
		std::cout << "This is a grayscale image." << std::endl;
		cvtColor(img, img1, CV_BGR2GRAY);  //(input, output, Color Depth)
		int mean = getGrayMean(img1);
		std::cout << "\nThe Average Pixel Intensity (Mean) of " << argv[1] << " is " << mean << ".\n" << std::endl;
		for (int r = 0; r < img1.rows; r++)
		{
			for (int c = 0; c < img1.cols; c++)
				if (img1.at<uchar>(r, c) < mean)
					img1.at<uchar>(r, c) = 0;
				else if (img1.at<uchar>(r, c) >= mean)
					img1.at<uchar>(r, c) = 255;
		}
		namedWindow("Threshold Image", WINDOW_AUTOSIZE);
		imshow("Threshold Image", img1);

	}

	waitKey(0);

	return (0);
}

int getGrayMean(Mat img)
{
	// Save image pixel values
	int intensity = 0;
	for (int r = 0; r < img.rows; r++)
	{
		for (int c = 0; c < img.cols; c++)
			intensity += img.at<uchar>(r, c);
	}
	return (int)round(intensity / (img.rows*img.cols));
}

Vec3b getColorMean(Mat img)
{
	// Save image pixel values
	Vec3b intensity;
	int B = 0, G = 0, R = 0;
	for (int r = 0; r < img.rows; r++)
	{
		for (int c = 0; c < img.cols; c++)
		{
			B += img.at<Vec3b>(r, c)[0];
			G += img.at<Vec3b>(r, c)[1];
			R += img.at<Vec3b>(r, c)[2];
		}
	}

	intensity[0] = B / (img.rows*img.cols);
	intensity[1] = G / (img.rows*img.cols);
	intensity[2] = R / (img.rows*img.cols);
	return intensity;
}

bool getChannels(Mat img)
{
	int pixels = 0;
	bool grayscale = false;

	for (int r = 0; r < img.rows; r++)
	{
		for (int c = 0; c < img.cols; c++) {
			int B = int(img.at<Vec3b>(r, c)[0]);  //B
			int G = int(img.at<Vec3b>(r, c)[1]);  //G
			int R = int(img.at<Vec3b>(r, c)[2]);  //R
			if (B == G && G == R)
				pixels++;
		}
	}

	if (pixels >= (img.rows / 2) * (img.cols / 2))
		grayscale = true;

	return grayscale;
}