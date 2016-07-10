#pragma once
#include"cv.h"
#include"highgui.h"

class ImageHandler{
public:
	IplImage *root;
	int setImage(IplImage *img);
	int setMask(IplImage *img);
	int getPixel(int x,int y);
	int setPixel(int x,int y,int color);
    int getRowPixels(int pixels[],int row);
	int getColPixels(int pixels[],int col);
	int setRowPixels(int pixels[],int row);
	int setColPixels(int pixels[],int col);
	int getGrayPixel(int x,int y);
	int setGrayPixel(int x,int y,int color);
    int getGrayRowPixels(int pixels[],int row);
	int getGrayColPixels(int pixels[],int col);
	int setGrayRowPixels(int pixels[],int row);
	int setGrayColPixels(int pixels[],int col);
	int templat(int *templt,int tw,int x,int y);
	int colortemplat(int *templt,int tw,int x,int y,int flag);
	void sobel();
	void Lapcial();
	ImageHandler(IplImage*root);
	ImageHandler();
	~ImageHandler();
};