// Sherd White Digital Image Processing Assignment #3.5

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<string>
#include<stdlib.h>     /* atoi */

using namespace cv;

bool getChannels(Mat);
void equalizeGrayscaleROI(Mat, int, int, int, int);
void equalizeColorROI(Mat, int, int, int, int);

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

	int height = 0, width = 0, x = 0, y = 0;

	std::cout << "What is the starting X value of the desired ROI:\n>";
	std::cin >> x;

	std::cout << "What is the starting Y value of the desired ROI:\n>";
	std::cin >> y;

	std::cout << "What is the height of the desired ROI:\n>";
	std::cin >> height;

	std::cout << "What is the width of the desired ROI:\n>";
	std::cin >> width;

	namedWindow("Original Image", WINDOW_AUTOSIZE);
	imshow("Original Image", img);

	bool grayscale = getChannels(img);

	if (!grayscale)
	{
		equalizeColorROI(img, x, y, height, width);

	}
	else if (grayscale)
	{
		std::cout << "This is a grayscale image." << std::endl;
		equalizeGrayscaleROI(img, x, y, height, width);
	}

	waitKey(0);

	return (0);
}

void equalizeGrayscaleROI(Mat img, int x, int y, int height, int width)
{
	Mat grayimg;
	cvtColor(img, grayimg, CV_BGR2GRAY);  //(input, output, Color Depth)

										  // declare histogram
	int histogram[256] = { 0 };

	// calculate the number of pixels for each intensity value
	for (int r = 0; r < grayimg.rows; r++) {
		for (int c = 0; c < grayimg.cols; c++) {
			histogram[grayimg.at<uchar>(r, c)]++;
		}
	}

	// Calculate PkRk 
	double PkRk[256] = { 0 };
	for (int i = 0; i < 256; i++)
	{
		PkRk[i] = (double)histogram[i] / (grayimg.rows*grayimg.cols);
	}

	// Calculate Sk  
	double Sk[256] = { 0 };
	Sk[0] = (255 * PkRk[0]);
	for (int i = 1; i < 256; i++)
	{
		Sk[i] = ((255 * PkRk[i]) + Sk[i - 1]);
		//std::cout << Sk[i] << std::endl;
	}

	// Equalize selected area
	int imgval = 0;

	if (x > grayimg.cols)
		x = 0;
	if (y > grayimg.rows)
		y = 0;
	if (width > grayimg.cols - x)
		width = grayimg.cols - x;
	if (height > grayimg.rows - y)
		height = grayimg.rows - y;

	for (int r = y; r < (y + height); r++)
		for (int c = x; c < (x + width); c++)
		{
			imgval = grayimg.at<uchar>(r, c);
			grayimg.at<uchar>(r, c) = (int)round(Sk[imgval]);
		}

	namedWindow("ROI Equalized Grayscale Image", WINDOW_AUTOSIZE);
	imshow("ROI Equalized Grayscale Image", grayimg);
}

void equalizeColorROI(Mat colorimg, int x, int y, int height, int width)
{
	std::cout << "This is a color image." << std::endl;

	// declare histogram
	int histogramB[256] = { 0 }, histogramG[256] = { 0 }, histogramR[256] = { 0 };

	// calculate the number of pixels for each Color Value
	for (int r = 0; r < colorimg.rows; r++) {
		for (int c = 0; c < colorimg.cols; c++) {
			histogramB[colorimg.at<Vec3b>(r, c)[0]]++;
			histogramG[colorimg.at<Vec3b>(r, c)[1]]++;
			histogramR[colorimg.at<Vec3b>(r, c)[2]]++;
		}
	}

	// Calculate PkRk 
	double PkRkB[256] = { 0 }, PkRkG[256] = { 0 }, PkRkR[256] = { 0 };

	for (int i = 0; i < 256; i++)
	{
		PkRkB[i] = (double)histogramB[i] / (colorimg.rows*colorimg.cols);
		PkRkG[i] = (double)histogramG[i] / (colorimg.rows*colorimg.cols);
		PkRkR[i] = (double)histogramR[i] / (colorimg.rows*colorimg.cols);
	}

	// Calculate Sk 
	double SkB[256] = { 0 }, SkG[256] = { 0 }, SkR[256] = { 0 };

	SkB[0] = (255 * PkRkB[0]);
	SkG[0] = (255 * PkRkG[0]);
	SkR[0] = (255 * PkRkR[0]);
	for (int i = 1; i < 256; i++)
	{
		SkB[i] = ((255 * PkRkB[i]) + SkB[i - 1]);
		SkG[i] = ((255 * PkRkG[i]) + SkG[i - 1]);
		SkR[i] = ((255 * PkRkR[i]) + SkR[i - 1]);
	}

	// Create equalized image
	int imgvalB = 0, imgvalG = 0, imgvalR = 0;

	if (x > colorimg.cols)
		x = 0;
	if (y > colorimg.rows)
		y = 0;
	if (width > colorimg.cols - x)
		width = colorimg.cols - x;
	if (height > colorimg.rows - y)
		height = colorimg.rows - y;

	for (int r = y; r < (y + height); r++)
		for (int c = x; c < (x + width); c++)
		{
			imgvalB = colorimg.at<Vec3b>(r, c)[0];
			imgvalG = colorimg.at<Vec3b>(r, c)[1];
			imgvalR = colorimg.at<Vec3b>(r, c)[2];
			colorimg.at<Vec3b>(r, c)[0] = (int)round(SkB[imgvalB]);
			colorimg.at<Vec3b>(r, c)[1] = (int)round(SkG[imgvalG]);
			colorimg.at<Vec3b>(r, c)[2] = (int)round(SkR[imgvalR]);
		}

	namedWindow("ROI Equalized Color Image", WINDOW_AUTOSIZE);
	imshow("ROI Equalized Color Image", colorimg);
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