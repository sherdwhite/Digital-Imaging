// Sherd White Digital Imaging Assignment #1

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<string>
#include<fstream>
#include<vector>

using namespace cv;
Mat haarWavelet(Mat, int);
Mat invHaarWavelet(Mat, int);
Mat daubechiesWavelet(Mat, int);
Mat invDaubechiesWavelet(Mat, int);

int main(int argc, char * argv[])
{

	if (argc <= 7)
	{
		std::cerr << "Not enough arguments." << std::endl;
		return (1);
	}
	
	std::string wstr = argv[1], typestr = argv[2], analzorsynthstr = argv[3], dstr = argv[4], depthstr = argv[5], outnamestr = argv[7];

	int type, depth;
	std::stringstream(typestr) >> type;
	std::stringstream(depthstr) >> depth;

	if (depth < 1)
		depth = 1;


	// Haar Analyzed
	if (type == 0 && analzorsynthstr == "-a") 
	{
		Mat img = cv::imread(argv[6], CV_LOAD_IMAGE_GRAYSCALE); // CV_LOAD_IMAGE_ANYCOLOR);

		namedWindow("Original Image", WINDOW_AUTOSIZE);
		imshow("Original Image", img);

		cv::Mat filterimg(img.rows, img.cols, img.type());
		img.copyTo(filterimg);

		if (img.empty())
		{
			std::cerr << "Unable to open the image " << argv[1] << "." << std::endl;
			return (1);
		}

		if ((img.rows % 2) != 0) 
		{
			img.rows += 1;
			for (int r = (img.rows - 1); r < img.rows; r++)
			{
				for (int c = 0; c < img.cols; c++)
				{
					img.at<uchar>(r, c) = 0;
				}
			}
		}
		if ((img.cols % 2) != 0) 
		{
			img.cols += 1;
			for (int r = 0; r < img.rows; r++)
			{
				for (int c = (img.cols - 1); c < img.cols; c++)
				{
					img.at<uchar>(r, c) = 0;
				}
			}
		}

		std::string valstr;
		int curdepth = 1;
		for (int i = 1; i <= depth; i++) {
			//std::cout << curdepth << std::endl;
			filterimg = haarWavelet(filterimg, curdepth);
			curdepth *= 2;
		}

		outnamestr = outnamestr + ".wl";
		std::ofstream file(outnamestr.c_str());
		//std::cout << outnamestr << std::endl;
		file << filterimg.cols << std::endl;
		file << filterimg.rows << std::endl;
		file << depth << std::endl;
		int value; 
		for (int r = 0; r < filterimg.rows; r++) 
		{
			for (int c = 0; c < filterimg.cols; c++) 
			{
				value = int(filterimg.at<uchar>(r, c));
				file << value << std::endl;
			}
		}
		file.close();

		namedWindow("Haar Analysis Image", WINDOW_AUTOSIZE);
		imshow("Haar Analysis Image", filterimg);
	}


	// Haar Synthesized
	if (type == 0 && analzorsynthstr == "-s") 
	{
		std::vector<int> list;
		int loc;
		std::ifstream fin(argv[6]);
		while (fin >> loc)
		{
			list.push_back(loc);
		}
		int width, height;
		width = int(list[0]);
		height = int(list[1]);
		depth = int(list[2]);
		int i = 3;
		cv::Mat filterimg(height, width, CV_8UC1);
		for (int r = 0; r < filterimg.rows; r++) 
		{
			for (int c = 0; c < filterimg.cols; c++) 
			{
				filterimg.at<uchar>(r, c) = list[i];
				i++;
			}
		}

		if ((filterimg.rows % 2) != 0) 
		{
			filterimg.rows += 1;
			for (int r = (filterimg.rows - 1); r < filterimg.rows; r++)
			{
				for (int c = 0; c < filterimg.cols; c++)
				{
					filterimg.at<uchar>(r, c) = 0;
				}
			}
		}
		if ((filterimg.cols % 2) != 0) 
		{
			filterimg.cols += 1;
			for (int r = 0; r < filterimg.rows; r++)
			{
				for (int c = (filterimg.cols - 1); c < filterimg.cols; c++)
				{
					filterimg.at<uchar>(r, c) = 0;
				}
			}
		}

		namedWindow("Haar Analyzed Image", WINDOW_AUTOSIZE);
		imshow("Haar Analyzed Image", filterimg);


		for (int i = depth; i >= 1; i--) {
			int curdepth = i;
			if (i > 2 && (i % 2) != 0)
				curdepth = ((i - 1) * 2);
			else if (i > 2 && (i % 2) == 0)
				curdepth = i * 2;
			//std::cout << curdepth << std::endl;
			filterimg = invHaarWavelet(filterimg, curdepth);
		}

		namedWindow("Haar Restored Image", WINDOW_AUTOSIZE);
		imshow("Haar Restored Image", filterimg);
		imwrite(outnamestr, filterimg);
	}

	
	// Daubechies Analyzed
	if (type == 1 && analzorsynthstr == "-a")
	{
		Mat img = cv::imread(argv[6], CV_LOAD_IMAGE_GRAYSCALE); // CV_LOAD_IMAGE_ANYCOLOR);

		namedWindow("Original Image", WINDOW_AUTOSIZE);
		imshow("Original Image", img);

		cv::Mat filterimg(img.rows, img.cols, img.type());
		img.copyTo(filterimg);

		if (img.empty())
		{
			std::cerr << "Unable to open the image " << argv[1] << "." << std::endl;
			return (1);
		}

		if ((img.rows % 2) != 0)
		{
			img.rows += 1;
			for (int r = (img.rows - 1); r < img.rows; r++)
			{
				for (int c = 0; c < img.cols; c++)
				{
					img.at<uchar>(r, c) = 0;
				}
			}
		}
		if ((img.cols % 2) != 0)
		{
			img.cols += 1;
			for (int r = 0; r < img.rows; r++)
			{
				for (int c = (img.cols - 1); c < img.cols; c++)
				{
					img.at<uchar>(r, c) = 0;
				}
			}
		}

		int curdepth = 1;
		for (int i = 1; i <= depth; i++) {
			//std::cout << curdepth << std::endl;
			filterimg = daubechiesWavelet(filterimg, curdepth);
			curdepth *= 2;
		}

		outnamestr = outnamestr + ".wl";
		std::ofstream file(outnamestr.c_str());
		//std::cout << outnamestr << std::endl;
		file << filterimg.cols << std::endl;
		file << filterimg.rows << std::endl;
		file << depth << std::endl;
		int value;
		for (int r = 0; r < filterimg.rows; r++)
		{
			for (int c = 0; c < filterimg.cols; c++)
			{
				value = int(filterimg.at<uchar>(r, c));
				file << value << std::endl;
			}
		}
		file.close();

		namedWindow("Daubachies Analysis Image", WINDOW_AUTOSIZE);
		imshow("Daubachies Analysis Image", filterimg);
	}


	// Daubechies Synthesized
	if (type == 1 && analzorsynthstr == "-s")
	{
		std::vector<int> list;
		int loc;
		std::ifstream fin(argv[6]);
		while (fin >> loc)
		{
			list.push_back(loc);
		}
		int width, height;
		width = int(list[0]);
		height = int(list[1]);
		depth = int(list[2]);
		int i = 3;
		cv::Mat filterimg(height, width, CV_8UC1);
		for (int r = 0; r < filterimg.rows; r++)
		{
			for (int c = 0; c < filterimg.cols; c++)
			{
				filterimg.at<uchar>(r, c) = list[i];
				i++;
			}
		}

		if ((filterimg.rows % 2) != 0)
		{
			filterimg.rows += 1;
			for (int r = (filterimg.rows - 1); r < filterimg.rows; r++)
			{
				for (int c = 0; c < filterimg.cols; c++)
				{
					filterimg.at<uchar>(r, c) = 0;
				}
			}
		}
		if ((filterimg.cols % 2) != 0)
		{
			filterimg.cols += 1;
			for (int r = 0; r < filterimg.rows; r++)
			{
				for (int c = (filterimg.cols - 1); c < filterimg.cols; c++)
				{
					filterimg.at<uchar>(r, c) = 0;
				}
			}
		}

		namedWindow("Daubechies Analyzed Image", WINDOW_AUTOSIZE);
		imshow("Daubechies Analyzed Image", filterimg);

		filterimg = invDaubechiesWavelet(filterimg, depth);

		namedWindow("Daubechies Restored Image", WINDOW_AUTOSIZE);
		imshow("Daubechies Restored Image", filterimg);
		imwrite(outnamestr, filterimg);
	}

	cv::waitKey(0);

	return (0);
}

