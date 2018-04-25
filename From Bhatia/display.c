#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

const char * DISPLAY = "Display";

int main( int argc, char ** argv )
{
    if ( argc == 1 )
    {
	std::cerr << "usage: " << argv[0] << " picture_file" << std::endl;
	return ( 1 );
    }

    cv::Mat img = cv::imread ( argv[1] );
    if ( img.empty() )
    {
	std::cerr << "Unable to open picture file " << argv[1] << std::endl;
	return ( 1 );
    }

    cv::namedWindow ( DISPLAY );
    cv::imshow ( DISPLAY, img );
    cv::waitKey ( 0 );

    return ( 0 );
}
