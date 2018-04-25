// Sherd White Digital Image Processing Assignment #2.1

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<string>
#include<fstream>

using namespace std;
using namespace cv;

int main(int argc, char * argv[])
{
	if (argc <= 2)
	{
		std::cerr << "Picture 1: " << argv[1] << " Picture 2:" << argv[2] << std::endl;
		return (1);
	}

	Mat img1 = cv::imread(argv[1]);
	Mat img2 = cv::imread(argv[2]);

	if (img1.empty())
	{
		std::cerr << "Unable to open the first image " << argv[1] << "." << std::endl;
		return (1);
	}

	if (img2.empty())
	{
		std::cerr << "Unable to open the second image " << argv[2] << "." << std::endl;
		return (1);
	}

	cvtColor(img1, img1, CV_BGR2GRAY);  //(input, output, Color Depth)
	cvtColor(img2, img2, CV_BGR2GRAY);  //(input, output, Color Depth)
										// Save image pixel values

	int intensity1 = 0, intensity2 = 0;
	for (int r = 0; r < img1.rows; r++)
	{
		for (int c = 0; c < img1.cols; c++)
			intensity1 += img1.at<uchar>(r, c);
	}

	for (int r = 0; r < img2.rows; r++)
	{
		for (int c = 0; c < img2.cols; c++)
			intensity2 += img2.at<uchar>(r, c);
	}

	//Calculate average pixel intensity of image1
	intensity1 = round(intensity1 / (img1.rows*img1.cols));
	intensity2 = round(intensity2 / (img2.rows*img2.cols));
	std::cerr << "The Average Pixel Intensity of " << argv[1] << " is " << intensity1 << std::endl;
	std::cerr << "The Average Pixel Intensity of " << argv[2] << " is " << intensity2 << std::endl;

	//Calculate the distance between the two images
	float distance = 0;
	if (intensity1 >= intensity2) {
		distance = intensity1 - intensity2;
	}
	else
		distance = intensity2 - intensity1;

	std::cerr << "The Average Distance between corresponding Pixel Intensity Levels is " << distance << "." << std::endl;

	namedWindow("Image 1", WINDOW_AUTOSIZE);
	imshow("Image 1", img1);

	namedWindow("Image 2", WINDOW_AUTOSIZE);
	imshow("Image 2", img2);

	waitKey(0);

	//write results to file
	char fn[80];
	sprintf(fn, "%s.asc", "results");
	FILE * fd = fopen(fn, "w");
	if (!fd)
	{
		fprintf(stderr, "Could not open file %s for writing\n", fn);
		return (1);
	}

	fprintf(fd, "Image 1 Ave Pixel Intensity: %d.\n", intensity1);
	fprintf(fd, "Image 2 Ave Pixel Intensity: %d.\n", intensity2);
	fprintf(fd, "The Average Distance between corresponding Pixel Intensity Levels is %d\n", distance);

	return (0);
}