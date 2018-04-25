// Sherd White Digital Imaging Assignment #2

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<string>
#include<fstream>
#include<bitset>

using namespace cv;

void erosion(Mat);
void dilation(Mat);
void opening(Mat);
void closing(Mat);
void geodesicer(Mat, Mat);
void geodesicdi(Mat, Mat);

int main(int argc, char * argv[])
{

	if (argc <= 1)
	{
		std::cerr << "Not enough arguments." << std::endl;
		return (1);
	}

	Mat img = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE); // CV_LOAD_IMAGE_ANYCOLOR);

	if (img.empty())
	{
		std::cerr << "Unable to open the image " << argv[1] << "." << std::endl;
		return (1);
	}

	namedWindow("Original Image", WINDOW_AUTOSIZE);
	imshow("Original Image", img);

	cv::Mat threshimg(img.rows, img.cols, img.type());
	// threshold the image
	for (int r = 0; r < img.rows; r++)
	{
		for (int c = 0; c < img.cols; c++)
			if (img.at<uchar>(r, c) < 100)
				threshimg.at<uchar>(r, c) = 0;
			else if (img.at<uchar>(r, c) >= 100)
				threshimg.at<uchar>(r, c) = 255;
	}
	
	namedWindow("Threshold Image", WINDOW_AUTOSIZE);
	imshow("Threshold Image", threshimg);

	// Erosion
	erosion(threshimg);

	// Dilation
	dilation(threshimg);

	// Opening
	opening(threshimg);

	// Closing
	closing(threshimg);

	cv::medianBlur(img, img, 3);
	cv::blur(img, img, cv::Size(5, 5));

	cv::Mat adaptivethreshimg;
	cv::adaptiveThreshold(img, adaptivethreshimg, 0XFF, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 3, 0);

	namedWindow("Threshold Image", WINDOW_AUTOSIZE);
	cv::imshow("Adaptive Threshold Image", adaptivethreshimg);

	// Geodesic dilation and erosion
	geodesicer(threshimg, adaptivethreshimg);
	geodesicdi(threshimg, adaptivethreshimg);

	cv::waitKey(0);

	return (0);
}

void erosion(Mat img)
{
	cv::Mat erodedimg(img.rows, img.cols, img.type());
	img.copyTo(erodedimg);

	for (int r = 1; r < img.rows - 1 ; r++)
	{
		for (int c = 1; c < img.cols - 1; c++)
		{
			if ((img.at<uchar>(r, c) == 255) && ((img.at<uchar>(r + 1, c) == 0) || (img.at<uchar>(r - 1, c) == 0) || (img.at<uchar>(r, c + 1) == 0) || (img.at<uchar>(r, c - 1) == 0)))
			{
				erodedimg.at<uchar>(r, c) = 0;
			}
		}
	}

	namedWindow("Eroded Image", WINDOW_AUTOSIZE);
	imshow("Eroded Image", erodedimg);
}

void dilation(Mat img)
{
	cv::Mat dilatedimg(img.rows, img.cols, img.type());
	img.copyTo(dilatedimg);

	for (int r = 1; r < img.rows - 1; r++)
	{
		for (int c = 1; c < img.cols - 1; c++)
		{
			if ((img.at<uchar>(r, c) == 0) && ((img.at<uchar>(r + 1, c) == 255) || (img.at<uchar>(r - 1, c) == 255) || (img.at<uchar>(r, c + 1) == 255) || (img.at<uchar>(r, c - 1) == 255)))
			{
				dilatedimg.at<uchar>(r, c) = 255;
			}
		}
	}

	namedWindow("Dilatedimg Image", WINDOW_AUTOSIZE);
	imshow("Dilatedimg Image", dilatedimg);
}

void opening(Mat img)	//erode image then dilate
{
	cv::Mat erodedimg(img.rows, img.cols, img.type());
	img.copyTo(erodedimg);
	cv::Mat openimg(img.rows, img.cols, img.type());

	for (int r = 1; r < img.rows - 1; r++)
	{
		for (int c = 1; c < img.cols - 1; c++)
		{
			if ((img.at<uchar>(r, c) == 255) && ((img.at<uchar>(r + 1, c) == 0) || (img.at<uchar>(r - 1, c) == 0) || (img.at<uchar>(r, c + 1) == 0) || (img.at<uchar>(r, c - 1) == 0)))
			{
				erodedimg.at<uchar>(r, c) = 0;
			}
		}
	}

	erodedimg.copyTo(openimg);

	for (int r = 1; r < img.rows - 1; r++)
	{
		for (int c = 1; c < img.cols - 1; c++)
		{
			if ((erodedimg.at<uchar>(r, c) == 0) && ((erodedimg.at<uchar>(r + 1, c) == 255) || (erodedimg.at<uchar>(r - 1, c) == 255) || (erodedimg.at<uchar>(r, c + 1) == 255) || (erodedimg.at<uchar>(r, c - 1) == 255)))
			{
				openimg.at<uchar>(r, c) = 255;
			}
		}
	}

	namedWindow("Opened Image", WINDOW_AUTOSIZE);
	imshow("Opened Image", openimg);
}

