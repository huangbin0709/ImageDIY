/* Exemplar-Based Inpainting

(c) 2008 Alexander Balakhnin (Fizick) http://avisynth.org.ru

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   Algorithm is based on article:
Object Removal by Exemplar-Based Inpainting.
A. Criminisi, P. Perez, K. Toyama.
In Proc. Conf. Comp. Vision Pattern Rec., Madison, WI, Jun 2003.
http://research.microsoft.com/vision/cambridge/papers/Criminisi_cvpr03.pdf

   Source code is based on:
http://www.cc.gatech.edu/grads/q/qszhang/project/inpainting.htm */
/*Author: Qiushuang Zhang */
/*E-mail: qszhang@cc.gatech.edu */
/*Nov.29, 2005 */

/* Modifications (changes):

10-12 January 2008 by Fizick:
 - replaced double to integer (scaled) for speed
 - removed image library
 - adopted to AviSynth API, RRB32, YV12
 - fixed wrong gray, small bugs
 - different winx, winy sizes
 - licensed under GNU GPL

*/

#include "inpainting.h"
#include <memory.h>
#include <math.h>
#include <cstdlib>
#include <stdio.h>
//#include <windows.h> // for wsprintf and OutpuDebugString only

#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))

inpainting::inpainting(int _width, int _height, int _pixel_format)
{
	m_width = _width;
	m_height = _height;
	pixel_format = _pixel_format;

	m_mark = new unsigned char[m_width*m_height];
	m_confid = new int[m_width*m_height];
	m_pri = new int[m_width*m_height];
	m_source = new unsigned char[m_width*m_height];

	if(pixel_format == RGB32 || pixel_format == RGB24 || pixel_format == RGBA)
		m_gray  = new unsigned char[m_width*m_height];

}


inpainting::~inpainting(void)
{
	if(m_mark)delete [] m_mark;
	if(m_confid)delete [] m_confid;
	if(m_pri)delete [] m_pri;
	if(m_source)delete [] m_source;
	if(m_gray && pixel_format != YV12 )delete [] m_gray;
}

/*********************************************************************/
void inpainting::Convert2Gray(void)
{
	unsigned char *psrc1 = psrc;

	if(pixel_format == RGB32 || pixel_format == RGBA)
	{
		for(int y = 0; y<m_height; y++)
		{
			for(int x = 0; x<m_width; x++)
			{
				int b = psrc1[x*4];
				int g = psrc1[x*4+1];
				int r = psrc1[x*4+2];
				m_gray[y*m_width+x] = ((b*3735 + g*19268 + r*9765)/32768);
			}
			psrc1 += src_pitch;
		}
	}
	else if (pixel_format == RGB24)
	{
		for(int y = 0; y<m_height; y++)
		{
			for(int x = 0; x<m_width; x++)
			{
				int b = psrc1[x*3];
				int g = psrc1[x*3+1];
				int r = psrc1[x*3+2];
				m_gray[y*m_width+x] = ((b*3735 + g*19268 + r*9765)/32768);
			}
			psrc1 += src_pitch;
		}
	}
	else if (pixel_format == YV12)
		m_gray = psrc; // gray is simply pointer to luma
}