Mat haarWavelet(Mat img, int depth){	
	cv::Mat firstpass(img.rows, img.cols, img.type());
	cv::Mat newimg(img.rows, img.cols, img.type());
	img.copyTo(newimg);

	int c2;
	for (int r = 0; r < (img.rows / depth); r++)
	{
		c2 = 0;
		for (int c = 0; c < (img.cols / depth); c+=2)
		{
			firstpass.at<uchar>(r, c2) = (img.at<uchar>(r, c) + img.at<uchar>(r, c + 1)) / 2;
			firstpass.at<uchar>(r, (c2 + (img.cols / (2 * depth)))) = (img.at<uchar>(r, c) - img.at<uchar>(r, c + 1)) / 2;
			c2++;
		}
	} 

	int r2;
	for (int c = 0; c < (img.cols / depth); c++)
	{
		r2 = 0;
		for (int r = 0; r < (img.rows / depth); r+=2) 
		{
			newimg.at<uchar>(r2, c) = (firstpass.at<uchar>(r, c) + firstpass.at<uchar>(r + 1, c)) / 2;
			newimg.at<uchar>(r2 + (firstpass.rows / (2 * depth)), c) = (firstpass.at<uchar>(r, c) - firstpass.at<uchar>(r + 1, c)) / 2;
			r2++;
		}
	}

	return newimg;
}

