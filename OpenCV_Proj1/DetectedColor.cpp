#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

#include "DetectedColor.h"

using namespace cv;
using namespace std;

Mat img;
vector<vector<int>> newPoints;
vector<vector<int>> myColors {{36, 93, 147, 59, 153, 255}, {97, 159, 157, 120, 255, 255}};
//structure: hmin, smin, vmin, hmax, smax, vmax  (all min, then all max  hsv order)

vector<Scalar> myColorValues { {0, 255, 180}, {255, 0, 0} };

Point getContours(Mat imgDil)
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

    Point myPoint(0, 0);

    for(int i = 0; i < contours.size(); i++)
    {
        double area = contourArea(contours[i]);
        cout << area << endl;
        
        if (area > 100)
        {
            double peri = arcLength(contours[i], true);     // breaks program (missing dll)
            approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true); // breaks program
            // cout << "0.02 * peri  = " << 0.02 * peri << endl;
            boundRect[i] = boundingRect(contours[i]);
            
            drawContours(img, contours, i, Scalar(0, 0, 0), 2);
            rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(255, 0, 0), 3);
            myPoint.x = boundRect[i].x + boundRect[i].width / 2;
            myPoint.y = boundRect[i].y;
        }
    }
    return myPoint;
    // Scalar(Blue, Green, Red)
}


vector<vector<int>> findColor(Mat img)
{
    Mat imgHSV;
    cvtColor(img, imgHSV, COLOR_BGR2HSV);

    for (int i = 0; i < myColors.size(); i++)
    {
        Scalar lower(myColors[i][0], myColors[i][1], myColors[i][2]);
        Scalar upper(myColors[i][3], myColors[i][4], myColors[i][5]);
        Mat mask;
        inRange(imgHSV, lower, upper, mask);
        Point myPoint = getContours(mask);
        if(myPoint.x != 0)
        {
            newPoints.push_back({myPoint.x, myPoint.y, i});  // i is the color from myColor and ? vector
        }
    }
    return newPoints;
}

void drawOnCanvas(vector<vector<int>> newPoints, vector<Scalar> myColorValues)
{
    for(int i = 0; i < newPoints.size(); i++)
    {
        circle(img, Point(newPoints[i][0], newPoints[i][1]), 10, myColorValues[newPoints[i][2]], FILLED);
    }
}



void DetectedColor()
{

    // video from webcam //
    VideoCapture cap(0);    // 0 is an id for attached camera 0 (first one)

    while (true) // ctrl + c  to quit the feed
    {
        cap.read(img);
        newPoints = findColor(img);
        drawOnCanvas(newPoints, myColorValues);

        imshow("Vid", img);
        waitKey(1);  // 1 means wait for 1 ms (milisecond) (very fast)
    }

}