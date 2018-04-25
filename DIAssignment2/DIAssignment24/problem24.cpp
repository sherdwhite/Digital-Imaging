// Sherd White Digital Image Processing Assignment #2.4

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<string>

using namespace std;
using namespace cv;

Mat subSample(Mat img)
{
	Mat newimg(img.rows / 2, img.cols / 2, CV_8UC1);
	for (int r = 0; r < img.rows; r += 2)
	{
		for (int c = 0; c < img.cols; c += 2) {
			newimg.at<uchar>(r / 2, c / 2) = img.at<uchar>(r, c);
		}
	}

	return newimg;
}

int main(int argc, char * argv[])
{
	if (argc <= 1)
	{
		std::cerr << "Picture 1: " << argv[1] << std::endl;
		return (1);
	}

	Mat img1 = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	Mat img2(img1.rows / 2, img1.cols / 2, CV_8UC1);

	if (img1.empty())
	{
		std::cerr << "Unable to open the image " << argv[1] << "." << std::endl;
		return (1);
	}

	img2 = subSample(img1);
	// Save image pixel values

	namedWindow("original", WINDOW_AUTOSIZE);
	imshow("original", img1);

	namedWindow("subsample", WINDOW_NORMAL);
	resizeWindow("subsample", img1.cols, img1.rows);
	imshow("subsample", img2);

	imwrite("subsample.jpg", img2);

	waitKey(0);

	return (0);
}