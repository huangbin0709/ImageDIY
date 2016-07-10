//=================================================================================
//=================================================================================
//
// PictureHandler.h: interface for the PictureHandler class.
//
// Copyright (c) 2007 Radhakrishna Achanta (asv.radhakrishna [at] gmail [dot] com)
// All rights reserved
//
//=================================================================================
//=================================================================================
#include <vector>
#include <algorithm>
#include "cv.h"
#include "highgui.h"
using namespace std;
//using namespace boost;

#ifndef PictureHandler_H
#define PictureHandler_H
typedef unsigned int UINT;
class PictureHandler  
{
public:
	PictureHandler();
	virtual ~PictureHandler();

	void							GetPictureBuffer(
		                                IplImage *image,
										vector<UINT>&		outBuff
									);

	IplImage*							getBGRIplImage(
										vector<UINT>&		imgBuffer,
										int width,
										int height
									);
};
#endif
