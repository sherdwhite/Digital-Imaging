#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main()
{
	try
	{
		cv::Mat img = cv::imread("astronaut.jpg");
		if (img.empty())
			throw (std::string("Could not open input image file"));

		cv::Mat bw;
		cv::cvtColor(img, bw, cv::COLOR_BGR2GRAY);
		cv::medianBlur(bw, bw, 3);
		cv::blur(bw, bw, cv::Size(5, 5));

		cv::Mat thresh;
		cv::adaptiveThreshold(bw, thresh, 0XFF, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 3, 0);

		cv::imshow("Input", thresh);
		cv::waitKey();
	}
	catch (std::string str)
	{
		std::cerr << "Error: " << str << std::endl;
		return (1);
	}

	return (0);
}