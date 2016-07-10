#include"Composition.h"
Composition::~Composition(){
  if(root!=0)
	  cvReleaseImage(&root);
  if(mainarea!=0)
	  cvReleaseImage(&mainarea);
  if(inpaint!=0)
	  cvReleaseImage(&inpaint);
  if(resize!=0)
	  cvReleaseImage(&resize);
}
void Composition::setRootImage(IplImage *img){
	root=cvCloneImage(img);
}
void Composition::setResizeImage(IplImage *img){
	resize=cvCloneImage(img);
}
void Composition::setInpaintImage(IplImage *img){
	inpaint=cvCloneImage(img);
}
void Composition::setMainareaImage(IplImage *img){
	mainarea=cvCloneImage(img);
}
Composition::Composition(){
	root=0;
	resize=0;
	mainarea=0;
	inpaint=0;
}
int Composition::Min(int a,int b){
	return (a>b?b:a);
}
bool Composition::isGreen(uchar *tmp){
	    int secha=5;
        if(tmp[2]<secha&&tmp[0]<secha&&tmp[1]>230)
			return true;
		else
			return false;
}
bool Composition::isBlack(uchar *tmp){
    	int secha=5;
    	/*int pixR,pixB,pixG;
    	pixR=tmp[2];
    	pixG=tmp[1];
    	pixB=tmp[0];
    	if(pixR<secha&&pixG<secha&&pixB<secha)*/
		if(tmp[0]<secha)
    		return true;
    	else
    		return false;
    }
