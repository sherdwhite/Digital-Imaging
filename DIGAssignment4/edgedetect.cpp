// Sherd White Digital Imaging Assignment #4

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<string>

using namespace cv;

int main(int argc, char * argv[])
{

	if (argc <= 1)
	{
		std::cerr << "Not enough arguments." << std::endl;
		return (1);
	}

	Mat img = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

	if (img.empty())
	{
		std::cerr << "Unable to open the image " << argv[1] << "." << std::endl;
		return (1);
	}

	namedWindow("Original Image", WINDOW_AUTOSIZE);
	imshow("Original Image", img);

	/// Create a matrix of the same type and size as src (for dst)
	cv::Mat canny_final(img.rows, img.cols, CV_8UC3);
	cv::Mat canny_edges(img.rows, img.cols, img.type());
	cv::Mat canny_edges_color(img.rows, img.cols, CV_8UC3);
	cv::Mat sobel_final(img.rows, img.cols, CV_8UC3);
	cv::Mat sobel_edges(img.rows, img.cols, img.type());
	cv::Mat sobel_edges_color(img.rows, img.cols, CV_8UC3);
	cv::Mat img_color(img.rows, img.cols, CV_8UC3);

	/// Reduce noise with a kernel 3x3
	//blur(img, img, Size(3, 3));

	/// Canny detector
	Canny(img, canny_edges, 50, 150, 3);

	/// Sobel detector
	Sobel(img, sobel_edges, CV_8U, 1, 0);

	/// Create a window
	namedWindow("Sobel Edge", CV_WINDOW_AUTOSIZE);
	imshow("Sobel Edge", sobel_edges);

	/// Create a window
	namedWindow("Canny Edge", CV_WINDOW_AUTOSIZE);
	imshow("Canny Edge", canny_edges);

	cvtColor(canny_edges, canny_edges_color, CV_GRAY2RGB);
	cvtColor(sobel_edges, sobel_edges_color, CV_GRAY2RGB);
	cvtColor(img, img_color, CV_GRAY2RGB);

	img_color.copyTo(canny_final);
	img_color.copyTo(sobel_final);

	for (int r = 0; r < canny_edges.rows; r++)
	{
		for (int c = 0; c < canny_edges.cols; c++) {
			if (canny_edges.at<uchar>(r, c) > 100) {
				canny_final.at<Vec3b>(r, c)[0] = 0;
				canny_final.at<Vec3b>(r, c)[1] = 0;
				canny_final.at<Vec3b>(r, c)[2] = 255;
			}
		}

	}

	for (int r = 0; r < sobel_edges.rows; r++)
	{
		for (int c = 0; c < sobel_edges.cols; c++) {
			if (sobel_edges.at<uchar>(r, c) > 30 && sobel_edges.at<uchar>(r, c) < 205) {
				sobel_final.at<Vec3b>(r, c)[0] = sobel_edges.at<uchar>(r, c) + 50;
				sobel_final.at<Vec3b>(r, c)[1] = 0;
				sobel_final.at<Vec3b>(r, c)[2] = 0;
			}
			else if (sobel_edges.at<uchar>(r, c) > 205){
				sobel_final.at<Vec3b>(r, c)[0] = 255;
				sobel_final.at<Vec3b>(r, c)[1] = 0;
				sobel_final.at<Vec3b>(r, c)[2] = 0;
			}
		}

	}

	/// Create a window
	namedWindow("Canny Final", CV_WINDOW_AUTOSIZE);
	imshow("Canny Final", canny_final);

	namedWindow("Sobel Final", CV_WINDOW_AUTOSIZE);
	imshow("Sobel Final", sobel_final);

	cv::waitKey(0);

	return (0);
}

