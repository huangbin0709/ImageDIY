#include "LineDetect.h"
#include "LineDetect.h"
#include "ImageHandler.h"
#include "ImageSeamOperations.h"
#include "ImageProcessor.h"
#include "inpainting.h"
IplImage* getNewSizeImage(IplImage*image,int seamnum,bool isDown){
	if(image==0)
	  return 0;
	IplImage* newimg=cvCreateImage(cvSize(image->width,image->height+seamnum),image->depth,image->nChannels);
 uchar *pchar;
 uchar *newimgchar;
  int width = image->width;                      //读取图像宽度 
  int heigh = image->height;                    //读取图像高度 
  int channel = image->nChannels;          //读取图像通道数 
int widthStep = image->widthStep;      //读取图像一行像素所占的字节数 
  int i,j; 
  for (i=0; i<heigh; i++) 
  { 
    pchar = (uchar*)image->imageData + i*image->widthStep; 
	if(!isDown)
		newimgchar=(uchar *)newimg->imageData+(i+seamnum)*newimg->widthStep;
	else
	newimgchar=(uchar*)newimg->imageData+i*newimg->widthStep;
    for (j=0; j<width; j++) 
    { 
	  uchar* temp = pchar + j*image->nChannels; 
	  uchar* newtmp=newimgchar+j*image->nChannels; 
	  newtmp[0]=temp[0];  
	  newtmp[1]=temp[1];
	  newtmp[2]=temp[2];
    } 
  }
  return newimg;
}
int getInpaintMask(IplImage*image,int seamnum,bool isDown){
if(image==0)
	  return -1;
 uchar *pchar;
  int width = image->width;                      //读取图像宽度 
  int heigh = image->height;                    //读取图像高度 
  int channel = image->nChannels;          //读取图像通道数 
int widthStep = image->widthStep;      //读取图像一行像素所占的字节数 
  int i,j; 
  if(isDown){
  for (i=heigh-1; i>=heigh-seamnum; i--) 
  { 
    pchar = (uchar*)image->imageData + i*image->widthStep; 
    for (j=0; j<width; j++) 
    { 
	  uchar* temp = pchar + j*image->nChannels; 
	  *temp=255;
    } 
  }
  }else{
     for (i=0; i<seamnum; i++) 
  { 
    pchar = (uchar*)image->imageData + i*image->widthStep; 
    for (j=0; j<width; j++) 
    { 
	  uchar* temp = pchar + j*image->nChannels; 
	  *temp=255;
    } 
  }
	 for (i=seamnum; i<heigh; i++) 
  { 
    pchar = (uchar*)image->imageData + i*image->widthStep; 
    for (j=0; j<width; j++) 
    { 
	  uchar* temp = pchar + j*image->nChannels; 
	  *temp=0;
    } 
  }
  }
  return 0;
}
bool isGreen(uchar *tmp){
    	int secha=5;
    	int pixR,pixB,pixG;
    	pixR=tmp[2];
    	pixG=tmp[1];
    	pixB=tmp[0];
    	if(pixR<secha&&pixG>255-secha&&pixB<secha)
    		return true;
    	else
    		return false;
    }
int ConverImage(IplImage* image){
 if(image==0)
	  return -1;
 uchar *pchar;
  int width = image->width;                      //读取图像宽度 
  int heigh = image->height;                    //读取图像高度 
  int channel = image->nChannels;          //读取图像通道数 
int widthStep = image->widthStep;      //读取图像一行像素所占的字节数 
  int i,j; 
  for (i=0; i<heigh; i++) 
  { 
    pchar = (uchar*)image->imageData + i*image->widthStep; 
    for (j=0; j<width; j++) 
    { 
	  uchar* temp = pchar + j*image->nChannels; 
	  if(!isGreen(temp))
	  {
      temp[0] = 0;//通道B 
      temp[1] = 0;//通道G 
      temp[2] = 0;//通道R 
	  }
    } 
  }
  return 0;
}
void DeleteCornerLine( IplImage* src, CvSeq* lines )
{
	// --------------- remove the corner lines ----------------
	int i, nLineCnt;
	int x0, y0, x1, y1;
	int nWidth = src->width, nHeight = src->height;
	int nW = CORNER_WIDTH; // corner width
	int nXlimit = nWidth  - nW, nYlimit = nHeight  - nW;
	
	nLineCnt = lines->total;
	for( i = 0; i < nLineCnt; i ++ ){
		LINE* line = (LINE*)cvGetSeqElem(lines,i);
		x0 = line->p0.x; y0 = line->p0.y; 
		x1 = line->p1.x; y1 = line->p1.y; 

		if( ( (x0 < nW) && (x1 < nW) ) || ( (x0 > nXlimit) && (x1 > nYlimit) ) || // find lines contained in corner
			( (y0 < nW) && (y1 < nW) ) || ( (y0 > nYlimit) && (y1 > nYlimit ) ) ) 
		{
			cvSeqRemove( lines, i );
			i --; nLineCnt --;
		}
	}
}

