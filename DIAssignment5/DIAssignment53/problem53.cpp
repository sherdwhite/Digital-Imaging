// Sherd White Digital Image Processing Assignment #5.3

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<string>

#define M_PI (3.14159265358979323846)

using namespace cv;

int main(int argc, char * argv[])
{
	if (argc <= 1)
	{
		std::cerr << "Not enough arguements." << std::endl;
		return (1);
	}

	Mat img = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);

	if (img.empty())
	{
		std::cerr << "Unable to open the image " << argv[1] << "." << std::endl;
		return (1);
	}

	namedWindow("Original Image", CV_WINDOW_AUTOSIZE);
	imshow("Original Image", img);

	Mat hsi(img.rows, img.cols, img.type());

	// Save image pixel values
	int b = 0, g = 0, re = 0;
	for (int r = 0; r < img.rows; r++)
	{
		for (int c = 0; c < img.cols; c++)
		{
			b += img.at<Vec3b>(r, c)[0];
			g += img.at<Vec3b>(r, c)[1];
			re += img.at<Vec3b>(r, c)[2];
		}
	}

	b = b / (img.rows*img.cols);
	g = g / (img.rows*img.cols);
	re = re / (img.rows*img.cols);

	std::cout << "\nThe Average Pixel Intensity (Mean) of Blue is " << b << ".\n" << std::endl;
	std::cout << "\nThe Average Pixel Intensity (Mean) of Green is " << g << ".\n" << std::endl;
	std::cout << "\nThe Average Pixel Intensity (Mean) of Red is " << re << ".\n" << std::endl;

	double B, G, R, H, S, I, h = 0, s = 0, i = 0;

	for (int r = 0; r < img.rows; r++)
	{
		for (int c = 0; c < img.cols; c++)
		{
			B = img.at<Vec3b>(r, c)[0];
			G = img.at<Vec3b>(r, c)[1];
			R = img.at<Vec3b>(r, c)[2];

			//std::cout << b << " " << g << " " << re << std::endl;

			// Normalize colors
			B = B / 255;
			G = G / 255;
			R = R / 255;

			//std::cout << B << " " << G << " " << R << std::endl;

			// BGR to HSI			
			double theta = acos((((R - G) + (R - B)) / 2) / (sqrt((pow((R - G), 2)) + ((R - B)*(G - B)))));

			if (B <= G)
			{
				H = theta;
			}

			if (B > G)
			{
				H = (2 * M_PI) - theta;
			}

			H = H / (2 * M_PI);

			//std::cout << H << std::endl;

			double min = std::min({ B, G, R });
			S = 1 - (3 * min) / (B + G + R);    // / (B + G + R)

			I = (B + G + R) / 3;

			//std::cout << S << " " << I << std::endl;

			hsi.at<Vec3b>(r, c)[0] = int(H * (180 / M_PI));
			hsi.at<Vec3b>(r, c)[1] = int(S * 100);
			hsi.at<Vec3b>(r, c)[2] = int(I * 255);

			h += hsi.at<Vec3b>(r, c)[0];
			s += hsi.at<Vec3b>(r, c)[1];
			i += hsi.at<Vec3b>(r, c)[2];
		}
	}

	h = int(h / (hsi.rows*hsi.cols));
	s = int(s / (hsi.rows*hsi.cols));
	i = int(i / (hsi.rows*hsi.cols));

	std::cout << "\nThe Average Pixel Hue is " << h << ".\n" << std::endl;
	std::cout << "\nThe Average Pixel Saturation is " << s << ".\n" << std::endl;
	std::cout << "\nThe Average Pixel Intensity (HSI) is " << i << ".\n" << std::endl;

	waitKey(0);

	return (0);
}