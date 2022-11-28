#include <opencv2/opencv.hpp>
#include <ctime>
#include <sys/time.h>

using namespace std;
using namespace cv;

int main()
{
    string path = "/home/vyacheslav/Documents/Programming/EVMnPU/Lab3-OpenCV/Images and Videos/VID_20221101_223911.mp4";
    VideoCapture cap(path);
    Mat src;
    Mat resized;
    Mat flipped;
    Mat cvtBGR_GREY;
    Mat canny_;

    Mat filtered;

    timeval frameStart;
    size_t  frameStart_ms = 0;

    timeval readTime;
    size_t  readTime_ms = 0;

    timeval modeTime;
    size_t  modeTime_ms = 0;

    timeval outputTime;
    size_t  outputTime_ms = 0;

    timeval finalTime;
    size_t  finalTime_ms = 0;

    double FPS = 0;

    for (;;)
    {
        gettimeofday(&frameStart, nullptr);
        cap.read(src);
        gettimeofday(&readTime, nullptr);

        //resize(src, resized, Size(), 0.5, 0.5);

        flip(src, flipped, 1);
        //imshow("2", flipped);

        GaussianBlur(flipped, filtered, Size(0, 0), 15);
        imshow("3", filtered);

        addWeighted(flipped, 1.5, filtered, -0.5, 10, filtered);
        //imshow("4", filtered);

        //cvtColor(flipped, cvtBGR_GREY, COLOR_BGR2GRAY);
        //imshow("5", cvtBGR_GREY);

        Canny(filtered, canny_, 100, 100);
        imshow("6", canny_);

        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;

        findContours(canny_, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        drawContours(flipped, contours, -1, Scalar(25, 25, 25), 1);
        //drawContours(cvtBGR_GREY, contours, -1, Scalar(5, 5, 5), 1);

        putText(flipped, "FPS: " + to_string(FPS),
                Point(10, 25), FONT_HERSHEY_COMPLEX,
                1, Scalar(255, 0, 255), 2);

        putText(flipped, "ReadTime: " + to_string(readTime_ms - frameStart_ms),
                Point (10, 50), FONT_HERSHEY_COMPLEX,
                1, Scalar(255, 0, 255), 2);

        putText(flipped, "ModeTime: " + to_string(modeTime_ms - readTime_ms),
                Point (10, 75), FONT_HERSHEY_COMPLEX,
                1, Scalar(255, 0, 255), 2);

        putText(flipped, "OutputTime: " + to_string(outputTime_ms - modeTime_ms),
                Point (10, 100), FONT_HERSHEY_COMPLEX,
                1, Scalar(255, 0, 255), 2);

        gettimeofday(&modeTime, nullptr);

        imshow("Final", flipped);
        //imshow("Final Gray", cvtBGR_GREY);
        imshow("Original", src);

        gettimeofday(&outputTime, nullptr);

        if (waitKey(1) >= 0)
        {
            break;
        }

        frameStart_ms = (frameStart.tv_sec * 1000000) + frameStart.tv_usec;
        readTime_ms   = (readTime  .tv_sec * 1000000) + readTime  .tv_usec;
        modeTime_ms   = (modeTime  .tv_sec * 1000000) + modeTime  .tv_usec;
        outputTime_ms = (outputTime.tv_sec * 1000000) + outputTime.tv_usec;

        gettimeofday(&finalTime, nullptr);
        finalTime_ms  = (finalTime .tv_sec * 1000000) + finalTime .tv_usec;
        FPS = (1. * 1000000.) / double(finalTime_ms - frameStart_ms);
    }

    return 0;
}
