#include "com_huangbin_test_diyimage_ImageUtilsJni.h"
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include "SalientRegionDetector.h"
#include "inpainting.h"
#include "Composition.h"
#include "ImageSeamOperations.h"
#include "grabcut.h"
#include "LineDetect.h"
using namespace cv;
using namespace std;
IplImage * pImage = NULL;
IplImage * maskImage=NULL;
GCApplication gcapp;
IplImage *gImage=NULL;
Mat image;
IplImage * loadPixels(int * pixels, int width, int height);
IplImage * getIplImageFromIntArray(JNIEnv* env, jintArray array_data, jint width, jint height);
JNIEXPORT jboolean JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_setMaskImageJni
  (JNIEnv *env, jclass thiz, jintArray photo_data, jint width, jint height){
	if(maskImage != NULL)
		    {
		        cvReleaseImage(&maskImage);
		        maskImage = NULL;
		    }

		    maskImage = getIplImageFromIntArray(env, photo_data, width, height);
		    if(maskImage == 0)
		    {
		        return 0;
		    }
		    return 1;
}
JNIEXPORT jboolean JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_setSourceImageJni
  (JNIEnv *env, jclass thiz, jintArray photo_data, jint width, jint height){
	if(pImage != NULL)
	    {
	        cvReleaseImage(&pImage);
	        pImage = NULL;
	    }

	    pImage = getIplImageFromIntArray(env, photo_data, width, height);
	    if(pImage == 0)
	    {
	        return 0;
	    }
	    return 1;
}
IplImage* getIplImageFromIntArray(JNIEnv* env, jintArray array_data,
    jint width, jint height) {
int *pixels = env->GetIntArrayElements(array_data, 0);
if (pixels == 0) {
    return 0;
}
IplImage *image = loadPixels(pixels, width, height);
env->ReleaseIntArrayElements(array_data, pixels, 0);
if (image == 0) {
    return 0;
}
return image;
}
IplImage* loadPixels(int* pixels, int width, int height) {
int x, y;
IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
unsigned char* base = (unsigned char*) (img->imageData);
unsigned char* ptr;
for (y = 0; y < height; y++) {
    ptr = base + y*img->widthStep;
    for (x = 0; x < width; x++) {
        // blue
        ptr[3*x] = pixels[x+y*width]&0xFF;
        // green
        ptr[3*x+1] = pixels[x+y*width]>>8&0xFF;
        // blue
        ptr[3*x+2] = pixels[x+y*width]>>16&0xFF;

    }
}
return img;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_getSourceImageJni
  (JNIEnv *env, jclass object){
	if(pImage == 0)
	    {
	        return NULL;
	    }
	    int len =(pImage->width)*(pImage->height);
	    jintArray rgbaData = env->NewIntArray(len);
	    if(pImage->nChannels == 4){
	        env->SetIntArrayRegion(rgbaData,0,len,(jint*)pImage->imageData);
	    }else if(pImage->nChannels == 3){
	        IplImage* t = cvCreateImage(cvGetSize(pImage),IPL_DEPTH_8U,4);
	        for(int h = 0; h< pImage->height; h++){
	            char* pt= t->imageData + h*t->widthStep;
	            char* pImg = pImage->imageData+ h*pImage->widthStep;
	            for(int w=0 ; w<pImage->width; w++){
	                memcpy(pt,pImg,3);
	                pt[3] = 255;//alpha
	                pt+=4;
	                pImg += 3;
	            }
	        }
	        env->SetIntArrayRegion(rgbaData,0,len,(jint*)t->imageData);
	        cvReleaseImage(&t);
	    }else {
	        // if pImage -> nChannels == 1 , handle it in the way you prefer
	        // I donot think your image is gray ,so forget about this situation
	    }
	    cvReleaseImage(&pImage);
	    return rgbaData;
}

JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_ImgFun
  (JNIEnv *env, jclass object, jintArray buf, jint w, jint h){
	jint *cbuf;
		cbuf = env->GetIntArrayElements(buf, NULL);
		if(cbuf == NULL)
		{
			return 0;
		}
	Mat myimg(h, w, CV_8UC4, (unsigned char*)cbuf);
		for(int j=0; j<myimg.rows/2; j++)
		{
			myimg.row(j).setTo(Scalar(0, 0, 0, 0));
		}
		int size=w*h;
			jintArray result = env->NewIntArray(size);
			env->SetIntArrayRegion(result, 0, size, cbuf);
			env->ReleaseIntArrayElements(buf, cbuf, 0);
			return result;
}
JNIEXPORT void JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_cvSmooth
(JNIEnv *env, jclass thiz){
	IplImage *dst=cvCreateImage(cvSize(pImage->width, pImage->height), IPL_DEPTH_8U, 3);
	cvSmooth(pImage,dst,CV_MEDIAN,3,0,0,0);
    pImage=cvCloneImage(dst);
	cvReleaseImage(&dst);
	return;
}
/*JNIEXPORT jint JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_Openlocalclassfier
  (JNIEnv *env, jclass thiz){
	CImageAestheticSystemView cImageSystem;
	int result=cImageSystem.Openlocalclassfier();
	return result;
}

JNIEXPORT jint JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_OpenPicture
  (JNIEnv *env, jclass thiz){
	    CImageAestheticSystemView cImageSystem;
		//int result=cImageSystem.OpenPicture(pImage);
	    cImageSystem.OpenPicture(pImage);
		//return result;
	    return 0;
}
*/
/*
 * Class:     com_huangbin_test_diyimage_ImageUtilsJni
 * Method:    Evaluate
 * Signature: ()Ljava/lang/String;
 */
/*JNIEXPORT jstring JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_Evaluate
  (JNIEnv *env, jclass thiz){
	 CImageAestheticSystemView cImageSystem;
	 char tmpstr[20];
	 jstring rtstr;
	 int result=cImageSystem.Openlocalclassfier();
	 if(result<0){
		 strcpy(tmpstr,"classfier");
		 rtstr = env->NewStringUTF(tmpstr);
		 return rtstr;
	 }
	 result=cImageSystem.OpenPicture(pImage);
	 if(result<0){
		     strcpy(tmpstr,"Picture");
	 		 rtstr = env->NewStringUTF(tmpstr);
	 		 return rtstr;
	 	 }
	 //cImageSystem.OpenPicture(pImage);
	 cvReleaseImage(&pImage);
	 string resultstr=cImageSystem.Recognize();
	 strcpy(tmpstr,resultstr.c_str());
	 rtstr = env->NewStringUTF(tmpstr);

	 return rtstr;

}*/
JNIEXPORT void JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_setImage
  (JNIEnv *env, jclass thiz){
pImage=cvLoadImage("/sdcard/1.jpg");
}
JNIEXPORT jint JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_getImageWidth
  (JNIEnv *env, jclass thiz){
	return pImage->width;
}
JNIEXPORT jint JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_getImageHeight
  (JNIEnv *env, jclass thiz){
	return pImage->height;
}
JNIEXPORT jint JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_setOpenCvImage
  (JNIEnv *env, jclass thiz, jstring path){
	const char* str;
	   str = env->GetStringUTFChars(path,0);
	   if(str == NULL) {
	       return -1; /* OutOfMemoryError already thrown */
	   }
   if(pImage!=0)
	   cvReleaseImage(&pImage);
   pImage=cvLoadImage(str);
    if(pImage==0)
    	return -1;
    else
    return 0;

}
/*int getMainArea(IplImage* dstImg){
	if(dstImg==0)
		return -1;
	double area;
	CvSeq *cont = 0;
	CvSeq *conttemp = 0;
	CvMemStorage *storage = 0;
	storage = cvCreateMemStorage(0);
	double maxArea = 0;
	cvFindContours( dstImg, storage, &cont, sizeof(CvContour),
	CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) ); //找到所有轮廓
	for(;cont;cont = cont->h_next)
	{
		area = fabs(cvContourArea( cont, CV_WHOLE_SEQ )); //获取当前轮廓面积
		cvDrawContours( dstImg, cont, CV_RGB(0, 0, 0), CV_RGB(0, 0, 0), 0,  CV_FILLED, 8);
		if(area > maxArea)
		{
			conttemp = cont;
			maxArea = area;
		}
	}
	cvDrawContours( dstImg, conttemp, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), 0,  CV_FILLED, 8);
	cvReleaseMemStorage(&storage);
	return 0;
}
bool isGreen(uchar *tmp){
	    int secha=5;
        if(tmp[2]<secha&&tmp[0]<secha&&tmp[1]>230)
			return true;
		else
			return false;
}
bool isBlack(uchar *tmp){
    	int secha=5;
    	/*int pixR,pixB,pixG;
    	pixR=tmp[2];
    	pixG=tmp[1];
    	pixB=tmp[0];
    	if(pixR<secha&&pixG<secha&&pixB<secha)
		if(tmp[0]<secha)
    		return true;
    	else
    		return false;
    }
int ConverImage(IplImage* image){
 if(image==0)
	  return -1;
 uchar *pchar;
  int width = image->width;                      //读取图像宽度
  int heigh = image->height;                    //读取图像高度
  int channel = image->nChannels;          //读取图像通道数
int widthStep = image->widthStep;      //读取图像一行像素所占的字节数
  int i,j;
  for (i=0; i<heigh; i++)
  {
    pchar = (uchar*)image->imageData + i*image->widthStep;
    for (j=0; j<width; j++)
    {
	  uchar* temp = pchar + j*image->nChannels;
	  if(!isGreen(temp))
	  {
      temp[0] = 0;//通道B
      temp[1] = 0;//通道G
      temp[2] = 0;//通道R
	  }
    }
  }
  return 0;
}
int setImageMask(IplImage* image,IplImage *mask){
  uchar *pchar;
  uchar *maskchar;
  if(image==0)
	  return -1;
  if(mask==0)
	  return -1;
  int width = mask->width;                      //读取图像宽度
  int heigh = mask->height;                    //读取图像高度
  int channel = mask->nChannels;          //读取图像通道数
int widthStep = mask->widthStep;      //读取图像一行像素所占的字节数
  int i,j;
  for (i=0; i<heigh; i++)
  {
    pchar = (uchar*)image->imageData + i*image->widthStep;
	maskchar=(uchar*)mask->imageData + i*widthStep;
    for (j=0; j<width; j++)
    {
	  uchar* temp = pchar + j*image->nChannels;
	  uchar* masktmp=maskchar+j*mask->nChannels;
	  if(!isBlack(masktmp))
	  {
      temp[0] = 0;//通道B
      temp[1] = 255;//通道G
      temp[2] = 0;//通道R
	  }
    }
  }
  return 0;
}*/
JNIEXPORT jint JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_reComposition
  (JNIEnv *env, jclass thiz){
	IplImage *mask;
	mask=cvCloneImage(maskImage);
	IplImage*img=cvCloneImage(pImage);
	Composition composite;
	composite.setRootImage(img);
	inpainting paint(img->width,img->height,RGB24);
				cvSmooth(mask,mask,CV_MEDIAN,3,0,0,0);
				ConverImage(mask);
				IplImage* graymask = cvCreateImage(cvSize(mask->width,mask->height),IPL_DEPTH_8U,1);
	            cvCvtColor(mask,graymask,CV_BGR2GRAY);
	            cvThreshold(graymask,graymask,2,255,CV_THRESH_BINARY);
				cvSmooth(graymask,graymask,CV_MEDIAN,3,0,0,0);
				double maxArea;
				getMainAreaAndS(graymask,maxArea);
				//getMainArea(graymask);
				composite.setMainareaImage(graymask);
				cvSmooth(graymask,graymask,CV_MEDIAN,3,0,0,0);
	            cvDilate(graymask,graymask,0,3);
	            paint.process((unsigned char * )img->imageData,img->widthStep, (const unsigned char *)graymask->imageData,graymask->widthStep,8,8,50,255,100000);
				composite.setInpaintImage(img);
	            composite.reComposition(1);
	            pImage=cvCloneImage(composite.inpaint);
	            cvReleaseImage(&mask);
	            cvReleaseImage(&graymask);
	            cvReleaseImage(&img);
	            cvReleaseImage(&maskImage);
	            return 0;
}
JNIEXPORT jint JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_SalientRegionDetectorJni
  (JNIEnv *env, jclass thiz, jint sigmaS, jfloat sigmaR){
	IplImage *img;
	img=cvCloneImage(pImage);
	SalientRegionDetector regiondetector;
	IplImage *region=regiondetector.DetectSaliencyAndSegment(img,sigmaS,sigmaR);
	pImage=cvCloneImage(region);
	cvReleaseImage(&region);
	cvReleaseImage(&img);
	return 0;
}
/*JNIEXPORT jint JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_SeamCarve
  (JNIEnv *env, jclass thiz, jint nw, jint nh){
	ImageHandler *image=new ImageHandler(pImage);
	ImageSeamOperations s(image);
	s.SeamCarve(nw,nh);
	pImage=cvCloneImage(s.getImg()->root);
	return 0;
}*/
JNIEXPORT void JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_mouseClickJni
  (JNIEnv *env, jclass thiz, jint left, jint top, jint right, jint bottom){
	gcapp.mouseClick( left, top, right, bottom,0);
}

/*
 * Class:     com_huangbin_test_diyimage_ImageUtilsJni
 * Method:    GrabInit
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_GrabInit
  (JNIEnv *env, jclass thiz){
	if(pImage==0)
		return -1;
	gImage=cvCloneImage(pImage);
	 image=Mat(gImage);
     gcapp.setImage(image);
     return 0;
}

/*
 * Class:     com_huangbin_test_diyimage_ImageUtilsJni
 * Method:    setGrabRect
 * Signature: (IIII)I
 */
JNIEXPORT jint JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_setGrabRect
  (JNIEnv *env, jclass thiz, jint left, jint top, jint right, jint bottom){
	gcapp.setRect(left,top,right,bottom);
}

/*
 * Class:     com_huangbin_test_diyimage_ImageUtilsJni
 * Method:    GrabCut
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_GrabCut
  (JNIEnv *env, jclass thiz){
		int iterCount = gcapp.getIterCount();
	            int newIterCount = gcapp.nextIter();
	            if( newIterCount > iterCount )
	            {
	                Mat res;
	            Mat binMask;
	            if( !gcapp.isInitialized )
	            gcapp.image->copyTo( res );
	            else
	           {
	             getBinMask( gcapp.mask, binMask );
	             gcapp.image->copyTo( res, binMask );
	             IplImage maskimage(binMask);
	             maskImage=cvCloneImage(&maskimage);
	            }
				IplImage resimage(res);
				IplImage*resimg=&resimage;
			    pImage=cvCloneImage(resimg);
	            }
	            else
	            return -1;
	    return 0;
}

/*
 * Class:     com_huangbin_test_diyimage_ImageUtilsJni
 * Method:    GrabreComposite
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_GrabreComposite
  (JNIEnv *env, jclass thiz){
	IplImage*img=cvCloneImage(pImage);
	Composition composite;
	composite.setRootImage(img);
	            IplImage *graymask=cvCloneImage(maskImage);
				gcapp.ConverImage(graymask);
				cvDilate(graymask,graymask,0,1);
				composite.setMainareaImage(graymask);
				cvSmooth(graymask,graymask,CV_MEDIAN,3,0,0,0);
	            cvDilate(graymask,graymask,0,3);
				inpainting paint(img->width,img->height,RGB24);
	            paint.process((unsigned char * )img->imageData,img->widthStep, (const unsigned char *)graymask->imageData,graymask->widthStep,8,8,50,255,100000);
				composite.setInpaintImage(img);
				//cvSaveImage("/sdcard/inpaint.jpg",img);
	            composite.reComposition(1);
	            pImage=cvCloneImage(composite.inpaint);
	            cvReleaseImage(&graymask);
	            cvReleaseImage(&img);
	            cvReleaseImage(&maskImage);
	            return 0;
}
JNIEXPORT jint JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_VisualBalance
  (JNIEnv *env, jclass thiz){
	IplImage*img=cvCloneImage(pImage);
	IplImage *graymask=cvCreateImage(cvSize(img->width,img->height),img->depth,1);
	IplImage*result=AutoResize(img,graymask);
	 pImage=cvCloneImage(result);
	 cvReleaseImage(&img);
	 cvReleaseImage(&result);
	 cvReleaseImage(&graymask);
	 return 0;
}

/*
 * Class:     com_huangbin_test_diyimage_ImageUtilsJni
 * Method:    Inpainting
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_Inpainting
  (JNIEnv *env, jclass thiz){
	IplImage*mask=cvCloneImage(maskImage);
	IplImage*img=cvCloneImage(pImage);
	cvSmooth(mask,mask,CV_MEDIAN,3,0,0,0);
				ConverImage(mask);
				IplImage* graymask = cvCreateImage(cvSize(mask->width,mask->height),IPL_DEPTH_8U,1);
	            cvCvtColor(mask,graymask,CV_BGR2GRAY);
	            cvThreshold(graymask,graymask,2,255,CV_THRESH_BINARY);
				cvSmooth(graymask,graymask,CV_MEDIAN,3,0,0,0);
	            cvDilate(graymask,graymask,0,3);
				inpainting paint(img->width,img->height,RGB24);
	            paint.process((unsigned char * )img->imageData,img->widthStep, (const unsigned char *)graymask->imageData,graymask->widthStep,8,8,50,255,100000);
	            pImage=cvCloneImage(img);
	            	            cvReleaseImage(&mask);
	            	            cvReleaseImage(&graymask);
	            	            cvReleaseImage(&img);
	            	            cvReleaseImage(&maskImage);
	            	            return 0;
}

/*
 * Class:     com_huangbin_test_diyimage_ImageUtilsJni
 * Method:    GrabreCompositeAndResize
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_GrabreCompositeAndResize
  (JNIEnv *env, jclass thiz){
	IplImage*img=cvCloneImage(pImage);
		Composition composite;
		composite.setRootImage(img);
		IplImage *graymask=cvCloneImage(maskImage);
	    gcapp.ConverImage(graymask);
	    cvSmooth(graymask,graymask,CV_MEDIAN,3,0,0,0);
	                //cvDilate(graymask,graymask,0,1);
	    			double maxArea=0;
	    			getMainAreaAndS(graymask,maxArea);
	    		//	printf("%lf maxArea ",maxArea);
	    		    double scale=1;
	    			double area=graymask->width*graymask->height;
	    		//	printf("%lf area",area);
	    			if(maxArea/area<0.1)
	    				{
	    					scale=sqrt(area*0.1/maxArea);
	    					if(scale>1.4)
	    						scale=1.4;
	    					int left,top,right,bottom,centerx,centery;
	    	getAreaRect(graymask,left,top,right,bottom,centerx,centery);
	    	int width=right-left;
	    	int height=bottom-top;
	    	int rleft=(left-5)<0?0:(left-5);
	        int rtop=(top-5)<0?0:(top-5);
	    	int rwidth=rleft+(width+10)>img->width?width:(width+10);
	    	int rheight=rtop+(height+10)>img->height?height:(height+10);
	    	cvSetImageROI(img,cvRect(rleft,rtop,rwidth,rheight));
	    	IplImage *clipimg=cvCreateImage(cvSize(rwidth,rheight),img->depth,img->nChannels);
	    	cvCopy(img,clipimg,0);
	        cvResetImageROI(img);
	    	cvSetImageROI(graymask,cvRect(rleft,rtop,rwidth,rheight));
	    	IplImage *clipmask=cvCreateImage(cvSize(rwidth,rheight),graymask->depth,graymask->nChannels);
	    	cvCopy(graymask,clipmask,0);
	        cvResetImageROI(graymask);
	        CvSize dst_cvsize;
	    	dst_cvsize.width=(int)(clipmask->width*scale);
	    	dst_cvsize.height=(int)(clipmask->height*scale);
	    	IplImage *graydst=cvCreateImage(dst_cvsize,clipmask->depth,clipmask->nChannels);
	    	IplImage *resizeimg=cvCreateImage(dst_cvsize,clipimg->depth,clipimg->nChannels);
	    	//cvResize(s.getImg()->root,dst,CV_INTER_LINEAR);//
	    	cvResize(clipmask,graydst,CV_INTER_CUBIC);
	    	cvResize(clipimg,resizeimg,CV_INTER_CUBIC);
	    	composite.setResizeImage(resizeimg);
	    			composite.setMainareaImage(graydst);
	    			cvSmooth(graymask,graymask,CV_MEDIAN,3,0,0,0);
	                cvDilate(graymask,graymask,0,3);
	    			inpainting paint(img->width,img->height,RGB24);
	    			IplImage* inpaintimg=cvCloneImage(img);
	                paint.process((unsigned char * )inpaintimg->imageData,inpaintimg->widthStep, (const unsigned char *)graymask->imageData,graymask->widthStep,8,8,50,255,100000);
	    			composite.setInpaintImage(inpaintimg);
	                composite.reCompositionAndResize(centerx,centery);
	                cvReleaseImage(&clipmask);
	                cvReleaseImage(&clipimg);
	                cvReleaseImage(&inpaintimg);
	    			}
	    			else
	    			{
	                   cvDilate(graymask,graymask,0,1);

	    			composite.setMainareaImage(graymask);
	    			cvSmooth(graymask,graymask,CV_MEDIAN,3,0,0,0);
	                cvDilate(graymask,graymask,0,3);
	    			inpainting paint(img->width,img->height,RGB24);
	    			IplImage* inpaintimg=cvCloneImage(img);
	                paint.process((unsigned char * )inpaintimg->imageData,inpaintimg->widthStep, (const unsigned char *)graymask->imageData,graymask->widthStep,8,8,50,255,100000);
	    			composite.setInpaintImage(inpaintimg);
	                composite.reComposition(1);
	                cvReleaseImage(&inpaintimg);
	    			}
	    			pImage=cvCloneImage(composite.inpaint);

	    		    cvReleaseImage(&graymask);
	    		    cvReleaseImage(&img);
                    return 0;
}

/*
 * Class:     com_huangbin_test_diyimage_ImageUtilsJni
 * Method:    reCompositeAndResize
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_reCompositeAndResize
  (JNIEnv *env, jclass thiz){
	IplImage *mask;
		mask=cvCloneImage(maskImage);
		IplImage*img=cvCloneImage(pImage);
		Composition composite;
		inpainting paint(img->width,img->height,RGB24);
					cvSmooth(mask,mask,CV_MEDIAN,3,0,0,0);
					ConverImage(mask);
					IplImage* graymask = cvCreateImage(cvSize(mask->width,mask->height),IPL_DEPTH_8U,1);
		            cvCvtColor(mask,graymask,CV_BGR2GRAY);
		            cvThreshold(graymask,graymask,2,255,CV_THRESH_BINARY);
					cvSmooth(graymask,graymask,CV_MEDIAN,3,0,0,0);
					double maxArea;
					getMainAreaAndS(graymask,maxArea);
					//cvDilate(graymask,graymask,0,1);
					double scale=1;
					double area=graymask->width*graymask->height;
				//	printf("%lf area",area);
					if(maxArea/area<0.1)
						{
							scale=sqrt(area*0.1/maxArea);
							if(scale>2)
								scale=1.6;
							int left,top,right,bottom,centerx,centery;
			getAreaRect(graymask,left,top,right,bottom,centerx,centery);
			int width=right-left;
				    	int height=bottom-top;
				    	int rleft=(left-5)<0?0:(left-5);
				        int rtop=(top-5)<0?0:(top-5);
				    	int rwidth=rleft+(width+10)>img->width?width:(width+10);
				    	int rheight=rtop+(height+10)>img->height?height:(height+10);
				    	cvSetImageROI(img,cvRect(rleft,rtop,rwidth,rheight));
				    	IplImage *clipimg=cvCreateImage(cvSize(rwidth,rheight),img->depth,img->nChannels);
				    	cvCopy(img,clipimg,0);
				        cvResetImageROI(img);
				    	cvSetImageROI(graymask,cvRect(rleft,rtop,rwidth,rheight));
				    	IplImage *clipmask=cvCreateImage(cvSize(rwidth,rheight),graymask->depth,graymask->nChannels);
				    	cvCopy(graymask,clipmask,0);
				        cvResetImageROI(graymask);
				        CvSize dst_cvsize;
				    	dst_cvsize.width=(int)(clipmask->width*scale);
				    	dst_cvsize.height=(int)(clipmask->height*scale);
				    	IplImage *graydst=cvCreateImage(dst_cvsize,clipmask->depth,clipmask->nChannels);
				    	IplImage *resizeimg=cvCreateImage(dst_cvsize,clipimg->depth,clipimg->nChannels);
				    	//cvResize(s.getImg()->root,dst,CV_INTER_LINEAR);//
				    	cvResize(clipmask,graydst,CV_INTER_CUBIC);
				    	cvResize(clipimg,resizeimg,CV_INTER_CUBIC);
				    	composite.setResizeImage(resizeimg);
				    			composite.setMainareaImage(graydst);
					cvSmooth(graymask,graymask,CV_MEDIAN,3,0,0,0);
					//cvSmooth(graymask,graymask,CV_MEDIAN,3,0,0,0);
		            cvDilate(graymask,graymask,0,3);
		            paint.process((unsigned char * )img->imageData,img->widthStep, (const unsigned char *)graymask->imageData,graymask->widthStep,8,8,50,255,100000);
					composite.setInpaintImage(img);
					composite.reCompositionAndResize(centerx,centery);
					 cvReleaseImage(&clipmask);
					cvReleaseImage(&clipimg);
					}
					else
					{
		            cvDilate(graymask,graymask,0,1);
					composite.setMainareaImage(graymask);
					cvSmooth(graymask,graymask,CV_MEDIAN,3,0,0,0);
		            cvDilate(graymask,graymask,0,3);
					inpainting paint(img->width,img->height,RGB24);
		            paint.process((unsigned char * )img->imageData,img->widthStep, (const unsigned char *)graymask->imageData,graymask->widthStep,8,8,50,255,100000);
					composite.setInpaintImage(img);
		            composite.reComposition(1);
					}
					pImage=cvCloneImage(composite.inpaint);

				    cvReleaseImage(&graymask);
				    cvReleaseImage(&img);
				    return 0;
}
JNIEXPORT void JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_addHeight
  (JNIEnv *env, jclass thiz, jint seamnum, jint lowheight, jint highheight){
	IplImage *img=cvCloneImage(pImage);
	ImageHandler *imagehandle=new ImageHandler(img);
	//ImageHandler *mask=new ImageHandler(graymask);
	ImageSeamOperations s(imagehandle);
	s.setHalfMask(highheight,s.getImg()->root->height-1);
	    s.addHeight(seamnum,lowheight,highheight);
	    pImage=cvCloneImage(s.getImg()->root);
	    cvReleaseImage(&img);
}