void GradeByDensityDistribution( IplImage* src, CvSeq* lines )
{
	int i, nLineCnt;
	int nWidth = src->width, nHeight = src->height;
	int polygonOrient; 

	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* polygon = cvCreateSeq( CV_SEQ_POLYGON | CV_32SC2, sizeof(CvSeq), sizeof(CvPoint), storage );

	nLineCnt = lines->total;
	for( i = 0; i < nLineCnt; i ++ ){ // for each line
		LINE* line = (LINE*)cvGetSeqElem( lines, i );

		// 1.2 find end point and polygon points
		FindLineEndPointInRect( line, nWidth, nHeight, polygon, polygonOrient );
		// 2. Calculate point density ratio in the 2 region
		Polygon2Density( src, line, polygon, polygonOrient );
	}
	// 3. Set density grade point mark
	DensRatio2Gradepoint( lines );

	cvReleaseMemStorage(&storage);
}

void FindLineConstant( CvPoint* p0, CvPoint* p1, float &rho, float &theta )
{
	float x0, y0, x1, y1;
	x0 = (float)p0->x; y0 = (float)p0->y; 
	x1 = (float)p1->x; y1 = (float)p1->y; 
	
	theta = atan( ( x1 - x0 ) / ( y0 - y1 ) );
	rho = x0 * cos( theta ) + y0 * sin( theta );  
}

void FindLineEndPointInRect( LINE* line, int nW, int nH, CvSeq * polygon, int &orient )
{
	bool direction[4];
	float x = 0, y = 0;
	float xx[2], yy[2];
	float sinTheta, cosTheta;
	int nEndPointNum = 0;
	int  i;

	for ( i = 0; i < 2; i ++ ) {
		xx[i] = 0; yy[i] = 0;
	}
	for ( i = 0; i < 4; i ++ ) 
		direction[i] = false;

	cvClearSeq( polygon );

	float theta = line->theta;
	float rho = line->rho;
	// 1. find possible end points
	// 1.1 with line x = 0
	sinTheta = sin(theta); 
	cosTheta = cos(theta); 
	if ( sinTheta != 0 ) {
		y = rho / sinTheta;
		if ( y > 0 && y < nH ) {
			direction[0] = true;
			xx[nEndPointNum] = 0;
			yy[nEndPointNum] = y;
			nEndPointNum ++;
		}
	}

	// 1.2 with line y = 0
	if ( cosTheta != 0 ) {
		x = rho / cosTheta;
		if ( x > 0 && x < nW ) {
			direction[1] = true;
			xx[nEndPointNum] = x;
			yy[nEndPointNum] = 0;
			nEndPointNum ++;
		}
	}

	// 1.3 with line x = W
	if (nEndPointNum < 2 && sinTheta != 0) {
		y = ( rho - nW * cosTheta ) / sinTheta;
		if ( y > 0 && y <= nH ) {
			direction[2] = true;
			xx[nEndPointNum] = nW;
			yy[nEndPointNum] = y;
			nEndPointNum ++;
		}
	}

	// 1.4 with line y = H
	if (nEndPointNum < 2 && cosTheta != 0) {
		x = ( rho - nH * sinTheta ) / cosTheta;
		if ( x > 0 && x <= nW ) {
			direction[3] = true;
			xx[nEndPointNum] = x;
			yy[nEndPointNum] = nH;
			nEndPointNum ++;
		}
	}

	if (nEndPointNum != 2) {
		printf("Error: The number of line's end point is not 2.\n");
		return;
	}

	line->endp0.x = xx[0]; line->endp0.y = yy[0]; // end points
	line->endp1.x = xx[1]; line->endp1.y = yy[1];
	
	// 2. 
	CvPoint point;
	if( direction[0] && direction[1] ){ // left side, top side
		point.x = 0;	point.y = 0;		cvSeqPush( polygon, &point );
		point.x = xx[0];point.y = yy[0]; 	cvSeqPush( polygon, &point );
		point.x = xx[1];point.y = yy[1]; 	cvSeqPush( polygon, &point );
		orient = 0;
	}
	else if( direction[0] && direction[2] ){ // left side, right side
		point.x = 0;	point.y = 0; 		cvSeqPush( polygon, &point );
		point.x = nW;	point.y = 0;		cvSeqPush( polygon, &point );
		point.x = xx[1];point.y = yy[1];	cvSeqPush( polygon, &point );
		point.x = xx[0];point.y = yy[0];	cvSeqPush( polygon, &point );
		orient = 1;
	}
	else if( direction[1] && direction[2] ){ // top side, right side
		point.x = nW;   point.y = 0;		cvSeqPush( polygon, &point );
		point.x = xx[0];point.y = yy[0]; 	cvSeqPush( polygon, &point );
		point.x = xx[1];point.y = yy[1]; 	cvSeqPush( polygon, &point );
		orient = 2;
	}
	else if( direction[1] && direction[3] ){ // top side, bottom side
		point.x = 0;	point.y = 0;		cvSeqPush( polygon, &point );
		point.x = xx[0];point.y = yy[0]; 	cvSeqPush( polygon, &point );
		point.x = xx[1];point.y = yy[1]; 	cvSeqPush( polygon, &point );
		point.x = 0;	point.y = nH;		cvSeqPush( polygon, &point );
		
		if (xx[0] > xx[1])
			orient = 0;
		else 
			orient = 3;
	}
	else if( direction[2] && direction[3] ){ // right side, bottom side
		point.x = nW;	point.y = nH;		cvSeqPush( polygon, &point );
		point.x = xx[0];point.y = yy[0]; 	cvSeqPush( polygon, &point );
		point.x = xx[1];point.y = yy[1]; 	cvSeqPush( polygon, &point );
		orient = 4;
	}
	else if( direction[3] && direction[0] ){ // left side, bottom side
		point.x = 0;    point.y = nH;		cvSeqPush( polygon, &point );
		point.x = xx[0];point.y = yy[0]; 	cvSeqPush( polygon, &point );
		point.x = xx[1];point.y = yy[1]; 	cvSeqPush( polygon, &point );
		orient = 5;
	}
}

