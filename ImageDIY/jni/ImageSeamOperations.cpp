#include "ImageSeamOperations.h"
#include <cstdlib>
#include <stdio.h>
#include <math.h>
#include <vector>
using namespace std;
void ImageSeamOperations::SeamCarveMe(int nw,int nh,int lowwidth,int highwidth,int lowheight,int highheight){
int width=img->root->width;
int height=img->root->height;
  int fnw;
  int fnh;
  if(nw>width){
     fnw = nw-width;
	 addWidth(fnw,lowwidth,highwidth);
  }
  else{
	  fnw=width-nw;
	  decWidth(fnw,lowwidth,highwidth);
  }
  if(nh>height){
     fnh = nh-height;
	 addHeight(fnh,lowheight,highheight);
  }
  else{
	  if(nh<height){
      fnh=height-nh;
	  decHeight(fnh,lowheight,highheight);}
  }
}
void ImageSeamOperations::SeamCarve(int nw,int nh,int lowwidth,int highwidth,int lowheight,int highheight){
int width=img->root->width;
int height=img->root->height;
  int fnw;
  int fnh;
  if(nw>width){
     fnw = nw-width;
	 addWidth(fnw,lowwidth,highwidth);
  }
  if(nh>height){
     fnh = nh-height;
	 addHeight(fnh,lowheight,highheight);
  }
  if(nw<width||nh<height){
  fnh=height-nh;
  fnw=width-nw;
  if(fnh<0)
	  fnh=0;
  if(fnw<0)
	  fnw=0;
  int h = fnh, w=fnw;
    for (int j = 0; j< max(fnh, fnw); j++) {
                    if (h>0) {
                        carveHorizontalSeam(lowheight,highheight);
						//cvShowImage("Example1",s.getImg()->root);
                        h--;
                    }
                   if (w>0) {
                        carveVerticalSeam(lowwidth,highwidth);
						//cvShowImage("Example1",s.getImg()->root);
                        w--;
                    }
                    
                }


  }
}
ImageHandler* ImageSeamOperations::getImg() {
        return img;
    }
ImageSeamOperations::ImageSeamOperations(ImageHandler *img, ImageHandler * energyAddon) {
        this->img = img;
        this->energyAddon = energyAddon;     
    }
ImageSeamOperations::~ImageSeamOperations(){
	if(img!=0)
		delete img;
	if(energyAddon!=0)
		delete energyAddon;
}
void ImageSeamOperations::setImage(ImageHandler * bmp){
	img->root=cvCloneImage(bmp->root);
    }
void ImageSeamOperations::setMask(ImageHandler *bmp){
	energyAddon->root=cvCloneImage(bmp->root);
}
void ImageSeamOperations::setHalfMask(int starty,int endy){
	int i=0,j=0;
	if(starty<0)
		starty=0;
	if(endy>energyAddon->root->height-1)
		endy=energyAddon->root->height-1;
	for(i=starty;i<endy;i++)
		for(j=0;j<energyAddon->root->width;j++)
			energyAddon->setGrayPixel(j,i,255);

}
ImageSeamOperations::ImageSeamOperations(ImageHandler * img) {
        this->img = img;
        energyAddon=0;
    }
void ImageSeamOperations::findOptimalVerticalSeam(int&seamstart,int**tablecur,int**tablenex,int lowwidth,int highwidth) {
       ImageHandler *energyImg = generateEnergyImage();
       int width=energyImg->root->width;
       int height=energyImg->root->height;
       createVerticalSeamTable(energyImg,tablecur,tablenex);
	   int energy = img->root->height*255;
         seamstart = 0;
		 if(lowwidth<1)
			 lowwidth=1;
		 if(lowwidth>width-1)
			 lowwidth=width-1;
		 if(highwidth>width-1)
			 highwidth=width-1;
		 if(highwidth<0)
			 highwidth=1;
		 for (int i = lowwidth; i < highwidth; i++) {          
			if(tablecur[i][height - 1]<energy){
				energy=tablecur[i][height - 1];
				seamstart=i;
			}
        }
        delete energyImg;
    }

