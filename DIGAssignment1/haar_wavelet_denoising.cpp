#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <cmath>
#include <cassert>

using namespace std;
using namespace cv;

void HaarDecomposeArray(float *A, int width);
void HaarDecomposeImage(float *A, int width, int height);
void HaarTransposeArray(float *A, int width, int height);
void HaarReconstructArray(float *A, int width);
void HaarReconstructImage(float *A, int width, int height);

void Denoise(const Mat &input, Mat &output, float threshold); // single channel image
void OnTrackbar( int, void* );

const int MAX_IMAGE_SIZE = 1024; // keep the image small for speed

int g_slider;
Mat g_img;
Mat g_noisy_img;
Mat g_padded_img;
Mat g_denoised_img;
Mat g_noise;
float g_threshold = 0;

int main(int argc, char **argv)
{
    if(argc != 2) {
        cout << "./prog image.jpg" << endl;
        return 0;
    }

    g_img = imread(argv[1]);
    assert(g_img.data);

    // Haar Wavelet requires image to be a power of two
    // We'll pad the borders and remove them later
    int max_dim = max(g_img.rows, g_img.cols);
    int padded_width = 0;
    int padded_height = 0;

    if(max_dim > MAX_IMAGE_SIZE) {
        resize(g_img, g_img, Size(g_img.cols*MAX_IMAGE_SIZE/max_dim, g_img.rows*MAX_IMAGE_SIZE/max_dim));
    }

    padded_width = pow(2, ceil(log(g_img.cols)/log(2)));
    padded_height = pow(2, ceil(log(g_img.rows)/log(2)));

    // Add some noise to the image
    //g_noise = Mat(g_img.size(), CV_8UC3);
    //randn(g_noise, 0, 50);
    g_noisy_img = g_img;// + g_noise;

    copyMakeBorder(g_noisy_img, g_padded_img, 0, padded_height-g_img.rows, 0, padded_width-g_img.cols, BORDER_CONSTANT);
    cout << "Padded image: " << g_padded_img.size() << endl;

    namedWindow("Haar wavelet denoising");
    createTrackbar("Threshold", "Haar wavelet denoising", &g_slider, 100, OnTrackbar);

    OnTrackbar(g_threshold, 0);

    waitKey(0);

    return 0;
}

void OnTrackbar( int, void* )
{
    g_threshold = 0.1 * g_slider / 100.0;

    if(g_padded_img.channels() == 3) {
        Mat bgr[3];
        split(g_padded_img, bgr);

        Denoise(bgr[0].clone(), bgr[0], g_threshold);
        Denoise(bgr[1].clone(), bgr[1], g_threshold);
        Denoise(bgr[2].clone(), bgr[2], g_threshold);

        merge(bgr, 3, g_denoised_img);
    }
    else {
        Denoise(g_padded_img, g_denoised_img, g_threshold);
    }

    g_denoised_img = g_denoised_img(Range(0, g_img.rows), Range(0, g_img.cols));

    imshow("Haar wavelet denoising", g_denoised_img);
}

void HaarDecomposeArray(float *A, int width)
{
    const float inv_sqrt2 = 1/sqrt(2);

    float norm = 1.0f/sqrt(width);

    for(int i=0; i < width; i++) {
        A[i] *= norm;
    }

    float *tmp = new float[width];

    while(width > 1) {
        width /= 2;

        for(int i=0; i < width; i++) {
            tmp[i] = (A[2*i] + A[2*i+1]) * inv_sqrt2;
            tmp[width + i] = (A[2*i] - A[2*i+1]) * inv_sqrt2;
        }

        memcpy(A, tmp, width*2*sizeof(float));
    }

    delete [] tmp;
}

void HaarTransposeArray(float *A, int width, int height)
{
    float *B = new float[width*height];

    for(int y=0; y < height; y++) {
        for(int x=0; x < width; x++) {
            B[x*height + y] = A[y*width + x];
        }
    }

    memcpy(A, B, sizeof(float)*width*height);

    delete [] B;
}

void HaarDecomposeImage(float *A, int width, int height)
{
    for(int i=0; i < height; i++) {
        HaarDecomposeArray(&A[i*width], width);
    }

    HaarTransposeArray(A, width, height);

    for(int i=0; i < width; i++) {
        HaarDecomposeArray(&A[i*height], height);
    }

    HaarTransposeArray(A, height, width);
}

void HaarReconstructArray(float *A, int width)
{
    const float inv_sqrt2 = 1/sqrt(2);
    float inv_norm = sqrt(width);

    float *tmp = new float[width];
    int k = 1;

    while(k < width)  {
        for(int i=0; i < k; i++) {
            tmp[2*i] = (A[i] + A[k+i]) * inv_sqrt2;
            tmp[2*i+1] = (A[i] - A[k+i]) * inv_sqrt2;
        }

        memcpy(A, tmp, sizeof(float)*(k*2));

        k *= 2;
    }

    for(int i=0; i < width; i++) {
        A[i] *= inv_norm;
    }

    delete [] tmp;
}

void HaarReconstructImage(float *A, int width, int height)
{
    for(int i=0; i < width; i++) {
        HaarReconstructArray(&A[i*height], height);
    }

    HaarTransposeArray(A, height, width);

    for(int i=0; i < height; i++) {
        HaarReconstructArray(&A[i*width], width);
    }

    HaarTransposeArray(A, width, height);
}

void Denoise(const Mat &input, Mat &output, float threshold)
{
    assert(input.data != output.data);

    input.convertTo(output, CV_32F);

    HaarDecomposeImage((float*)output.data, output.cols, output.rows);

    for(int y=0; y < output.rows; y++) {
        float *ptr = (float*)output.ptr(y);

        for(int x=0; x < output.cols; x++) {
            // signbit returns 1 for negative numbers, and 0 otherwise
            // Haar wavelet shrinkage using soft thresholding
            ptr[x] = (signbit(ptr[x]) == 1 ? -1 : 1) * max(0.f, fabs(ptr[x]) - threshold);
        }
    }

    HaarReconstructImage((float*)output.data, output.rows, output.cols);

    output.convertTo(output, CV_8U);
}
