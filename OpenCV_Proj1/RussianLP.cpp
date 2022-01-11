#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

#include "RussianLP.h"

using namespace cv;
using namespace std;

/*
    Webcam feed looks for Russian License plates and stores them in a folder as .png images
*/
void RussianLP()
{
    VideoCapture cap(0);    // 0 is an id for attached camera 0 (first one)
    Mat img;

    CascadeClassifier plateCascade;
    plateCascade.load("C:/Users/Eureka/Downloads/OpenCV/OpenCV_Proj1/Resources/haarcascade_russian_plate_number.xml");
    if(plateCascade.empty()) { cout << "XML file not loaded" << endl; }
    vector<Rect> plates;

    for(;;) // ctrl + c  to quit the loop
    {
        cap.read(img);
        plateCascade.detectMultiScale(img, plates, 1.1, 10);
        
        for (int i = 0; i < plates.size(); i++)
        {
            Mat imgCrop = img(plates[i]);
            imwrite("C:/Users/Eureka/Downloads/OpenCV/OpenCV_Proj1/Resources/Plates/" + to_string(i) + "_plate.png", imgCrop);
            imshow("Plate_" + to_string(i), imgCrop);
            rectangle(img, plates[i].tl(), plates[i].br(), Scalar(0, 255, 0), 3);
        }
        imshow("Vid", img);
        waitKey(1);
    }
}