void ImageSeamOperations::findOptimalHorizontalSeam(int& seamstart,int**tablecur,int**tablenex,int lowheight,int highheight) {
        ImageHandler* energyImg = generateEnergyImage();
		int width=energyImg->root->width;
       int height=energyImg->root->height;	
        createHorizontalSeamTable(energyImg,tablecur,tablenex);
   // printf("create seam\n");
       float energy = img->root->width* 255;
		//double energy=0xffffffff;
         seamstart = 0;
		 int i=0;
		 if(lowheight<1)
			 lowheight=1;
		 if(lowheight>height-1)
			 lowheight=height-1;
		 if(highheight>height-1)
			 highheight=height-1;
		 if(highheight<0)
			 highheight=1;
        for (int i = lowheight; i < highheight; i++) {
            if (tablecur[width - 1][i]< energy) {
                energy = tablecur[width - 1][i];
                seamstart = i;
            }
        }

		delete energyImg;
	
    }
void ImageSeamOperations::createHorizontalSeamTable(ImageHandler* energyImage,int**tablecur,int**tablenex) {
        
        int h = energyImage->root->height;
        int w = energyImage->root->width;
		int *pixelCol= new int[h];
        energyImage->getGrayColPixels(pixelCol,0);
       
        for (int y = 0; y < h; y++) {
            tablecur[0][y]= pixelCol[y];

		//	printf("%d ",pixelCol[y]);
			tablenex[0][y]=0;

        }
        // now for the following rows find the next component in the seam
        int right, left, min, middle, currentEnergy;
        int nextStep;
        for (int x = 1; x < w; x++) {
          energyImage->getGrayColPixels(pixelCol,x);
        //	getColPixels(energyImage, x, pixelCol);
            for (int y = 0; y < h; y++) {
                // find 'next in path'
                middle = tablecur[x - 1][y];//result[x - 1][y][0];
                currentEnergy = pixelCol[y];
			 //   printf("%d ",pixelCol[y]);
                nextStep = 0;
                if (y > 1 && y < h - 2) {
                    left = tablecur[x - 1][y - 1];//result[x - 1][y - 1][0];
                    right = tablecur[x - 1][y + 1];//result[x - 1][y + 1][0];
                    nextStep = 0;
                    if (right < middle || left < middle) {
                        if (right <= left) {
                            min = right;
                            nextStep = 1;
                        } else {
                            min = left;
                            nextStep = -1;
                        }
                    } else {
                        min = middle;
                    }
				//	printf("curr%d min%d ",(int)currentEnergy,(int)min);
                    currentEnergy = currentEnergy + min;
				//	printf("currentEnergy1 ");
                } else if (y > 1) { // left border of the image, no left possible
                    right = tablecur[x - 1][y - 1];//result[x - 1][y - 1][0];
                    if (right < middle) {
                        min = right;
                        nextStep = -1;
                    } else {
                        min = middle;
                        nextStep = 0;
                    }
				//	printf("cur%d min%d ",(int)currentEnergy,(int)min);
                    currentEnergy = currentEnergy + min;
					//printf("currentEnergy2 ");
                } else if (y < h - 2) { // right border of the image, no right possible
                    left =tablecur[x - 1][y + 1]; //result[x - 1][y + 1][0];
                    if (left < middle) {
                        min = left;
                        nextStep = 1;
                    } else {
                        min = middle;
                        nextStep = 0;
                    }
				//	printf("cur%d min%d ",(int)currentEnergy,(int)min);
                    currentEnergy = currentEnergy + min;
                }
               tablecur[x][y] = currentEnergy;// result[x][y][0] = currentEnergy;
               tablenex[x][y] = nextStep;// result[x][y][1] = nextStep;
			 //  printf("%d->%d  ",(int)currentEnergy,(int)nextStep);
			   //printf("%f ",tablenex[x][y]);
            }
        }

        delete pixelCol;
    }