Mat invHaarWavelet(Mat img, int depth){
	std::cout << depth << std::endl;
	cv::Mat firstpass(img.rows, img.cols, img.type());
	cv::Mat newimg(img.rows, img.cols, img.type());
	img.copyTo(firstpass);

	for (int c = 0; c < (img.cols / (depth)); c++)
	{
		for (int r = 0; r < (img.rows / (2 * depth)); r+=2)
		{
			firstpass.at<uchar>(r, c) = (img.at<uchar>(r, c) + img.at<uchar>((r + (img.rows / (depth * 2))), c));  
			firstpass.at<uchar>(r + 1, c) = (img.at<uchar>(r, c) - img.at<uchar>(r + (img.rows / (depth * 2)), c));
			firstpass.at<uchar>((r + (img.rows / (depth * 2))), c) = (img.at<uchar>(r + 1, c) + img.at<uchar>((r + 1 + (img.rows / (depth * 2))), c));
			firstpass.at<uchar>((r + 1 + (img.rows / (depth * 2))), c) = (img.at<uchar>(r + 1, c) - img.at<uchar>((r + 1 + (img.rows / (depth * 2))), c));
		}
	}

	firstpass.copyTo(newimg);
	for (int r = 0; r < (img.rows / depth); r++)
	{
		for (int c = 0; c < (img.cols / (2 * depth)); c+=2)
		{
			newimg.at<uchar>(r, c) = (firstpass.at<uchar>(r, c) + firstpass.at<uchar>(r, (c + (firstpass.cols / (depth * 2)))));
			newimg.at<uchar>(r, c + 1) = (firstpass.at<uchar>(r, c) - firstpass.at<uchar>(r, (c + (firstpass.cols / (depth * 2)))));
			newimg.at<uchar>(r, (c + (firstpass.cols / (depth * 2)))) = (firstpass.at<uchar>(r, c + 1) + firstpass.at<uchar>(r, (c + 1 + (firstpass.cols / (depth * 2)))));
			newimg.at<uchar>(r, (c + 1 + (firstpass.cols / (depth * 2)))) = (firstpass.at<uchar>(r, c + 1) - firstpass.at<uchar>(r, (c + 1 + (firstpass.cols / (depth * 2)))));
		}
	}

	return newimg;
}

