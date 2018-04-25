// Sherd White Digital Image Processing Assignment #3.3

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<string>

using namespace cv;

bool getChannels(Mat);
void equalizeGrayscale(Mat);
void equalizeColor(Mat);

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

	if (!grayscale)
	{
		equalizeColor(img);

	}
	else if (grayscale)
	{
		std::cout << "This is a grayscale image." << std::endl;
		equalizeGrayscale(img);
	}

	waitKey(0);

	return (0);
}

void equalizeGrayscale(Mat img)
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

	Mat histimg = Mat::zeros(256, 256, CV_8UC1);  // create black image to draw histogram

	int max = histogram[0];
	for (int i = 1; i < 256; i++) {
		if (max < histogram[i]) {
			max = histogram[i];
		}
	}

	// normalize the histogram between 0 and 255
	int scaledhistogram[256] = { 0 };
	for (int i = 0; i < 256; i++) {
		scaledhistogram[i] = (int)round(((double)histogram[i] / max) * 255);
	}

	// draw the histogram using the CV line function
	for (int i = 0; i < 256; i++)
	{
		line(histimg, Point(i, 255), Point(i, 255 - scaledhistogram[i]), Scalar(255));
	}

	namedWindow("Original Grayscale Histogram", WINDOW_AUTOSIZE);
	imshow("Original Grayscale Histogram", histimg);

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

	// Create equalized image
	Mat eqimg(grayimg.rows, grayimg.cols, CV_8UC1);
	int imgval = 0;

	for (int r = 0; r < grayimg.rows; r++)
		for (int c = 0; c < grayimg.cols; c++)
		{
			imgval = grayimg.at<uchar>(r, c);
			eqimg.at<uchar>(r, c) = (int)round(Sk[imgval]);
		}

	// declare histogram
	int newhistogram[256] = { 0 };

	// calculate the number of pixels for each intensity value
	for (int r = 0; r < grayimg.rows; r++) {
		for (int c = 0; c < grayimg.cols; c++) {
			newhistogram[eqimg.at<uchar>(r, c)]++;
		}
	}

	Mat newhistimg = Mat::zeros(256, 256, CV_8UC1);  // create black image to draw histogram

	int newmax = newhistogram[0];
	for (int i = 1; i < 256; i++) {
		if (newmax < newhistogram[i]) {
			newmax = newhistogram[i];
		}
	}

	// normalize the histogram between 0 and 255
	int newscaledhistogram[256] = { 0 };
	for (int i = 0; i < 256; i++) {
		newscaledhistogram[i] = (int)round(((double)newhistogram[i] / newmax) * 255);
	}

	// draw the equalized histogram using the CV line function
	for (int i = 0; i < 256; i++)
	{
		line(newhistimg, Point(i, 255), Point(i, 255 - newscaledhistogram[i]), Scalar(255));
	}

	namedWindow("Equalized Grayscale Image", WINDOW_AUTOSIZE);
	imshow("Equalized Grayscale Image", eqimg);

	namedWindow("Equalized Grayscale Histogram", WINDOW_AUTOSIZE);
	imshow("Equalized Grayscale Histogram", newhistimg);
}

void equalizeColor(Mat colorimg)
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
	Mat eqcolorimg = Mat::zeros(colorimg.rows, colorimg.cols, CV_8UC3);
	int imgvalB = 0, imgvalG = 0, imgvalR = 0;

	for (int r = 0; r < colorimg.rows; r++)
		for (int c = 0; c < colorimg.cols; c++)
		{
			imgvalB = colorimg.at<Vec3b>(r, c)[0];
			imgvalG = colorimg.at<Vec3b>(r, c)[1];
			imgvalR = colorimg.at<Vec3b>(r, c)[2];
			eqcolorimg.at<Vec3b>(r, c)[0] = (int)round(SkB[imgvalB]);
			eqcolorimg.at<Vec3b>(r, c)[1] = (int)round(SkG[imgvalG]);
			eqcolorimg.at<Vec3b>(r, c)[2] = (int)round(SkR[imgvalR]);
		}

	namedWindow("Equalized Color Image", WINDOW_AUTOSIZE);
	imshow("Equalized Color Image", eqcolorimg);

	equalizeGrayscale(colorimg);
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
