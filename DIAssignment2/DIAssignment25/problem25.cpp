// Sherd White Digital Image Processing Assignment #2.5

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<string>

using namespace std;
using namespace cv;

int main(int argc, char * argv[])
{
	if (argc <= 1)
	{
		std::cerr << "Picture 1: " << argv[1] << std::endl;
		return (1);
	}

	Mat img1 = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

	if (img1.empty())
	{
		std::cerr << "Unable to open the image " << argv[1] << "." << std::endl;
		return (1);
	}

	Mat bit = Mat::zeros(img1.rows, img1.cols, CV_8UC1);

	for (int r = 0; r < img1.rows; r++) {
		for (int c = 0; c < img1.cols; c++) {
			/*if (img1.at<uchar>(r, c) & uchar(!0))
			bit.at<uchar>(r, c) = uchar(255);
			else
			bit.at<uchar>(r, c) = uchar(0);*/
			// This is better.  
			bit.at<uchar>(r, c) = (img1.at<uchar>(r, c) & uchar(!0)) ? uchar(255) : uchar(0);  //If non-zero set to 255, else set to 0
		}
	}

	namedWindow("bit planes", WINDOW_AUTOSIZE);
	imshow("bit planes", bit);

	imwrite("bitplanes.jpg", bit);

	waitKey(0);

	return (0);
}