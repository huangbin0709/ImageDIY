#include "ImageHandler.h"

ImageHandler::ImageHandler(IplImage*root){
	this->root=cvCloneImage(root);
}
ImageHandler::ImageHandler(){

}
ImageHandler::~ImageHandler(){
	if(root!=0)
		cvReleaseImage(&root);
}
int ImageHandler::setImage(IplImage *img){
	root=cvCloneImage(img);
	return 0;
}
int ImageHandler::getPixel(int x,int y){
	if(x<0||x>(root->width-1)||y<0||y>(root->height-1))
	 {
		 printf("getPixelx%d y%d\n");
		 getchar();
	 }
	uchar *pchar; 
	pchar = (uchar*)root->imageData + y*root->widthStep;
	uchar *temp = pchar + x*root->nChannels; 
	int color=0;
	color=(temp[2]&0xff)<<16|(temp[1]&0xff)<<8|(temp[0]&0xff);
	return color;
}
int ImageHandler::setPixel(int x,int y,int color){
	if(x<0||x>(root->width-1)||y<0||y>(root->height-1))
	 {
		 printf("setPixel%d y%d\n");
		 getchar();
	 }
	uchar *pchar; 
	pchar = (uchar*)root->imageData + y*root->widthStep;
	uchar *temp = pchar + x*root->nChannels; 
	//printf("%d %d \n",x,y);
    	temp[0] = color&0xff;//通道B 
      temp[1] =(color>>8)&0xff;//通道G 
      temp[2] =(color>>16)&0xff;//通道R 
	  return 0;
}
 int ImageHandler::getRowPixels(int pixels[],int row){
	 if(row<0||row>(root->height-1))
		 return -1;
	 int x=0;
	 for(x=0;x<root->width;x++)
		 pixels[x]=getPixel(x,row);
	 return 0;
 }
 int ImageHandler::setRowPixels(int pixels[],int row){
	 if(row<0||row>(root->height-1))
		 return -1;
	 int x=0;
	 for(x=0;x<root->width;x++)
		 setPixel(x,row,pixels[x]);
	 return 0;
 }
 int ImageHandler::getColPixels(int pixels[],int col){
	 if(col<0||col>(root->width-1))
		 return -1;
	 int y=0;
	 for(y=0;y<root->height;y++)
		 pixels[y]=getPixel(col,y);
	 return 0;
 }
 int ImageHandler::setColPixels(int pixels[],int col){
	 if(col<0||col>(root->width-1))
		 return -1;
	 int y=0;
	 for(y=0;y<root->height;y++)
		 setPixel(col,y,pixels[y]);
	 return 0;
 }
 int ImageHandler::getGrayPixel(int x,int y){
	 if(x<0||x>(root->width-1)||y<0||y>(root->height-1))
	 {
		 printf("getGrayPixelx%d y%d\n");
		 getchar();
	 }
	uchar *pchar; 
	pchar = (uchar*)root->imageData + y*root->widthStep;
	uchar *temp = pchar + x*root->nChannels; 
	int color=0;
	//printf("x%d y%d ",x,y);
	color=temp[0]&0xff;
	return color;
}
int ImageHandler::setGrayPixel(int x,int y,int color){
	uchar *pchar; 
	pchar = (uchar*)root->imageData + y*root->widthStep;
	uchar *temp = pchar + x*root->nChannels; 
	temp[0] = color&0xff;
	return 0;
}
int ImageHandler::getGrayRowPixels(int pixels[],int row){
	 if(row<0||row>(root->height-1))
		 return -1;
	 int x=0;
	 for(x=0;x<root->width;x++)
		 pixels[x]=getGrayPixel(x,row);
	 return 0;
 }
 int ImageHandler::setGrayRowPixels(int pixels[],int row){
	 if(row<0||row>(root->height-1)){
		 printf("setGrayRow");
		 return -1;
	 }
	 int x=0;
	 for(x=0;x<root->width;x++)
		 setGrayPixel(x,row,pixels[x]);
	 return 0;
 }
 int ImageHandler::getGrayColPixels(int pixels[],int col){
	 if(col<0||col>(root->width-1)){
		 printf("getGrayCol");
		 return -1;
	 }
	 int y=0;
	 for(y=0;y<root->height;y++)
		 pixels[y]=getGrayPixel(col,y);
	 return 0;
 }
 int ImageHandler::setGrayColPixels(int pixels[],int col){
	 if(col<0||col>(root->width-1)){
		 printf("setGrayCol");
		 return -1;
	 }
	 int y=0;
	 for(y=0;y<root->height;y++)
		 setGrayPixel(col,y,pixels[y]);
	 return 0;
 }
 int ImageHandler::colortemplat(int *templt,int tw,int x,int y,int flag){
	 int i,j;
	int m=0;
	int px,py;
	int color=0;
	int rgb=0;
	for(i=0;i<tw;i++)
	{
		for(j=0;j<tw;j++)
		{
			py=y-tw/2+i;
			px=x-tw/2+j;
			if(px<0||px>root->width-1||py<0||py>root->height-1)
			{
				printf("px%d py%d",px,py);
				getchar();
			}
		  rgb=getPixel(px,py);	
		  if(flag==1)
			  color=rgb>>16&0xff;
		  else if(flag==2)
			  color=rgb>>8&0xff;
		  else
			  color=rgb&0xff;
	      m+=color*templt[i*tw+j];
	   }
	}
	return m;
 }
 int ImageHandler::templat(int *templt,int tw,int x,int y)
{
	int i,j;
	int m=0;
	int px,py;
	int color=0;
	for(i=0;i<tw;i++)
	{
		for(j=0;j<tw;j++)
		{
			py=y-tw/2+i;
			px=x-tw/2+j;
			if(px<0||px>root->width-1||py<0||py>root->height-1)
			{
				printf("px%d py%d",px,py);
				getchar();
			}
		  color=getGrayPixel(px,py);	
	      m+=color*templt[i*tw+j];
	   }
	}
	return m;
}
 void ImageHandler::sobel(){
	    int width=root->width;
		int height=root->height;
        int size=width * height;
		int *tmppixels1=new int[size];
	    int templttest1[]={1,2,1,0,0,0,-1,-2,-1};
	    int templttest2[]={-1,0,1,-2,0,2,-1,0,1};
	    int dGradOne,dGradTwo,dGrad;
	    int i,k;
	    for(i=0;i<height; i++)
	    	{	for(k=0;k<width;k++)
	    	       {	
					    if(i==0||i==height-1||k==0||k==width-1)
						{
							tmppixels1[i*width+k]=0;
							continue;
						}
					    dGradOne=templat(templttest1,3,k,i);
	    	            dGradTwo=templat(templttest2,3,k,i);
	    	            dGrad=sqrt((double)dGradOne*dGradOne+dGradTwo*dGradTwo);
						//dGrad=abs(dGradOne)+abs(dGradTwo);
						if(dGrad<0)
							dGrad=0;
						if(dGrad>255)
							dGrad=255;
	    	            tmppixels1[i*width+k]=dGrad;
	    	       }
	    	}
	    for(i=0;i<height; i++)
	        {	for(k=0;k<width;k++)
	        	    {
	        	       dGrad=tmppixels1[i*width+k];
                       setGrayPixel(k,i,dGrad);
	        	    }
	        }
	    	free(tmppixels1);
 }
 void ImageHandler::Lapcial(){
        int width=root->width;
		int height=root->height;
	    int templttest[]={-1,-1,-1,-1,9,-1,-1,-1,-1};
	    int i,k;
		unsigned int r,g,b;
		int color=0;
	    for(i=0;i<height; i++)
	    	{	for(k=0;k<width;k++)
	    	       {	
					    if(i==0||i==height-1||k==0||k==width-1)
						{
							continue;
						}
					    r=colortemplat(templttest,3,k,i,1);
						if(r<0)
							r=0;
						if(r>255)
							r=255;
						g=colortemplat(templttest,3,k,i,2);
						if(g<0)
							g=0;
						if(g>255)
							g=255;
	    	            b=colortemplat(templttest,3,k,i,3);
						if(b<0)
							b=0;
						if(b>255)
							b=255;
						color=(r&0xff)<<16|(g&0xff)<<8|(b&0xff);
						setPixel(k,i,color);
	    	       }
	    	}
 }