/*********************************************************************/
int inpainting::process(unsigned char * _psrc, int _src_pitch,
						const unsigned char * _maskp, int _mask_pitch,
					   int _xsize, int _ysize, int _radius, int _maskcolor, int maxsteps)
{ // wrapper for interleave RGB

	return process3planes(_psrc, _src_pitch,
					   0, 0,
					   0,
						_maskp, _mask_pitch,
						0, 0,
						0,
					    _xsize, _ysize, _radius, _maskcolor, maxsteps);

}
/*********************************************************************/
int inpainting::process3planes(unsigned char * _psrc, int _src_pitch,
					   unsigned char * _psrcU, int _src_pitchU,
					   unsigned char * _psrcV,
						const unsigned char * _maskp, int _mask_pitch,
						const unsigned char * _maskpU, int _mask_pitchU,
						const unsigned char * _maskpV,
					   int _xsize, int _ysize, int _radius, int _maskcolor, int maxsteps)
{// the main function to process the whole image

	psrc = _psrc;
	src_pitch = _src_pitch;
	psrcU = _psrcU;
	src_pitchUV = _src_pitchU;
	// assume pitchV=pitchU (it is always in AviSynth) for speed
	psrcV = _psrcV;
	pmask = _maskp;
	mask_pitch = _mask_pitch;
	pmaskU = _maskpU;
	mask_pitchUV = _mask_pitchU;
	pmaskV = _maskpV;
	winxsize = _xsize/2; // window is half of full side size
	winysize = _ysize/2;
	radius = _radius; // 0 for full search, radius > winsize
	maskcolor = _maskcolor;

	m_top = m_height;  // initialize the rectangle area
    m_bottom = 0;
	m_left = m_width;
	m_right = 0;

	Convert2Gray();  // create  gray image from RGB source
	memset( m_confid, 0, m_width*m_height*sizeof(int) ); // init
	GetMask();
	DrawBoundary();  // first time draw boundary
	draw_source();   // find the patches that can be used as sample texture
	memset(m_pri, 0, m_width*m_height*sizeof(int));
	for(int j= m_top; j<=m_bottom; j++)
	    for(int i = m_left; i<= m_right; i++)
			if(m_mark[j*m_width+i] == BOUNDARY)
				m_pri[j*m_width+i] = priority(i,j);//if it is boundary, calculate the priority
	int count=0;
	while(TargetExist() && count<maxsteps)
	{
		count++;
		int max_pri = -1; // m_pri may be 0 in flat regions (Fizick)
		int pri_x,pri_y;
		for(int j= m_top; j<=m_bottom; j++)
	    for(int i = m_left; i<= m_right; i++)
		{
			if(m_mark[j*m_width+i] == BOUNDARY && m_pri[j*m_width+i]>max_pri)// find the boundary pixel with highest priority
			{
				pri_x = i;
				pri_y = j;
				max_pri = m_pri[j*m_width+i];
			}
		}
//	char buf[80];
//	wsprintf(buf,"Inpaint: pri_x=%d, pri_y=%d, max_pri=%d", pri_x, pri_y, max_pri);
//	OutputDebugString(buf);
		if (max_pri==-1) return -count; // probably bad mask, no boundary (e.g. full frame is mask)
		int patch_x, patch_y;
		bool found = PatchTexture(pri_x, pri_y, patch_x, patch_y);  // find the most similar source patch
		if (!found) return count; // patch not found at this step
		int conf = ComputeConfidence(pri_x,pri_y); // update confidence
		update(pri_x, pri_y, patch_x,patch_y, conf );// inpaint this area
		UpdateBoundary(pri_x, pri_y); // update boundary near the changed area
		UpdatePri(pri_x, pri_y);  //  update priority near the changed area
	}
	return count; // number of inpainting steps (iterations)
}
/*********************************************************************/
void inpainting::GetMask(void)// first time mask
{
	int confid1 = 2048;// scaled  for int division

	if (pixel_format == RGB32)
	{
		const unsigned int * intmask = reinterpret_cast<const unsigned int *>(pmask);
		int intmask_pitch = mask_pitch/4;

		for(int y = 0; y<m_height; y++)
		{
			for(int x = 0; x<m_width; x++)
			{
				// get mask data for this point
				unsigned int color = *(intmask + x) & 0xFFFFFF; // without alpha
				if((int)color == maskcolor)// if the pixel is specified as mask
				{
					m_mark[y*m_width+x] = TARGET;
					m_confid[y*m_width+x] = 0;
				}
				else {
					m_mark[y*m_width+x] = SOURCE;
					m_confid[y*m_width+x] = confid1;
				}
			}
			intmask += intmask_pitch;
		}
	}
	else if (pixel_format == RGBA) // really ARGB in avisynth (Alpha - high byte)
	{
		const unsigned char * pmask1 = psrc; // use source clip, not mask

		for(int y = 0; y<m_height; y++)
		{
			for(int x = 0; x<m_width; x++)
			{
				// get mask data for this point
				int color = *(pmask1 + x*4 + 3) ; // alpha
				if(color > 127)// if the pixel is specified as mask, unike other modes, use threshold
				{
					m_mark[y*m_width+x] = TARGET;
					m_confid[y*m_width+x] = 0;
				}
				else {
					m_mark[y*m_width+x] = SOURCE;
					m_confid[y*m_width+x] = confid1;
				}
			}
			pmask1 += src_pitch;
		}
	}
	else if (pixel_format == RGB24)
	{
		const unsigned char * pmask1 = pmask;
		//printf("%d maskcolor",maskcolor);
		for(int y = 0; y<m_height; y++)
		{
			for(int x = 0; x<m_width; x++)
			{
				// get mask data for this point
				//int color = *(pmask1 + x*3) | *(pmask1 + x*3 + 1)<<8 | *(pmask1 + x*3 + 2)<<16; // bgr
				int color=*(pmask1+x);
			//	printf("%d ",color);
				if(color == maskcolor)// if the pixel is specified as mask
				{
					m_mark[y*m_width+x] = TARGET;
					m_confid[y*m_width+x] = 0;
				}
				else {
					m_mark[y*m_width+x] = SOURCE;
					m_confid[y*m_width+x] = confid1;
				}
			}
			pmask1 += mask_pitch;
		}
	}
	else if (pixel_format == YV12)
	{
		const unsigned char * pmask1 = pmask;
		const unsigned char * pmaskU1 = pmaskU;
		const unsigned char * pmaskV1 = pmaskV;

		for(int y = 0; y<m_height; y++)
		{
			for(int x = 0; x<m_width; x++)
			{
				// get mask data for this point
				int color = pmaskV1[x>>1] | pmaskU1[x>>1]<<8 | pmask1[x]<<16; // yuv
				if(color == maskcolor)// if the pixel is specified as mask
				{
					m_mark[y*m_width+x] = TARGET;
					m_confid[y*m_width+x] = 0;
				}
				else {
					m_mark[y*m_width+x] = SOURCE;
					m_confid[y*m_width+x] = confid1;
				}
			}
			pmask1 += mask_pitch;
			if(y%2) // chroma is vertically subsampled too
			{
				pmaskU1 += mask_pitchUV;
				pmaskV1 += mask_pitchUV;
			}
		}
	}

}
/*********************************************************************/
void inpainting::DrawBoundary(void)// fist time draw boundary
{

	for(int j= 0; j< m_height; j++)
	    for(int i = 0; i< m_width; i++)
		{
			if(m_mark[j*m_width+i]==TARGET)
			{
				if(i<m_left)m_left = i; // resize the rectangle to the range of target area
				if(i>m_right)m_right = i;
				if(j>m_bottom)m_bottom = j;
				if(j<m_top)m_top = j;
				//if one of the four neighbours is source pixel, then this should be a boundary
				if(j==m_height-1||j==0||i==0||i==m_width-1||m_mark[(j-1)*m_width+i]==SOURCE||m_mark[j*m_width+i-1]==SOURCE
					||m_mark[j*m_width+i+1]==SOURCE||m_mark[(j+1)*m_width+i]==SOURCE)m_mark[j*m_width+i] = BOUNDARY;
				//if(m_mark[(j-1)*m_width+i]==SOURCE||m_mark[j*m_width+i-1]==SOURCE
					//||m_mark[j*m_width+i+1]==SOURCE||m_mark[(j+1)*m_width+i]==SOURCE)m_mark[j*m_width+i] = BOUNDARY;
			}
		}
}