void SortPath(int **m_path,int seamNum,int size){
    int i,j,k;
	int temp;
	for(k=0;k<size;k++)
	{
		for(i=1;i<seamNum;i++)
		{
			temp=m_path[i][k];	
			for(j=i-1;j>=0;j--)
			{
				if(m_path[j][k]>temp)
				{
					m_path[j+1][k]=m_path[j][k];
				}
				else
				{
					m_path[j+1][k]=temp;
					break;
				}
			}
			if(j==-1)
				m_path[0][k]=temp;
		}
	}
}
void ImageSeamOperations::decWidth(int seamnum,int lowwidth,int highwidth){
	    ImageHandler* energyImg = generateEnergyImage();
	    int seamstart;
		int**tablecur;
		int**tablenex;
		int height = img->root->height;
        int width=img->root->width;
		tablecur=new int*[width];
	    tablenex=new int*[width];
	   int i=0;
	   for(i=0;i<width;i++){
		   tablecur[i]=new int[height];
		   tablenex[i]=new int[height];
	   }
	   int **m_SomeMinPath=new int*[seamnum];
	   for(i=0;i<seamnum;i++)
		   m_SomeMinPath[i]=new int[height];
	   int*seamLookup = new int[height];	
		int j;
		for(j=0;j<seamnum;j++){		
			createVerticalSeamTable(energyImg,tablecur,tablenex);
		   //cvNamedWindow( "Example3",CV_WINDOW_AUTOSIZE);
		  // cvShowImage("Example3",energyImg->root);
		   int energy = img->root->height*255*255;
         seamstart = 0;
		if(lowwidth<1)
			 lowwidth=1;
		 if(lowwidth>width-1)
			 lowwidth=width-1;
		 if(highwidth>width-1)
			 highwidth=width-1;
		 if(highwidth<0)
			 highwidth=1;
        for ( i = lowwidth; i < highwidth; i++) {          
			if(tablecur[i][height - 1]<energy){
				energy=tablecur[i][height - 1];
				seamstart=i;
			}
        }
        int nextX = seamstart;//s->startX;
	//	printf("seamstart%d ",seamstart);
        seamLookup[height - 1] = nextX;
		int y;
        for (y = height - 2; y >=0; y--) {
            nextX += tablenex[nextX][y];
            seamLookup[y] = nextX;		
        }
		int k;
		for(k=0;k<height;k++){
           m_SomeMinPath[j][k]=seamLookup[k];
		   energyImg->setGrayPixel(seamLookup[k],k,255);
		}
		
		}
		SortPath(m_SomeMinPath,seamnum,height);
        IplImage* cvtarget=cvCreateImage(cvSize(width-seamnum,height),img->root->depth,img->root->nChannels);
        ImageHandler *target=new ImageHandler(cvtarget);
		int row,col;
	    int tempSeamNum;
	    int position;
	for(row=0;row<height;row++)  //前m_height_bef-1行
	{
		tempSeamNum=seamnum;
		position=width-1;
		int j=0;
		for(j=tempSeamNum-1;j>=0;j--)
		{
			for(col=position;col>m_SomeMinPath[j][row];col--)
			{
				//printf("col%d ",col);
				//printf("tempSeamNum%d ",tempSeamNum);
				//printf("row%d ",row);
				int newcol=col-tempSeamNum;
				if(newcol<0)
					newcol=0;
				target->setPixel(newcol,row,img->getPixel(col,row));
				
			}
			//if(m_SomeMinPath[j][row]==m_SomeMinPath[j+1][row])
			col--;
			//target->setPixel(col+tempSeamNum,row,img->getPixel(col+1,row));
			tempSeamNum--;
			position=col;
		}
		for(col=position;col>=0;col--)
			{
				target->setPixel(col,row,img->getPixel(col,row));
			}
	}
		setImage(target);
		if (energyAddon!=0) {
		IplImage* cvenergytarget=cvCreateImage(cvSize(width-seamnum,height),energyAddon->root->depth,energyAddon->root->nChannels);
        ImageHandler *energytarget=new ImageHandler(cvenergytarget);
	for(row=0;row<height;row++)  //前m_height_bef-1行
	{
		tempSeamNum=seamnum;
		position=width-1;
		int j=0;
		for(j=tempSeamNum-1;j>=0;j--)
		{
			for(col=position;col>m_SomeMinPath[j][row];col--)
			{
				
				int newcol=col-tempSeamNum;
				if(newcol<0)
					newcol=0;
				energytarget->setGrayPixel(newcol,row,energyAddon->getGrayPixel(col,row));
				
			}
			//if(m_SomeMinPath[j][row]==m_SomeMinPath[j+1][row])
			col--;
			//target->setPixel(col+tempSeamNum,row,img->getPixel(col+1,row));
			tempSeamNum--;
			position=col;
		}
		for(col=position;col>=0;col--)
			{
				energytarget->setGrayPixel(col,row,energyAddon->getGrayPixel(col,row));
			}
	}
		setMask(energytarget);
		delete energytarget;
        	 }
		delete target;
		delete tablecur;
		delete tablenex;
		delete energyImg;
		delete m_SomeMinPath;
		delete seamLookup;
}
void ImageSeamOperations::addWidth(int seamnum,int lowwidth,int highwidth){
	    ImageHandler* energyImg = generateEnergyImage();
	    int seamstart;
		int**tablecur;
		int**tablenex;
		int height = img->root->height;
        int width=img->root->width;
		tablecur=new int*[width];
	    tablenex=new int*[width];
	   int i=0;
	   for(i=0;i<width;i++){
		   tablecur[i]=new int[height];
		   tablenex[i]=new int[height];
	   }
	   int **m_SomeMinPath=new int*[seamnum];
	   for(i=0;i<seamnum;i++)
		   m_SomeMinPath[i]=new int[height];
	   int*seamLookup = new int[height];	
		int j;
		for(j=0;j<seamnum;j++){		
			createVerticalSeamTable(energyImg,tablecur,tablenex);
			//cvNamedWindow( "Example5",CV_WINDOW_AUTOSIZE);
		  // cvShowImage("Example5",energyImg->root);
		   int energy = img->root->height*255;
         seamstart = 0;
		if(lowwidth<1)
			 lowwidth=1;
		 if(lowwidth>width-1)
			 lowwidth=width-1;
		 if(highwidth>width-1)
			 highwidth=width-1;
		 if(highwidth<0)
			 highwidth=1;
        for ( i = lowwidth; i < highwidth; i++) {          
			if(tablecur[i][height - 1]<energy){
				energy=tablecur[i][height - 1];
				seamstart=i;
			}
        }
        int nextX = seamstart;//s->startX;
	//	printf("seamstart%d ",seamstart);
        seamLookup[height - 1] = nextX;
		int y;
        for (y = height - 2; y >=0; y--) {
            nextX += tablenex[nextX][y];
            seamLookup[y] = nextX;		
        }
		int k;
		for(k=0;k<height;k++){
           m_SomeMinPath[j][k]=seamLookup[k];
		   energyImg->setGrayPixel(seamLookup[k],k,255);
		}
		
		}
		SortPath(m_SomeMinPath,seamnum,height);
        IplImage* cvtarget=cvCreateImage(cvSize(width+seamnum,height),img->root->depth,img->root->nChannels);
        ImageHandler *target=new ImageHandler(cvtarget);
		int x,y;
		for(y=0;y<height;y++)
			for(x=0;x<width;x++)
				target->setPixel(x,y,img->getPixel(x,y));
		int row,col;
	    int tempSeamNum;
	    int position;
	for(row=0;row<height;row++)  //前m_height_bef-1行
	{
		tempSeamNum=seamnum;
		position=width-1;
		for(i=tempSeamNum-1;i>=0;i--)
		{
			for(col=position;col>=m_SomeMinPath[i][row];col--)
			{
				target->setPixel(col+tempSeamNum,row,img->getPixel(col,row));
			}
			target->setPixel(col+tempSeamNum,row,img->getPixel(col+1,row));
			tempSeamNum--;
			position=col;
		}
	}
		setImage(target);
		if(energyAddon!=0){
         IplImage* cvenergytarget=cvCreateImage(cvSize(width+seamnum,height),energyAddon->root->depth,energyAddon->root->nChannels);
        ImageHandler *energytarget=new ImageHandler(cvenergytarget);
		for(y=0;y<height;y++)
			for(x=0;x<width;x++)
				energytarget->setGrayPixel(x,y,energyAddon->getGrayPixel(x,y));
	for(row=0;row<height;row++)  //前m_height_bef-1行
	{
		tempSeamNum=seamnum;
		position=width-1;
		for(i=tempSeamNum-1;i>=0;i--)
		{
			for(col=position;col>=m_SomeMinPath[i][row];col--)
			{
				energytarget->setGrayPixel(col+tempSeamNum,row,energyAddon->getGrayPixel(col,row));
			}
			energytarget->setGrayPixel(col+tempSeamNum,row,energyAddon->getGrayPixel(col+1,row));
			tempSeamNum--;
			position=col;
		}
	}
		setMask(energytarget);
		delete energytarget;
		}
		delete target;
		delete tablecur;
		delete tablenex;
		delete energyImg;
		delete m_SomeMinPath;
		delete seamLookup;
}
 void ImageSeamOperations::decHeight(int seamnum,int lowheight,int highheight){
	    ImageHandler* energyImg = generateEnergyImage();
	    int seamstart;
		int**tablecur;
		int**tablenex;
		int height = img->root->height;
        int width=img->root->width;
		tablecur=new int*[width];
	    tablenex=new int*[width];
	   int i=0;
	   for(i=0;i<width;i++){
		   tablecur[i]=new int[height];
		   tablenex[i]=new int[height];
	   }
	   int **m_SomeMinPath=new int*[seamnum];
	   for(i=0;i<seamnum;i++)
		   m_SomeMinPath[i]=new int[width];
	   int*seamLookup = new int[width];
		
		int j;
		for(j=0;j<seamnum;j++){		
			int energy = img->root->width*255*255;
         seamstart = 0;
			createHorizontalSeamTable(energyImg,tablecur,tablenex);
			//cvNamedWindow( "Example4",CV_WINDOW_AUTOSIZE);
		   //cvShowImage("Example4",energyImg->root);
		    if(lowheight<1)
			 lowheight=1;
		 if(lowheight>height-1)
			 lowheight=height-1;
		 if(highheight>height-1)
			 highheight=height-1;
		 if(highheight<0)
			 highheight=1;
		   for ( i = lowheight; i < highheight; i++) {          
			if(tablecur[width-1][i]<energy){
				energy=tablecur[width-1][i];
				seamstart=i;
			}
        }
        int nextY = seamstart;//s->startY;
	//	printf("%d seamstart ",seamstart);
        seamLookup[width - 1] = nextY;
        for (int x = width - 2; x >= 0; x--) {
		//	printf("%d ",x);
            nextY += (int)tablenex[x][nextY];// s->table[x][nextY][1];
            seamLookup[x] = nextY;
			//printf("nextY%d ",nextY);
        }
		int k;
		for(k=0;k<width;k++){
           m_SomeMinPath[j][k]=seamLookup[k];
		   energyImg->setGrayPixel(k,seamLookup[k],255);
		}
		
		}
		SortPath(m_SomeMinPath,seamnum,width);
        IplImage* cvtarget=cvCreateImage(cvSize(width,height-seamnum),img->root->depth,img->root->nChannels);
        ImageHandler *target=new ImageHandler(cvtarget);
		int row,col;
	    int tempSeamNum;
	    int position;
	for(col=0;col<width;col++)  //
	{
		tempSeamNum=seamnum;
		position=height-1;
		int j=0;
		for(j=tempSeamNum-1;j>=0;j--)
		{
			for(row=position;row>m_SomeMinPath[j][col];row--)
			{		
				int newrow=row-tempSeamNum;
				if(newrow<0)
					newrow=0;
				target->setPixel(col,newrow,img->getPixel(col,row));
				
			}
			//if(m_SomeMinPath[j][row]==m_SomeMinPath[j+1][row])
			row--;
			//target->setPixel(col+tempSeamNum,row,img->getPixel(col+1,row));
			tempSeamNum--;
			position=row;
		}
		for(row=position;row>=0;row--)
			{
				target->setPixel(col,row,img->getPixel(col,row));
			}
	}
		setImage(target);
		if(energyAddon!=0)
		{
		IplImage* cvenergytarget=cvCreateImage(cvSize(width,height-seamnum),energyAddon->root->depth,energyAddon->root->nChannels);
        ImageHandler *energytarget=new ImageHandler(cvenergytarget);
	for(col=0;col<width;col++)  //
	{
		tempSeamNum=seamnum;
		position=height-1;
		int j=0;
		for(j=tempSeamNum-1;j>=0;j--)
		{
			for(row=position;row>m_SomeMinPath[j][col];row--)
			{		
				int newrow=row-tempSeamNum;
				if(newrow<0)
					newrow=0;
				energytarget->setGrayPixel(col,newrow,energyAddon->getGrayPixel(col,row));
				
			}
			//if(m_SomeMinPath[j][row]==m_SomeMinPath[j+1][row])
			row--;
			//target->setPixel(col+tempSeamNum,row,img->getPixel(col+1,row));
			tempSeamNum--;
			position=row;
		}
		for(row=position;row>=0;row--)
			{
				energytarget->setGrayPixel(col,row,energyAddon->getGrayPixel(col,row));
			}
	}
		setMask(energytarget);
		delete energytarget;
		}
		delete target;
		delete tablecur;
		delete tablenex;
		delete energyImg;
		delete m_SomeMinPath;
		delete seamLookup;
 }
