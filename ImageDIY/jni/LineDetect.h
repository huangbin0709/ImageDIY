#pragma once
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#include "math.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;
using namespace std;

//#define MAXNUM_LINE_CANDI 30
#define MAXNUM_LINE_CANDI 30
//#define CORNER_WIDTH 10
#define CORNER_WIDTH 10
#define GRADE_MAX 1000
#define TOP_N 3

#define WEIGHT_DENSITY 3/4
#define WEIGHT_LENGTH 1/4
//#define WEIGHT_DENSITY 6/10
//#define WEIGHT_LENGTH 4/10
#define DELTA_RHO 2
//#define DELTA_THETA 1/57.18 // 2 degree
#define DELTA_THETA 1/114.36
struct  GRADE_INDEX {
	float densityRatio;
	float length;
	float sum;
};

struct LINE{
	CvPoint p0, p1, endp0, endp1; // points
	float rho, theta; // constants in math
	float angle; // degree fit to the reality
	bool isUp; // orientation
	float length; 
	float densityRatio; // density ratio
	GRADE_INDEX grade;
};
bool isGreen(uchar *tmp);
int ConverImage(IplImage* image);
void verifyVersion();
void Engine();
void DeleteCornerLine( IplImage* src, CvSeq* lines );
void GradeByDensityDistribution( IplImage* src, CvSeq* lines );
void FindLineConstant( CvPoint* p0, CvPoint* p1, float &rho, float &theta );
void FindLineEndPointInRect( LINE* line, int nW, int nH, CvSeq * polygon, int &orientPolygon );
void Polygon2Density( IplImage * src, LINE* line, CvSeq * polygon, int & polygonOrient );
int getMaxInt(int* f, int size, int &nNo);
int getMinInt(int* f, int size, int &nNo);
float GetMaxDensity( CvSeq* lines );
float GetMinDensity( CvSeq* lines );
float GetMaxLength( CvSeq* lines );
float GetMinLength( CvSeq* lines );
void DensRatio2Gradepoint( CvSeq * lines );
void Length2Gradepoint( CvSeq* lines );
static int cmp_lineGrade( const void* _a, const void* _b, void* userdata );
bool DisplayResult( IplImage *src, IplImage *dst, CvSeq* lines,LINE &aLine,CvPoint &cPt,bool flag);
void GradeByLength( IplImage * src, CvSeq * lines, GRADE_INDEX * gradeIndex );
void GetLineCenter(  CvPoint &line0, CvPoint &line1, CvPoint &cPt );
float GetRealAngle( float theta, bool isUp );
void SelectByLength( CvSeq * lines );
void IntegrateLines( CvSeq * orilines, CvSeq * newlines );
void GetLineConstants( CvSeq* lines );
void InitLines( CvSeq * lines_temp, LINE * lines_mem, CvSeq * lines );
bool HorizonDetect( IplImage* src0, LINE & aLine, CvPoint &cPt,bool flag);
//IplImage* AutoResize(IplImage*image,IplImage*mask,float sigmaR=15,float minRegion=0.167);
IplImage* AutoResize(IplImage*image,IplImage*mask);
//IplImage* ManualResize(IplImage*image,IplImage*mask,int flag,float sigmaR=15,float minRegion=0.167);
IplImage* ManualResize(IplImage*image,IplImage*mask,int flag);
IplImage* DiagonalResize(IplImage*image,IplImage*mask,float sigmaR=15,float minRegion=0.167);
IplImage* getNewSizeImage(IplImage*image,int seamnum,bool isDown);
int getInpaintMask(IplImage*mask,int seamnum,bool isDown);
bool isBlack(uchar *tmp);
int getAreaRect(IplImage*img,int &left,int&top,int& right,int&bottom,int&centerx,int &centery);
int getMainAreaAndS(IplImage* dstImg,double &maxArea);
int getTotalArea(IplImage*dstImg,double &maxArea);
