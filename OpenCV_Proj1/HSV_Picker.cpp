#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

#include "HSV_Picker.h"

using namespace cv;
using namespace std;

void HSVpicker()
{
    Mat imgHSV, mask, mask2;
    int hmin = 0, smin = 0, vmin = 0;
    int hmax = 180, smax = 255, vmax = 255;

    cout << "\nChapter 6 Color detection" << endl;
    VideoCapture cap(0);    // Takes a camera / webcam stream as input
    Mat img;
    
    namedWindow("Trackbars", WINDOW_AUTOSIZE );
    createTrackbar("Hue Min", "Trackbars", &hmin, 180);
    createTrackbar("Hue Max", "Trackbars", &hmax, 180);

    createTrackbar("Sat Min", "Trackbars", &smin, 255);
    createTrackbar("Sat Max", "Trackbars", &smax, 255);

    createTrackbar("Val Min", "Trackbars", &vmin, 255);
    createTrackbar("Val Max", "Trackbars", &vmax, 255);
    
    while (true)
    {
        cap.read(img);
        cvtColor(img, imgHSV, COLOR_BGR2HSV);

        Scalar lower(hmin, smin, vmin);
        Scalar upper(hmax, smax, vmax);

        inRange(imgHSV, lower, upper, mask);
        inRange(img, lower, upper, mask2);
        imshow("Image", img);
        imshow("ImageHSV", imgHSV);
        imshow("mask", mask);
        cout << hmin << ", " << smin << ", " << vmin << ", " << hmax << ", " << smax << ", " << vmax << endl;

        waitKey(1); 
    }
}