void Polygon2Density( IplImage * src, LINE* line, CvSeq * polygon, int & polygonOrient )
{
	int i, x, y;
	int nWidth = src->width, nHeight = src->height;
	int nSum[2];
	BYTE var;
	float density[2], retVal = 0, area[2];

	for ( i = 0; i < 2 ; i ++ )	{
		nSum[i] = 0; density[i] = 0; area[i] = 0;
	}

	area[0] = fabs(cvContourArea( polygon, CV_WHOLE_SEQ) );
	area[1] = nWidth * nHeight - area[0];

	if (area[0] == 0 || area[1] == 0)
		printf( "Error: The area is 0.\n" );	

	// 1. count, accumulate for the points belong to the polygon 
	for( y = 0; y < nHeight; y ++ ){ 
		for( x = 0; x < nWidth; x ++ ){
			var = (BYTE)src->imageData[ y * src->widthStep + x ]; // image pixel info: binary: 0, 1
			if (var == 0) 
				continue;
			CvPoint2D32f pt;	pt.x = x; pt.y = y; 
 			double dist = cvPointPolygonTest( polygon, pt, 0 );
			if ( dist > 0 )
				nSum[0] += var; 
			else if( dist < 0 )
				nSum[1] += var; 
			else if( dist != 0 )
				printf( "Error: The return value of Polygon2Density() is invalid: %f\n", dist );	
		}
	}

	// 2. calculate the density of the polygon
	for ( i = 0; i < 2; i ++ )
		density[i] = (float)nSum[i] / area[i];

	float densityDiff = density[1] - density[0];

	line->isUp = true;
	if ( polygonOrient == 0 || polygonOrient == 1 || polygonOrient == 2 ){
		if ( density[0] > density[1] ) 
			line->isUp = false;
	}
	else if ( polygonOrient == 3 || polygonOrient == 4 || polygonOrient == 5 ){
		if ( density[0] < density[1] ) 
			line->isUp = false;
	}

	retVal = ( densityDiff > 0 ) ? (density[1] / density[0]) : (density[0] / density[1]);
	line->densityRatio = retVal;
} 

float GetMaxDensity( CvSeq* lines ) {
	int nLines = lines->total;
	LINE* line = (LINE *)cvGetSeqElem( lines, 0 );
	float max = line->densityRatio;
	float temp;
	
	for (int i = 0; i < nLines; i++){
		line = (LINE *)cvGetSeqElem( lines, i );
		temp = line->densityRatio;
		if ( temp > max ) max = temp;
	}
		
	return max;
}

float GetMinDensity( CvSeq* lines ) {
	int nLines = lines->total;
	LINE* line = (LINE *)cvGetSeqElem( lines, 0 );
	float min = line->densityRatio;
	float temp;
	
	for (int i = 0; i < nLines; i++){
		line = (LINE *)cvGetSeqElem( lines, i );
		temp = line->densityRatio;
		if ( temp < min ) min = temp;
	}
		
	return min;
}

float GetMaxLength( CvSeq* lines ) {
	int nLines = lines->total;
	LINE* line = (LINE *)cvGetSeqElem( lines, 0 );
	float max = line->length;
	float temp;
	
	for (int i = 0; i < nLines; i++){
		line = (LINE *)cvGetSeqElem( lines, i );
		temp = line->length;
		if ( temp > max ) max = temp;
	}
		
	return max;
}

float GetMinLength( CvSeq* lines ) {
	int nLines = lines->total;
	LINE* line = (LINE *)cvGetSeqElem( lines, 0 );
	float min = line->length;
	float temp;
	
	for (int i = 0; i < nLines; i++){
		line = (LINE *)cvGetSeqElem( lines, i );
		temp = line->length;
		if ( temp < min ) min = temp;
	}
		
	return min;
}

int getMaxInt(int* f, int size, int &nNo) {
	int max = f[0];
	int order = 0;
	
	for (int i = 0; i < size; i++)
		if (f[i] > max) {
			max = f[i]; order = i;
		}

	nNo = order;	
	return max;
}

int getMinInt(int* f, int size, int &nNo) {
	int min = f[0];
	int order = 0;
	
	for (int i = 0; i < size; i++)
		if (f[i] < min) {
			min = f[i]; order = i;
		}
	
	nNo = order;	
	return min;
}

