#pragma once

/*Author: Qiushuang Zhang */
/*E-mail: qszhang@cc.gatech.edu */
/*Nov.29, 2005 */

/* Modified version:
(c)2008 Alexander Balakhnin (Fizick) http://avisynth.org.ru under GNU GPL
*/

#ifndef INPAINTING_H
#define INPAINTING_H

#define SOURCE 0
#define TARGET 1
#define BOUNDARY 2
//#define WINSIZE 4  // the window size

// pixel_formats
#define RGBA 33
#define RGB32 32
#define RGB24 24
#define YV12 12
#define YUY2 2

typedef struct
{
	int grad_x;
	int grad_y;
}gradient; //the structure that record the gradient

typedef struct
{
	int norm_x;
	int norm_y;
}Mnorm;  // the structure that record the norm

class inpainting
{
public:

	int m_width; // image width
	int m_height; // image height
	int pixel_format;

	int maskcolor;

	unsigned char * psrc;
	int src_pitch;
	unsigned char *psrcU;
	unsigned char *psrcV;
	int src_pitchUV;
	const unsigned char * pmask;
	int mask_pitch;
	const unsigned char *pmaskU;
	const unsigned char *pmaskV;
	int mask_pitchUV;
	int winxsize;
	int winysize;
	int radius; // search radius (0 - full frame)

	int m_top, m_bottom, m_left, m_right; // the rectangle of inpaint area


	unsigned char * m_mark;// mark it as source or to-be-inpainted target area or boundary.
	int * m_confid;// record the confidence for every pixel
	int * m_pri; // record the priority for pixels. only boudary pixels will be used
	unsigned char * m_gray; // the gray image
	unsigned char * m_source; // whether this pixel can be used as an example texture center

	inpainting(int _width, int _height, int _pixel_format);
	~inpainting(void);
	int process(unsigned char * _psrc, int _src_pitch, const unsigned char * _pmask, int _mask_pitch,
					   int _xsize, int _ysize, int _radius, int _maskcolor, int _maxsteps);  // the main function to process the whole image
	int process3planes(unsigned char * _psrc, int _src_pitch,
					   unsigned char * _psrcU, int _src_pitchU,
					   unsigned char * _psrcV,
						const unsigned char * _maskp, int _mask_pitch,
						const unsigned char * _maskpU, int _mask_pitchU,
						const unsigned char * _maskpV,
					   int _xsize, int _ysize, int _radius, int _maskcolor, int _maxsteps);
	void DrawBoundary(void);  // the first time to draw boundary on the image.
	void GetMask(void);// fist time mask
	int ComputeConfidence(int i, int j); // the function to compute confidence
	int priority(int x, int y); // the function to compute priority
	int ComputeData(int i, int j);//the function to compute data item
	void Convert2Gray(void);  // convert the input image to gray image
	gradient GetGradient(int i, int j); // calculate the gradient at one pixel
	Mnorm GetNorm(int i, int j);  // calculate the norm at one pixel
	bool draw_source(void);  // find out all the pixels that can be used as an example texture center
	bool PatchTexture(int x, int y,int &patch_x,int &patch_y);// find the most similar patch from sources.
	bool update(int target_x, int target_y, int source_x, int source_y, int confid);// inpaint this patch and update pixels' confidence within this area
	bool TargetExist(void);// test whether this is still some area to be inpainted.
	void UpdateBoundary(int i, int j);// update boundary
	void UpdatePri(int i, int j); //update priority for boundary pixels.
};


#endif
