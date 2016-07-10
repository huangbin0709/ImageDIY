//=================================================================================
//=================================================================================
//
// PictureHandler.cpp: implementation of the PictureHandler class.
//
// Copyright (c) 2007 Radhakrishna Achanta (asv.radhakrishna [at] gmail [dot] com)
// All rights reserved
//
//=================================================================================
//=================================================================================
#include "PictureHandler.h"
//#include <mbctype.h>  

//=================================================================================
// Construction/Destruction
//=================================================================================

PictureHandler::PictureHandler()
{
	
}

PictureHandler::~PictureHandler()
{
	
}

//=================================================================================
//	GetPictureBuffer
//
//	Returns a buffer of the picture just opened
//=================================================================================
void PictureHandler::GetPictureBuffer(
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
IplImage* PictureHandler::getBGRIplImage(vector<UINT>&imgBuffer,int width,int height)
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