#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace std;
using namespace cv;
extern void getBinMask( const Mat& comMask, Mat& binMask );

class GCApplication
{
public:
    enum{ NOT_SET = 0, IN_PROCESS = 1, SET = 2 };
    static const int radius = 2;
    static const int thickness = -1;
//	void getBinMask( const Mat& comMask, Mat& binMask );
    void reset();
	bool isBlack(uchar *tmp);
    int ConverImage(IplImage* image);
	void setImage( const Mat& _image);
	void setRect(int left,int top,int right,int bottom);
    //void setImageAndWinName( const Mat& _image, const string& _winName );
    void showImage() const;
    void mouseClick( int event, int x, int y, int flags, void* param );
    int nextIter();
    int getIterCount() const { return iterCount; }
//private:
    void setRectInMask();
    void setLblsInMask( int flags, Point p, bool isPr );

   // const string* winName;
    const Mat* image;
    Mat mask;
    Mat bgdModel, fgdModel;

    uchar rectState, lblsState, prLblsState;
    bool isInitialized;

    Rect rect;
    vector<Point> fgdPxls, bgdPxls, prFgdPxls, prBgdPxls;
    int iterCount;
};