void DensRatio2Gradepoint( CvSeq * lines ){
	// 1 calculate Range = highest and lowest val difference
	// 2 gradePoint = nHighestPoint * (myVal - minVal) / Range  
	float fmax, fmin, fRange;
	int nCnt = lines->total;
	if ( nCnt < 1 )
		return;
	
	fmax = GetMaxDensity( lines ); 
	fmin = GetMinDensity( lines ); 

	fRange = fmax - fmin;
	for(int i = 0; i < nCnt; i ++){
		LINE * line = (LINE *)cvGetSeqElem( lines, i );
		if ( fRange == 0 )
			line->grade.densityRatio = GRADE_MAX;
		else
			line->grade.densityRatio = GRADE_MAX * (line->densityRatio - fmin) / fRange;
	}
}

void Length2Gradepoint( CvSeq *lines ){
	// 1 calculate Range = highest and lowest val difference
	// 2 gradePoint = nHighestPoint * (myVal - minVal) / Range  
	float fmax, fmin, fRange;
	int nCnt = lines->total;
	if ( nCnt < 1 )
		return;

	fmax = GetMaxLength( lines ); 
	fmin = GetMinLength( lines ); 

	fRange = fmax - fmin;
	for(int i = 0; i < nCnt; i ++){
		LINE * line = (LINE *)cvGetSeqElem( lines, i );
		if ( fRange == 0 )
			line->grade.length = GRADE_MAX;
		else
			line->grade.length = GRADE_MAX * (line->length - fmin) / fRange;
	}
}

/* Sort lines in grade order */
static int cmp_lineGrade( const void* _a, const void* _b, void* userdata )
{
    LINE* a = (LINE*)_a;
    LINE* b = (LINE*)_b;
    return b->grade.sum - a->grade.sum;
}

bool DisplayResult( IplImage *src, IplImage *dst, CvSeq* lines,LINE &aLine,CvPoint &cPt,bool flag)
{
	// 1. source image
	//cvNamedWindow( "Source", 0 );		cvShowImage( "Source", src );
	
	//char saveFileName[512];
	//char text[256];	
	bool result;
	CvFont font; 
	int i;
	//IplImage * resultImage;
	cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, 8 );   
	//CvPoint cPt;

	// 2. processed image
	//IplImage* color_dst = cvCreateImage( cvGetSize(dst), 8, 3 );
	//cvCvtColor( dst, color_dst, CV_GRAY2BGR );
	int nLineCnt = lines->total;
	/*for( i = 0; i < nLineCnt; i ++ ){
		LINE * line = (LINE*)cvGetSeqElem(lines,i);
		cvLine( color_dst, line->p0, line->p1, CV_RGB(255,0,0), 2, 8 );
	}
	// 3. ultimate image
	resultImage = cvCloneImage( src );*/
	LINE*bestline=0;
	float minangle=1;
	//cvRectangle( resultImage, cvPoint( 5, 10 ), cvPoint( 350, 90 ), CV_RGB(0,0,0), CV_FILLED, 8, 0 );
	for( i = 0; i < MIN( nLineCnt, TOP_N ); i ++ ){
		LINE * line = (LINE*)cvGetSeqElem(lines,i);
		float angle = abs(GetRealAngle( line->theta, line->isUp ));
		if(i==0)
		{
			if(flag){
			minangle=angle;
		     }
		    else{
			minangle=abs(angle-45);
		     }			
			bestline=line;
		}
		if(angle>=270)
			angle=360-angle;
		else if(angle>90&&angle<=180)
			angle=180-angle;
		else if(angle>180)
			angle=angle-180;
		if(flag){
		if(minangle-angle>1.0){
			minangle=angle;
			bestline=line;
		}
		}
		else{
            if(abs(angle-45)<minangle){
			minangle=abs(angle-45);
			bestline=line;
		} 
		}
	}
	if(bestline!=0){
	GetLineCenter( bestline->endp0, bestline->endp1, cPt );
	aLine=*bestline;
	result=true;
	if(flag){
	if(minangle>10)
		result=false;
	    
	}else{
      if(minangle>25)
		result=false;
	}
	
}else
	result=false;
	//cvReleaseImage( &resultImage );
	//cvReleaseImage( &color_dst );
	return result;
}


void SelectByLength( CvSeq * lines )
{
	int nLineCnt = lines->total;

	// 1. get line length
	for( int i = 0; i < nLineCnt; i ++ ){ // for each line
		LINE* line = (LINE*)cvGetSeqElem( lines, i );
		float dx = line->p0.x - line->p1.x;
		float dy = line->p0.y - line->p1.y;
		line->length = sqrt(dx * dx + dy * dy);
		line->grade.sum = line->length;
	}

	// 2. sort by line length
	cvSeqSort( lines, cmp_lineGrade, 0 /* userdata is not used here */ );
}

void GetLineCenter( CvPoint &line0, CvPoint &line1, CvPoint &cPt )
{
	float x0, y0, x1, y1;
	x0 = line0.x; y0 = line0.y;
	x1 = line1.x; y1 = line1.y;

	cPt.x = x0 + ( x1 - x0 ) / 4;
	cPt.y = y0 + ( y1 - y0 ) / 4;
}