/*********************************************************************/
int inpainting::priority(int i, int j)
{
	int confidence, data;
	confidence = ComputeConfidence(i,j); // confidence term
	data = ComputeData(i,j);// data term
	return confidence*data;
}

/*********************************************************************/
int inpainting::ComputeConfidence(int i, int j)
{
	int confidence=0;
	for(int y = MAX(j -winysize,0); y<= MIN(j+winysize,m_height-1); y++)
		for(int x = MAX(i-winxsize,0); x<=MIN(i+winxsize, m_width-1); x++)
			confidence+= m_confid[y*m_width+x];
	confidence /= (winxsize*2+1)*(winysize*2+1);
	return confidence;

}
/*********************************************************************/
int inpainting::ComputeData(int i, int j)
{
	gradient grad, temp, grad_T;
	grad.grad_x=0;
	grad.grad_y=0;
	int result;
	int magnitude;
	int magmax=0;
	int x, y;
	for(y = MAX(j -winysize,0); y<= MIN(j+winysize,m_height-1); y++)
	{
		for( x = MAX(i-winxsize,0); x<=MIN(i+winxsize, m_width-1); x++)
		{
			// find the greatest gradient in this patch, this will be the gradient of this pixel(according to "detail paper")
			if(m_mark[y*m_width+x] == SOURCE) // source pixel
			{
				//since I use four neighbors to calculate the gradient, make sure this four neighbors do not touch target region(big jump in gradient)
				if( (x+1<m_width && m_mark[y*m_width+x+1]!=SOURCE) // add bound check (Fizick)
					|| (x-1>=0 && m_mark[y*m_width+x-1]!=SOURCE)
					|| (y+1<m_height && m_mark[(y+1)*m_width+x]!=SOURCE)
					|| (y-1>=0 && m_mark[(y-1)*m_width+x]!=SOURCE))
					continue;
 				temp = GetGradient(x,y);
				magnitude = temp.grad_x*temp.grad_x+temp.grad_y*temp.grad_y;
				if(magnitude>magmax)
				{
					grad.grad_x = temp.grad_x;
					grad.grad_y = temp.grad_y;
					magmax = magnitude;
				}
			}
		}
	}
		grad_T.grad_x = grad.grad_y;// perpendicular to the gradient: (x,y)->(y, -x)
		grad_T.grad_y = -grad.grad_x;

	Mnorm nn = GetNorm(i,j);
	result = nn.norm_x*grad_T.grad_x+nn.norm_y*grad_T.grad_y; // dot product
//	result/=255; //"alpha" in the paper: normalization factor (it is not important, disable for interger)
	result = abs(result);
	return result;
}