void ImageSeamOperations::addHeight(int seamnum,int lowheight,int highheight){
	    ImageHandler* energyImg = generateEnergyImage();
	    int seamstart;
		int**tablecur;
		int**tablenex;
		int height = img->root->height;
        int width=img->root->width;
		tablecur=new int*[width];
	    tablenex=new int*[width];
	   int i=0;
	   for(i=0;i<width;i++){
		   tablecur[i]=new int[height];
		   tablenex[i]=new int[height];
	   }
	   int **m_SomeMinPath=new int*[seamnum];
	   for(i=0;i<seamnum;i++)
		   m_SomeMinPath[i]=new int[width];
	   int*seamLookup = new int[width];
		
		int j;
		for(j=0;j<seamnum;j++){		
			int energy = img->root->width*255;
         seamstart = 0;
			createHorizontalSeamTable(energyImg,tablecur,tablenex);
			//cvNamedWindow( "Example4",CV_WINDOW_AUTOSIZE);
		  // cvShowImage("Example4",energyImg->root);
		   if(lowheight<1)
			 lowheight=1;
		 if(lowheight>height-1)
			 lowheight=height-1;
		 if(highheight>height-1)
			 highheight=height-1;
		 if(highheight<0)
			 highheight=1;
		   for ( i = lowheight; i < highheight; i++) {          
			if(tablecur[width-1][i]<energy){
				energy=tablecur[width-1][i];
				seamstart=i;
			}
        }
        int nextY = seamstart;//s->startY;
	//	printf("%d seamstart ",seamstart);
        seamLookup[width - 1] = nextY;
        for (int x = width - 2; x >= 0; x--) {
		//	printf("%d ",x);
            nextY += (int)tablenex[x][nextY];// s->table[x][nextY][1];
            seamLookup[x] = nextY;
			//printf("nextY%d ",nextY);
        }
		int k;
		for(k=0;k<width;k++){
           m_SomeMinPath[j][k]=seamLookup[k];
		   energyImg->setGrayPixel(k,seamLookup[k],255);
		}
		
		}
		SortPath(m_SomeMinPath,seamnum,width);
        IplImage* cvtarget=cvCreateImage(cvSize(width,height+seamnum),img->root->depth,img->root->nChannels);
        ImageHandler *target=new ImageHandler(cvtarget);
		int x,y;
		for(y=0;y<height;y++)
			for(x=0;x<width;x++)
				target->setPixel(x,y,img->getPixel(x,y));
		int row,col;
	int tempSeamNum;
	int position;
	for(col=0;col<width;col++)  //前m_width_bef-1列
	{
		tempSeamNum=seamnum;
		position=height-1;
		for(i=tempSeamNum-1;i>=0;i--)
		{
			for(row=position;row>=m_SomeMinPath[i][col];row--)
			{
				target->setPixel(col,row+tempSeamNum,img->getPixel(col,row));
			}
			target->setPixel(col,row+tempSeamNum,img->getPixel(col,row+1));
			tempSeamNum--;
			position=row;
		}
	}
		setImage(target);
		cvReleaseImage(&cvtarget);
		/*if(energyAddon!=0)
		{
           IplImage* cvenergytarget=cvCreateImage(cvSize(width,height+seamnum),energyAddon->root->depth,energyAddon->root->nChannels);
        ImageHandler *energytarget=new ImageHandler(cvenergytarget);
		for(y=0;y<height;y++)
			for(x=0;x<width;x++)
				energytarget->setGrayPixel(x,y,energyAddon->getGrayPixel(x,y));
	for(col=0;col<width;col++)  //前m_width_bef-1列
	{
		tempSeamNum=seamnum;
		position=height-1;
		for(i=tempSeamNum-1;i>=0;i--)
		{
			for(row=position;row>=m_SomeMinPath[i][col];row--)
			{
				energytarget->setGrayPixel(col,row+tempSeamNum,energyAddon->getGrayPixel(col,row));
			}
			energytarget->setGrayPixel(col,row+tempSeamNum,energyAddon->getGrayPixel(col,row+1));
			tempSeamNum--;
			position=row;
		}
	}
		setMask(energytarget);
		delete energytarget;
		}*/
		delete target;
		delete tablecur;
		delete tablenex;
		delete energyImg;
		delete m_SomeMinPath;
		delete seamLookup;
}
void ImageSeamOperations::carveVerticalSeam(int lowwidth,int highwidth) {
        int seamstart;
		//vector<vector<vector<float>>> table;
		int**tablecur;
		int**tablenex;
		int height = img->root->height;
        int width=img->root->width;
		 tablecur=new int*[width];
	   tablenex=new int*[width];
	   int i=0;
	   for(i=0;i<width;i++){
		   tablecur[i]=new int[height];
		   tablenex[i]=new int[height];
	   }
	    findOptimalVerticalSeam(seamstart,tablecur,tablenex,lowwidth,highwidth);
        
        // lookup table for the seam coordinates: y -> x as result of the backtracking.
        int*seamLookup = new int[height];
        // backtracking:
        int nextX = seamstart;//s->startX;
	//	printf("startX%d ",nextX);
        seamLookup[height - 1] = nextX;
        for (int y = height - 2; y >=0; y--) {
		//	printf("%d ",y);
            nextX += tablenex[nextX][y];//table[nextX][y][1];
            seamLookup[y] = nextX;
			
        }
        IplImage* cvtarget=cvCreateImage(cvSize(width-1,height),img->root->depth,img->root->nChannels);
        ImageHandler *target=new ImageHandler(cvtarget);
		int pixel;
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
				
                pixel=img->getPixel(x, y);
			
                int targetX = x;
                if (seamLookup[y] != x) {
                    if (seamLookup[y] <= x) {
                        targetX--;
                    }
                    target->setPixel(targetX,y, pixel);
                }
            }
        }
		img->root=cvCloneImage(target->root);
		cvReleaseImage(&cvtarget);
		delete target;
		delete tablecur;
		delete tablenex;
       // img = target;

       /* if (energyAddon!=0) {
        	 // Bitmap energytarget=Bitmap.createBitmap(energyAddon.getWidth()-1,energyAddon.getHeight(),Bitmap.Config.ARGB_8888); 
        	 int Awidth=energyAddon->root->width;
        	 int Aheight=energyAddon->root->height;  
        	 IplImage* cvenergytarget=cvCreateImage(cvSize(Awidth-1,Aheight),energyAddon->root->depth,energyAddon->root->nChannels);
           ImageHandler *energytarget=new ImageHandler(cvenergytarget);      
            row = new int[width];
            newRow = new int[width- 1];
            for (int y = 0; y < height; y++) {
               // energyAddon.getPixels(0, y, energyAddon.getWidth(), 1, row);
               // energyAddon.getPixels(row,0,energyAddon.getWidth(),0,0,energyAddon.getWidth(),1);
               energyAddon->getRowPixels(row,y);
            	//getRowPixels(energyAddon,y,row);
                int skipX = seamLookup[y];
              //  System.arraycopy(row, 0, newRow, 0, skipX);
              //  System.arraycopy(row, (skipX + 1), newRow, skipX, (target.getWidth() - skipX - 1));
              memcpy(newRow,row,skipX*sizeof(int));
            memcpy(newRow+skipX*sizeof(int),row+(skipX+1)*sizeof(int),(energytarget->root->width-skipX-1)*sizeof(int));
                
               energytarget->setRowPixels(newRow,y);
                //setRowPixels(energytarget,y,newRow);
            }
			energyAddon->root=cvCloneImage(energytarget->root);
			delete energytarget;
           // energyAddon = energytarget;
        }*/
