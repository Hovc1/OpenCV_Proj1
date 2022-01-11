#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

#include "HSV_Picker.h"
#include "DetectedColor.h"
#include "PaperScanner.h"
#include "RussianLP.h"
#include "FaceDetection.h"

using namespace cv;
using namespace std;

void main()  
{
    /*
        HSV Picker (for draw on detected color)
    */
    // HSVpicker();

    /*
        Draw on detected color
    */
    // DetectedColor();

    /*
        Paper Scanner
        Takes an image of a sheet of paper, and warps it into a rectangle
    */
    // PaperScanner();

    /*
        Russian License Plate detection
    */
    // RussianLP();

    /*
        Face Detection
    */
    FaceDetection();

}