void closing(Mat img)		//dilate image then erode
{
	cv::Mat dilatedimg(img.rows, img.cols, img.type());
	img.copyTo(dilatedimg);
	cv::Mat closedimg(img.rows, img.cols, img.type());

	for (int r = 1; r < img.rows - 1; r++)
	{
		for (int c = 1; c < img.cols - 1; c++)
		{
			if ((img.at<uchar>(r, c) == 0) && ((img.at<uchar>(r + 1, c) == 255) || (img.at<uchar>(r - 1, c) == 255) || (img.at<uchar>(r, c + 1) == 255) || (img.at<uchar>(r, c - 1) == 255)))
			{
				dilatedimg.at<uchar>(r, c) = 255;
			}
		}
	}

	dilatedimg.copyTo(closedimg);

	for (int r = 1; r < img.rows - 1; r++)
	{
		for (int c = 1; c < img.cols - 1; c++)
		{
			if ((dilatedimg.at<uchar>(r, c) == 255) && ((dilatedimg.at<uchar>(r + 1, c) == 0) || (dilatedimg.at<uchar>(r - 1, c) == 0) || (dilatedimg.at<uchar>(r, c + 1) == 0) || (dilatedimg.at<uchar>(r, c - 1) == 0)))
			{
				closedimg.at<uchar>(r, c) = 0;
			}
		}
	}

	namedWindow("Closed Image", WINDOW_AUTOSIZE);
	imshow("Closed Image", closedimg);
}

void geodesicer(Mat img, Mat mask) 
{
	cv::Mat geoerotionimg(img.rows, img.cols, img.type(), Scalar(255));
	mask.copyTo(geoerotionimg);

	for (int r = 1; r < img.rows - 1; r++)
	{
		for (int c = 1; c < img.cols - 1; c++)
		{
			if ((img.at<uchar>(r, c) == 0) && (img.at<uchar>(r + 1, c) == 0) && (img.at<uchar>(r - 1, c) == 0) &&
				(img.at<uchar>(r, c + 1) == 0) && (img.at<uchar>(r, c - 1) == 0) && (img.at<uchar>(r - 1, c - 1) == 0) &&
				(img.at<uchar>(r + 1, c - 1) == 0) && (img.at<uchar>(r - 1, c + 1) == 0) && (img.at<uchar>(r + 1, c + 1) == 0))
			{
				geoerotionimg.at<uchar>(r, c) = 0;
			}
		}
	}

	namedWindow("Geodesic Erosion Image", WINDOW_AUTOSIZE);
	imshow("Geodesic Erosion Image", geoerotionimg);
}

void geodesicdi(Mat img, Mat mask)
{
	cv::Mat geodilatedimg(img.rows, img.cols, img.type(), Scalar(255));

	for (int r = 1; r < img.rows - 1; r++)
	{
		for (int c = 1; c < img.cols - 1; c++)
		{
			if ((mask.at<uchar>(r, c) == 0) && ((img.at<uchar>(r, c) == 0) || (img.at<uchar>(r + 1, c) == 0) || (img.at<uchar>(r - 1, c) == 0) || 
				(img.at<uchar>(r, c + 1) == 0) || (img.at<uchar>(r, c - 1) == 0) || (img.at<uchar>(r - 1, c - 1) == 0) || 
				(img.at<uchar>(r + 1, c - 1) == 0) || (img.at<uchar>(r - 1, c + 1) == 0) || (img.at<uchar>(r + 1, c + 1) == 0)))
			{
				geodilatedimg.at<uchar>(r, c) = 0;
			}
		}
	}

	namedWindow("Geodesic Dilated Image", WINDOW_AUTOSIZE);
	imshow("Geodesic Dilated Image", geodilatedimg);
}