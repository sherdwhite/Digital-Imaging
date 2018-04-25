// Sherd White Digital Image Processing Assignment #5.4

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<string>

#define M_PI (3.14159265358979323846)

using namespace cv;
Mat Imanipulation(Mat, int);
Mat Smanipulation(Mat, int);
void tbCallback(int, void*);
void tbCallback2(int, void*);

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

	namedWindow("Original Image", WINDOW_AUTOSIZE);
	imshow("Original Image", img);

	cv::Mat newimg = Imanipulation(img, 0);
	newimg = Smanipulation(img, 0);

	namedWindow("Image", WINDOW_AUTOSIZE);
	imshow("Image", newimg);

	// Create a Trackbar in the window just created
	int tbVal = 0;
	int tbMaxVal = 50;
	void *pImg = &img;
	cv::createTrackbar("Intensity", "Image", &tbVal, tbMaxVal, tbCallback, pImg);

	// Create a Trackbar in the window just created
	int tbVal2 = 0;
	int tbMaxVal2 = 50;
	void *pImg2 = &img;
	cv::createTrackbar("Saturation", "Image", &tbVal2, tbMaxVal2, tbCallback2, pImg);

	waitKey(0);

	return (0);
}

Mat Imanipulation(Mat img, int intensity)
{
	cv::Mat hsi(img.rows, img.cols, img.type());
	cv::Mat newBGRimg = cv::Mat::zeros(img.rows, img.cols, img.type());

	double B, G, R, H, S, I;
	int intB, intG, intR;

	for (int r = 0; r < img.rows; r++)
	{
		for (int c = 0; c < img.cols; c++)
		{
			B = img.at<Vec3b>(r, c)[0] / 255.0;
			G = img.at<Vec3b>(r, c)[1] / 255.0;
			R = img.at<Vec3b>(r, c)[2] / 255.0;

			I = (B + G + R) / 3.0;

			if (B == G && G == R)
			{
				H = 0;
				S = 0;
			}
			else
			{
				// BGR to HSI			
				H = acos((((R - G) + (R - B)) / 2.0) / (sqrt((pow((R - G), 2.0)) + ((R - B)*(G - B))) + .0000000001));

				if (B > G)
				{
					H = (2.0 * M_PI) - H;
				}

				double min = std::min({ B, G, R });
				if ((B + G + R) == 0)
					S = 1.0;
				else
					S = 1 - (3.0 * min) / (B + G + R);
			}

			I = I + (intensity/100);

			//std::cout << H << " " << S << " " << I <<  " " << r << " " << c << std::endl;

			// HSI to BGR
			if (H >= 0.0 && H < (2.0 * M_PI / 3.0))
			{

				B = I * (1.0 - S);
				R = I * (1.0 + ((S * cos(H)) / (cos((M_PI / 3.0) - H))));
				G = (3.0 * I) - (R + B);

				B *= 255.0;
				G *= 255.0;
				R *= 255.0;

				if (B > 255.0)
					B = 255.0;
				if (G > 255.0)
					G = 255.0;
				if (R > 255.0)
					R = 255.0;
				if (B < 0.0)
					B = 0.0;
				if (G < 0.0)
					G = 0.0;
				if (R < 0.0)
					R = 0.0;

				intB = (int)round(B);
				intG = (int)round(G);
				intR = (int)round(R);
				//std::cout << intB << " " << intG << " " << intG <<  " " << r << " " << c << std::endl;

				newBGRimg.at<Vec3b>(r, c)[0] = intB;
				newBGRimg.at<Vec3b>(r, c)[1] = intG;
				newBGRimg.at<Vec3b>(r, c)[2] = intR;
			}
			else if (H >= (2.0 * M_PI / 3.0) && H < (4.0 * M_PI / 3.0))
			{
				H = H - (2.0 * M_PI) / 3.0;
				R = I * (1.0 - S);
				G = I * (1.0 + ((S * cos(H)) / (cos((M_PI / 3.0) - H))));
				B = (3.0 * I) - (R + G);

				B *= 255.0;
				G *= 255.0;
				R *= 255.0;

				if (B > 255.0)
					B = 255.0;
				if (G > 255.0)
					G = 255.0;
				if (R > 255.0)
					R = 255.0;
				if (B < 0.0)
					B = 0.0;
				if (G < 0.0)
					G = 0.0;
				if (R < 0.0)
					R = 0.0;

				intB = (int)round(B);
				intG = (int)round(G);
				intR = (int)round(R);

				newBGRimg.at<Vec3b>(r, c)[0] = intB;
				newBGRimg.at<Vec3b>(r, c)[1] = intG;
				newBGRimg.at<Vec3b>(r, c)[2] = intR;
			}
			else if (H >= (4.0 * M_PI / 3.0))
			{
				H = H - (4.0 * M_PI) / 3.0;
				G = I * (1.0 - S);
				B = I * (1.0 + ((S * cos(H)) / (cos((M_PI / 3.0 - H)))));
				R = (3.0 * I) - (G + B);

				B *= 255.0;
				G *= 255.0;
				R *= 255.0;

				if (B > 255.0)
					B = 255.0;
				if (G > 255.0)
					G = 255.0;
				if (R > 255.0)
					R = 255.0;
				if (B < 0.0)
					B = 0.0;
				if (G < 0.0)
					G = 0.0;
				if (R < 0.0)
					R = 0.0;

				intB = (int)round(B);
				intG = (int)round(G);
				intR = (int)round(R);

				newBGRimg.at<Vec3b>(r, c)[0] = intB;
				newBGRimg.at<Vec3b>(r, c)[1] = intG;
				newBGRimg.at<Vec3b>(r, c)[2] = intR;
			}
		}
	}

	return newBGRimg;
}