float GetRealAngle( float theta, bool isUp )
{
	float fRet = 0;

	if (isUp){
		if( theta > 0 )
			fRet = CV_PI / 2 - theta; // radian
		else
			fRet = 2 * CV_PI - ( CV_PI / 2 + theta );
	}
	else{
		if( theta > 0 )
			fRet = 2 * CV_PI - (CV_PI / 2 + theta);
		else
			fRet = CV_PI / 2 - theta;
	}

	fRet *= (float)( 180 / CV_PI ); // degree
	return fRet;
}

void IntegrateLines( CvSeq * orilines, CvSeq * newlines )
{
	int i, j, nLineCnt;
	nLineCnt = orilines->total;

	// 1. get line constants
	GetLineConstants( orilines );
	// 2. arrange orilines
	cvClearSeq( newlines );
	for( i = 0; i < nLineCnt; i ++ ){
		LINE* line = (LINE*)cvGetSeqElem( orilines, i );
		int nNewLines = newlines->total;
		for ( j = 0; j < nNewLines; j ++ ){
			LINE* newline = (LINE*)cvGetSeqElem( newlines, j );

			float dRho = fabs( line->rho - newline->rho );
			float dTheta = fabs( line->theta - newline->theta );

			if ( dRho < DELTA_RHO && dTheta < DELTA_THETA ){ // if similar, change the original line
				int x[4], y[4], nNo;
				x[0] = line->p0.x; x[1] = line->p1.x; x[2] = newline->p0.x; x[3] = newline->p1.x;
				y[0] = line->p0.y; y[1] = line->p1.y; y[2] = newline->p0.y; y[3] = newline->p1.y;
				newline->p0.x = getMinInt( x, 4, nNo ); newline->p0.y = y[nNo];
				newline->p1.x = getMaxInt( x, 4, nNo ); newline->p1.y = y[nNo];
				break;
			}
		}
		if (j == nNewLines) // if no exist
			cvSeqPush( newlines, line );
	}
	nLineCnt = newlines->total;
}

void GetLineConstants( CvSeq* lines )
{
	float rho, theta;
	int nLineCnt = lines->total;
	for( int i = 0; i < nLineCnt; i ++ ){ // for each line
		LINE* line = (LINE*)cvGetSeqElem( lines, i );
		FindLineConstant( &line->p0, &line->p1, rho, theta );
		line->rho = rho; line->theta = theta;
	}
}

void InitLines( CvSeq * lines_temp, LINE * lines_mem, CvSeq * lines )
{
	int nLineCnt = lines_temp->total;
	int nnn = sizeof(LINE);
	memset( lines_mem, 0, sizeof(LINE) * nLineCnt ); // 0 initialize
	for (int i = 0; i < nLineCnt; i ++){
		CvPoint* line = (CvPoint*)cvGetSeqElem( lines_temp, i );
		LINE * line_mem = &lines_mem[i];
		line_mem->p0.x = line[0].x;	line_mem->p0.y = line[0].y;
		line_mem->p1.x = line[1].x;	line_mem->p1.y = line[1].y;
		cvSeqPush( lines, line_mem );
	}
}

bool HorizonDetect( IplImage* src0, LINE & aLine,CvPoint &cPt,bool flag)
{
	bool bRet = false;
	IplImage* src		= cvCreateImage( cvGetSize(src0), 8, 1);
	IplImage* dst		= cvCreateImage( cvGetSize(src0), 8, 1 ); // 
	cvCvtColor(src0, src, CV_RGB2GRAY); // RGB -> GRAY

	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* lines_temp = 0;
	IplImage*src1=cvCloneImage(src0);
	//IplImage *tmp=cvCreateImage( cvGetSize(src0), 8, 3);
	//cvSmooth(src1,tmp,CV_GAUSSIAN,5,5);
	Mat srcmat(src1);
	Mat sharpened;  
	Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);   
	cv::filter2D(srcmat, sharpened,srcmat.depth(), kernel);
	IplImage sharpimg(sharpened);
	IplImage *sharpimage=&sharpimg;
	//cvNamedWindow( "lap", 1 );	
	//cvShowImage( "lap", sharpimage);
	cvCvtColor(sharpimage, src, CV_BGR2GRAY);
/*	IplImage* canny= cvCreateImage( cvGetSize(src), 8, 1);
	cvCanny(src,canny,70,110,3);  
	cvNamedWindow( "Canny", 1 );	
	cvShowImage( "Canny", canny);
	src=cvCloneImage(canny);*/
	// --------------- 1. Adaptive Threshold Binarize ------------------------------------------------
	int adaptive_method = CV_ADAPTIVE_THRESH_MEAN_C, threshold_type = CV_THRESH_BINARY;
	double max_value = 255; // CV_THRESH_BINARY and CV_THRESH_BINARY_INV 
	//int block_size = 7, param = 5;
    int block_size = 5, param = 5;
	cvAdaptiveThreshold( src, dst, max_value, adaptive_method, threshold_type, block_size, param ); // ###########
	//---------------------------------------------------------------------------------------------
	//cvAdaptiveThreshold(sharpimage, dst, max_value, adaptive_method, threshold_type, block_size, param );
	// 2. binary inversion
	cvThreshold( dst, dst, 1, 255, CV_THRESH_BINARY_INV ); 
	//cvReleaseImage(&src1);
	/*cvCanny(src,dst,30,50,3);  
	cvNamedWindow( "Canny", 1 );	
	cvShowImage( "Canny", dst);*/
	// 3. PPHT -> fine all lines
	//lines_temp = cvHoughLines2( dst, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 160, 40, 5 ); // #######
	lines_temp = cvHoughLines2( dst, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 160,40, 2 ); // #######

	int nLineCnt = lines_temp->total;
	LINE * lines_mem = (LINE *) malloc( sizeof( LINE ) * nLineCnt );
	CvSeq* orilines = cvCreateSeq( NULL, sizeof(CvSeq), sizeof(LINE), storage ); // ? NULL
	InitLines( lines_temp, lines_mem, orilines );
	
	// ========================= Main part ============================================
	CvSeq* lines = cvCreateSeq( NULL, sizeof(CvSeq), sizeof(LINE), storage ); // ? NULL
	IntegrateLines( orilines, lines );
	SelectByLength( lines );
	
