// Sherd White Digital Image Processing Assignment #3.4

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<stdio.h>

using namespace cv;

bool getChannels(Mat);
std::vector<int> equalizeGrayscale(Mat);
std::vector<int> equalizeColor(Mat);
std::string getFileExt(const std::string&);

int main(int argc, char * argv[])
{
	if (argc <= 2)
	{
		std::cerr << "Picture 1: " << argv[1] << " Picture 2:" << argv[2] << std::endl;
		return (1);
	}

	std::string path = argv[2];

	std::string ext = getFileExt(path);

	Mat img1 = cv::imread(argv[1]);
	Mat img2;
	if (ext == "jpg")
	{
		img2 = cv::imread(argv[2]);
	}

	if (img1.empty())
	{
		std::cerr << "Unable to open the first image " << argv[1] << "." << std::endl;
		return (1);
	}

	if (img2.empty() && ext == "jpg")
	{
		std::cerr << "Unable to open the control image " << argv[2] << "." << std::endl;
		return (1);

	}

	std::vector<int> control;
	int loc;

	
	if (ext == "txt")
	{
		std::ifstream fin(argv[2]);

		while (fin >> loc)
		{
			control.push_back(loc);
		}
	}
	

	namedWindow("Original Image", WINDOW_AUTOSIZE);
	imshow("Original Image", img1);

	if (ext == "jpg")
	{
		namedWindow("Control Image", WINDOW_AUTOSIZE);
		imshow("Control Image", img2);
	}

	bool grayscale1 = getChannels(img1);
	bool grayscale2 = getChannels(img2);

	std::vector<int> eqhist;
	std::vector<int> desiredeqhist;

	if (!(grayscale1) && (!(grayscale2) || ext == "txt"))
	{
		eqhist = equalizeColor(img1);
		if (ext == "jpg")
		{
			desiredeqhist = equalizeColor(img2);
		}
		else if (ext == "txt")
		{
			desiredeqhist = control;
		}

		std::vector<int> map(256);
		for (int i = 0; i < 256; i++)
		{
			for (int j = 0; j < 256; j++)
			{
				if (eqhist[i] == desiredeqhist[j])
				{
					map[i] = j;
					break;
				}
				else if ((eqhist[i] > desiredeqhist[j]) && j > 0)
				{
					map[i] = j - 1;
				}
				else if ((eqhist[i] > desiredeqhist[j]) && (j = 0))
				{
					map[i] = j;
				}
			}
			std::cout << "i: " << i << " eq: " << eqhist[i] << " des: " << desiredeqhist[i] << " map: " << map[i] << std::endl;
		}

		/// Create matched image
		int imgvalB = 0, imgvalG = 0, imgvalR = 0;

		for (int r = 0; r < img1.rows; r++){
			for (int c = 0; c < img1.cols; c++)
			{
				imgvalB = map[img1.at<Vec3b>(r, c)[0]];
				imgvalG = map[img1.at<Vec3b>(r, c)[1]];
				imgvalR = map[img1.at<Vec3b>(r, c)[2]];
				img1.at<Vec3b>(r, c)[0] = imgvalB;
				img1.at<Vec3b>(r, c)[1] = imgvalG;
				img1.at<Vec3b>(r, c)[2] = imgvalR;
			}
		} 

		namedWindow("Matched Image", WINDOW_AUTOSIZE);
		imshow("Matched Image", img1);
		
	}
	else if (grayscale1 && (grayscale2 || ext == "txt"))
	{
		eqhist = equalizeGrayscale(img1);
		if (ext == "jpg")
		{
			desiredeqhist = equalizeGrayscale(img2);
		}
		else if (ext == "txt")
		{
			desiredeqhist = control;
		}
		
		std::vector<int> map(256);
		for (int i = 0; i < 256; i++)
		{
			for (int j = 0; j < 256; j++)
			{
				if (eqhist[i] == desiredeqhist[j])
				{
					map[i] = j;
					break;
				}
				else if (eqhist[i] > desiredeqhist[j])
				{
					map[i] = j - 1;
				}
			}
			std::cout << "i: " << i << " eq: " << eqhist[i] << " des: " << desiredeqhist[i] << " map: " << map[i] << std::endl;
		}

		

		// Create matched image
		cvtColor(img1, img1, CV_RGB2GRAY);
		int imgval = 0;

		for (int r = 0; r < img1.rows; r++) {
			for (int c = 0; c < img1.cols; c++)
			{
				imgval = map[img1.at<uchar>(r, c)];
				img1.at<uchar>(r, c) = imgval;
			}
		}

		namedWindow("Matched Image", WINDOW_AUTOSIZE);
		imshow("Matched Image", img1);
	}
	else 
	{
		std::cerr << "The image channels don't match." << std::endl;
	}

	waitKey(0);

	return (0);
}

std::vector<int> equalizeGrayscale(Mat img)
{
	std::cout << "This is a grayscale image." << std::endl; 
	
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
		//std::cout << i << " " << Sk[i] << std::endl;
	}

	int Skint[256] = { 0 };
	for (int i = 0; i < 256; i++)
	{
		Skint[i] = (int)round(Sk[i]);
	}

	std::vector<int> SkVec(Skint, Skint + 256);

	return SkVec;
}

std::vector<int> equalizeColor(Mat img)
{
	std::cout << "This is a color image." << std::endl;

	Mat ycrcb;
	cvtColor(img, ycrcb, CV_BGR2YCrCb);

	int histogram[256] = { 0 };

	// calculate the number of pixels for each intensity value
	for (int r = 0; r < ycrcb.rows; r++) {
		for (int c = 0; c < ycrcb.cols; c++) {
			histogram[ycrcb.at<Vec3b>(r, c)[0]]++;
		}
	}

	// Calculate PkRk 
	double PkRk[256] = { 0 };
	for (int i = 0; i < 256; i++)
	{
		PkRk[i] = (double)histogram[i] / (ycrcb.rows*ycrcb.cols);
	}

	// Calculate Sk  
	double Sk[256] = { 0 };
	Sk[0] = (255 * PkRk[0]);
	for (int i = 1; i < 256; i++)
	{
		Sk[i] = ((255 * PkRk[i]) + Sk[i - 1]);
		//std::cout << i << " " << Sk[i] << std::endl;
	}

	int Skint[256] = { 0 };
	for (int i = 0; i < 256; i++)
	{
		Skint[i] = (int)round(Sk[i]);
	}

	std::vector<int> SkVec(Skint, Skint + 256);

	return SkVec;
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

std::string getFileExt(const std::string& s) {

	size_t i = s.rfind('.', s.length());
	if (i != std::string::npos) {
		return(s.substr(i + 1, s.length() - i));
	}

	return("");
}