Mat Smanipulation(Mat img, int saturation)
{
	cv::Mat hsi(img.rows, img.cols, img.type());
	cv::Mat newBGRimg = cv::Mat::zeros(img.rows, img.cols, img.type());

	double B, G, R, H, S, I;
	int intB, intG, intR;

	for (int r = 0; r < img.rows; r++)
	{
		for (int c = 0; c < img.cols; c++)
		{
			B = img.at<Vec3b>(r, c)[0] / 255.0;
			G = img.at<Vec3b>(r, c)[1] / 255.0;
			R = img.at<Vec3b>(r, c)[2] / 255.0;

			I = (B + G + R) / 3.0;

			if (B == G && G == R)
			{
				H = 0;
				S = 0;
			}
			else
			{
				// BGR to HSI			
				H = acos((((R - G) + (R - B)) / 2.0) / (sqrt((pow((R - G), 2.0)) + ((R - B)*(G - B))) + .0000000001));

				if (B > G)
				{
					H = (2.0 * M_PI) - H;
				}

				double min = std::min({ B, G, R });
				if ((B + G + R) == 0)
					S = 1.0;
				else
					S = 1 - (3.0 * min) / (B + G + R);
			}

			S = S + (saturation / 100);

			//std::cout << H << " " << S << " " << I <<  " " << r << " " << c << std::endl;

			// HSI to BGR
			if (H >= 0.0 && H < (2.0 * M_PI / 3.0))
			{

				B = I * (1.0 - S);
				R = I * (1.0 + ((S * cos(H)) / (cos((M_PI / 3.0) - H))));
				G = (3.0 * I) - (R + B);

				B *= 255.0;
				G *= 255.0;
				R *= 255.0;

				if (B > 255.0)
					B = 255.0;
				if (G > 255.0)
					G = 255.0;
				if (R > 255.0)
					R = 255.0;
				if (B < 0.0)
					B = 0.0;
				if (G < 0.0)
					G = 0.0;
				if (R < 0.0)
					R = 0.0;

				intB = (int)round(B);
				intG = (int)round(G);
				intR = (int)round(R);
				//std::cout << intB << " " << intG << " " << intG <<  " " << r << " " << c << std::endl;

				newBGRimg.at<Vec3b>(r, c)[0] = intB;
				newBGRimg.at<Vec3b>(r, c)[1] = intG;
				newBGRimg.at<Vec3b>(r, c)[2] = intR;
			}
			else if (H >= (2.0 * M_PI / 3.0) && H < (4.0 * M_PI / 3.0))
			{
				H = H - (2.0 * M_PI) / 3.0;
				R = I * (1.0 - S);
				G = I * (1.0 + ((S * cos(H)) / (cos((M_PI / 3.0) - H))));
				B = (3.0 * I) - (R + G);

				B *= 255.0;
				G *= 255.0;
				R *= 255.0;

				if (B > 255.0)
					B = 255.0;
				if (G > 255.0)
					G = 255.0;
				if (R > 255.0)
					R = 255.0;
				if (B < 0.0)
					B = 0.0;
				if (G < 0.0)
					G = 0.0;
				if (R < 0.0)
					R = 0.0;

				intB = (int)round(B);
				intG = (int)round(G);
				intR = (int)round(R);

				newBGRimg.at<Vec3b>(r, c)[0] = intB;
				newBGRimg.at<Vec3b>(r, c)[1] = intG;
				newBGRimg.at<Vec3b>(r, c)[2] = intR;
			}
			else if (H >= (4.0 * M_PI / 3.0))
			{
				H = H - (4.0 * M_PI) / 3.0;
				G = I * (1.0 - S);
				B = I * (1.0 + ((S * cos(H)) / (cos((M_PI / 3.0 - H)))));
				R = (3.0 * I) - (G + B);

				B *= 255.0;
				G *= 255.0;
				R *= 255.0;

				if (B > 255.0)
					B = 255.0;
				if (G > 255.0)
					G = 255.0;
				if (R > 255.0)
					R = 255.0;
				if (B < 0.0)
					B = 0.0;
				if (G < 0.0)
					G = 0.0;
				if (R < 0.0)
					R = 0.0;

				intB = (int)round(B);
				intG = (int)round(G);
				intR = (int)round(R);

				newBGRimg.at<Vec3b>(r, c)[0] = intB;
				newBGRimg.at<Vec3b>(r, c)[1] = intG;
				newBGRimg.at<Vec3b>(r, c)[2] = intR;
			}
		}
	}

	return newBGRimg;
}

// function tbCallback
void tbCallback(int intensity, void* img) {

	// Cast the void pointer to Mat*
	cv::Mat* filterimg;
	filterimg = static_cast<cv::Mat*>(img);

	// Use the Trackbar value and _data to modify the image displayed
	cv::Mat img_modified = Imanipulation(*filterimg, intensity);

	// Display the modified image
	imshow("Image", img_modified);

}

// function tbCallback
void tbCallback2(int saturation, void* img) {

	// Cast the void pointer to Mat*
	cv::Mat* filterimg;
	filterimg = static_cast<cv::Mat*>(img);

	// Use the Trackbar value and _data to modify the image displayed
	cv::Mat img_modified = Smanipulation(*filterimg, saturation);

	// Display the modified image
	imshow("Image", img_modified);

}