#ifndef _DEBUG
#endif
	// 4. delete lines in the sequence back
 	if ( lines->total > MAXNUM_LINE_CANDI )
 		cvSeqPopMulti( lines, NULL, (lines->total - MAXNUM_LINE_CANDI), CV_BACK );

	// 5. delete corner line
	DeleteCornerLine( dst, lines );
	
	// 6. grade lines due to its possibility
	// 6.1 due to its density ratio
#ifndef _DEBUG
#endif
	GradeByDensityDistribution( dst, lines ); // ######
	// 6.2 due to its line length
	Length2Gradepoint( lines );

	// 7. calc grade sum
	nLineCnt = lines->total;
	for ( int i = 0; i < nLineCnt; i ++ ){
		LINE * line = (LINE *)cvGetSeqElem( lines, i );
		GRADE_INDEX * pGrade = &line->grade;
		pGrade->sum = pGrade->densityRatio * WEIGHT_DENSITY + pGrade->length * WEIGHT_LENGTH;
	}

	// 8. sort by grade value
	cvSeqSort( lines, cmp_lineGrade, NULL );

	/*if ( lines->total > 0 ){
		bRet = true;
		LINE * bestline = (LINE *)cvGetSeqElem( lines, 0 );
		aLine = *bestline;
	}*/
	
	// 9. display result
	bRet=DisplayResult( src0, dst, lines,aLine,cPt,flag);
	// 10. release
	// ---------------------------
	cvReleaseImage( &src ); 
	cvReleaseImage( &dst );

	cvReleaseMemStorage(&storage);

	free( lines_mem );
	//-----------------------------
	return bRet;
}
IplImage* AutoResize(IplImage*image,IplImage*graymask){
 LINE aLine;
CvPoint cPt;
IplImage* lineimg=cvCloneImage(image);
if ( HorizonDetect(image, aLine,cPt,true) ){
	//cvLine( lineimg, aLine.endp0, aLine.endp1, CV_RGB(255,0,0), 2, 8 );
			ImageHandler *imagehandle=new ImageHandler(image);
			ImageHandler *mask=new ImageHandler(graymask);
ImageSeamOperations s(imagehandle,mask);
int seamnum;
float scale=1;
if(cPt.y>s.getImg()->root->height-cPt.y-15){
if(s.getImg()->root->height-cPt.y<0.618*cPt.y)
{
	seamnum=1.618*cPt.y-s.getImg()->root->height;
	IplImage*newimg=getNewSizeImage(s.getImg()->root,seamnum,true);
	IplImage*graymask=cvCreateImage(cvSize(newimg->width,newimg->height),image->depth,1);
	getInpaintMask(graymask,seamnum,true);
	inpainting paint(newimg->width,newimg->height,RGB24);
    paint.process((unsigned char * )newimg->imageData,newimg->widthStep, (const unsigned char *)graymask->imageData,graymask->widthStep,8,8,1.2*seamnum,255,100000);
	ImageHandler *newhandler=new ImageHandler(newimg);
	s.setImage(newhandler);
	/*seamnum=(1.618*cPt.y-s.getImg()->root->height)/0.618;
	scale=(s.getImg()->root->height-seamnum)*1.0/s.getImg()->root->height;
	s.decWidth(s.getImg()->root->width*(1-scale),1,s.getImg()->root->width);
	s.setHalfMask(cPt.y,s.getImg()->root->height);
	s.decHeight(seamnum,0,cPt.y);*/
}else{
seamnum=(s.getImg()->root->height-1.618*cPt.y)/0.618;
scale=(s.getImg()->root->height+seamnum)*1.0/s.getImg()->root->height;
//s.addWidth(s.getImg()->root->width*(scale-1),1,s.getImg()->root->width);
s.setHalfMask(cPt.y,s.getImg()->root->height-1);
s.addHeight(seamnum,1,cPt.y);
/*IplImage*newimg=getNewSizeImage(s.getImg()->root,seamnum,false);
	IplImage*graymask=cvCreateImage(cvSize(newimg->width,newimg->height),image->depth,1);
	getInpaintMask(graymask,seamnum,false);
	cvNamedWindow("mask",1);
	cvShowImage("mask",graymask);
	inpainting paint(newimg->width,newimg->height,RGB24);
    paint.process((unsigned char * )newimg->imageData,newimg->widthStep, (const unsigned char *)graymask->imageData,graymask->widthStep,12,12,seamnum*1.1,255,100000);
	ImageHandler *newhandler=new ImageHandler(newimg);
	s.setImage(newhandler);*/
}

			}
else{
	if(cPt.y<0.618*(s.getImg()->root->height-cPt.y)){
	seamnum=0.618*s.getImg()->root->height-1.618*cPt.y;
	//seamnum=(s.getImg()->root->height-1.618*cPt.y)/0.618;
    //scale=(s.getImg()->root->height+seamnum)*1.0/s.getImg()->root->height;
    //s.addWidth(s.getImg()->root->width*(scale-1),1,s.getImg()->root->width);
    s.setHalfMask(cPt.y,s.getImg()->root->height-1);
    s.addHeight(seamnum,1,cPt.y);
	/*IplImage*newimg=getNewSizeImage(s.getImg()->root,seamnum,false);
	IplImage*graymask=cvCreateImage(cvSize(newimg->width,newimg->height),image->depth,1);
	getInpaintMask(graymask,seamnum,false);
	inpainting paint(newimg->width,newimg->height,RGB24);
    paint.process((unsigned char * )newimg->imageData,newimg->widthStep, (const unsigned char *)graymask->imageData,graymask->widthStep,8,8,50,255,100000);
	ImageHandler *newhandler=new ImageHandler(newimg);
	s.setImage(newhandler);*/
	}else{
    seamnum=cPt.y/0.618+cPt.y-s.getImg()->root->height;
	if(seamnum<(s.getImg()->root->height-cPt.y)/2){
	IplImage*newimg=getNewSizeImage(s.getImg()->root,seamnum,true);
	IplImage*graymask=cvCreateImage(cvSize(newimg->width,newimg->height),image->depth,1);
	getInpaintMask(graymask,seamnum,true);
	inpainting paint(newimg->width,newimg->height,RGB24);
    paint.process((unsigned char * )newimg->imageData,newimg->widthStep, (const unsigned char *)graymask->imageData,graymask->widthStep,8,8,seamnum*1.2,255,100000);
	ImageHandler *newhandler=new ImageHandler(newimg);
	s.setImage(newhandler);}
	else{
	s.setHalfMask(0,cPt.y);
	s.addHeight(seamnum,cPt.y,s.getImg()->root->height-1);
	}
	/*scale=(s.getImg()->root->height+seamnum)*1.0/s.getImg()->root->height;
	s.addWidth(s.getImg()->root->width*(scale-1),1,s.getImg()->root->width);
	s.setHalfMask(0,cPt.y);
	s.addHeight(seamnum,cPt.y,s.getImg()->root->height-1);*/
	}

}
IplImage*resizeimg=cvCloneImage(s.getImg()->root);
return resizeimg;
			}
			else{
				return image;
			}
}
IplImage* ManualResize(IplImage*image,IplImage*graymask,int flag){
 LINE aLine;
CvPoint cPt;	
IplImage *lineimg=cvCloneImage(image);
/*ImageProcessor process;
IplImage*segimage=process.MeanShiftSegment(image,sigmaR,minRegion);
cvNamedWindow( "segment", 1 );	
cvShowImage( "segment", segimage);*/
if ( HorizonDetect(image, aLine,cPt,true) ){
			//printf("cPt.y%d\n",cPt.y);
	cvLine( lineimg, aLine.endp0, aLine.endp1, CV_RGB(255,0,0), 2, 8 );
	cvNamedWindow( "Destination", 1 );	
	cvShowImage( "Destination", lineimg);
			ImageHandler *imagehandle=new ImageHandler(image);
			ImageHandler *mask=new ImageHandler(graymask);
ImageSeamOperations s(imagehandle,mask);
int seamnum;
float scale=1;
if(flag==1){
if(s.getImg()->root->height-cPt.y<0.618*cPt.y)
{
	seamnum=1.618*cPt.y-s.getImg()->root->height;
	IplImage*newimg=getNewSizeImage(s.getImg()->root,seamnum,true);
	IplImage*graymask=cvCreateImage(cvSize(newimg->width,newimg->height),image->depth,1);
	getInpaintMask(graymask,seamnum,true);
	inpainting paint(newimg->width,newimg->height,RGB24);
    paint.process((unsigned char * )newimg->imageData,newimg->widthStep, (const unsigned char *)graymask->imageData,graymask->widthStep,8,8,seamnum*1.2,255,100000);
	ImageHandler *newhandler=new ImageHandler(newimg);
	s.setImage(newhandler);
}else{
seamnum=(s.getImg()->root->height-1.618*cPt.y)/0.618;
scale=(s.getImg()->root->height+seamnum)*1.0/s.getImg()->root->height;
//s.addWidth(s.getImg()->root->width*(scale-1),1,s.getImg()->root->width);
s.setHalfMask(cPt.y,s.getImg()->root->height-1);
s.addHeight(seamnum,1,cPt.y);
}

			}
else{
	if(cPt.y<0.618*(s.getImg()->root->height-cPt.y)){
	seamnum=0.618*s.getImg()->root->height-1.618*cPt.y;
	seamnum=(s.getImg()->root->height-1.618*cPt.y)/0.618;
    //scale=(s.getImg()->root->height+seamnum)*1.0/s.getImg()->root->height;
    //s.addWidth(s.getImg()->root->width*(scale-1),1,s.getImg()->root->width);
    s.setHalfMask(cPt.y,s.getImg()->root->height-1);
    s.addHeight(seamnum,1,cPt.y);
	}else{
    seamnum=cPt.y/0.618+cPt.y-s.getImg()->root->height;
	//if(seamnum<(s.getImg()->root->height-cPt.y)/2){
	IplImage*newimg=getNewSizeImage(s.getImg()->root,seamnum,true);
	IplImage*graymask=cvCreateImage(cvSize(newimg->width,newimg->height),image->depth,1);
	getInpaintMask(graymask,seamnum,true);
	inpainting paint(newimg->width,newimg->height,RGB24);
    paint.process((unsigned char * )newimg->imageData,newimg->widthStep, (const unsigned char *)graymask->imageData,graymask->widthStep,8,8,seamnum*1.2,255,100000);
	ImageHandler *newhandler=new ImageHandler(newimg);
	s.setImage(newhandler);
	/*}
	else{
	s.setHalfMask(0,cPt.y);
	s.addHeight(seamnum,cPt.y,s.getImg()->root->height-1);
	}*/
	}

}
CvSize dst_cvsize;
dst_cvsize.width=image->width;
dst_cvsize.height=image->height;
//IplImage *resizeimg=cvCreateImage(dst_cvsize,s.getImg()->root->depth,s.getImg()->root->nChannels);
//cvResize(s.getImg()->root,resizeimg,CV_INTER_CUBIC);	
IplImage*resizeimg=cvCloneImage(s.getImg()->root);
return resizeimg;
			}
			else{
				return image;
			}
}
bool isBlack(uchar *tmp){
    	int secha=5;
    	/*int pixR,pixB,pixG;
    	pixR=tmp[2];
    	pixG=tmp[1];
    	pixB=tmp[0];
    	if(pixR<secha&&pixG<secha&&pixB<secha)*/
		if(tmp[0]<secha)
    		return true;
    	else
    		return false;
    }
