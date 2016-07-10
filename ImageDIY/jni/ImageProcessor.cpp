#include "ImageProcessor.h"
IplImage*ImageProcessor::MeanShiftSegment(IplImage*image,float sigmaR,float minRegion){
vector<UINT> imgvector;
vector<UINT> segimg;
vector<int> labels(0);
int numlabels(0);
int sz=image->width*image->height;
ImageProcessor process;
IplImage*pimg=cvCreateImage(cvSize(image->width, image->height), IPL_DEPTH_8U, 4);
cvCvtColor(image,pimg,CV_BGR2BGRA);
process.GetPictureBuffer(pimg,imgvector);
process.DoMeanShiftSegmentation(imgvector, image->width, image->height, segimg, 5,sigmaR,sz*minRegion, labels, numlabels);
process.DrawContoursAroundSegments(segimg, pimg->width, pimg->height, 0xffffff);
IplImage *segimage=process.getBGRIplImage(segimg, image->width, image->height);//·Ö¸îºóµÄÍ¼Ïñ
cvReleaseImage(&pimg);
return segimage;
}
void ImageProcessor::DrawContoursAroundSegments(
	vector<UINT>&							segmentedImage,
	const int&								width,
	const int&								height,
	const UINT&								color)
{
	// Pixel offsets around the centre pixels starting from left, going clockwise
	const int dx8[8] = {-1, -1,  0,  1, 1, 1, 0, -1};
	const int dy8[8] = { 0, -1, -1, -1, 0, 1, 1,  1};

	int sz = segmentedImage.size();
	vector<bool> istaken(sz, false);
	int mainindex(0);
	for( int j = 0; j < height; j++ )
	{
		for( int k = 0; k < width; k++ )
		{
			int np(0);
			for( int i = 0; i < 8; i++ )
			{
				int x = k + dx8[i];
				int y = j + dy8[i];

				if( (x >= 0 && x < width) && (y >= 0 && y < height) )
				{
					int index = y*width + x;
					if( false == istaken[index] )
					{
						if( (int)segmentedImage[mainindex] != (int)segmentedImage[index] ) np++;
					}
				}
			}
			if( np > 2 )//1 for thicker lines and 2 for thinner lines
			{
				segmentedImage[j*width + k] = color;
				istaken[mainindex] = true;
			}
			mainindex++;
		}
	}
}
void ImageProcessor::DoMeanShiftSegmentation(
	const vector<UINT>&						inputImg,
	const int&								width,
	const int&								height,
	vector<UINT>&							segimg,
	const int&								sigmaS,
	const float&							sigmaR,
	const int&								minRegion,
	vector<int>&							labels,
	int&									numlabels)
{
	int sz = width*height;
	BYTE* bytebuff = new BYTE[sz*3];
	{int i(0);
	for( int p = 0; p < sz; p++ )
	{
		bytebuff[i+0] = inputImg[p] >> 16 & 0xff;
		bytebuff[i+1] = inputImg[p] >>  8 & 0xff;
		bytebuff[i+2] = inputImg[p]       & 0xff;
		i += 3;
	}}
	msImageProcessor mss;
	mss.DefineImage(bytebuff, COLOR, height, width);		
	mss.Segment(sigmaS, sigmaR, minRegion, HIGH_SPEEDUP);
	mss.GetResults(bytebuff);

	int* p_labels = new int[sz];
	numlabels = mss.GetLabels(p_labels);
	labels.resize(sz);
	for( int n = 0; n < sz; n++ ) labels[n] = p_labels[n];
	if(p_labels) delete [] p_labels;

	segimg.resize(sz);
	int bsz = sz*3;
	{int i(0);
	for( int p = 0; p < bsz; p += 3 )
	{
		segimg[i] = bytebuff[p] << 16 | bytebuff[p+1] << 8 | bytebuff[p+2];
		i++;
	}}
	if(bytebuff) delete [] bytebuff;
}
void ImageProcessor::GetPictureBuffer(
	IplImage*           image,
	vector<UINT>&		imgBuffer
	)
{
	long imgSize			= image->height*image->width;
	imgBuffer.resize(imgSize);
	UINT* tempBuff = (UINT*)image->imageData;
	for( int p = 0; p < imgSize; p++ ) imgBuffer[p] = tempBuff[p];
}


//=================================================================================
//	SavePicture
//
//	Saves the given buffer as a JPEG or BMP image depeding on which encoder CLSID
//	is used.
//=================================================================================
IplImage* ImageProcessor::getBGRIplImage(vector<UINT>&imgBuffer,int width,int height)
{
	
	IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
    unsigned char* base = (unsigned char*) (img->imageData);
    unsigned char* ptr;
	int y=0,x=0;
    for (y = 0; y < height; y++) {
    ptr = base + y*img->widthStep;
    for (x = 0; x < width; x++) {
        // blue
        ptr[3*x] =  imgBuffer[x+y*width]&0xFF;
        // green
        ptr[3*x+1] =imgBuffer[x+y*width]>>8&0xFF;
        // blue
        ptr[3*x+2] =imgBuffer[x+y*width]>>16&0xFF;

    }
	}
	return img;
	
}