/*********************************************************************/

gradient inpainting::GetGradient(int i, int j)
{
    // scale gradient to factor 2 to be integer, add bound check (Fizick)
	gradient result;

	if(i==0)result.grad_x = ((int)m_gray[j*m_width+i+1] - (int)m_gray[j*m_width+i])*2;
	else if(i==m_width-1)result.grad_x = ((int)m_gray[j*m_width+i] - (int)m_gray[j*m_width+i-1])*2;
	else result.grad_x = ((int)m_gray[j*m_width+i+1] - (int)m_gray[j*m_width+i-1]);// /2.0;

	if(j==0)result.grad_y = ((int)m_gray[(j+1)*m_width +i] - (int)m_gray[j*m_width+i])*2;
	else if(j==m_height-1)result.grad_y = ((int)m_gray[j*m_width +i] - (int)m_gray[(j-1)*m_width+i])*2;
	else 	result.grad_y = ((int)m_gray[(j+1)*m_width +i] - (int)m_gray[(j-1)*m_width+i]);// /2.0;

	return result;
}

/*********************************************************************/
Mnorm inpainting::GetNorm(int i, int j)
{
	Mnorm result;
	int num=0;
	int neighbor_x[9];
	int neighbor_y[9];
	int record[9];
	int count = 0;
	for(int y = MAX(j-1,0); y<=MIN(j+1,m_height-1); y++)
		for(int x = MAX(i-1,0); x<=MIN(i+1,m_width-1); x++)
		{
			count++;
			if(x==i&&y==j)continue;
			if(m_mark[y*m_width+x]==BOUNDARY)
			{
				num++;
				neighbor_x[num] = x;
				neighbor_y[num] = y;
				record[num]=count;
			}
		}
		if(num==0||num==1) // if it doesn't have two neighbors, give it a random number to proceed
		{
			result.norm_x = 181;//0.6;
			result.norm_y = 182;//0.8;
			return result;
		}
		// draw a line between the two neighbors of the boundary pixel, then the norm is the perpendicular to the line
			int n_x = neighbor_x[2]-neighbor_x[1];
			int n_y = neighbor_y[2]-neighbor_y[1];
			int temp=n_x;
			n_x = n_y;
			n_y = temp;
			double square = sqrt(double(n_x*n_x + n_y*n_y));

	result.norm_x = n_x*256/square; // scaled int to divide
	result.norm_y = n_y*256/square;
	return result;
}