int getAreaRect(IplImage*img,int &left,int&top,int& right,int&bottom,int&centerx,int &centery){
	if(img==0)
		return -1;
	int i,j; 
	uchar*pchar;
	int sumi=0,sumj=0;
	int count=0;
	bool isfirstpoint=true;
  for(i=0;i<img->height;i++)
  {
	  pchar=(uchar*)img->imageData + i*img->widthStep;
	  for(j=0;j<img->width;j++)
	  {
		  uchar* temp=pchar+j*img->nChannels;
		  if(!isBlack(temp)){
			  if(isfirstpoint)
			  {
				  left=j;
				  right=j;
				  top=i;
				  bottom=i;
				  isfirstpoint=false;
			  }
			if(left>j)
				left=j;
			if(right<j)
				right=j;
			if(top>i)
				top=i;
			if(bottom<i)
				bottom=i;
			 sumi+=i;
			  sumj+=j;
			  count++;
		  }
	  }
  }
  if(count==0)
	  count=1;
   centerx=sumj/count;
   centery=sumi/count;
 //  printf("%d %d ",centerx,centery);
   return 0;
}
int getMainAreaAndS(IplImage* dstImg,double &maxArea){
	if(dstImg==0)
		return -1;
	double area;
	CvSeq *cont = 0;
	CvSeq *conttemp = 0;
	CvMemStorage *storage = 0;
	storage = cvCreateMemStorage(0); 
//	double maxArea = 0;
	cvFindContours( dstImg, storage, &cont, sizeof(CvContour), 
	CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) ); //找到所有轮廓
	for(;cont;cont = cont->h_next)
	{
		area = fabs(cvContourArea( cont, CV_WHOLE_SEQ )); //获取当前轮廓面积
		cvDrawContours( dstImg, cont, CV_RGB(0, 0, 0), CV_RGB(0, 0, 0), 0,  CV_FILLED, 8);
		//cvDrawContours( dstImg, cont, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), 0,  CV_FILLED, 8);
		if(area > maxArea)
		{
			conttemp = cont;
			maxArea = area;
		}
		//maxArea+=area;
	}
	cvDrawContours( dstImg, conttemp, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), 0,  CV_FILLED, 8);
	cvReleaseMemStorage(&storage);
	return 0;
}
int getTotalArea(IplImage*dstImg,double &maxArea){
    if(dstImg==0)
		return -1;
	double area;
	CvSeq *cont = 0;
	CvSeq *conttemp = 0;
	CvMemStorage *storage = 0;
	storage = cvCreateMemStorage(0); 
    maxArea = 0;
	cvFindContours( dstImg, storage, &cont, sizeof(CvContour), 
	CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) ); //找到所有轮廓
	for(;cont;cont = cont->h_next)
	{
		area = fabs(cvContourArea( cont, CV_WHOLE_SEQ )); //获取当前轮廓面积
		maxArea+=area;
	}
	//cvDrawContours( dstImg, conttemp, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), 0,  CV_FILLED, 8);
	cvReleaseMemStorage(&storage);
	return 0;
}
