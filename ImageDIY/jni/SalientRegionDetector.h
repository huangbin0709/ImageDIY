
// SalientRegionDetectorDlg.h : header file
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

#include <vector>
#include <string>
using namespace std;
#include "cv.h"
#include "highgui.h"
#pragma once
typedef unsigned int UINT;

// CSalientRegionDetectorDlg dialog
class SalientRegionDetector
{
// Construction
public:
	SalientRegionDetector();	// standard constructor
	~SalientRegionDetector();
	IplImage* DetectSaliencyAndSegment(IplImage*,int sigmaS=7 ,float sigmaR=15);
	//IplImage* image;
private:
	
	bool							m_segmentationflag;
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

	void DrawContoursAroundSegments(
		vector<UINT>&							segmentedImage,
		const int&								width,
		const int&								height,
		const UINT&								color);

	void ChooseSalientPixelsToShow(
		const vector<double>&					salmap,
		const int&								width,
		const int&								height,
		const vector<int>&						labels,
		const int&								numlabels,
		vector<bool>&							choose);

	void DoMeanShiftSegmentationBasedProcessing(
		const vector<UINT>&						inputImg,
		const int&								width,
		const int&								height,
	//	const string&							filename,
		const vector<double>&					salmap,
		const int&								sigmaS,
		const float&							sigmaR,
		const int&								minRegion,
		vector<UINT>&							segimg,
		vector<UINT>&							segobj);
};
