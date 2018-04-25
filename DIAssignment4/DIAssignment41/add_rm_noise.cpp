// Sherd White Digital Image Processing Assignment #4.1

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<string>
#include<fstream>
#include<time.h>       /* time */

using namespace cv;

int main(int argc, char * argv[])
{
	if (argc <= 1)
	{
		std::cerr << "Picture 1: " << argv[1] << " Percentage: " << argv[2] << std::endl;
		return (1);
	}

	Mat img = cv::imread(argv[1], CV_8UC1);
	std::vector<int> sum(img.rows*img.cols);
	Mat addedimg = Mat::zeros(img.rows, img.cols, CV_8UC1);

	if (img.empty())
	{
		std::cerr << "Unable to open the first image " << argv[1] << "." << std::endl;
		return (1);
	}


	namedWindow("Original Image", WINDOW_AUTOSIZE);
	imshow("Original Image", img);

	int pixel = 0, counter = 0, randr = 0, randc = 0, randbit = 0;
	double max = 0, percentage = 0;
	int imgsize = img.rows * img.cols;
	

	for (int x = 1; x <= 10; x++)
	{
		counter = 0, max = 0;
		std::cout << "Enter percentage of noise to generate (i.e. 10 for 10%):\n>";
		std::cin >> percentage;

		Mat noiseimg = img.clone();

		max = round(8 * (imgsize) * (percentage / 100));
		//std::cout << "max: " << max << std::endl;
		randr = rand() % img.rows;  //random row
		randc = rand() % img.cols; //random col

		for(int i = 0; i <= max; i++)
		{
			pixel = img.at<uchar>(randr, randc);
			//std::cout << "before: " << pixel << " ";
			randbit = rand() % (7 + 1);  //random bit
			pixel |= (1u << randbit);    // flip random bit
			//std::cout << "after: " << pixel << std::endl;
			noiseimg.at<uchar>(randr, randc) = pixel;
			randr = rand() % img.rows;  //random row
			randc = rand() % img.cols; //random col
		}
		
		namedWindow(format("noiseimg%d.jpg", x), WINDOW_AUTOSIZE);
		imshow(format("noiseimg%d.jpg", x), noiseimg);
		imwrite(format("noiseimg%d.jpg", x), noiseimg);

		int count = 0;

		for (int r = 0; r < noiseimg.rows; r++)
		{
			for (int c = 0; c < noiseimg.cols; c++)
			{
				sum[count] += (int)noiseimg.at<uchar>(r, c);
				count++;
			}
		}
	}

	for (int i = 0; i < imgsize; i++)
	{
		sum[i] /= 10;
	}

	int count = 0;
	// create added image
	for (int r = 0; r < img.rows; r++)
	{
		for (int c = 0; c < img.cols; c++) 
		{
			addedimg.at<uchar>(r, c) = sum[count];
			count++;
		}
			
	}

	namedWindow("Added Image", WINDOW_AUTOSIZE);
	imshow("Added Image", addedimg);

	int intensity1 = 0, intensity2 = 0;
	for (int r = 0; r < img.rows; r++)
	{
		for (int c = 0; c < img.cols; c++)
			intensity1 += (int)img.at<uchar>(r, c);
	}

	for (int i = 0; i < imgsize; i++)
	{
		intensity2 += sum[i];
	}

	//Calculate average pixel intensity of image1
	intensity1 = (int)round(intensity1 / (imgsize));
	intensity2 = (int)round(intensity2 / (imgsize));
	std::cerr << "The Average Pixel Intensity of the original image is " << intensity1 << std::endl;
	std::cerr << "The Average Pixel Intensity of the summed noise images is " << intensity2 << std::endl;

	//Calculate the distance between the two images
	float distance = 0;
	if (intensity1 >= intensity2) {
		distance = intensity1 - intensity2;
	}
	else
		distance = intensity2 - intensity1;

	std::cerr << "The Average Distance between corresponding Pixel Intensity Levels is " << distance << "." << std::endl;

	waitKey(0);
}
