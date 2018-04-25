// Sherd White Digital Image Processing Assignment #4.3

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<string>

using namespace cv;
Mat smFilter(Mat, int);
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

	//std::string filtersizestr = argv[2];

	//int filtersize;
	//std::stringstream(filtersizestr) >> filtersize;

	namedWindow("Original Image", WINDOW_AUTOSIZE);
	imshow("Original Image", img);

	cv::Mat filterimg = smFilter(img, 7);

	namedWindow("Smooth Image", WINDOW_AUTOSIZE);
	imshow("Smooth Image", filterimg);

	// Create a Trackbar in the window just created
	int tbVal = 7;
	int tbMaxVal = 15;
	void *pImg = &img;
	cv::createTrackbar("Smoothing", "Smooth Image", &tbVal, tbMaxVal, tbCallback, pImg);

	waitKey(0);

	return (0);
}

Mat smFilter(Mat img, int filtersize) 
{
	Mat filterimg; // = Mat::zeros(img.rows, img.cols, CV_8UC1);
	img.copyTo(filterimg);
	double average = 0;
	int pixels = 0;
	int range = (filtersize / 2);

	// Apply spacial filter (average)
	for (int r = range; r < img.rows - range; r++)
	{
		for (int c = range; c < img.cols - range; c++)
		{
			for (int i = 0; i < filtersize; i++)
			{
				for (int j = 0; j < filtersize; j++)
				{
					average = average + img.at<uchar>(r + (i - range), c + (j - range));
					pixels++;
				}
			}
			average = (average / pixels);
			filterimg.at<uchar>(r, c) = (int)average;
			average = 0;
			pixels = 0;
		}
	}

	//imshow("Smooth Image", filterimg);
	return filterimg;
}

// function tbCallback
void tbCallback(int filtersize, void* img) {

	// Cast the void pointer to Mat*
	cv::Mat* filterimg;
	filterimg = static_cast<cv::Mat*>(img);

	// Use the Trackbar value and _data to modify the image displayed
	cv::Mat img_modified = smFilter(*filterimg, filtersize);

	// Display the modified image
	imshow("Smooth Image", img_modified);

}