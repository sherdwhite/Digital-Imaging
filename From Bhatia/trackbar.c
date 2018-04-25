#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// Global variables

std::string DISPLAY = "Display";
std::string TRACKBAR_NAME = "Threshold";
typedef unsigned int	uint;

const uint thresh_max = 255;
int	thresh;
cv::Mat	img;
cv::Mat out_img;


// Trackbar callback function

void on_trackbar ( int, void * )
{
    cv::threshold ( img, out_img, thresh, thresh_max, cv::THRESH_BINARY );
    cv::imshow ( DISPLAY, out_img );
    cv::waitKey ( 1 );
}

int main( int argc, char ** argv )
{
    if ( argc == 1 )
    {
	std::cerr << "usage: " << argv[0] << " picture_file" << std::endl;
	return ( 1 );
    }

    img = cv::imread ( argv[1] );
    if ( img.empty() )
    {
	std::cerr << "Unable to open picture file " << argv[1] << std::endl;
	return ( 1 );
    }

    cv::cvtColor ( img, img, CV_BGR2GRAY );

    cv::namedWindow ( DISPLAY );
    cv::imshow ( DISPLAY, img );
    cv::waitKey ( 1 );

    thresh = 0;
    cv::createTrackbar ( TRACKBAR_NAME, DISPLAY, &thresh, thresh_max, on_trackbar );

    on_trackbar ( thresh, 0 );

    cv::waitKey ( 0 );

    return ( 0 );
}