//		delete row;
//		delete newRow;
		delete seamLookup;
		//delete s;
    }

     void ImageSeamOperations::carveHorizontalSeam(int lowheight,int highheight) {
		int seamStart;
		//vector<vector<vector<float>>> table;
		int**tablecur=0;
		int**tablenex=0;
		int width = img->root->width;
        int height =img->root->height;
		tablecur=new int*[width];
	    tablenex=new int*[width];
		int i=0;
	   for(i=0;i<width;i++){
		   tablecur[i]=new int[height];
		   tablenex[i]=new int[height];
	   }

        findOptimalHorizontalSeam(seamStart,tablecur,tablenex,lowheight,highheight);
	//	printf("findOptimalHorizontalSeam\n");
//        int height = img.getHeight();
        
        // lookup table for the seam coordinates: x -> y as result of the backtracking.
        int *seamLookup = new int[width];
        // backtracking:
        int nextY = seamStart;//s->startY;
	//	printf("%d seamstart ",seamStart);
        seamLookup[width - 1] = nextY;
        for (int x = width - 2; x >= 0; x--) {
		//	printf("%d ",x);
            nextY += (int)tablenex[x][nextY];// s->table[x][nextY][1];
            seamLookup[x] = nextY;
			//printf("nextY%d ",nextY);
        }
        IplImage* cvtarget=cvCreateImage(cvSize(width,height-1),img->root->depth,img->root->nChannels);
        ImageHandler *target=new ImageHandler(cvtarget);
        int pixel;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
				
                pixel=img->getPixel(x, y);
			
                int targetY = y;
                if (seamLookup[x] != y) {
                    if (seamLookup[x] <= y) {
                        targetY--;
                    }
                    target->setPixel(x,targetY, pixel);
                }
            }
        }
		setImage(target);
		delete target;
		delete tablecur;
		delete tablenex;
		cvReleaseImage(&cvtarget);
       /* if (energyAddon!=0) {
        	int Awidth=energyAddon->root->width;
        	int Aheight=energyAddon->root->height;
        	IplImage* cvenergytarget=cvCreateImage(cvSize(Awidth,Aheight-1),energyAddon->root->depth,energyAddon->root->nChannels);
           ImageHandler *energytarget=new ImageHandler(cvenergytarget);  
           // Bitmap energytarget=Bitmap.createBitmap(energyAddon.getWidth(),energyAddon.getHeight()-1,Bitmap.Config.ARGB_8888);            
           // pixel = new int[1];
            int energypixel;
			for (int y = 0; y < energyAddon->root->height; y++) {
                for (int x = 0; x < energyAddon->root->width; x++) {
                   energypixel= energyAddon->getPixel(x, y);
                    int targetY = y;
                    if (seamLookup[x] != y) {
                        if (seamLookup[x] <= y) {
                            targetY--;
                        }
                        energytarget->setPixel(x, targetY, energypixel);
                    }
                }
            }
			energyAddon->root=cvCloneImage(energytarget->root);
            //energyAddon = energytarget;
			delete energytarget;
        }*/
		delete seamLookup;
		//delete s;
    }
	 void ImageSeamOperations::createVerticalSeamTable(ImageHandler* energyImage,int**tablecur,int**tablenex) {
        int h = energyImage->root->height;
        int w = energyImage->root->width;
    	  int *pixelRow= new int[w];
        energyImage->getGrayRowPixels(pixelRow,0);
       // getRowPixels(energyImage,0,pixelRow);
        for (int x = 0; x < w; x++) {
            tablecur[x][0] = pixelRow[x];//result[x][0][0] = pixelRow[x];
			//printf("%d ",pixelRow[x]);
			tablenex[x][0]=0;
			
        }
        // now for the following rows find the next component in the seam
        int right, left, min, middle, currentEnergy;
        int nextStep;
        for (int y = 1; y < h; y++) {
          energyImage->getGrayRowPixels(pixelRow,y);
        	//getRowPixels(energyImage, y, pixelRow);
            for (int x = 0; x < w; x++) {
                // find 'next in path'
                middle = tablecur[x][y - 1];//result[x][y - 1][0];
                currentEnergy = pixelRow[x];
			//	printf("%d ",pixelRow[x]);
                nextStep = 0;
                if (x > 1 && x < w - 2) {
                    left = tablecur[x - 1][y - 1];//result[x - 1][y - 1][0];
                    right =tablecur[x + 1][y - 1];// result[x + 1][y - 1][0];
                    nextStep = 0;
                    if (right < middle || left < middle) {
                        if (right < left) {
                            min = right;
                            nextStep = 1;
                        } else {
                            min = left;
                            nextStep = -1;
                        }
                    } else {
                        min = middle;
                    }
                    currentEnergy = currentEnergy + min;
                } else if (x > 1) { // left border of the image, no left possible
                    right = tablecur[x - 1][y - 1];//result[x - 1][y - 1][0];
                    if (right < middle) {
                        min = right;
                        nextStep = -1;
                    } else {
                        min = middle;
                        nextStep = 0;
                    }
                    currentEnergy = currentEnergy + min;
                } else if (x < w - 2) { // right border of the image, no right possible
                    left = tablecur[x + 1][y - 1];//result[x + 1][y - 1][0];
                    if (left < middle) {
                        min = left;
                        nextStep = 1;
                    } else {
                        min = middle;
                        nextStep = 0;
                    }
                    currentEnergy = currentEnergy + min;
                }
               tablecur[x][y] = currentEnergy;// result[x][y][0] = currentEnergy;
               tablenex[x][y] = nextStep;//result[x][y][1] = nextStep;
			//	printf("%d->%d  ",currentEnergy,nextStep);
            }
        }
		delete pixelRow;
    }

    /**
     * This method generates the energy image. Change this one to use another one besides the gradient.
     *
     * @return a grayscale image denoting the energy of the pixels.
     */
    ImageHandler* ImageSeamOperations::generateEnergyImage() {
   
        IplImage *gray=cvCreateImage(cvGetSize(img->root),8,1);
        cvCvtColor(img->root,gray,CV_BGR2GRAY);//转为灰度图像
		ImageHandler *grad01=new ImageHandler(gray);
		grad01->sobel();
		  if (energyAddon!=0) {			
		int temp=0;
      for (int x = 0; x < grad01->root->width; x++) {
            for (int y = 0; y < grad01->root->height; y++) {   
				int color=energyAddon->getGrayPixel(x,y);
                    if (color>250) {
                       // temp= 200;
						grad01->setGrayPixel(x,y,255);
						//printf("color%d ",color);
                    }   
					
               // grad01->setGrayPixel(x,y,temp);
            }
        }
	     } 
        cvReleaseImage(&gray);
        return grad01;
    }
