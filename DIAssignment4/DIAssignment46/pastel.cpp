// Sherd White Digital Image Processing Assignment #4.6

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<string>

using namespace cv;
Mat pastel(Mat, int);
void tbCallback(int, void*);

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

	namedWindow("Original Image", WINDOW_AUTOSIZE);
	imshow("Original Image", img);

	cv::Mat filterimg = pastel(img, 20);

	namedWindow("Smooth Image", WINDOW_AUTOSIZE);
	imshow("Smooth Image", filterimg);

	// Create a Trackbar in the window just created
	int tbVal = 20;

	int tbMaxVal;
	if (img.rows > img.cols)
		tbMaxVal = img.rows - 1;
	else
		tbMaxVal = img.cols - 1;

	void *pImg = &img;
	cv::createTrackbar("Smoothing", "Smooth Image", &tbVal, tbMaxVal, tbCallback, pImg);

	waitKey(0);

	return (0);
}

Mat pastel(Mat img, int blur)
{
	Mat filterimg; //= Mat::zeros(img.rows, img.cols, CV_8UC1);
	img.copyTo(filterimg);

	double distance = 0;
	double value = 0;
	int i = 0;

	// Apply spacial filter (average) for top quadrant
	for (int r = 0; r <= blur; r++)
	{
		for (int c = i; c < img.cols - i; c++)
		{
			distance = i;

			value = (int)img.at<uchar>(r, c) + ((blur - distance) / blur)*(255 - (int)img.at<uchar>(r, c));
			filterimg.at<uchar>(r, c) = (int)value;
		}
		i++;
	}

	i = 0;
	value = 0;

	// Apply spacial filter (average) for bottom quadrant
	for (int r = img.rows - 1; r >= img.rows - blur; r--)
	{
		for (int c = i; c < img.cols - i; c++)
		{
			distance = i;

			value = (int)img.at<uchar>(r, c) + ((blur - distance) / blur)*(255 - (int)img.at<uchar>(r, c));
			filterimg.at<uchar>(r, c) = (int)value;
		}
		i++;
	}

	i = 0;
	value = 0;

	// Apply spacial filter (average) for left quadrant
	for (int c = 0; c <= blur; c++)
	{
		for (int r = i; r <= img.rows - i - 1; r++)
		{
			distance = i;

			value = (int)img.at<uchar>(r, c) + ((blur - distance) / blur)*(255 - (int)img.at<uchar>(r, c));
			filterimg.at<uchar>(r, c) = (int)value;
		}
		i++;
	}

	i = 0;
	value = 0;

	// Apply spacial filter (average) for right quadrant
	for (int c = img.cols - 1; c >= img.cols - blur; c--)
	{
		for (int r = i; r <= img.rows - i - 1; r++)
		{
			distance = i;

			value = (int)img.at<uchar>(r, c) + ((blur - distance) / blur)*(255 - (int)img.at<uchar>(r, c));
			filterimg.at<uchar>(r, c) = (int)value;
		}
		i++;
	}

	//imshow("Smooth Image", filterimg);
	return filterimg;
}

// function tbCallback
void tbCallback(int blur, void* img) {

	// Cast the void pointer to Mat*
	cv::Mat* filterimg;
	filterimg = static_cast<cv::Mat*>(img);

	// Use the Trackbar value and _data to modify the image displayed
	cv::Mat img_modified = pastel(*filterimg, blur);

	// Display the modified image
	imshow("Smooth Image", img_modified);

}