/*********************************************************************/
bool inpainting::draw_source(void)
{
	// draw a window around the pixel, if all of the points within the window are source pixels, then this patch can be used as a source patch
	bool flag;
	for(int j = 0; j<m_height; j++)
	{
		for(int i = 0; i<m_width; i++)
		{
			flag=true;
			if(i<winxsize||j<winysize||i>=m_width-winxsize||j>=m_height-winysize)
				m_source[j*m_width+i]=0;//cannot form a complete window
			else
			{
				for(int y = j-winysize; y<=j+winysize; y++)
				{
					for(int x = i-winxsize; x<=i+winxsize; x++)
					{
						if(m_mark[y*m_width+x]!=SOURCE)
						{
							m_source[j*m_width+i]=0;
							flag = false;
							break;
						}
					}
					if(flag==false)break;
				}
			    if(flag!=false)m_source[j*m_width+i]=1;
			}
		}
	}
	return true;
}

/*********************************************************************/
bool inpainting::PatchTexture(int x, int y, int &patch_x, int &patch_y)
{
	// find the most similar patch, according to SSD

#define MIN_INITIAL 99999999

    int ymin, ymax, xmin, xmax;

	if (radius>0) // added by Fizick
    {
        ymin = MAX(y-radius, 0);
        ymax = MIN(y+radius, m_height);
        xmin = MAX(x-radius, 0);
        xmax = MIN(x+radius, m_width);
    }
    else // full frame search (slow)
    {
        ymin = 0;
        ymax = m_height;
        xmin = 0;
        xmax = m_width;
    }


	long min=MIN_INITIAL;
	long sum;
	int source_x, source_y;
	int target_x, target_y;

	if(pixel_format == RGB32 || pixel_format == RGBA)
	{

		unsigned int * intsrc = reinterpret_cast<unsigned int *>(psrc);
		int intsrc_pitch = src_pitch/4; // in int

		for(int j = ymin; j<ymax; j++)
		{
			for(int i = xmin; i<xmax; i++)
			{
				if(m_source[j*m_width+i]==0)continue; // not good patch source
				sum=0;
				for(int iter_y=(-1)*winysize; iter_y<=winysize; iter_y++)
				{
					source_y = j+iter_y;
					target_y = y+iter_y;
					if(target_y<0||target_y>=m_height)continue;
					for(int iter_x=(-1)*winxsize; iter_x<=winxsize; iter_x++)
					{
						source_x = i+iter_x;
						target_x = x+iter_x;
						if(target_x<0||target_x>=m_width)continue;

						// it is the most time-comsuming part of code:
						if(m_mark[target_y*m_width+target_x]==SOURCE) // compare
						{
//							int temp_b = psrc[target_y*src_pitch+target_x*4]-psrc[source_y*src_pitch+source_x*4];
//							int temp_g = psrc[target_y*src_pitch+target_x*4+1]-psrc[source_y*src_pitch+source_x*4+1];
//							int temp_r = psrc[target_y*src_pitch+target_x*4+2]-psrc[source_y*src_pitch+source_x*4+2];

							unsigned int targetcolor = *(intsrc + target_y*intsrc_pitch + target_x);
							unsigned int srccolor = *(intsrc + source_y*intsrc_pitch + source_x);
							int temp_b = (int)(targetcolor & 0xFF) - (int)(srccolor & 0xFF);
							int temp_g = (int)((targetcolor>>8) & 0xFF) - (int)((srccolor>>8) & 0xFF);
							int temp_r = (int)((targetcolor>>16) & 0xFF) - (int)((srccolor>>16) & 0xFF);

							sum += temp_r*temp_r + temp_g*temp_g + temp_b*temp_b; // SSD
						}
					}
				}
				if(sum<min)
				{
					min=sum;
					patch_x = i;
					patch_y = j;
				}
			}
		}
	}
	else if(pixel_format == RGB24)
	{
		for(int j = ymin; j<ymax; j++)
		{
			for(int i = xmin; i<xmax; i++)
			{
				if(m_source[j*m_width+i]==0)continue; // not good patch source
				sum=0;
				for(int iter_y=(-1)*winysize; iter_y<=winysize; iter_y++)
				{
					source_y = j+iter_y;
					target_y = y+iter_y;
					if(target_y<0||target_y>=m_height)continue;
					for(int iter_x=(-1)*winxsize; iter_x<=winxsize; iter_x++)
					{
						source_x = i+iter_x;
						target_x = x+iter_x;
						if(target_x<0||target_x>=m_width)continue;
						// it is the most time-comsuming part of code:
						if(m_mark[target_y*m_width+target_x]==SOURCE) // compare
						{
							int temp_b = psrc[target_y*src_pitch+target_x*3]-psrc[source_y*src_pitch+source_x*3];
							int temp_g = psrc[target_y*src_pitch+target_x*3+1]-psrc[source_y*src_pitch+source_x*3+1];
							int temp_r = psrc[target_y*src_pitch+target_x*3+2]-psrc[source_y*src_pitch+source_x*3+2];

							sum += temp_r*temp_r + temp_g*temp_g + temp_b*temp_b; // SSD
//							sum += abs(temp_r) + abs(temp_g) + abs(temp_b); // SAD
						}
					}
				}
				if(sum<min)
				{
					min=sum;
					patch_x = i;
					patch_y = j;
				}
			}
		}
	}

	else if(pixel_format == YV12)
	{
		for(int j = ymin; j<ymax; j++)
		{
			for(int i = xmin; i<xmax; i++)
			{
				if(m_source[j*m_width+i]==0)continue; // not good patch source
				sum=0;
				for(int iter_y=(-1)*winysize; iter_y<=winysize; iter_y++)
				{
					source_y = j+iter_y;
					target_y = y+iter_y;
					if(target_y<0||target_y>=m_height)continue;
					for(int iter_x=(-1)*winxsize; iter_x<=winxsize; iter_x++)
					{
						source_x = i+iter_x;
						target_x = x+iter_x;
						if(target_x<0||target_x>=m_width)continue;
						// it is the most time-comsuming part of code:
						if(m_mark[target_y*m_width+target_x]==SOURCE) // compare
						{
							int temp_y = psrc[target_y*src_pitch+target_x]-psrc[source_y*src_pitch+source_x];
							int temp_u = psrcU[(target_y>>1)*src_pitchUV+(target_x>>1)]-psrcU[(source_y>>1)*src_pitchUV+(source_x>>1)];
							int temp_v = psrcV[(target_y>>1)*src_pitchUV+(target_x>>1)]-psrcV[(source_y>>1)*src_pitchUV+(source_x>>1)];

							sum += temp_y*temp_y + temp_u*temp_u + temp_v*temp_v; // SSD
						}
					}
				}
				if(sum<min)
				{
					min=sum;
					patch_x = i;
					patch_y = j;
				}
			}
		}
	}

	if (min == MIN_INITIAL)
		return false; // patch not found
	else
		return true; // found
}

