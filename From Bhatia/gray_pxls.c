#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

const char * DISPLAY = "Display";
typedef unsigned int	uint;

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

    cv::cvtColor ( img, img, CV_BGR2GRAY );

    cv::namedWindow ( DISPLAY );
    cv::imshow ( DISPLAY, img );
    cv::waitKey ( 0 );

     // Save image pixel values

     char fn[80];
     sprintf ( fn, "%s.asc", argv[1] );
     FILE * fd = fopen ( fn, "w" );
     if ( ! fd )
     {
	fprintf ( stderr, "Could not open file %s for writing\n", fn );
	return ( 1 );
     }

     fprintf ( fd, "%d  %d\n", img.rows, img.cols );
     for ( uint r = 0; r < img.rows; r++ )
     {
	for ( uint c = 0; c < img.cols; c++ )
	    fprintf ( fd, "%4d", img.at<uchar>( r, c ) );
	fprintf ( fd, "\n" );
     }

    return ( 0 );
}
