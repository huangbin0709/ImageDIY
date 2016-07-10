#pragma once
#include "msImageProcessor.h"
#include "cv.h"
#include "highgui.h"
#include <vector>
using namespace std;
typedef unsigned int UINT;
typedef unsigned char BYTE;
class ImageProcessor{
public:
IplImage*MeanShiftSegment(IplImage*image,float sigmaR=15,float minRegion=0.167);
void DrawContoursAroundSegments(
	vector<UINT>&							segmentedImage,
	const int&								width,
	const int&								height,
	const UINT&								color);
void DoMeanShiftSegmentation(
	const vector<UINT>&						inputImg,
	const int&								width,
	const int&								height,
	vector<UINT>&							segimg,
	const int&								sigmaS,
	const float&							sigmaR,
	const int&								minRegion,
	vector<int>&							labels,
	int&									numlabels);
void GetPictureBuffer(
	IplImage*           image,
	vector<UINT>&		imgBuffer
	);
IplImage* getBGRIplImage(vector<UINT>&imgBuffer,int width,int height);
};