void Composition::reComposition(int flag){
  uchar *inpaintchar; 
  uchar *maskchar;
  uchar *rootchar;
  int width = inpaint->width;                      //读取图像宽度 
  int height = inpaint->height;                    //读取图像高度 
  int channel =inpaint->nChannels;          //读取图像通道数 
int widthStep =inpaint->widthStep;      //读取图像一行像素所占的字节数 
int centerarrx[2]={0};
int centerarry[2]={0};
int k=0;
centerarrx[0]=width/3;
centerarrx[1]=width/3*2;
centerarry[0]=height/3;
centerarry[1]=height/3*2;
int dcenterx=0;
        int dcentery=0;
        int sumi=0,sumj=0,count=0;
        int centerx,centery;
		int maxi=0;
  int i,j; 
  for(i=0;i<height;i++)
  {
	  maskchar=(uchar*)mainarea->imageData + i*mainarea->widthStep;
	  for(j=0;j<width;j++)
	  {
		  uchar* masktemp=maskchar+j*mainarea->nChannels;
		  if(flag==1){
		  if(!isBlack(masktemp)){
			  sumi+=i;
			  sumj+=j;
			  count++;
			  if(i>maxi)
				  maxi=i;
		  }}
		  else if(flag==2){
            if(isGreen(masktemp)){
			//  printf("can check green");
			  sumi+=i;
			  sumj+=j;
			  count++;
		  }
		  }
	  }
  }
  if(count==0)
	  count=1;
  centerx=sumj/count;
  centery=sumi/count;
  int d1=(centerx-centerarrx[0])*(centerx-centerarrx[0])+(centery-centerarry[0])*(centery-centerarry[0]);
  int d2=(centerx-centerarrx[0])*(centerx-centerarrx[0])+(centery-centerarry[1])*(centery-centerarry[1]);
  int d3=(centerx-centerarrx[1])*(centerx-centerarrx[1])+(centery-centerarry[0])*(centery-centerarry[0]);
  int d4=(centerx-centerarrx[1])*(centerx-centerarrx[1])+(centery-centerarry[1])*(centery-centerarry[1]);
  int d=Min(Min(d1+100,d2),Min(d3+100,d4));
  if(d==d1){
	  dcenterx=centerarrx[0];
	  dcentery=centerarry[0];
  }
  else if(d==d2)
  {   dcenterx=centerarrx[0];
	  dcentery=centerarry[1];
  }
  else if(d==d3)
  {
	  dcenterx=centerarrx[1];
	  dcentery=centerarry[0];
  }else{
	  dcenterx=centerarrx[1];
	  dcentery=centerarry[1];
  }

  for (i=0; i<height; i++) 
  { 
	maskchar=(uchar*)mainarea->imageData + i*mainarea->widthStep;
	rootchar = (uchar*)root->imageData + i*widthStep; 
    for (j=0; j<width; j++) 
    { 
      uchar* masktemp=maskchar+j*mainarea->nChannels;
	  if(flag==1){
      if(!isBlack(masktemp)){
         int m=dcenterx-centerx+j;
		 int n=dcentery-centery+i;
		 if(abs(n-i)>30){
			 if(n>i)
			     n=i+20;
			 else
				 n=i-20;
		 }
		 if(maxi>height-30)
			     n=i;
		 if(m<0||m>width-1||n<0||n>height-1)
			 continue;
		 uchar* roottemp = rootchar + j*channel; 
		 inpaintchar = (uchar*)inpaint->imageData + n*widthStep; 
	     uchar* inpainttemp = inpaintchar + m*channel; 
		 inpainttemp[0]=roottemp[0];
		 inpainttemp[1]=roottemp[1];
		 inpainttemp[2]=roottemp[2];
         }  
	  }
	  else if(flag==2){
		if(isGreen(masktemp)){
         int m=dcenterx-centerx+j;
		 int n=dcentery-centery+i;
		 if(abs(n-i)>10)
			 n=i;
		 if(m<0||m>width||n<0||n>height)
			 continue;
		 uchar* roottemp = rootchar + j*channel; 
		 inpaintchar = (uchar*)inpaint->imageData + n*widthStep; 
	     uchar* inpainttemp = inpaintchar + m*channel; 
		 inpainttemp[0]=roottemp[0];
		 inpainttemp[1]=roottemp[1];
		 inpainttemp[2]=roottemp[2];
         } 
	  }
    } 
  } 
  
}
void Composition::reCompositionAndResize(int centerx,int centery){
  uchar *inpaintchar; 
  uchar *maskchar;
  uchar *rootchar;
  int width = mainarea->width;                      //读取图像宽度 
  int height =mainarea->height;                    //读取图像高度 
  int channel =mainarea->nChannels;          //读取图像通道数 
int widthStep =mainarea->widthStep;      //读取图像一行像素所占的字节数 
int rcenterarrx[2]={0};
int rcenterarry[2]={0};
int rcenterx=0,rcentery=0;
rcenterarrx[0]=width/3;
rcenterarrx[1]=width/3*2;
rcenterarry[0]=height/3;
rcenterarry[1]=height/3*2;
int centerarrx[2]={0};
int centerarry[2]={0};
centerarrx[0]=inpaint->width/3;
centerarrx[1]=inpaint->width/3*2;
centerarry[0]=inpaint->height/3;
centerarry[1]=inpaint->height/3*2;
//printf("w%d h%d",inpaint->width,inpaint->height);
int dcenterx=0;
int dcentery=0;
int sumi=0,sumj=0,count=0;
int i,j;
  for(i=0;i<height;i++)
  {
	  maskchar=(uchar*)mainarea->imageData + i*mainarea->widthStep;
	  for(j=0;j<width;j++)
	  {
		  uchar* masktemp=maskchar+j*mainarea->nChannels;		
		  if(!isBlack(masktemp)){
			  sumi+=i;
			  sumj+=j;
			  count++;
		  }	
	  }
  }
  if(count==0)
	  count=1;
  rcenterx=sumj/count;
  rcentery=sumi/count;
  int d=0;
  int rleft=rcenterx;
  int rtop=rcentery;
  int rright=mainarea->width-rcenterx;
  int rbottom=mainarea->height-rcentery;
  int d1=(centerx-centerarrx[0])*(centerx-centerarrx[0])+(centery-centerarry[0])*(centery-centerarry[0]);
  int d2=(centerx-centerarrx[0])*(centerx-centerarrx[0])+(centery-centerarry[1])*(centery-centerarry[1]);
  int d3=(centerx-centerarrx[1])*(centerx-centerarrx[1])+(centery-centerarry[0])*(centery-centerarry[0]);
  int d4=(centerx-centerarrx[1])*(centerx-centerarrx[1])+(centery-centerarry[1])*(centery-centerarry[1]);
  if(rtop>inpaint->height/3){
	  if(rright<inpaint->width/3&&rleft<inpaint->width/3)
		  d=Min(d2,d4);
	  else if(rright<inpaint->width/3)
		  d=d4;
	  else
		  d=d2;

  }
  else if(rbottom>inpaint->height/3){
	  if(rright<inpaint->width/3&&rleft<inpaint->width/3)
		  d=Min(d1,d3);
	  else if(rright<inpaint->width/3)
		  d=d3;
	  else
		  d=d1;
  }else if(rright>inpaint->width/3){
	  if(rtop<inpaint->height/3&&rbottom<inpaint->height/3)
		  d=Min(d1,d2);
	  else if(rtop<inpaint->height/3)
		  d=d1;
	  else
		  d=d2;
  }else if(rleft>inpaint->width/3){
	  if(rtop<inpaint->height/3&&rbottom<inpaint->height/3)
		  d=Min(d3,d4);
	  else if(rtop<inpaint->height/3)
		  d=d3;
	  else
		  d=d4;
  }else{
	  d=Min(Min(d1,d2),Min(d3,d4));
  }
  //int 
  if(d==d1){
	  dcenterx=centerarrx[0];
	  dcentery=centerarry[0];
	  if(centery-dcentery>30){
	      dcentery=centery-30;
	  }
	 // if(abs(dcenterx-centerx)>10)
	 // dcenterx=centerx-5;
  }
  else if(d==d2)
  {   dcenterx=centerarrx[0];
	  dcentery=centerarry[1];
	  if(abs(dcentery-centery)>30){
		  if(centery<dcentery)
	  dcentery=centery+30;
		  else
			  dcentery=centery-30;
	  }
	  //if(abs(dcenterx-centerx)>10)
	 // dcenterx=centerx-5;
  }
  else if(d==d3)
  {
	  dcenterx=centerarrx[1];
	  dcentery=centerarry[0];
	  if(abs(dcentery-centery)>30){
       if(centery>dcentery)
	  dcentery=centery-30;
	  }
	 // if(abs(dcenterx-centerx)>10)
	 // dcenterx=centerx+5;
  }else{
	  dcenterx=centerarrx[1];
	  dcentery=centerarry[1];
	  if(abs(dcentery-centery)>30){
      if(centery<dcentery)
	  dcentery=centery+30;
	  else
		  dcentery=centery-30;
	  }
	 // if(abs(dcenterx-centerx)>10)
	//  dcenterx=centerx+5;
  }
 // printf("cenx%d ceny%d",dcenterx,dcentery);
  for (i=0; i<height; i++) 
  { 
	maskchar=(uchar*)mainarea->imageData + i*mainarea->widthStep;
	rootchar = (uchar*)resize->imageData + i*resize->widthStep; 
    for (j=0; j<width; j++) 
    { 
      uchar* masktemp=maskchar+j*mainarea->nChannels;
      if(!isBlack(masktemp)){
         int m=j-rcenterx+dcenterx;
		 int n=i-rcentery+dcentery;		 
		 if(m<0||m>inpaint->width-1||n<0||n>inpaint->height-1)
			 continue;
		 uchar* roottemp = rootchar + j*resize->nChannels; 
		 inpaintchar = (uchar*)inpaint->imageData + n*inpaint->widthStep; 
		 uchar* inpainttemp = inpaintchar + m*inpaint->nChannels; 
		 inpainttemp[0]=roottemp[0];
		 inpainttemp[1]=roottemp[1];
		 inpainttemp[2]=roottemp[2];
         }  
	  }
	  
	  }
    } 
