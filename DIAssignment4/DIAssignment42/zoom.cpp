// Sherd White Digital Image Processing Assignment #4.2

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<string>
#include<sstream>

using namespace cv;


int main(int argc, char * argv[])
{
	if (argc <= 6)
	{
		std::cerr << "Not enough arguments." << std::endl;
		return (1);
	}

	Mat img = cv::imread(argv[6], CV_8UC1);

	if (img.empty())
	{
		std::cerr << "Unable to open the image " << argv[1] << "." << std::endl;
		return (1);
	}

	std::string blockstr = argv[1];
	std::string centerrowstr = argv[2], centercolstr = argv[3], heightstr = argv[4], widthstr = argv[5], outname = argv[6];

	int height, width, centerrow, centercol, block;
	std::stringstream(centerrowstr) >> centerrow;
	std::stringstream(centercolstr) >> centercol;
	std::stringstream(heightstr) >> height;
	std::stringstream(widthstr) >> width;
	std::stringstream(blockstr) >> block;

	namedWindow("Original Image", WINDOW_AUTOSIZE);
	imshow("Original Image", img);
	
	// Start locations to iterate through
	int startr = centerrow - ((height / 2) / block);
	int startc = centercol - ((width / 2) / block);
	int maxr = centerrow + ((height / 2) / block);
	int maxc = centercol + ((width / 2) / block);

	Mat zoomimg(height, width, CV_8UC1);
	
	// Verify that we aren't trying to grab pixels outside the image
	if (centerrow - (height / 2) < 0)
		startr = 0;
	if (centerrow + (height / 2) > img.rows)
		maxr = img.rows;
	if (centercol - (width / 2) < 0)
		startc = 0;
	if (centercol + (width / 2) > img.cols)
		maxc = img.cols;

	/*// Copy selected area to temp image (zoomimg)
	int r2 = 0, c2 = 0;
	for (int r = startr; r < maxr; r++)
	{
		for (int c = startc; c < maxc; c++)
		{
			zoomimg.at<uchar>(r2, c2) = img.at<uchar>(r, c);
			c2++;
		}
		c2 = 0;
		r2++;
	}*/
	
	// Copy selected area zoomed to factor (block) to temp image (zoomimg)
	int r2 = 0, c2 = 0;
	int range = (block / 2);
	r2 = range;
	c2 = range;
	for (int r = startr; r < maxr; r++)
	{
		for (int c = startc; c < maxc; c++)
		{
			for (int i = 0; i < block; i++)
			{
				for (int j = 0; j < block; j++)
				{
					zoomimg.at<uchar>(r2 + (i - range), c2 + (j - range)) = img.at<uchar>(r, c);
				}
			}
			c2 += block;
		}
		c2 = range;
		r2+= block;
	}

	// Copy zoomed section back to the image
	int r3 = startr, c3 = startc;
	for (int r = 0; r < zoomimg.rows; r++)
	{
		for (int c = 0; c < zoomimg.cols; c++)
		{
			if (r3 < img.rows && c3 < img.cols)
			{
				img.at<uchar>(r3, c3) = zoomimg.at<uchar>(r, c);
				c3++;
			}
		}
	c3 = startc;
	r3++;
	}

	namedWindow("Zoom", WINDOW_AUTOSIZE);
	imshow("Zoom", zoomimg);

	namedWindow("Zoom in Image", WINDOW_AUTOSIZE);
	imshow("Zoom in Image", img);

	waitKey(0);

	return (0);
}