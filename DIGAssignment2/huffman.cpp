// Sherd White Digital Imaging Assignment #2

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<string>
#include<fstream>
#include<bitset>

using namespace cv;

void hufEncode(Mat, std::string);
void hufDecode(std::string);

int compare(const void * a, const void * b)
{
	return (*(int*)a - *(int*)b);
}

int comparerev(const void * a, const void * b)
{
	return (*(int*)b - *(int*)a);
}


int main(int argc, char * argv[])
{

	if (argc <= 1)
	{
		std::cerr << "Not enough arguments, filename needed!" << argv[1] << std::endl;
		return (1);
	}

	std::string filestr = argv[1];

	if (filestr.substr(filestr.find_last_of(".") + 1) == "jpg") {
		Mat img = cv::imread(filestr, CV_LOAD_IMAGE_GRAYSCALE);

		if (img.empty())
		{
			std::cerr << "Unable to open the image " << filestr << "." << std::endl;
			return (1);
		}

		namedWindow("Original Image", WINDOW_AUTOSIZE);
		imshow("Original Image", img);

		hufEncode(img, filestr);
	}
	else if (filestr.substr(filestr.find_last_of(".") + 1) == "huf") {
		hufDecode(filestr);
	}
	else {
		std::cerr << "File type not supported." << std::endl;
	}

	cv::waitKey(0);

	return (0);
}

void hufEncode(Mat img, std::string filestr){	
	// declare histogram
	int histogram[256] = { 0 };
	int sorting[256][2] = { 0 };
	int encoded[256][2] = { 0 };

	size_t lastindex = filestr.find_last_of(".");
	std::string filename = filestr.substr(0, lastindex);

	// calculate the number of pixels for each intensity value
	for (int r = 0; r < img.rows; r++) {
		for (int c = 0; c < img.cols; c++) {
			histogram[img.at<uchar>(r, c)]++;
		}
	}

	for (int i = 0; i < 256; i++)
	{
		sorting[i][0] = histogram[i];
		sorting[i][1] = i;
	//	std::cout << "Before: " << sorting[i][1] << ": " << sorting[i][0] << std::endl;
	}

	// send to qsort
	qsort(sorting, sizeof(sorting) / sizeof(sorting[0]), sizeof(sorting[0]), comparerev);

	for (int i = 0; i < 256; i++)
	{
		std::string number = std::bitset<8>(i).to_string();
		encoded[i][0] = sorting[i][1];
		encoded[i][1] = std::stoi(number);
		//std::cout << "Encoded: " << encoded[i][0] << ": " << encoded[i][1] << std::endl;
	}

	// send to qsort
	qsort(encoded, sizeof(encoded) / sizeof(encoded[0]), sizeof(encoded[0]), compare);

	std::ofstream file(filename + ".huf", std::ios::binary);
	file << img.cols << " ";  //"width: " << img.cols << std::endl;
	file << img.rows << " ";  // "height: " << img.rows << std::endl;

	for (int i = 0; i < 256; i++)
	{
		//std::cout << "Encoded Sorted: " << encoded[i][0] << ": " << encoded[i][1] << std::endl;
		file << encoded[i][1] << " "; // << i << ": " << encoded[i][1] << std::endl;

	}

	for (int r = 0; r < img.rows; r++)
	{
		for (int c = 0; c < img.cols; c++)
		{
			file << encoded[img.at<uchar>(r, c)][1] << " ";
		}
	}

	file.close();
}

void hufDecode(std::string filestr){
	size_t lastindex = filestr.find_last_of(".");
	std::string filename = filestr.substr(0, lastindex);

	std::vector<int> list;
	std::vector<int> encodedimg;
	int encoded[256][2] = { 0 };

	int loc;
	std::ifstream fin(filestr);
	while (fin >> loc)
	{
		list.push_back(loc);
	}

	//std::cout << "size: " << list.size() << std::endl;
	int width, height;
	width = int(list[0]);
	height = int(list[1]);
	//std::cout << "width: " << width << std::endl;
	//std::cout << "height: " << height << std::endl;

	for (int i = 0; i < 256; i++)
	{
		encoded[i][0] = i;
		encoded[i][1] = list[i+2];
		//std::cout << "Encoded: " << encoded[i][0] << ": " << encoded[i][1] << std::endl;

	}

	int i = 258;
	int value;
	cv::Mat decodedimg(height, width, CV_8UC1);
	for (int r = 0; r < decodedimg.rows; r++)
	{
		for (int c = 0; c < decodedimg.cols; c++)
		{
			for (int j = 0; j < 256; j++)
			{
				if (encoded[j][1] == list[i]) {
					value = encoded[j][0];
					break;
				}
			}
			decodedimg.at<uchar>(r, c) = value;
			i++;
		}
	}

	namedWindow("Decoded Image", WINDOW_AUTOSIZE);
	imshow("Decoded Image", decodedimg);
	cv::imwrite("decoded" + filename + ".jpg", decodedimg);
}


