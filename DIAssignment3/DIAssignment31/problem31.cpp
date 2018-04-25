// Sherd White Digital Image Processing Assignment #3.1

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<string>
#include<fstream>

using namespace cv;

bool grayscale;

void getMousePos(int, int, int, int, void*);
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
		std::cerr << "Unable to open the first image " << argv[1] << "." << std::endl;
		return (1);
	}

	bool grayscale = getChannels(img);
	
	if (!grayscale)
	{
		std::cout << "This is a color image." << std::endl;
	}
	else if (grayscale)
	{
		std::cout << "This is a grayscale image." << std::endl;
	}

	std::cout << "Left-Click on image with the mouse to get color value at pixel." << std::endl;

	namedWindow("Image", WINDOW_AUTOSIZE);

	//set the callback function for any mouse event
	setMouseCallback("Image", getMousePos, (void*)&img);

	imshow("Image", img);

	waitKey(0);
}

void getMousePos(int event, int x, int y, int, void* image)
{
	if (event != CV_EVENT_LBUTTONDOWN)
		return;

	Mat img = *((cv::Mat*)image);

	Point point = Point(x, y);
	std::cout << "X = " << point.x << "\t Y = " << point.y << "\n";

	String label;

	if(!grayscale)
	{
		Vec3b colors = img.at<Vec3b>(y, x);
		std::cout << "The color [B, G, R] values at this pixel are " << colors << "." << std::endl;
	
	}
	else if (grayscale)
	{
		int greyval = img.at<Vec3b>(y, x)[0];
		std::cout << "The grayscale intensity at this pixel is " << greyval << "." << std::endl;
	}
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