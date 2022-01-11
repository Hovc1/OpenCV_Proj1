#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

#include "FaceDetection.h"

using namespace cv;
using namespace std;

/*
    Face Detection (front)
*/
void FaceDetection()
{
    VideoCapture cap(0);    // 0 is an id for attached camera 0 (first one)
    Mat img;

    CascadeClassifier faceCascade;
    faceCascade.load("C:/Users/Eureka/Downloads/OpenCV/OpenCV_Proj1/Resources/haarcascade_frontalface_default.xml");
    if(faceCascade.empty()) { cout << "XML file not loaded" << endl; }

    vector<Rect> faces;
    
    for(;;)
    {
        cap.read(img);
        faceCascade.detectMultiScale(img, faces, 1.1, 10);

        for (int i = 0; i < faces.size(); i++)
        {
            rectangle(img, faces[i].tl(), faces[i].br(), Scalar(255, 0, 0), 3);
            putText(img, "Face", {faces[i].x, faces[i].y-5}, FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255), 2);
            // Scalar(Blue, Green, Red)
        }
        imshow("Image", img);
        waitKey(1);
    }
}
