#pragma once
#include "ImageHandler.h"
#include <vector>
using namespace std;
class ImageHandler;
class ImageSeamOperations {
public:ImageHandler *img;
       ImageHandler *energyAddon;
	   ImageSeamOperations(ImageHandler *img, ImageHandler * energyAddon);
	   void setImage(ImageHandler * bmp);
	   void setMask(ImageHandler *bmp);
	   void setHalfMask(int y1,int y2);
	   ImageSeamOperations(ImageHandler * img);
	  // VerticalSeam* findOptimalVerticalSeam();
	   void findOptimalVerticalSeam(int&seamstart,int**tablecur,int**tablenex,int lowwidth,int hightwidth);
	  // HorizontalSeam* findOptimalHorizontalSeam();
	   void findOptimalHorizontalSeam(int& seamstart,int**table,int**tablenex,int lowheight,int highheight);
	   void createHorizontalSeamTable(ImageHandler* energyImage,int**tablecur,int**tablenex);
	   void carveVerticalSeam(int lowwidth,int highwidth);
	   void carveHorizontalSeam(int lowheight,int highheight);
	   void addWidth(int seamnum,int lowwidth,int highwidth);
	   void decWidth(int seamnum,int lowwidth,int highwidth);
	   void addHeight(int seamnum,int lowheight,int highheight);
	   void decHeight(int seamnum,int lowheight,int highheight);
	   void SeamCarve(int nw,int nh,int lowwidth,int highwidth,int lowheight,int highheight);
	   void SeamCarveMe(int nw,int nh,int lowwidth,int highwidth,int lowheight,int highheight);
	   void createVerticalSeamTable(ImageHandler* energyImage,int**tablecur,int**tablenex);
	   ImageHandler* generateEnergyImage();
       ImageHandler* getImg();
	   ~ImageSeamOperations();
};