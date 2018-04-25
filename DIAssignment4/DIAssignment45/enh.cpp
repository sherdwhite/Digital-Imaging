// Sherd White Digital Image Processing Assignment #4.5

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<string>

using namespace cv;
void enh(Mat);

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

	namedWindow("Enhanced Image", WINDOW_AUTOSIZE);

	enh(img);

	waitKey(0);

	return (0);
}

void enh(Mat img)
{
	Mat filterimg; // = Mat::zeros(img.rows, img.cols, CV_8UC1);
	img.copyTo(filterimg);
	int average = 0;

	// Apply spacial filter (average)
	for (int r = 1; r < img.rows - 1; r++)
	{
		for (int c = 1; c < img.cols - 1; c++)
		{
			average = (8 * (int)img.at<uchar>(r, c)) - ((int)img.at<uchar>(r - 1, c - 1) + (int)img.at<uchar>(r - 1, c) +
				(int)img.at<uchar>(r - 1, c + 1) + (int)img.at<uchar>(r, c - 1) +
				(int)img.at<uchar>(r, c + 1) + (int)img.at<uchar>(r + 1, c - 1) +
				(int)img.at<uchar>(r + 1, c) + (int)img.at<uchar>(r + 1, c + 1));

			//std::cout << average << std::endl;
			if ((img.at<uchar>(r, c) + average) > 0 && (img.at<uchar>(r, c) + average <= 255))
				filterimg.at<uchar>(r, c) = (int)img.at<uchar>(r, c) + average;
			else if ((img.at<uchar>(r, c) + average) < 0)
				filterimg.at<uchar>(r, c) = 0;
			else if ((img.at<uchar>(r, c) + average) > 255)
				filterimg.at<uchar>(r, c) = 255;				
		}
	}

	imshow("Enhanced Image", filterimg);
}