#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

#include "PaperScanner.h"

using namespace cv;
using namespace std;

Mat imgOriginal, imgGray, imgCanny, imgThre, imgBlur, imgDilate, imgErode, imgWarp, imgCrop;
vector<Point> initialPoints, docPoints;

float w = 420, h = 596; // A4 paper dimensions times 2

Mat preProcessing(Mat img)
{
    cvtColor(img, imgGray, COLOR_BGR2GRAY);         // Color conversion
    GaussianBlur(img, imgBlur, Size(7,7), 5, 0);    // Blure efect
    Canny(imgBlur, imgCanny, 50, 150);              // Edge detection / outline

    Mat kernel = getStructuringElement(MORPH_RECT, Size(5,5));
    dilate(imgCanny, imgDilate, kernel);            // Canny line thickness increase 
    // erode(imgDilate, imgErode, kernel);          // Canny line thickness reduction

    return imgDilate;
}


vector<Point> getContours(Mat imgDil)
{
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;        // a 4 int vector where each slot contains 4 ints
    
    findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    // drawContours(img, contours, -1, Scalar(0, 0, 0), 2);
    // input, ?, -1 means all contour numbers, Scalar is color, 2 is the thickness

    vector<vector<Point>> conPoly(contours.size());
        // contains the number of edges a shape has  triangle = 3 etc...
    vector<Rect> boundRect(contours.size());
    string objectType;

    vector<Point> biggest;
    int maxArea = 0;

    for(int i = 0; i < contours.size(); i++)
    {
        double area = contourArea(contours[i]);
        cout << area << endl;
        if (area > 1000)
        {
            double peri = arcLength(contours[i], true);     // breaks program (missing dll)
            approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true); // breaks program

            if (area > maxArea && conPoly[i].size() == 4 )
            {
                biggest = {conPoly[i][0], conPoly[i][1], conPoly[i][2], conPoly[i][3]};
                maxArea = area;
                drawContours(imgOriginal, conPoly, i, Scalar(255, 0, 0), 2);
                // Scalar(Blue, Green, Red)
            }
            // drawContours(imgOriginal, contours, i, Scalar(0, 0, 0), 2);
            // rectangle(imgOriginal, boundRect[i].tl(), boundRect[i].br(), Scalar(255, 0, 0), 3);
        }
    }
    return biggest;
}


void drawPoints(vector<Point> points, Scalar color)
{
    for (int i = 0; i < points.size(); i++)
    {
        circle(imgOriginal, points[i], 10, color, FILLED);
        putText(imgOriginal, to_string(i), points[i], FONT_HERSHEY_PLAIN, 4, color, 4);
    }
}


Mat getWarp(Mat img, vector<Point> points, float w, float h)
{
    Point2f src[4] = { points[0], points[3], points[1], points[2] };
    Point2f dst[4] = {{0.0f,0.0f}, {w,0.0f}, {0.0f,h}, {w,h}};

    Mat matrix = getPerspectiveTransform(src, dst);
    warpPerspective(img, imgWarp, matrix, Point(w, h));

    return imgWarp;
}


void PaperScanner()
{
    string path = "C:/Users/Eureka/Downloads/OpenCV/OpenCV_Proj1/Resources/paper.jpg";
    // string path = "C:/Users/Eureka/Downloads/OpenCV/OpenCV_Proj1/Resources/paper2.jpg";
    imgOriginal = imread(path);
    // resize(imgOriginal, imgOriginal, Size(), 0.3, 0.3);
    // Mat img = imread(path); //Mat is a matrix datatype, imaige read

    //preprocessing
    imgThre = preProcessing(imgOriginal);

    //Get Contours
    initialPoints = getContours(imgThre);
    drawPoints(initialPoints, Scalar(0, 0, 255));  // Scalar(Blue, Green, Red)

    //Warp
    imgWarp = getWarp(imgOriginal, initialPoints, w, h);

    // Crop
    int cropVal = 15;
    Rect roi(5, 5, w - (2*cropVal), h - (2*cropVal));
    imgCrop = imgWarp(roi);

    imshow("ImageO", imgOriginal);  // Origional Image
    imshow("ImageT", imgThre);      // Edge Detection Image
    imshow("ImageW", imgWarp);      // Warped Image
    imshow("ImageC", imgCrop);      // Warped and Cropped Image

    waitKey(); 
}
