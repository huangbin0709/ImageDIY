
// SalientRegionDetectorDlg.cpp : implementation file
//
//===========================================================================
// This code implements the saliency detection and segmentation method described in:
//
// R. Achanta, S. Hemami, F. Estrada and S. Süsstrunk, Frequency-tuned Salient Region Detection,
// IEEE International Conference on Computer Vision and Pattern Recognition (CVPR), 2009
//===========================================================================
//	Copyright (c) 2010 Radhakrishna Achanta [EPFL].
//===========================================================================
// Email: firstname.lastname@epfl.ch
//////////////////////////////////////////////////////////////////////
//===========================================================================
//	  This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//===========================================================================
#include "SalientRegionDetector.h"
#include "PictureHandler.h"
#include "Saliency.h"
#include "msImageProcessor.h"
SalientRegionDetector::SalientRegionDetector()
{
	
}
SalientRegionDetector::~SalientRegionDetector()
{
	
}
/*SalientRegionDetector::setImage(IplImage*img){
	image=cvCloneImage(img);
	if(image!=0)
		return 0;
		else
		return -1;
	}*/
//===========================================================================
///	DoMeanShiftSegmentation
//===========================================================================
void SalientRegionDetector::DoMeanShiftSegmentation(
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

//=================================================================================
/// DrawContoursAroundSegments
//=================================================================================
void SalientRegionDetector::DrawContoursAroundSegments(
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


//=================================================================================
// ChooseSalientPixelsToShow
//=================================================================================
void SalientRegionDetector::ChooseSalientPixelsToShow(
	const vector<double>&					salmap,
	const int&								width,
	const int&								height,
	const vector<int>&						labels,
	const int&								numlabels,
	vector<bool>&							choose)
{
	int sz = width*height;
	//----------------------------------
	// Find average saliency per segment
	//----------------------------------
	vector<double> salperseg(numlabels,0);
	vector<int> segsz(numlabels,0);
	vector<bool> touchborders(numlabels, false);
	{int i(0);
	for( int j = 0; j < height; j++ )
	{
		for( int k = 0; k < width; k++ )
		{
			salperseg[labels[i]] += salmap[i];
			segsz[labels[i]]++;
			
			if(false == touchborders[labels[i]] && (j == height-1 || j == 0 || k == width-1 || k == 0) )
			{
				touchborders[labels[i]] = true;
			}
			i++;
		}
	}}

	double avgimgsal(0);
	{for( int n = 0; n < numlabels; n++ )
	{
		if(true == touchborders[n])
		{
			salperseg[n] = 0;
		}
		else
		{
			avgimgsal += salperseg[n];
			salperseg[n] /= segsz[n];
		}
	}}

	//--------------------------------------
	// Compute average saliency of the image
	//--------------------------------------
	avgimgsal /= sz;


	//----------------------------------------------------------------------------
	// Choose segments that have average saliency twice the average image saliency
	//----------------------------------------------------------------------------
	vector<bool> segtochoose(numlabels, false);
	{for( int n = 0; n < numlabels; n++ )
	{
		if( salperseg[n] > (avgimgsal+avgimgsal) ) segtochoose[n] = true;
		//if( salperseg[n] > (avgimgsal*2.5) ) segtochoose[n] = true;
	}}

	choose.resize(sz, false);
	bool atleastonesegmentchosent(false);
	{for( int s = 0; s < sz; s++ )
	{
		//if( salperseg[labels[s]] > (avgimgsal+avgimgsal) )
		//if(true == segtochoose[labels[s]])
		{
			choose[s] = segtochoose[labels[s]];
			atleastonesegmentchosent = choose[s];
		}
	}}

	//----------------------------------------------------------------------------
	// If not a single segment has been chosen, then take the brightest one available
	//----------------------------------------------------------------------------
	if( false == atleastonesegmentchosent )
	{
		int maxsalindex(-1);
		double maxsal(DBL_MIN);
		for( int n = 0; n < numlabels; n++ )
		{
			if( maxsal < salperseg[n] )
			{
				maxsal = salperseg[n];
				maxsalindex = n;
			}
		}
		for( int s = 0; s < sz; s++ )
		{
			if(maxsalindex == labels[s]) choose[s] = true;
		}
	}
}

//===========================================================================
///	DoMeanShiftSegmentationBasedProcessing
///
///	Do the segmentation of salient region based on K-Means segmentation
//===========================================================================
void SalientRegionDetector::DoMeanShiftSegmentationBasedProcessing(
	const vector<UINT>&						inputImg,
	const int&								width,
	const int&								height,
	//const string&							filename,
	const vector<double>&					salmap,
	const int&								sigmaS,
	const float&							sigmaR,
	const int&								minRegion,
	vector<UINT>&							segimg,
	vector<UINT>&							segobj) 
{
	int sz = width*height;
	
	//--------------------------------------------------------------------
	// Segment the image using mean-shift algo. Segmented image in segimg.
	//--------------------------------------------------------------------
	vector<int> labels(0);
	int numlabels(0);
	DoMeanShiftSegmentation(inputImg, width, height, segimg, sigmaS, sigmaR, minRegion, labels, numlabels);
	//-----------------
	// Choose segments
	//-----------------
	vector<bool> choose(0);
	ChooseSalientPixelsToShow(salmap, width, height, labels, numlabels, choose);
	//-----------------------------------------------------------------------------
	// Take up only those pixels that are allowed by finalPixelMap
	//-----------------------------------------------------------------------------
	segobj.resize(sz, 0);
/*	for( int p = 0; p < sz; p++ )
	{
		if( choose[p] )
		{
			segobj[p] = inputImg[p];
		}
	}*/
	for( int p = 0; p < sz; p++ )
	{
		if( choose[p] )
		{
			segobj[p] = (0<<16)|(255<<8)|(0);
		}
		else{
			segobj[p] = inputImg[p];
		}
	}
}


//===========================================================================
///	OnBnClickedButtonDetectSaliency
///
///	The main function
//===========================================================================
IplImage* SalientRegionDetector::DetectSaliencyAndSegment(IplImage*image,int segmaS,float segmaR)
{
	PictureHandler picHand;
	IplImage*pimg=cvCreateImage(cvSize(image->width, image->height), IPL_DEPTH_8U, 4);
	cvCvtColor(image,pimg,CV_BGR2BGRA);
		vector<UINT> img(0);// or UINT* imgBuffer;
	  picHand.GetPictureBuffer(pimg, img);
		int sz = pimg->width*pimg->height;
		Saliency sal;
		vector<double> salmap(0);
		sal.GetSaliencyMap(img, pimg->width, pimg->height, salmap, true);
		vector<UINT> outimg(sz);
		for( int i = 0; i < sz; i++ )
		{
			int val = salmap[i] + 0.5;
			outimg[i] = val << 16 | val << 8 | val;
		}
			vector<UINT> segimg, segobj;
			DoMeanShiftSegmentationBasedProcessing(img, pimg->width, pimg->height,salmap,segmaS,segmaR,60, segimg, segobj);
			//DrawContoursAroundSegments(segimg, pimg->width, pimg->height, 0xffffff);
			//DrawContoursAroundSegments(segobj, pimg->width, pimg->height, 0xffffff);
			//IplImage *segimage=picHand.getBGRIplImage(segimg, pimg->width, pimg->height);
			IplImage* segobjimage=picHand.getBGRIplImage(segobj, pimg->width, pimg->height);
			//cvSaveImage("seg.jpg",segimage);
		//	cvReleaseImage(&segimage);
		  cvReleaseImage(&pimg);
		  return segobjimage;
	  //  AfxMessageBox(L"Done!", 0, 0);
}