/*********************************************************************/
bool inpainting::update(int target_x, int target_y, int source_x, int source_y, int confid)
{
	// apply patch

	if(pixel_format==RGB32 || pixel_format == RGBA)
	{

		unsigned int * intsrc = reinterpret_cast<unsigned int *>(psrc); // faster access
		int intsrc_pitch = src_pitch/4; // in int

		int x0,y0,x1,y1;
		for(int iter_y=MAX(-winysize, -target_y); iter_y<=MIN(winysize, m_height-target_y-1); iter_y++)// add bound check - Fizick
		{
				y0 = source_y+iter_y;
				y1 = target_y + iter_y;

			for(int iter_x=MAX(-winxsize, -target_x); iter_x<=MIN(winxsize, m_width-target_x-1); iter_x++)// add bound check
			{
				x0 = source_x+iter_x;
				x1 = target_x + iter_x;

				if(m_mark[y1*m_width+x1]!=SOURCE)
				{
					m_mark[y1*m_width+x1] = SOURCE; // now filled
					m_gray[y1*m_width+x1] = m_gray[y0*m_width+x0]; // inpaint the gray
					m_confid[y1*m_width+x1] = confid; // update the confidence
					*(intsrc + y1*intsrc_pitch + x1) = *(intsrc + y0*intsrc_pitch + x0);// inpaint the color and alpha
				}
			}
		}
	}
	else if (pixel_format==RGB24)
	{

		int x0,y0,x1,y1;
		for(int iter_y=MAX(-winysize, -target_y); iter_y<=MIN(winysize, m_height-target_y-1); iter_y++)// add bound check - Fizick
		{
				y0 = source_y+iter_y;
				y1 = target_y + iter_y;

			for(int iter_x=MAX(-winxsize, -target_x); iter_x<=MIN(winxsize, m_width-target_x-1); iter_x++)// add bound check
			{
				x0 = source_x+iter_x;
				x1 = target_x + iter_x;

				if(m_mark[y1*m_width+x1]!=SOURCE)
				{
					m_mark[y1*m_width+x1] = SOURCE; // now filled
					m_gray[y1*m_width+x1] = m_gray[y0*m_width+x0]; // inpain the gray
					m_confid[y1*m_width+x1] = confid; // update the confidence
					*(psrc + y1*src_pitch + x1*3) = *(psrc + y0*src_pitch + x0*3);// inpaint the color B
					*(psrc + y1*src_pitch + x1*3+1) = *(psrc + y0*src_pitch + x0*3+1);// inpaint the color G
					*(psrc + y1*src_pitch + x1*3+2) = *(psrc + y0*src_pitch + x0*3+2);// inpaint the color R
				}
			}
		}
	}
	else if (pixel_format==YV12)
	{

		int x0,y0,x1,y1;
		for(int iter_y=MAX(-winysize, -target_y); iter_y<=MIN(winysize, m_height-target_y-1); iter_y++)// add bound check - Fizick
		{
				y0 = source_y+iter_y;
				y1 = target_y + iter_y;

			for(int iter_x=MAX(-winxsize, -target_x); iter_x<=MIN(winxsize, m_width-target_x-1); iter_x++)// add bound check
			{
				x0 = source_x+iter_x;
				x1 = target_x + iter_x;

				if(m_mark[y1*m_width+x1]!=SOURCE)
				{
					m_mark[y1*m_width+x1] = SOURCE; // now filled
					m_confid[y1*m_width+x1] = confid; // update the confidence
					*(psrc + y1*src_pitch + x1) = *(psrc + y0*src_pitch + x0);// inpaint Y
					// gray is impainted as luma Y
					*(psrcU + (y1>>1)*src_pitchUV + (x1>>1)) = *(psrcU + (y0>>1)*src_pitchUV + (x0>>1));// inpaint the U
					*(psrcV + (y1>>1)*src_pitchUV + (x1>>1)) = *(psrcV + (y0>>1)*src_pitchUV + (x0>>1));// inpaint the V
				}
			}
		}
	}
	return true;
}

