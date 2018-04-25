// Sherd White Digital Image Processing Assignment #2.3

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

	// Save image pixel values
	int intensity = 0;
	for (int r = 0; r < img1.rows; r++)
	{
		for (int c = 0; c < img1.cols; c++)
			intensity += img1.at<uchar>(r, c);
	}

	//Calculate average pixel intensity of image
	intensity = (int)round(intensity / (img1.rows*img1.cols));
	std::cerr << "\nThe Average Pixel Intensity of " << argv[1] << " is " << intensity << ".\n" << std::endl;

	// declare histogram
	int histogram[256] = { 0 };

	// calculate the number of pixels for each intensity value
	for (int r = 0; r < img1.rows; r++) {
		for (int c = 0; c < img1.cols; c++) {
			histogram[img1.at<uchar>(r, c)]++;
		}
	}

	Mat histimg = Mat::zeros(256, 256, CV_8UC1);  // create black image to draw histogram

												  // find the maximum from histogram
	int max = histogram[0];
	for (int i = 1; i < 256; i++) {
		if (max < histogram[i]) {
			max = histogram[i];
		}
	}

	// normalize the histogram between 0 and 255
	for (int i = 0; i < 256; i++) {
		histogram[i] = ((double)histogram[i] / max) * 255;
	}

	// draw the histogram using the CV line function
	for (int i = 0; i < 256; i++)
	{
		line(histimg, Point(i, 255), Point(i, 255 - histogram[i]), Scalar(255));
	}

	//calculate standard deviation
	int sum = 0;
	for (int i = 0; i < 256; i++)
	{
		sum += (histogram[i] - intensity) * (histogram[i] - intensity);
	}
	sum /= 256;
	int deviation = sqrt(sum);
	std::cerr << "Standard Deviation: " << deviation << ".\n" << std::endl;

	// Determine brightness from histogram
	int light = 0, dark = 0;
	for (int i = 1; i <= 128; i++) {
		dark += histogram[i];
	}
	for (int i = 129; i < 256; i++) {
		light += histogram[i];
	}

	//for (int i = 0; i < 256; i++) //go through all elements
	//	std::cerr << i << ':' << histogram[i] << " " << std::endl; //print indexnumber and value

	if (dark > light) {
		std::cerr << "The image brightness trends darker.\n" << std::endl;
	}
	else {
		std::cerr << "The image brightness trends lighter.\n" << std::endl;
	}

	int contrast = deviation;

	std::cerr << "The image contrast is " << contrast << "." << std::endl;

	if (contrast <= 128) {
		std::cerr << "The image contrast trends lower.\n" << std::endl;
	}
	else {
		std::cerr << "The image contrast trends higher.\n" << std::endl;
	}

	// display histogram
	namedWindow("Histogram", CV_WINDOW_AUTOSIZE);
	imshow("Histogram", histimg);

	namedWindow("Image", WINDOW_AUTOSIZE);
	imshow("Image", img1);

	waitKey(0);

	//write results to file
	char fn[80];
	sprintf(fn, "%s.asc", "results");
	FILE * fd = fopen(fn, "w");
	if (!fd)
	{
		fprintf(stderr, "Could not open file %s for writing.\n", fn);
		return (1);
	}

	fprintf(fd, "Image Ave Pixel Intensity: %d.\n\n", intensity);

	fprintf(fd, "Standard Deviation: %d.\n\n", deviation);

	if (dark > light) {
		fprintf(fd, "The image brightness trends darker.\n\n");
	}
	else {
		fprintf(fd, "The image brightness trends lighter.\n\n");
	}

	fprintf(fd, "The image contrast is %d.\n", contrast);

	if (contrast <= 128) {
		fprintf(fd, "The image contrast trends darker.\n\n");
	}
	else {
		fprintf(fd, "The image contrast trends lighter.\n\n");
	}

	return (0);
}