Mat daubechiesWavelet(Mat img, int depth) {
	// forward transform scaling coefficients
	double hneg4 = 0.026748757411, hneg3 = -0.016864118443, hneg2 = -0.078223266529, hneg1 = 0.266864118443,
		h0 = 0.602949018236, h1 = 0.266864118443, h2 = -0.078223266529, h3 = -0.016864118443, h4 = 0.026748757411;
	// forward transform wave coefficients
	double gneg4 = 0, gneg3 = 0.091271763114, gneg2 = -0.057543526229, gneg1 = -0.591271763114,
		g0 = 1.11508705, g1 = -0.591271763114, g2 = -0.057543526229, g3 = 0.091271763114, g4 = 0;

	cv::Mat firstpass(img.rows, img.cols, img.type());
	cv::Mat hhimg(img.rows, img.cols, img.type());
	cv::Mat hgimg(img.rows, img.cols, img.type());
	cv::Mat ghimg(img.rows, img.cols, img.type());
	cv::Mat ggimg(img.rows, img.cols, img.type());
	cv::Mat newimg(img.rows, img.cols, img.type());

	img.copyTo(newimg);

	
	for (int r = 0; r < (img.rows / depth); r++)
	{
		for (int c = 0; c < (img.cols / depth); c++)
		{
			firstpass.at<uchar>(r, c) = img.at<uchar>(r, c);
		}
	}

	// HH
	for (int r = 4; r < (img.rows / depth) - 4; r++)
	{
		for (int c = 4; c < (img.cols / depth) - 4; c++)
		{
			firstpass.at<uchar>(r, c) = (hneg4 * (int)img.at<uchar>(r, c - 4)) + (hneg3 * (int)img.at<uchar>(r, c - 3)) + (hneg2 * (int)img.at<uchar>(r, c - 2))
				+ (hneg1 * (int)img.at<uchar>(r, c - 1)) + (h0 * (int)img.at<uchar>(r, c)) + (h1 * (int)img.at<uchar>(r, c + 1)) + (h2 * (int)img.at<uchar>(r, c + 2)) +
				(h3 * (int)img.at<uchar>(r, c + 3)) + (h4 * (int)img.at<uchar>(r, c + 4));
		}
	}

	for (int c = 0; c < (img.cols / depth); c++)
	{
		for (int r = 0; r < (img.rows / depth); r++)
		{
			hhimg.at<uchar>(r, c) = firstpass.at<uchar>(r, c);
		}
	}

	for (int c = 4; c < (img.cols / depth) - 4; c++)
	{
		for (int r = 4; r < (img.rows / depth) - 4; r++)
		{
			hhimg.at<uchar>(r, c) = (hneg4 * (int)firstpass.at<uchar>(r - 4, c)) + (hneg3 * (int)firstpass.at<uchar>(r - 3, c)) + (hneg2 * (int)firstpass.at<uchar>(r - 2, c))
				+ (hneg1 * (int)firstpass.at<uchar>(r - 1, c)) + (h0 * (int)firstpass.at<uchar>(r, c)) + (h1 * (int)firstpass.at<uchar>(r + 1, c)) + (h2 * (int)firstpass.at<uchar>(r + 2, c)) +
				(h3 * (int)firstpass.at<uchar>(r + 3, c)) + (h4 * (int)firstpass.at<uchar>(r + 4, c));
		}
	}

	int r2 = 0, c2 = 0;
	for (int r = 0; r < (img.rows / depth) / 2; r++)
	{
		c2 = 0;
		for (int c = 0; c < (img.cols / depth) / 2; c++)
		{
			newimg.at<uchar>(r, c) = hhimg.at<uchar>(r2, c2);
			c2 += 2;
		}
		if (r2 < img.rows)
			r2 += 2;
		else
			r2 = 0;
	}

	// HG
	for (int r = 0; r < (img.rows / depth); r++)
	{
		for (int c = 0; c < (img.cols / depth); c++)
		{
			firstpass.at<uchar>(r, c) = img.at<uchar>(r, c);
		}
	}

	for (int r = 4; r < (img.rows / depth) - 4; r++)
	{
		for (int c = 4 ; c < (img.cols / depth) - 4; c++)
		{
			firstpass.at<uchar>(r, c) = (hneg4 * (int)img.at<uchar>(r, c - 4)) + (hneg3 * (int)img.at<uchar>(r, c - 3)) + (hneg2 * (int)img.at<uchar>(r, c - 2))
				+ (hneg1 * (int)img.at<uchar>(r, c - 1)) + (h0 * (int)img.at<uchar>(r, c)) + (h1 * (int)img.at<uchar>(r, c + 1)) + (h2 * (int)img.at<uchar>(r, c + 2)) +
				(h3 * (int)img.at<uchar>(r, c + 3)) + (h4 * (int)img.at<uchar>(r, c + 4));
		}
	}

	for (int c = 0; c < (img.cols / depth); c++)
	{
		for (int r = 0; r < (img.rows / depth); r++)
		{
			hgimg.at<uchar>(r, c) = firstpass.at<uchar>(r, c);
		}
	}

	for (int c = 4; c < (img.cols / depth) - 4; c++)
	{
		for (int r = 4; r < (img.rows / depth) - 4; r++)
		{
			hgimg.at<uchar>(r, c) = (gneg4 * (int)firstpass.at<uchar>(r - 4, c)) + (gneg3 * (int)firstpass.at<uchar>(r - 3, c)) + (gneg2 * (int)firstpass.at<uchar>(r - 2, c))
				+ (gneg1 * (int)firstpass.at<uchar>(r - 1, c)) + (g0 * (int)firstpass.at<uchar>(r, c)) + (g1 * (int)firstpass.at<uchar>(r + 1, c)) + (g2 * (int)firstpass.at<uchar>(r + 2, c)) +
				(g3 * (int)firstpass.at<uchar>(r + 3, c)) + (g4 * (int)firstpass.at<uchar>(r + 4, c));
		}
	}

	r2 = 0;
	for (int r = 0; r < (img.rows / depth) / 2; r++)
	{
		c2 = 0;
		for (int c = (img.cols / depth) / 2; c < (img.cols / depth); c++)
		{
			newimg.at<uchar>(r, c) = hgimg.at<uchar>(r2, c2);
			c2 += 2;
		}
		if (r2 < img.rows)
			r2 += 2;
		else
			r2 = 0;
	}

	// GH
	for (int r = 0; r < (img.rows / depth); r++)
	{
		for (int c = 0; c < (img.cols / depth); c++)
		{
			firstpass.at<uchar>(r, c) = img.at<uchar>(r, c);
		}
	}

	for (int r = 4; r < (img.rows / depth) - 4; r++)
	{
		for (int c = 4; c < (img.cols / depth) - 4; c++)
		{
			firstpass.at<uchar>(r, c) = (gneg4 * (int)img.at<uchar>(r - 4, c)) + (gneg3 * (int)img.at<uchar>(r - 3, c)) + (gneg2 * (int)img.at<uchar>(r - 2, c))
				+ (gneg1 * (int)img.at<uchar>(r - 1, c)) + (g0 * (int)img.at<uchar>(r, c)) + (g1 * (int)img.at<uchar>(r + 1, c)) + (g2 * (int)img.at<uchar>(r + 2, c)) +
				(g3 * (int)img.at<uchar>(r + 3, c)) + (g4 * (int)img.at<uchar>(r + 4, c));
		}
	}

	for (int c = 0; c < (img.cols / depth); c++)
	{
		for (int r = 0; r < (img.rows / depth); r++)
		{
			ghimg.at<uchar>(r, c) = (r, c);
		}
	}

	for (int c = 4; c < (img.cols / depth) - 4; c++)
	{
		for (int r = 4; r < (img.rows / depth) - 4; r++)
		{
			ghimg.at<uchar>(r, c) = (hneg4 * (int)firstpass.at<uchar>(r, c - 4)) + (hneg3 * (int)firstpass.at<uchar>(r, c - 3)) + (hneg2 * (int)firstpass.at<uchar>(r, c - 2))
				+ (hneg1 * (int)firstpass.at<uchar>(r, c - 1)) + (h0 * (int)firstpass.at<uchar>(r, c)) + (h1 * (int)firstpass.at<uchar>(r, c + 1)) + (h2 * (int)firstpass.at<uchar>(r, c + 2)) +
				(h3 * (int)firstpass.at<uchar>(r, c + 3)) + (h4 * (int)firstpass.at<uchar>(r, c + 4));
		}
	}

	r2 = 0;
	for (int r = (img.rows / depth) / 2; r < (img.rows / depth); r++)
	{
		c2 = 0;
		for (int c = 0; c < (img.cols / depth) / 2; c++)
		{
			newimg.at<uchar>(r, c) = ghimg.at<uchar>(r2, c2);
			c2 += 2;
		}
		if (r2 < img.rows)
			r2 += 2;
		else
			r2 = 0;
	}

	// GG
	for (int r = 0; r < (img.rows / depth); r++)
	{
		for (int c = 0; c < (img.cols / depth); c++)
		{
			firstpass.at<uchar>(r, c) = img.at<uchar>(r, c);
		}
	}

	for (int r = 4; r < (img.rows / depth) - 4; r++)
	{
		for (int c = 4; c < (img.cols / depth) - 4; c++)
		{
			firstpass.at<uchar>(r, c) = (gneg4 * (int)img.at<uchar>(r - 4, c)) + (gneg3 * (int)img.at<uchar>(r - 3, c)) + (gneg2 * (int)img.at<uchar>(r - 2, c))
				+ (gneg1 * (int)img.at<uchar>(r - 1, c)) + (g0 * (int)img.at<uchar>(r, c)) + (g1 * (int)img.at<uchar>(r + 1, c)) + (g2 * (int)img.at<uchar>(r + 2, c)) +
				(g3 * (int)img.at<uchar>(r + 3, c)) + (g4 * (int)img.at<uchar>(r + 4, c));
		}
	}

	for (int c = 0; c < (img.cols / depth); c++)
	{
		for (int r = 0; r < (img.rows / depth); r++)
		{
			ggimg.at<uchar>(r, c) = firstpass.at<uchar>(r, c);
		}
	}

	for (int c = 4; c < (img.cols / depth) - 4; c++)
	{
		for (int r = 4; r < (img.rows / depth) - 4; r++)
		{
			ggimg.at<uchar>(r, c) = (gneg4 * (int)firstpass.at<uchar>(r - 4, c)) + (gneg3 * (int)firstpass.at<uchar>(r - 3, c)) + (gneg2 * (int)firstpass.at<uchar>(r - 2, c))
				+ (gneg1 * (int)firstpass.at<uchar>(r - 1, c)) + (g0 * (int)firstpass.at<uchar>(r, c)) + (g1 * (int)firstpass.at<uchar>(r + 1, c)) + (g2 * (int)firstpass.at<uchar>(r + 2, c)) +
				(g3 * (int)firstpass.at<uchar>(r + 3, c)) + (g4 * (int)firstpass.at<uchar>(r + 4, c));
		}
	}

	r2 = 0;
	for (int r = (img.rows / depth) / 2; r < (img.rows / depth); r++)
	{
		c2 = 0;
		for (int c = (img.cols / depth) / 2; c < (img.cols / depth); c++)
		{
			newimg.at<uchar>(r, c) = ggimg.at<uchar>(r2, c2);
			c2 += 2;
		}
		if (r2 < img.rows)
			r2 += 2;
		else
			r2 = 0;
	}

	return newimg;
}