/*********************************************************************/
bool inpainting::TargetExist(void)
{
		for(int j= m_top; j<=m_bottom; j++)
			for(int i = m_left; i<= m_right; i++)
				if(m_mark[j*m_width+i]!=SOURCE)
					return true;
	return false;
}

/*********************************************************************/
void inpainting::UpdateBoundary(int i, int j)// just update the area near the changed patch. (+-2 pixels)
{
	int x, y;

	for(y = MAX(j -winysize-2,0); y<= MIN(j+winysize+2,m_height-1); y++)
		for( x = MAX(i-winxsize-2,0); x<=MIN(i+winxsize+2, m_width-1); x++)
		{
            if (m_mark[y*m_width+x]!=SOURCE)// was target or boundary and was not patched
			    m_mark[y*m_width+x] = TARGET;
		}

	for(y = MAX(j -winysize-2,0); y<= MIN(j+winysize+2,m_height-1); y++)
		for( x = MAX(i-winxsize-2,0); x<=MIN(i+winxsize+2, m_width-1); x++)
		{
			if(m_mark[y*m_width+x]==TARGET)
			{
				if(y==m_height-1||y==0||x==0||x==m_width-1
					|| m_mark[(y-1)*m_width+x]==SOURCE || m_mark[y*m_width+x-1]==SOURCE
					|| m_mark[y*m_width+x+1]==SOURCE || m_mark[(y+1)*m_width+x]==SOURCE)
						m_mark[y*m_width+x] = BOUNDARY;
			}
		}
}

/*********************************************************************/
void inpainting::UpdatePri(int i, int j) // just update the area near the changed patch. (+-3 pixels)
{
	int x,y;
	for(y = MAX(j -winysize-3,0); y<= MIN(j+winysize+3,m_height-1); y++)
		for( x = MAX(i-winxsize-3,0); x<=MIN(i+winxsize+3, m_width-1); x++)
			if(m_mark[y*m_width+x] == BOUNDARY)
				m_pri[y*m_width+x] = priority(x,y);

}