Mat invDaubechiesWavelet(Mat img, int depth){

	// inverse transform scaling coefficients
	double ihneg4 = 0, ihneg3 = -0.091271763114, ihneg2 = -0.057543526229, ihneg1 = 0.591271763114,
		ih0 = 1.11508705, ih1 = 0.591271763114, ih2 = -0.057543526229, ih3 = -0.091271763114, ih4 = 0;
	// inverse transform wave coefficients
	double igneg4 = 0.026748757411, igneg3 = -0.016864118443, igneg2 = -0.078223266529, igneg = -0.266864118443,
		ig0 = 0.602949018236, ig1 = -0.266864118443, ig2 = -0.078223266529, ig3 = -0.016864118443, ig4 = 0.026748757411;
	cv::Mat firstpass(img.rows, img.cols, img.type());
	cv::Mat hhimg(img.rows, img.cols, img.type());
	cv::Mat hgimg(img.rows, img.cols, img.type());
	cv::Mat ghimg(img.rows, img.cols, img.type());
	cv::Mat ggimg(img.rows, img.cols, img.type());
	cv::Mat newimg(img.rows, img.cols, img.type());

	// HH
	int r2 = 0, c2 = 0;
	for (int r = 4; r < img.rows / 2; r++)
	{
		c2 = 0;
		for (int c = 4; c < img.cols / 2; c++)
		{
			firstpass.at<uchar>(r2, c2) = img.at<uchar>(r, c);
			c2 += 2;
		}
		if (r2 < img.rows - 4)
			r2 += 2;
		else
			r2 = 0;
	}

	for (int r = 4; r < img.rows - 4; r++)
	{
		for (int c = 4; c < img.cols - 4; c++)
		{
			hhimg.at<uchar>(r, c) = (ihneg4 * (int)img.at<uchar>(r, c - 4)) + (ihneg3 * (int)img.at<uchar>(r, c - 3)) + (ihneg2 * (int)img.at<uchar>(r, c - 2))
				+ (ihneg1 * (int)img.at<uchar>(r, c - 1)) + (ih0 * (int)img.at<uchar>(r, c)) + (ih1 * (int)img.at<uchar>(r, c + 1)) + (ih2 * (int)img.at<uchar>(r, c + 2)) +
				(ih3 * (int)img.at<uchar>(r, c + 3)) + (ih4 * (int)img.at<uchar>(r, c + 4));
		}
	}

	for (int c = 4; c < img.cols - 4; c++)
	{
		for (int r = 4; r < img.rows - 4; r++)
		{
			newimg.at<uchar>(r, c) = (ihneg4 * (int)firstpass.at<uchar>(r - 4, c)) + (ihneg3 * (int)firstpass.at<uchar>(r - 3, c)) + (ihneg2 * (int)firstpass.at<uchar>(r - 2, c))
				+ (ihneg1 * (int)firstpass.at<uchar>(r - 1, c)) + (ih0 * (int)firstpass.at<uchar>(r, c)) + (ih1 * (int)firstpass.at<uchar>(r + 1, c)) + (ih2 * (int)firstpass.at<uchar>(r + 2, c)) +
				(ih3 * (int)firstpass.at<uchar>(r + 3, c)) + (ih4 * (int)firstpass.at<uchar>(r + 4, c));
		}
	} 
	
	return newimg;
}