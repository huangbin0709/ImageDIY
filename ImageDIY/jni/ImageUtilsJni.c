#include "com_huangbin_test_diyimage_ImageUtilsJni.h"
#include <jni.h>
#include <stdlib.h>
#include <math.h>
#define MAX_VALUE 255

//Dalvik虚拟机加载C库时，第一件事是调用JNI_OnLoad()函数
jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    jint result = JNI_ERR;

    result = JNI_VERSION_1_4;

end:
    return result;
}

int Max(int a,int b){
	return (a>b?a:b);
}
int Min(int a,int b){
	return (a>b?b:a);
}
int ave_smooth(int *cpixels,int width,int x,int y)
{
   int m=0,n=0;
   int ave=0;
   int pixColor=0;
   int pixR=0,pixG=0,pixB=0;
   int tr=0,tb=0,tg=0;
   for(n=y-2;n<=y+2;n++)
	   for(m=x-2;m<=x+2;m++)
	   {
           pixColor=cpixels[n*width+m];
           pixR=((pixColor&0x00FF0000) >> 16);
           pixG=((pixColor&0x0000FF00) >> 8);
           pixB=pixColor&0x000000FF;
           tr+=pixR;
           tg+=pixG;
           tb+=pixB;
	   }
       ave=MAX_VALUE<<24|(tr/25)<<16|(tg/25)<<8|(tb/25);;
	   return ave;
}
int middle_smooth(int *cpixels,int width,int x,int y)
{
   int tmppixels[25]={0};
   int m=0,n=0;
   int k=0,i=0,j=0;
   int mid=0;
   int tmp=0;
   for(n=y-2;n<=y+2;n++)
	   for(m=x-2;m<=x+2;m++)
	   {
           tmppixels[k]=cpixels[n*width+m];
           k++;
	   }
	   for(j=k-1;j>0;j--)
		   for(i=0;i<j;i++)
		   { if(tmppixels[i]>tmppixels[i+1])
			   {   tmp=tmppixels[i];
	               tmppixels[i]=tmppixels[i+1];
	               tmppixels[i+1]=tmp;
			   }
		   }
	   if(tmppixels[(k-1)/2]==0)
		   mid=tmppixels[0];
	   else
	       mid=tmppixels[(k-1)/2];
	   return mid;
}
int sketch_templt(int *cpixels,int width,int height,int x,int y,int colorflag)
{
	int templt0[]={1,1,1,1,-8,1,1,1,1};
	int templttest1[]={1,1,-1,1,0,-1,1,-1,-1};
	int templttest2[]={1,1,1,-1,0,1,-1,-1,-1};
	double scale=2;
	int a,b,b1,b2;
	a=templt(cpixels,width,height,templt0,3,x,y,colorflag);
	b1=abs(templt(cpixels,width,height,templttest1,3,x,y,colorflag));
    b2=abs(templt(cpixels,width,height,templttest2,3,x,y,colorflag));
    b=b1>b2?b1:b2;
	if(b<25)a=0;
    else
	{
	a=(int)(a*scale);
	if(a>255)a=255;
	else if(a<32)a=0;
	}
	a=255-a;
	return a;
}
int templt(int *cpixels,int width,int height,int *templt,int tw,int x,int y,int colorflag)
{
	int i,j;
	int m=0;
	int px,py;
	int pixColor=0;
	int color=0;
	for(i=0;i<tw;i++)
	{
		for(j=0;j<tw;j++)
		{
			py=y-tw/2+i;
			px=x-tw/2+j;
			pixColor=cpixels[py*width +px];
			if(colorflag==0)
	        color=((pixColor&0x00FF0000) >> 16);
	        else if(colorflag==1)
	        color=((pixColor&0x0000FF00) >> 8);
	        else
	        color=pixColor&0x000000FF;
	        m+=color*templt[i*tw+j];
	   }
	}
	return m;
}

JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_toFilmJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height){
	jint *cpixels;
	cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
	if (cpixels == NULL) {
	   return 0; /* exception occurred */
	   }
	int pixR=0,pixG=0,pixB=0;
	int pixColor=0;
	int newR=0,newG=0,newB=0;
	int i=1,k=1;
	for(i=1;i<height-1; i++)
	   {
	      for(k=1;k<width-1;k++)
	         {
	         pixColor=cpixels[i*width +k];
	         pixR=((pixColor&0x00FF0000) >> 16);
	         pixG=((pixColor&0x0000FF00) >> 8);
	         pixB=pixColor&0x000000FF;

	         newR=MAX_VALUE -pixR;
	         newG=MAX_VALUE -pixG;
	         newB=MAX_VALUE -pixB;

	         newR=Min(MAX_VALUE, Max(0,newR));
	         newG=Min(MAX_VALUE, Max(0,newG));
	         newB=Min(MAX_VALUE, Max(0,newB));

	         cpixels[i*width+k]=MAX_VALUE<<24|newR<<16|newG<<8|newB;
	         }
	         }
	         int size=width * height;
	         jintArray result = (*env)->NewIntArray(env,size);
	         (*env)->SetIntArrayRegion(env,result, 0, size, cpixels);
	         (*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
	         return result;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_sharpenImageJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width , jint height){
	jint *cpixels;
	cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
    if (cpixels == NULL) {
    	return 0; /* exception occurred */
	}
    int laplacian[]={-1,-1,-1,-1,9,-1,-1,-1,-1};
    int size=width * height;
    int *tmppixels=(int *)malloc(sizeof(int)*size);
	int newR=0,newG=0,newB=0;
	int i=1,k=1;
    float delta=0.3;
    for(i=1;i<height-1; i++)
    {	for(k=1;k<width-1;k++)
        {
    	 newR=delta*templt(cpixels,width,height,laplacian,3,k,i,0);
		 newG=delta*templt(cpixels,width,height,laplacian,3,k,i,1);
		 newB=delta*templt(cpixels,width,height,laplacian,3,k,i,2);
    	 newR=Min(MAX_VALUE, Max(0,newR));
    	 newG=Min(MAX_VALUE, Max(0,newG));
    	 newB=Min(MAX_VALUE, Max(0,newB));
    	 tmppixels[i*width+k]=MAX_VALUE<<24|newR<<16|newG<<8|newB;
        }
    }
    jintArray result = (*env)->NewIntArray(env,size);
    (*env)->SetIntArrayRegion(env,result, 0, size, tmppixels);
    (*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
    free(tmppixels);
    return result;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_toGray
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height){
	jint *cpixels;
	cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
	if (cpixels == NULL) {
	    return 0; /* exception occurred */
	}
	int pixR=0,pixG=0,pixB=0;
	int pixColor=0;
	int newR=0,newG=0,newB=0,Gray=0;
	int i=1,k=1;
	for(i=0;i<height; i++)
		{	for(k=0;k<width;k++)
	        	{	pixColor=cpixels[i*width +k];
		            pixR=((pixColor&0x00FF0000) >> 16);
		            pixG=((pixColor&0x0000FF00) >> 8);
		            pixB=pixColor&0x000000FF;
		            Gray=pixR*0.299+pixG*0.587+pixB*0.114;
		            newR=Gray;
		            newG=Gray;
		            newB=Gray;
		            newR=Min(MAX_VALUE, Max(0,newR));
		            newG=Min(MAX_VALUE, Max(0,newG));
		            newB=Min(MAX_VALUE, Max(0,newB));
                    cpixels[i*width+k]=MAX_VALUE<<24|newR<<16|newG<<8|newB;
	        	}
		}
	int size=width * height;
	jintArray result = (*env)->NewIntArray(env,size);
    (*env)->SetIntArrayRegion(env,result, 0, size, cpixels);
	(*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
	return result;

}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_sketchImageJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height){
	    jint *cpixels;
		cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
		if (cpixels == NULL) {
		    	return 0; /* exception occurred */
			}
		int size=width * height;
		int *tmppixels=(int *)malloc(sizeof(int)*size);
		int templtave[]={1,1,1,1,4,1,1,1,1};
	    int i=1,k=1;
	    int newR=0,newG=0,newB=0;
	    for(i=1;i<height-1; i++)
	    	{	for(k=1;k<width-1;k++)
	    	       {	newR=sketch_templt(cpixels,width,height,k,i,0);
	    	            newG=sketch_templt(cpixels,width,height,k,i,1);
	    	            newB=sketch_templt(cpixels,width,height,k,i,2);
	                    tmppixels[i*width+k]=MAX_VALUE<<24|newR<<16|newG<<8|newB;
	    	       }
	    	}
	    for(i=1;i<height-1; i++)
	        {	for(k=1;k<width-1;k++)
	        	    {
	        		   newR=templt(tmppixels,width,height,templtave,3,k,i,0)/12;
	        		   newG=templt(tmppixels,width,height,templtave,3,k,i,1)/12;
	        		   newB=templt(tmppixels,width,height,templtave,3,k,i,2)/12;
	        		   newR=Min(MAX_VALUE, Max(0,newR));
	        		   newG=Min(MAX_VALUE, Max(0,newG));
	        		   newB=Min(MAX_VALUE, Max(0,newB));
	        		   cpixels[i*width+k]=MAX_VALUE<<24|newR<<16|newG<<8|newB;
	        	    }
	        }
	    	jintArray result = (*env)->NewIntArray(env,size);
	        (*env)->SetIntArrayRegion(env,result, 0, size, cpixels);
	    	(*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
	    	free(tmppixels);
	    	return result;
}
/*JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_template
  (JNIEnv *env, jclass thiz, jint flag){
	jint *cpixels;
		cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
		if (cpixels == NULL) {
		    	return 0;
			}
		int size=width * height;
		int *tmppixels1=(int *)malloc(sizeof(int)*size);
	//	int *tmppixels2=(int *)malloc (sizeof(int)*size);
	    int templttest1[]={-1,-1,-1,0,0,0,1,1,1};
	    int templttest2[]={1,0,-1,1,0,-1,1,0,-1};
	    int dGradOne,dGradTwo,dGrad;
	    int i=1,k=1;
	    for(i=1;i<height-1; i++)
	    	{	for(k=1;k<width-1;k++)
	    	       {	dGradOne=templt(cpixels,width,height,templttest1,3,k,i,3);
	    	            dGradTwo=templt(cpixels,width,height,templttest2,3,k,i,3);
	    	            dGrad=sqrt(dGradOne*dGradOne+dGradTwo*dGradTwo);
	    	            tmppixels1[i*width+k]=dGrad;
	    	       }
	    	}
}*/
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_bianYuanJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height){
	jint *cpixels;
		cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
		if (cpixels == NULL) {
		    	return 0; /* exception occurred */
			}
		int size=width * height;
		int *tmppixels1=(int *)malloc(sizeof(int)*size);
	//	int *tmppixels2=(int *)malloc (sizeof(int)*size);
	    int templttest1[]={1,2,1,0,0,0,-1,-2,-1};
	    int templttest2[]={-1,0,1,-2,0,2,-1,0,1};
	    int dGradOne,dGradTwo,dGrad;
	    int i=1,k=1;
	    for(i=0;i<height; i++)
	    	{	for(k=0;k<width;k++)
	    	       {	dGradOne=templt(cpixels,width,height,templttest1,3,k,i,3);
	    	            dGradTwo=templt(cpixels,width,height,templttest2,3,k,i,3);
	    	            dGrad=sqrt(dGradOne*dGradOne+dGradTwo*dGradTwo);
	    	            dGrad=Min(MAX_VALUE,Max(0,dGrad));
	    	            tmppixels1[i*width+k]=dGrad;
	    	       }
	    	}
	    for(i=0;i<height; i++)
	        {	for(k=0;k<width;k++)
	        	    {
	        	       dGrad=tmppixels1[i*width+k];
                       cpixels[i*width+k]=MAX_VALUE<<24|(dGrad&0xff)<<16|(dGrad&0xff)<<8|(dGrad&0xff);
	        	    }
	        }
	    	jintArray result = (*env)->NewIntArray(env,size);
	        (*env)->SetIntArrayRegion(env,result, 0, size,cpixels);
	    	(*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
	    	free(tmppixels1);
	    //	free(tmppixels2);
	    	return result;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_angulationJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height){
	jint *cpixels;
		cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
	    if (cpixels == NULL) {
	    	return 0; /* exception occurred */
		}
	    int size=width * height;
	    int *tmppixels=(int *)malloc(sizeof(int)*size);
	    int i=0,k=0;
	    for(i=1;i<height-1; i++)
	    {  for(k=1;k<width-1;k++)
	    	{
	    	tmppixels[i*width+k]=cpixels[i*width+k];
	    	}
	    }
		//int newR=0,newG=0,newB=0;
		int m=0,n=0;
		//int pixcolor=0;
		for(i=1;i<height-1; i++)
		   {	for(k=1;k<width-1;k++)
		        {
                   m=k-20*cos(0.03*i);
                  // n=i-20*cos(0.03*k);
                   n=i;
                   if(m>width||n>height||m<0||n<0)
                	   continue;
                   tmppixels[n*width+m]=cpixels[i*width+k];
		        }
		   }
		jintArray result = (*env)->NewIntArray(env,size);
		    (*env)->SetIntArrayRegion(env,result, 0, size, tmppixels);
		    (*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
		    free(tmppixels);
		    return result;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_angulationBallJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height){
	jint *cpixels;
			cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
		    if (cpixels == NULL) {
		    	return 0; /* exception occurred */
			}
		    int size=width * height;
		    int *tmppixels=(int *)malloc(sizeof(int)*size);
		    int i=0,k=0;
		    for(i=1;i<height-1; i++)
		    {  for(k=1;k<width-1;k++)
		    	{
		    	  //tmppixels[i*width+k]=cpixels[i*width+k];
		    	  tmppixels[i*width+k]=0;
		    	}
		    }
            float s=0,d=0,R=(width/2)>(height/2)?(width/2):(height/2);
            int x0=width/2,y0=height/2;
			int m=0,n=0;
			//int pixcolor=0;
			for(i=1;i<height-1; i++)
			   {	for(k=1;k<width-1;k++)
			        {
				       s=sqrt((k-x0)*(k-x0)+(i-y0)*(i-y0));
				       if(s<=R){
				    	   d=sin(3.14*s/(2*R))*R;
				    	   //d=(R/2)*asin(2*s/R);
				    	   m=(d/s)*(k-x0)+x0;
				    	   // n=i-20*cos(0.03*k);
				    	   n=(d/s)*(i-y0)+y0;
				    	   if(m>width||n>height||m<0||n<0)
				    	   	     continue;
				    	   if(sqrt((m-x0)*(m-x0)+(n-y0)*(n-y0))<=R){
				    		   tmppixels[n*width+m]=cpixels[i*width+k];
				    	   }
				    	   }
				       else{
				    	   tmppixels[i*width+k]=cpixels[i*width+k];
				       }

			        }
			   }
			for(i=1;i<height-1; i++)
			       {  for(k=1;k<width-1;k++)
			    	     {
			    	        if(tmppixels[i*width+k]==0)
			    	        	cpixels[i*width+k]=middle_smooth(tmppixels,width,k,i);
			    	        else
			    	        cpixels[i*width+k]=tmppixels[i*width+k];
			    	     }
			        }
			jintArray result = (*env)->NewIntArray(env,size);
			    (*env)->SetIntArrayRegion(env,result, 0, size, cpixels);
			    (*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
			    free(tmppixels);
			    return result;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_gauss_1SmoothJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height){
	jint *cpixels;
		cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
	    if (cpixels == NULL) {
	    	return 0; /* exception occurred */
		}
	    int gauss[]={1,2,1,2,4,2,1,2,1};
	    int size=width * height;
	    //int *tmppixels=(int *)malloc(sizeof(int)*size);
		int newR=0,newG=0,newB=0;
		int i=1,k=1;
	   // float delta=1.0/16;
	    for(i=1;i<height-1; i++)
	    {	for(k=1;k<width-1;k++)
	        {
	    	 newR=templt(cpixels,width,height,gauss,3,k,i,0)/16;
			 newG=templt(cpixels,width,height,gauss,3,k,i,1)/16;
			 newB=templt(cpixels,width,height,gauss,3,k,i,2)/16;
	    	 newR=Min(MAX_VALUE, Max(0,newR));
	    	 newG=Min(MAX_VALUE, Max(0,newG));
	    	 newB=Min(MAX_VALUE, Max(0,newB));
	    	 //tmppixels[i*width+k]=MAX_VALUE<<24|newR<<16|newG<<8|newB;
	    	 cpixels[i*width+k]=MAX_VALUE<<24|newR<<16|newG<<8|newB;
	        }
	    }
	    jintArray result = (*env)->NewIntArray(env,size);
	    (*env)->SetIntArrayRegion(env,result, 0, size, cpixels);
	    (*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
	   // free(tmppixels);
	    return result;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_polishJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height){
	jint *cpixels;
		cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
		if (cpixels == NULL) {
		    return 0; /* exception occurred */
		}
		int m,n;
		int R=1;
		int i=1,j=1;
		int size=width * height;
		for(i=1;i<height-1; i++)
			{	for(j=1;j<width-1;j++)
		        	{
		        	    m=i+rand()%(2*R+1)-R;
		        	    n=j+rand()%(2*R+1)-R;
		        	    if(m>height-1)
		        	    	m=height-1;
		        	    if(n>width-1)
		        	    	n=width-1;
	                    cpixels[i*width+j]=cpixels[m*width+n];
		        	}
			}
		jintArray result = (*env)->NewIntArray(env,size);
	    (*env)->SetIntArrayRegion(env,result, 0, size, cpixels);
		(*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
		return result;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_daoZhiJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height){
	jint *cpixels;
			cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
		    if (cpixels == NULL) {
		    	return 0; /* exception occurred */
			}
		    int size=width * height;
		    int *tmppixels=(int *)malloc(sizeof(int)*size);
			//int newR=0,newG=0,newB=0;
			int i=1,k=1;
		    for(i=0;i<width; i++)
		    {	for(k=0;k<height;k++)
		        {
		    		tmppixels[i*height+k]=cpixels[k*width+i];
		        }
		    }
		    jintArray result = (*env)->NewIntArray(env,size);
		    	    (*env)->SetIntArrayRegion(env,result, 0, size, tmppixels);
		    	    (*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
		    	    free(tmppixels);
		    	    return result;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_addFrameJni
  (JNIEnv *env, jclass class, jintArray dstpixels, jintArray framepixels, jint width, jint height, jint flag){
	jint *cdstpixels;
	jint *cframepixels;
	cdstpixels = (*env)->GetIntArrayElements(env,dstpixels, NULL);
	cframepixels = (*env)->GetIntArrayElements(env,framepixels, NULL);
	if (cdstpixels == NULL||cframepixels==NULL) {
	   return 0; /* exception occurred */
	}
    int size=width * height;
    int fpixcolor=0,pixR=0,pixG=0,pixB=0;
    int secha=20;
    int i=1,k=1;
    int alpha=0;
    for(i=0;i<height; i++)
    {	for(k=0;k<width;k++)
    	   {
    	    fpixcolor=cframepixels[i*width +k];
    	    if(flag==1){
    	      if(((fpixcolor&0xff000000)>>24)>200)
    	    	   alpha=1;
    	      else
    	    	   alpha=0;
    	    }
    	    else if(flag==2){
    	    	pixR=((fpixcolor&0x00FF0000) >> 16);
    	    	pixG=((fpixcolor&0x0000FF00) >> 8);
    	    	pixB=fpixcolor&0x000000FF;
    	    	if(pixR>(255-secha)&&pixG>(255-secha)&&pixB>(255-secha))
    	    	   alpha=0;
    	    	else
    	    	   alpha=1;
    	    }else
    	    {
    	        pixR=((fpixcolor&0x00FF0000) >> 16);
    	    	pixG=((fpixcolor&0x0000FF00) >> 8);
    	    	pixB=fpixcolor&0x000000FF;
    	    	if(pixR<secha&&pixG<secha&&pixB<secha)
    	    		alpha=0;
    	    	else
    	    		alpha=1;
    	    }
    	    cdstpixels[i*width+k]=cframepixels[i*width+k]*alpha+cdstpixels[i*width+k]*(1-alpha);
    	   }
    }
    jintArray result = (*env)->NewIntArray(env,size);
    (*env)->SetIntArrayRegion(env,result, 0, size, cdstpixels);
    (*env)->ReleaseIntArrayElements(env,dstpixels, cdstpixels, 0);
    (*env)->ReleaseIntArrayElements(env,framepixels, cframepixels, 0);
    return result;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_miHongJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height, jint flag){
	jint *cpixels;
		cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
		if (cpixels == NULL) {
		    return 0; /* exception occurred */
		}
		int pixR=0,pixG=0,pixB=0;
		int pixR2=0,pixG2=0,pixB2=0;
		int pixR3=0,pixG3=0,pixB3=0;
		int pixR1=0,pixG1=0,pixB1=0;
		int r1,r2,g1,g2,b1,b2;
		int pixColor=0;
		int newR=0,newG=0,newB=0;
		int i=1,k=1;
		for(i=1;i<height-1; i++)
			{	for(k=1;k<width-1;k++)
		        	{	pixColor=cpixels[i*width +k];
			            pixR=((pixColor&0x00FF0000) >> 16);
			            pixG=((pixColor&0x0000FF00) >> 8);
			            pixB=pixColor&0x000000FF;
			            pixColor=cpixels[(i+1)*width +k+1];
			            pixR1=((pixColor&0x00FF0000) >> 16);
			            pixG1=((pixColor&0x0000FF00) >> 8);
			            pixB1=pixColor&0x000000FF;
			            pixColor=cpixels[(i+1)*width +k];
			            pixR2=((pixColor&0x00FF0000) >> 16);
			            pixG2=((pixColor&0x0000FF00) >> 8);
			            pixB2=pixColor&0x000000FF;
			            pixColor=cpixels[i*width +k+1];
			            pixR3=((pixColor&0x00FF0000) >> 16);
			            pixG3=((pixColor&0x0000FF00) >> 8);
			            pixB3=pixColor&0x000000FF;
			            r1=(pixR-pixR1)*(pixR-pixR1);
			            r2=(pixR3-pixR2)*(pixR3-pixR2);
			            newR=sqrt(r1+r2);
			            g1=(pixG-pixG1)*(pixG-pixG1);
			            g2=(pixG3-pixG2)*(pixG3-pixG2);
			            newG=sqrt(g1+g2);
			            b1=(pixB-pixB1)*(pixB-pixB1);
			            b2=(pixB3-pixB2)*(pixB3-pixB2);
			            newB=sqrt(b1+b2);
			            if(flag>0){
			            	newR=newR+pixR;
			            	newB=newB+pixB;
			            	newG=newG+pixG;
			            }
			            newR=Min(MAX_VALUE, Max(0,newR));
			            newG=Min(MAX_VALUE, Max(0,newG));
			            newB=Min(MAX_VALUE, Max(0,newB));
			            cpixels[i*width+k]=MAX_VALUE<<24|newR<<16|newG<<8|newB;
		        	}
			}
		int size=width*height;
		jintArray result = (*env)->NewIntArray(env,size);
			    (*env)->SetIntArrayRegion(env,result, 0, size, cpixels);
				(*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
				return result;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_lineChangeJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height){
	jint *cpixels;
		cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
		if (cpixels == NULL) {
		    return 0; /* exception occurred */
		}
		int pixR=0,pixG=0,pixB=0;
		int pixColor=0;
		int newR=0,newG=0,newB=0,Gray=0;
		int i=1,k=1;
		float a=1.4;
		int b=5;
		for(i=0;i<height; i++)
			{	for(k=0;k<width;k++)
		        	{	pixColor=cpixels[i*width +k];
			            pixR=((pixColor&0x00FF0000) >> 16);
			            pixG=((pixColor&0x0000FF00) >> 8);
			            pixB=pixColor&0x000000FF;
			            newR=a*pixR+b;
			            newG=a*pixG+b;
			            newB=a*pixG+b;
			            newR=Min(MAX_VALUE, Max(0,newR));
			            newG=Min(MAX_VALUE, Max(0,newG));
			            newB=Min(MAX_VALUE, Max(0,newB));
	                    cpixels[i*width+k]=MAX_VALUE<<24|newR<<16|newG<<8|newB;
		        	}
			}
		int size=width * height;
		jintArray result = (*env)->NewIntArray(env,size);
	    (*env)->SetIntArrayRegion(env,result, 0, size, cpixels);
		(*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
		return result;

}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_grayFaZhiJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height){
	jint *cpixels;
			cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
			if (cpixels == NULL) {
			    return 0; /* exception occurred */
			}
			int pixR=0,pixG=0,pixB=0;
			int pixColor=0;
			int newR=0,newG=0,newB=0,Gray=0;
			int i=1,k=1;
			int T=110;
			for(i=0;i<height; i++)
				{	for(k=0;k<width;k++)
			        	{	pixColor=cpixels[i*width +k];
				            pixR=((pixColor&0x00FF0000) >> 16);
				            pixG=((pixColor&0x0000FF00) >> 8);
				            pixB=pixColor&0x000000FF;
				            newR=0;
				            newG=0;
				            newB=0;
				            if(pixR>T)
				            	newR=255;
				            if(pixG>T)
				            	newG=255;
				            if(pixB>T)
				            	newB=255;
		                    cpixels[i*width+k]=MAX_VALUE<<24|newR<<16|newG<<8|newB;
			        	}
				}
			int size=width * height;
			jintArray result = (*env)->NewIntArray(env,size);
		    (*env)->SetIntArrayRegion(env,result, 0, size, cpixels);
			(*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
			return result;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_windowJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height){
	jint *cpixels;
				cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
				if (cpixels == NULL) {
				    return 0; /* exception occurred */
				}
				int pixR=0,pixG=0,pixB=0;
				int pixColor=0;
				int newR=0,newG=0,newB=0,Gray=0;
				int i=1,k=1;
				int T1=50,T2=200;
				for(i=0;i<height; i++)
					{	for(k=0;k<width;k++)
				        	{	pixColor=cpixels[i*width +k];
					            pixR=((pixColor&0x00FF0000) >> 16);
					            pixG=((pixColor&0x0000FF00) >> 8);
					            pixB=pixColor&0x000000FF;
					            if(pixR>T2)
					            	newR=255;
					            else if(pixR<T1)
					            	newR=0;
					            else
					            	newR=pixR;
					            if(pixR>T2)
					                newG=255;
					            else if(pixR<T1)
					                newG=0;
					            else
					            	newG=pixG;
					            if(pixB>T2)
					            	newB=255;
					            else if(pixB<T1)
					                newB=0;
					            else
					                newB=pixB;
			                    cpixels[i*width+k]=MAX_VALUE<<24|newR<<16|newG<<8|newB;
				        	}
					}
				int size=width * height;
				jintArray result = (*env)->NewIntArray(env,size);
			    (*env)->SetIntArrayRegion(env,result, 0, size, cpixels);
				(*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
				return result;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_exposureJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height){
	jint *cpixels;
				cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
				if (cpixels == NULL) {
				    return 0; /* exception occurred */
				}
				int pixR=0,pixG=0,pixB=0;
				int pixColor=0;
				int newR=0,newG=0,newB=0;
				int i=1,k=1;
				int T=128;
				for(i=0;i<height; i++)
					{	for(k=0;k<width;k++)
				        	{	pixColor=cpixels[i*width +k];
					            pixR=((pixColor&0x00FF0000) >> 16);
					            pixG=((pixColor&0x0000FF00) >> 8);
					            pixB=pixColor&0x000000FF;
					            newR=0;
					            newG=0;
					            newB=0;
					            if(pixR<T)
					            	newR=255-pixR;
					            if(pixG<T)
					            	newG=255-pixG;
					            if(pixB<T)
					            	newB=255-pixB;
			                    cpixels[i*width+k]=MAX_VALUE<<24|newR<<16|newG<<8|newB;
				        	}
					}
				int size=width * height;
				jintArray result = (*env)->NewIntArray(env,size);
			    (*env)->SetIntArrayRegion(env,result, 0, size, cpixels);
				(*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
				return result;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_magicMirrorJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height){
	jint *cpixels;
		cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
	    if (cpixels == NULL) {
	    	return 0; /* exception occurred */
		}
	    int size=width * height;
	    int *tmppixels=(int *)malloc(sizeof(int)*size);
	    int i=0,k=0;
	    for(i=1;i<height-1; i++)
	    {  for(k=1;k<width-1;k++)
	    	{
	    	tmppixels[i*width+k]=cpixels[i*width+k];
	    	}
	    }
	    int centerX = width/2;
	    int centerY = height/2;
	    double theta, radius;
	    double newX, newY;
	    int offsetX = 0, offsetY = 0;
	    double factor=12.0;
		int ta = 0, tr = 0, tg = 0, tb = 0;
		int index=0;
		for(i=1;i<height-1; i++)
		   {	for(k=1;k<width-1;k++)
		        {
			       int trueX = k - centerX;
			       int trueY = i - centerY;
			   	   theta = atan2(trueY,trueX);
			       radius = sqrt(trueX*trueX + trueY*trueY);
			       double newRadius = sqrt(radius) * factor;
			       newX = centerX + (newRadius * cos(theta));
			       newY = centerY + (newRadius * sin(theta));
			       if (newX > 0 && newX < width) {
			       offsetX = (int)newX;
			       }
			       else {
			       offsetX = 0;
			       }

			       if (newY > 0 && newY < height) {
			               offsetY = (int)newY;
			       } else
			       {
			          offsetY = 0;
			       }
			       index = offsetY * width + offsetX;
			       ta = (cpixels[index] >> 24) & 0xff;
			       tr = (cpixels[index] >> 16) & 0xff;
			       tg = (cpixels[index] >> 8) & 0xff;
			       tb = cpixels[index] & 0xff;
			       tmppixels[i*width+k]=(ta << 24) | (tr << 16) | (tg << 8) | tb;

		        }
		   }
		jintArray result = (*env)->NewIntArray(env,size);
		    (*env)->SetIntArrayRegion(env,result, 0, size, tmppixels);
		    (*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
		    free(tmppixels);
		    return result;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_swirlJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height){
	jint *cpixels;
			cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
		    if (cpixels == NULL) {
		    	return 0; /* exception occurred */
			}
		    int size=width * height;
		    int *tmppixels=(int *)malloc(sizeof(int)*size);
		    int i=0,k=0;
		    for(i=1;i<height-1; i++)
		    {  for(k=1;k<width-1;k++)
		    	{
		    	tmppixels[i*width+k]=cpixels[i*width+k];
		    	}
		    }
		    int centerX = width/2;
		    int centerY = height/2;
		    double theta, radius;
		    double newX, newY;
		    int offsetX = 0, offsetY = 0;
		    double degree=0.01;
			int ta = 0, tr = 0, tg = 0, tb = 0;
			int index=0;
			for(i=1;i<height-1; i++)
			   {	for(k=1;k<width-1;k++)
			        {
				       int trueX = k - centerX;
				       int trueY = i - centerY;
				   	   theta = atan2(trueY,trueX);
				       radius = sqrt(trueX*trueX + trueY*trueY);
				       newX = centerX + (radius * cos(theta+degree*radius));
				       newY = centerY + (radius * sin(theta+degree*radius));
				       if (newX > 0 && newX < width) {
				       offsetX = (int)newX;
				       }
				       else {
				       offsetX = 0;
				       }

				       if (newY > 0 && newY < height) {
				               offsetY = (int)newY;
				       } else
				       {
				          offsetY = 0;
				       }
				       index = offsetY * width + offsetX;
				       ta = (cpixels[index] >> 24) & 0xff;
				       tr = (cpixels[index] >> 16) & 0xff;
				       tg = (cpixels[index] >> 8) & 0xff;
				       tb = cpixels[index] & 0xff;
				       tmppixels[i*width+k]=(ta << 24) | (tr << 16) | (tg << 8) | tb;

			        }
			   }
			jintArray result = (*env)->NewIntArray(env,size);
			    (*env)->SetIntArrayRegion(env,result, 0, size, tmppixels);
			    (*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
			    free(tmppixels);
			    return result;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_fuDiaoJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height){
	jint *cpixels;
		cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
		if (cpixels == NULL) {
		    return 0; /* exception occurred */
		}
		int pixR=0,pixG=0,pixB=0;
		int pixColor=0;
		int newR=0,newG=0,newB=0;
		int i=1,k=1;
		int pos=0;
		for(i=0;i<height-1; i++)
			{	for(k=0;k<width-1;k++)
		        	{	pos=i*width+k;
				        pixColor=cpixels[pos];
			            pixR=((pixColor&0x00FF0000) >> 16);
			            pixG=((pixColor&0x0000FF00) >> 8);
			            pixB=pixColor&0x000000FF;
			            pixColor=cpixels[pos+1];
			            newR=((pixColor&0x00FF0000) >> 16)-pixR+128;
			            newG=((pixColor&0x0000FF00) >> 8)-pixG+128;
			            newB=(pixColor&0x000000FF)-pixB+128;
			            newR=Min(MAX_VALUE, Max(0,newR));
			            newG=Min(MAX_VALUE, Max(0,newG));
			            newB=Min(MAX_VALUE, Max(0,newB));
	                    cpixels[pos]=MAX_VALUE<<24|newR<<16|newG<<8|newB;
		        	}
			}
		int size=width * height;
		jintArray result = (*env)->NewIntArray(env,size);
	    (*env)->SetIntArrayRegion(env,result, 0, size, cpixels);
		(*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
		return result;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_bianYuanSharpJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height){
	jint *cpixels;
			cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
			if (cpixels == NULL) {
			    return 0; /* exception occurred */
			}
			int pixR=0,pixG=0,pixB=0;
			int pixR2=0,pixG2=0,pixB2=0;
			int pixColor=0;
			int newR=0,newG=0,newB=0;
			float m=0.5;
			int i=1,k=1;
			for(i=1;i<height; i++)
				{	for(k=1;k<width;k++)
			        	{	pixColor=cpixels[i*width +k];
				            pixR=((pixColor&0x00FF0000) >> 16);
				            pixG=((pixColor&0x0000FF00) >> 8);
				            pixB=pixColor&0x000000FF;
				            pixColor=cpixels[(i-1)*width +k-1];
				            pixR2=((pixColor&0x00FF0000) >> 16);
				            pixG2=((pixColor&0x0000FF00) >> 8);
				            pixB2=pixColor&0x000000FF;
				            newR=pixR+m*abs(pixR-pixR2);
				            newG=pixG+m*abs(pixG-pixG2);
				            newB=pixB+m*abs(pixB-pixB2);
				            newR=Min(MAX_VALUE, Max(0,newR));
				            newG=Min(MAX_VALUE, Max(0,newG));
				            newB=Min(MAX_VALUE, Max(0,newB));
				            cpixels[i*width+k]=MAX_VALUE<<24|newR<<16|newG<<8|newB;
			        	}
				}
			int size=width*height;
			jintArray result = (*env)->NewIntArray(env,size);
				    (*env)->SetIntArrayRegion(env,result, 0, size, cpixels);
					(*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
					return result;
}

JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_addSaltAndPepperNoiseJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height){
	jint *cpixels;
			cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
			if (cpixels == NULL) {
			    return 0; /* exception occurred */
			}
			int index = 0;
			int size=width*height;
			float SNR=0.98;
			int count = (int)(size* (1-SNR));
			//int count=3;
			int i=0;
		    for(i=0; i<count; i++) {
			int row = rand()%height;
			int col = rand()%width;
			index = row * width + col;
			cpixels[index] = (255 << 24) | (255 << 16) | (255 << 8) | 255;
			       }
			jintArray result = (*env)->NewIntArray(env,size);
				    (*env)->SetIntArrayRegion(env,result, 0, size, cpixels);
					(*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
					return result;
}
double gaussrand()
{
    static double V1, V2, S;
    static int phase = 0;
    double X;

    if ( phase == 0 ) {
        do {
            double U1 = (double)rand() / RAND_MAX;
            double U2 = (double)rand() / RAND_MAX;

            V1 = 2 * U1 - 1;
            V2 = 2 * U2 - 1;
            S = V1 * V1 + V2 * V2;
        } while(S >= 1 || S == 0);

        X = V1 * sqrt(-2 * log(S) / S);
    } else
        X = V2 * sqrt(-2 * log(S) / S);

    phase = 1 - phase;

    return X;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_addGauseNoiseJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height){
	jint *cpixels;
				cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
				if (cpixels == NULL) {
				    return 0; /* exception occurred */
				}
				int pixR=0,pixG=0,pixB=0,pixA=0;
				int pixColor=0;
				int newR=0,newG=0,newB=0;
				int i=1,k=1;
				float delta=0,sig=20.0;
				for(i=0;i<height; i++)
					{	for(k=0;k<width;k++)
				        	{	pixColor=cpixels[i*width +k];
				        	    pixA=((pixColor&0xFF000000) >> 24);
					            pixR=((pixColor&0x00FF0000) >> 16);
					            pixG=((pixColor&0x0000FF00) >> 8);
					            pixB=pixColor&0x000000FF;
					            newR=pixR+sig*gaussrand()+delta;
					            newG=pixG+sig*gaussrand()+delta;
					            newB=pixB+sig*gaussrand()+delta;
					            newR=Min(MAX_VALUE, Max(0,newR));
					            newG=Min(MAX_VALUE, Max(0,newG));
					            newB=Min(MAX_VALUE, Max(0,newB));
			                    cpixels[i*width+k]=pixA<<24|newR<<16|newG<<8|newB;
				        	}
					}
				int size=width * height;
				jintArray result = (*env)->NewIntArray(env,size);
			    (*env)->SetIntArrayRegion(env,result, 0, size, cpixels);
				(*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
				return result;

}

JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_middle_1SmoothJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height){
	jint *cpixels;
			cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
			if (cpixels == NULL) {
			    return 0; /* exception occurred */
			}
			int i=1,k=1;
			for(i=0;i<height; i++)
				{	for(k=0;k<width;k++)
			        	{
					      cpixels[i*width+k]=middle_smooth(cpixels,width,k,i);
			        	}
				}
			int size=width * height;
			jintArray result = (*env)->NewIntArray(env,size);
		    (*env)->SetIntArrayRegion(env,result, 0, size, cpixels);
			(*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
			return result;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_ave_1SmoothJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height){
	jint *cpixels;
			cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
			if (cpixels == NULL) {
			    return 0; /* exception occurred */
			}
			int i=1,k=1;
			for(i=0;i<height; i++)
				{	for(k=0;k<width;k++)
			        	{
					      cpixels[i*width+k]=ave_smooth(cpixels,width,k,i);
			        	}
				}
			int size=width * height;
			jintArray result = (*env)->NewIntArray(env,size);
		    (*env)->SetIntArrayRegion(env,result, 0, size, cpixels);
			(*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
			return result;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_move_1SmoothJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height, jfloat distance, jfloat zoom,jint angle){
	jint *cpixels;
			cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
			if (cpixels == NULL) {
			    return 0; /* exception occurred */
			}
			int size=width * height;
			int *tmppixels=(int *)malloc(sizeof(int)*size);
			int index = 0;
			//int angle=0;
			int cx = width/2;
			int cy = height/2;
 // calculate the triangle geometry value
			float sinAngle = sin(angle/180.0f * 3.14);
			float coseAngle =cos(angle/180.0f * 3.14);
 // calculate the distance, same as box blur
			float imageRadius = sqrt(cx*cx + cy*cy);
			float maxDistance = distance + imageRadius * zoom;
            int iteration = (int)maxDistance;
            int row=0,col=0;
			for(row=0; row<height; row++) {
			   int ta = 0, tr = 0, tg = 0, tb = 0;
			for( col=0; col<width; col++) {
			   int newX= col, count = 0;
			   int newY = row;
// iterate the source pixels according to distance
			   float m11 = 0.0f, m22 = 0.0f;
			   int i=0;
			   for(i=0; i<iteration; i++) {
			         newX = col;
			         newY = row;
			         // calculate the operator source pixel
			        if(distance > 0) {
			            newY = newY + i*sinAngle;
			            newX = newX + i*coseAngle;
			         }
			        float f = (float)i/iteration;
			            if (newX < 0 || newX >= width) {
			                        break;
			              }
			            if (newY < 0 || newY >= height) {
			                        break;
			              }
			                    // scale the pixels
			            float scale = 1-zoom*f;
			             m11 = cx - cx*scale;
			             m22 = cy - cy*scale;
			             newY =newY * scale + m22;
			             newX =newX * scale + m11;
			                    // blur the pixels, here
			                    count++;
			             int rgb = cpixels[newY*width+newX];
			                    ta += (rgb >> 24) & 0xff;
			                    tr += (rgb >> 16) & 0xff;
			                    tg += (rgb >> 8) & 0xff;
			                    tb += rgb & 0xff;
			                }

			                // fill the destination pixel with final RGB value
			                if (count == 0) {
			                    tmppixels[index] = cpixels[index];
			                } else {
			                    ta = Min(MAX_VALUE, Max(0,ta/count));
			                    tr = Min(MAX_VALUE, Max(0,tr/count));
			                    tg = Min(MAX_VALUE, Max(0,tg/count));
			                    tb = Min(MAX_VALUE, Max(0,tb/count));
			                    tmppixels[index] = (ta << 24) | (tr << 16) | (tg << 8) | tb;
			                }
			                index++;
			            }
			        }
			jintArray result = (*env)->NewIntArray(env,size);
						    (*env)->SetIntArrayRegion(env,result, 0, size, tmppixels);
						    (*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
						    free(tmppixels);
						    return result;
}
int getNewintensityRate(int grayHis[], double totalPixelNumber, int index) {
        double sum = 0;
        int i=0;
        for(i=0; i<=index; i++) {
            sum += ((double)grayHis[i])/totalPixelNumber;
        }
        return (int)(sum * 255.0);
    }
void generateHEData(int newrgbhis[3][256], int rgbhis[3][256], int totalPixelNumber, int grayLevel) {
    int i=0;
	for(i=0; i<grayLevel; i++) {
            newrgbhis[0][i] = getNewintensityRate(rgbhis[0], totalPixelNumber, i);
            newrgbhis[1][i] = getNewintensityRate(rgbhis[1], totalPixelNumber, i);
            newrgbhis[2][i] = getNewintensityRate(rgbhis[2], totalPixelNumber, i);
        }
    }
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_histogramFilterJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height){
	jint *cpixels;
			cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
			if (cpixels == NULL) {
			    return 0; /* exception occurred */
			}
			int rgbhis[3][256]={0}; // RGB
			int newrgbhis[3][256]={0}; // after HE
			int i=0,j=0;
			int index = 0;
			int totalPixelNumber = height * width;
			int *tmppixels=(int *)malloc(sizeof(int)*totalPixelNumber);
			int row=0,col=0;
			for(row=0; row<height; row++) {
			    int ta = 0, tr = 0, tg = 0, tb = 0;
			    for(col=0; col<width; col++) {
			       index = row * width + col;
			       ta = (cpixels[index] >> 24) & 0xff;
			       tr = (cpixels[index] >> 16) & 0xff;
			       tg = (cpixels[index] >> 8) & 0xff;
			       tb = cpixels[index] & 0xff;

			                // generate original source image RGB histogram
			                rgbhis[0][tr]++;
			                rgbhis[1][tg]++;
			                rgbhis[2][tb]++;
			            }
			        }

			        // generate original source image RGB histogram
			        generateHEData(newrgbhis, rgbhis, totalPixelNumber, 256);
			        for(row=0; row<height; row++) {
			            int ta = 0, tr = 0, tg = 0, tb = 0;
			            for(col=0; col<width; col++) {
			                index = row * width + col;
			                ta = (cpixels[index] >> 24) & 0xff;
			                tr = (cpixels[index] >> 16) & 0xff;
			                tg = (cpixels[index] >> 8) & 0xff;
			                tb = cpixels[index] & 0xff;

			                // get output pixel now...
			                tr = newrgbhis[0][tr];
			                tg = newrgbhis[1][tg];
			                tb = newrgbhis[2][tb];

			                tmppixels[index] = (ta << 24) | (tr << 16) | (tg << 8) | tb;
			            }
			        }
			        jintArray result = (*env)->NewIntArray(env,totalPixelNumber);
			        (*env)->SetIntArrayRegion(env,result, 0, totalPixelNumber, tmppixels);
			        (*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
			        free(tmppixels);
			        return result;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_mosaicJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height){
	jint *cpixels;
			cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
			if (cpixels == NULL) {
			    return 0; /* exception occurred */
			}
			int length=width * height;
			int *tmppixels=(int *)malloc(sizeof(int)*length);
			int size=5;
			int index = 0;
			int offsetX = 0, offsetY = 0;
			int newX = 0, newY = 0;
			int total = size*size;
			int sumred = 0, sumgreen = 0, sumblue = 0;
			int row=0,col=0;
			        for(row=0; row<height; row++) {
			            int ta = 0, tr = 0, tg = 0, tb = 0;
			            for(col=0; col<width; col++) {
			                newY = (row/size) * size;
			                newX = (col/size) * size;
			                offsetX = newX + size;
			                offsetY = newY + size;
			                int subRow=0,subCol=0;
			                for(subRow =newY; subRow < offsetY; subRow++) {
			                    for(subCol =newX; subCol < offsetX; subCol++) {
			                        if(subRow <0 || subRow >= height) {
			                            continue;
			                        }
			                        if(subCol < 0 || subCol >=width) {
			                            continue;
			                        }
			                        index = subRow * width + subCol;
			                        ta = (cpixels[index] >> 24) & 0xff;
			                        sumred += (cpixels[index] >> 16) & 0xff;
			                        sumgreen += (cpixels[index] >> 8) & 0xff;
			                        sumblue += cpixels[index] & 0xff;
			                    }
			                }
			                index = row * width + col;
			                tr = (int)(sumred/total);
			                tg = (int)(sumgreen/total);
			                tb = (int)(sumblue/total);
			                tmppixels[index] = (ta << 24) | (tr << 16) | (tg << 8) | tb;

			                sumred = sumgreen = sumblue = 0; // reset them...
			            }
			        }
			        jintArray result = (*env)->NewIntArray(env,length);
			        			        (*env)->SetIntArrayRegion(env,result, 0, length, tmppixels);
			        			        (*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
			        			        free(tmppixels);
			        			        return result;
}
int Multiply(int v1,int v2){
	return v1*v2/255;
}
int Dissolve(int v1,int v2){
	float t=0.6;
	return v1*t+v2*(1-t);
}
int Screen(int v1,int v2){
	return v1+v2-v1*v2/255;
}
int Overlay(int v1,int v2){
	return (v2<128)?(2*v1*v2/255):(255-2*(255-v1)*(255-v2)/255);
}
int SoftLight(int v1,int v2){
	if (v1>127.5){
	         return v2+(255-v2)*((v1-127.5)/127.5)*(0.5-abs(v2-127.5)/255);
	      }else{
	         return v2-v2*((127.5-v1)/127.5)*(0.5-abs(v2-127.5)/255);
	      }
}
int HardLight(int v1,int v2){
	if (v1>127.5){
	         return v2+(255-v2)*((v1-127.5)/127.5);
	      }else{
	         return v2*v1/127.5;
	      }
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_blendingJni
  (JNIEnv *env, jclass class, jintArray dstpixels, jintArray framepixels, jint width, jint height, jint flag){
	jint *cdstpixels;
	jint *cframepixels;
	cdstpixels = (*env)->GetIntArrayElements(env,dstpixels, NULL);
	cframepixels = (*env)->GetIntArrayElements(env,framepixels, NULL);
	if (cdstpixels == NULL||cframepixels==NULL) {
	   return 0; /* exception occurred */
	}
    int size=width * height;
    int dpixcolor=0,pixR=0,pixG=0,pixB=0;
    int fpixcolor=0,pixR2=0,pixG2=0,pixB2=0;
    int newR=0,newG=0,newB=0;
    int i=1,k=1;
    for(i=0;i<height; i++)
    {	for(k=0;k<width;k++)
    	   {
    	    dpixcolor=cdstpixels[i*width +k];
    	    pixR=((dpixcolor&0x00FF0000) >> 16);
    	    pixG=((dpixcolor&0x0000FF00) >> 8);
    	    pixB=dpixcolor&0x000000FF;
    	    fpixcolor=cframepixels[i*width +k];
    	    pixR2=((fpixcolor&0x00FF0000) >> 16);
    	    pixG2=((fpixcolor&0x0000FF00) >> 8);
    	    pixB2=fpixcolor&0x000000FF;
    	    switch(flag){
    	            	    case 1:
    	                    newR=Multiply(pixR2,pixR);
    	                    newG=Multiply(pixG2,pixG);
    	                    newB=Multiply(pixB2,pixB);
    	                    break;
    	            	    case 2:
    	            	    newR=Dissolve(pixR2,pixR);
    	            	    newG=Dissolve(pixG2,pixG);
    	            	    newB=Dissolve(pixB2,pixB);
    	            	    break;
    	            	    case 3:
    	            	    newR=Screen(pixR2,pixR);
    	            	    newG=Screen(pixG2,pixG);
    	            	    newB=Screen(pixB2,pixB);
    	                    break;
    	            	    case 4:
    	            	    newR=Overlay(pixR2,pixR);
    	            	    newG=Overlay(pixG2,pixG);
    	            	    newB=Overlay(pixB2,pixB);
    	            	    break;
    	            	    case 5:
    	            	    newR=SoftLight(pixR2,pixR);
    	            	    newG=SoftLight(pixG2,pixG);
    	            	    newB=SoftLight(pixB2,pixB);
    	            	    break;
    	            	    case 6:
    	            	    newR=HardLight(pixR2,pixR);
    	            	    newG=HardLight(pixG2,pixG);
    	            	    newB=HardLight(pixB2,pixB);
    	            	    break;
    	            	    }
    	    cdstpixels[i*width+k]=MAX_VALUE<<24|newR<<16|newG<<8|newB;
    	   }
    }
    jintArray result = (*env)->NewIntArray(env,size);
    (*env)->SetIntArrayRegion(env,result, 0, size, cdstpixels);
    (*env)->ReleaseIntArrayElements(env,dstpixels, cdstpixels, 0);
    (*env)->ReleaseIntArrayElements(env,framepixels, cframepixels, 0);
    return result;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_ChannelMixFilterJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height){
	jint *cpixels;
		cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
		if (cpixels == NULL) {
		    return 0; /* exception occurred */
		}
		int pixR=0,pixG=0,pixB=0;
		int pixColor=0;
		int newR=0,newG=0,newB=0;
		int i=1,k=1;
		int pos=0;
        int intoR=220,blueGreen=127,intoG=100,intoB=80,greenRed=127,redBlue=200;
		for(i=0;i<height-1; i++)
			{	for(k=0;k<width-1;k++)
		        	{	pos=i*width+k;
				        pixColor=cpixels[pos];
			            pixR=((pixColor&0x00FF0000) >> 16);
			            pixG=((pixColor&0x0000FF00) >> 8);
			            pixB=pixColor&0x000000FF;
			            pixColor=cpixels[pos+1];
			            newR=(intoR*(blueGreen*pixG+(255-blueGreen)*pixB)/255 + (255-intoR)*pixR)/255;
			            newG=(intoG*(redBlue*pixB+(255-redBlue)*pixR)/255+(255-intoG)*pixG)/255;
			            newB=(intoB*(greenRed*pixR+(255-greenRed)*pixG)/255+(255-intoB)*pixB)/255;
			            newR=Min(MAX_VALUE, Max(0,newR));
			            newG=Min(MAX_VALUE, Max(0,newG));
			            newB=Min(MAX_VALUE, Max(0,newB));
	                    cpixels[pos]=MAX_VALUE<<24|newR<<16|newG<<8|newB;
		        	}
			}
		int size=width * height;
		jintArray result = (*env)->NewIntArray(env,size);
	    (*env)->SetIntArrayRegion(env,result, 0, size, cpixels);
		(*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
		return result;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_MaskFilterJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height){
	jint *cpixels;
		cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
		if (cpixels == NULL) {
		    return 0; /* exception occurred */
		}
		int msk=0xff00ffff;
		int pixColor=0;
		int i=1,k=1;
		int pos=0;
		for(i=0;i<height-1; i++)
			{	for(k=0;k<width-1;k++)
		        	{	pos=i*width+k;
				        pixColor=cpixels[pos];
	                    cpixels[pos]=pixColor&msk;
		        	}
			}
		int size=width * height;
		jintArray result = (*env)->NewIntArray(env,size);
	    (*env)->SetIntArrayRegion(env,result, 0, size, cpixels);
		(*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
		return result;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_SolarizeFilterJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height){
	jint *cpixels;
				cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
				if (cpixels == NULL) {
				    return 0; /* exception occurred */
				}
				int pixR=0,pixG=0,pixB=0;
				int pixColor=0;
				int newR=0,newG=0,newB=0;
				int i=1,k=1;
				int T=127;
				for(i=0;i<height; i++)
					{	for(k=0;k<width;k++)
				        	{	pixColor=cpixels[i*width +k];
					            pixR=((pixColor&0x00FF0000) >> 16);
					            pixG=((pixColor&0x0000FF00) >> 8);
					            pixB=pixColor&0x000000FF;
					            newR=pixR>T?(2*pixR-255):(255-2*pixR);
					            newG=pixG>T?(2*pixG-255):(255-2*pixG);
					            newB=pixB>T?(2*pixB-255):(255-2*pixB);
			                    cpixels[i*width+k]=MAX_VALUE<<24|newR<<16|newG<<8|newB;
				        	}
					}
				int size=width * height;
				jintArray result = (*env)->NewIntArray(env,size);
			    (*env)->SetIntArrayRegion(env,result, 0, size, cpixels);
				(*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
				return result;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_PosterizeFilterJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height){
	jint *cpixels;
				cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
				if (cpixels == NULL) {
				    return 0; /* exception occurred */
				}
				int pixR=0,pixG=0,pixB=0;
				int pixColor=0;
				int newR=0,newG=0,newB=0;
				int i=1,k=1;
				int T=127;
				int levels=8;
				for(i=0;i<height; i++)
					{	for(k=0;k<width;k++)
				        	{	pixColor=cpixels[i*width +k];
					            pixR=((pixColor&0x00FF0000) >> 16);
					            pixG=((pixColor&0x0000FF00) >> 8);
					            pixB=pixColor&0x000000FF;
					            newR=255*(levels*pixR/256)/(levels-1);
					            newG=255*(levels*pixG/256)/(levels-1);
					            newB=255*(levels*pixB/256)/(levels-1);
			                    cpixels[i*width+k]=MAX_VALUE<<24|newR<<16|newG<<8|newB;
				        	}
					}
				int size=width * height;
				jintArray result = (*env)->NewIntArray(env,size);
			    (*env)->SetIntArrayRegion(env,result, 0, size, cpixels);
				(*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
				return result;
}
int clamp(int v,int min,int max){
	return Min(max, Max(min,v));
}
void blur( int in[], int out[], int width, int height, int radius ) {
        int widthMinus1 = width-1;
        int r = radius;
        int tableSize = 2*r+1;
        //int divide[256*tableSize] = {0};
        int *divide=(int *)malloc(sizeof(int)*(256*tableSize));
        int i=0;
        for ( i = 0; i < 256*tableSize; i++ )
            divide[i] = i/tableSize;

        int inIndex = 0;
        int y=0;
        for ( y = 0; y < height; y++ ) {
            int outIndex = y;
            int ta = 0, tr = 0, tg = 0, tb = 0;
            int i=0;
            for ( i = -r; i <= r; i++ ) {
                int rgb = in[inIndex +clamp(i, 0, width-1)];
                ta += (rgb >> 24) & 0xff;
                tr += (rgb >> 16) & 0xff;
                tg += (rgb >> 8) & 0xff;
                tb += rgb & 0xff;
            }
            int x=0;
            for ( x = 0; x < width; x++ ) {
                out[ outIndex ] = (divide[ta] << 24) | (divide[tr] << 16) | (divide[tg] << 8) | divide[tb];

                int i1 = x+r+1;
                if ( i1 > widthMinus1 )
                    i1 = widthMinus1;
                int i2 = x-r;
                if ( i2 < 0 )
                    i2 = 0;
                int rgb1 = in[inIndex+i1];
                int rgb2 = in[inIndex+i2];

                ta += ((rgb1 >> 24) & 0xff)-((rgb2 >> 24) & 0xff);
                tr += ((rgb1 & 0xff0000)-(rgb2 & 0xff0000)) >> 16;
                tg += ((rgb1 & 0xff00)-(rgb2 & 0xff00)) >> 8;
                tb += (rgb1 & 0xff)-(rgb2 & 0xff);
                outIndex += height;
            }
            inIndex += width;
        }
        free(divide);
    }
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_BoxBlurFilterJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height){
	jint *cpixels;
				cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
				if (cpixels == NULL) {
				    return 0; /* exception occurred */
				}
				int hRadius=3,vRadius=5;
				int iterations=1;
				int length=width * height;
				int *tmppixels=(int *)malloc(sizeof(int)*length);
				int i=0;
				for (i = 0; i < iterations; i++ ) {
				    blur( cpixels, tmppixels, width, height, hRadius );
				    blur( tmppixels, cpixels, height, width, vRadius );
				 }
				jintArray result = (*env)->NewIntArray(env,length);
			    (*env)->SetIntArrayRegion(env,result, 0, length, cpixels);
				(*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
				free(tmppixels);
				return result;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_BumpFilterJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height){
	jint *cpixels;
	cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
    if (cpixels == NULL) {
    	return 0; /* exception occurred */
	}
    int matrix[]={-1,-1,0,-1,1,1,0,1,1};
    int size=width * height;
    int *tmppixels=(int *)malloc(sizeof(int)*size);
	int newR=0,newG=0,newB=0;
	int i=1,k=1;
    float delta=0.3;
    for(i=1;i<height-1; i++)
    {	for(k=1;k<width-1;k++)
        {
    	 newR=delta*templt(cpixels,width,height,matrix,3,k,i,0);
		 newG=delta*templt(cpixels,width,height,matrix,3,k,i,1);
		 newB=delta*templt(cpixels,width,height,matrix,3,k,i,2);
    	 newR=Min(MAX_VALUE, Max(0,newR));
    	 newG=Min(MAX_VALUE, Max(0,newG));
    	 newB=Min(MAX_VALUE, Max(0,newB));
    	 tmppixels[i*width+k]=MAX_VALUE<<24|newR<<16|newG<<8|newB;
        }
    }
    jintArray result = (*env)->NewIntArray(env,size);
    (*env)->SetIntArrayRegion(env,result, 0, size, tmppixels);
    (*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
    free(tmppixels);
    return result;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_WaterFilterJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height){
	jint *cpixels;
	cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
    if (cpixels == NULL) {
    	return 0; /* exception occurred */
	}
    int size=width * height;
    int *tmppixels=(int *)malloc(sizeof(int)*size);
    int period=128;
    int wave=10;
    int index = 0, index2 = 0;
            int xoffset = 0, yoffset = 0;
            int row=0;
            for(row=0; row<height; row++) {
            	int ta = 0, tr = 0, tg = 0, tb = 0;
            	int col=0;
                for(col=0; col<width; col++) {
                    xoffset = wave*sin(2.0*3.14*row/period);
                    yoffset = wave*cos(2.0*3.14*col/period);
                    xoffset = xoffset + col;
                    yoffset = yoffset + row;
                    if(xoffset < 0 || xoffset >=width) {
                        xoffset = 0;
                    }
                    if(yoffset < 0 || yoffset >=height) {
                        yoffset = 0;
                    }
                    index = row * width + col;
                    index2 =yoffset*width+xoffset;
                    ta = (cpixels[index2] >> 24) & 0xff;
                    tr = (cpixels[index2] >> 16) & 0xff;
                    tg = (cpixels[index2] >> 8) & 0xff;
                    tb = cpixels[index2] & 0xff;
                    tmppixels[index] = (ta << 24) | (tr << 16) | (tg << 8) | tb;
                }
            }
            jintArray result = (*env)->NewIntArray(env,size);
                (*env)->SetIntArrayRegion(env,result, 0, size, tmppixels);
                (*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
                free(tmppixels);
                return result;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_LightFilterGrayJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height){
	jint *cpixels;
	cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
    if (cpixels == NULL) {
    	return 0; /* exception occurred */
	}
    int size=width * height;
    float threshold=0.6;
    int threshold3 = (int)(threshold*3*255);
    int y=0,x=0;
            for (y = 0; y < height; y++ ) {
                for (x = 0; x < width; x++ ) {
                    int rgb = cpixels[y*width+x];
                    int a = rgb & 0xff000000;
                    int r = (rgb >> 16) & 0xff;
                    int g = (rgb >> 8) & 0xff;
                    int b = rgb & 0xff;
                    int l = r + g + b;
                    if (l < threshold3)
                        l=0;
                    else {
                        l /= 3;
                    }
                    rgb = a | (l << 16) | (l << 8) | l;
                    cpixels[y*width+x] = rgb;
                }
            }
            jintArray result = (*env)->NewIntArray(env,size);
                            (*env)->SetIntArrayRegion(env,result, 0, size, cpixels);
                            (*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
                            return result;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_LightFilterBlendJni
  (JNIEnv *env, jclass class, jintArray srcpixels, jintArray graypixels, jint width, jint height){
	jint *csrcpixels;
	jint *cgraypixels;
	csrcpixels = (*env)->GetIntArrayElements(env,srcpixels, NULL);
	cgraypixels = (*env)->GetIntArrayElements(env,graypixels, NULL);
	if (csrcpixels == NULL||cgraypixels==NULL) {
	   return 0; /* exception occurred */
	}
    int size=width * height;
    int y=0,x=0;
    float strength=1.0;
    for (y = 0; y < height; y++ ) {
        for ( x = 0; x < width; x++ ) {
                    int rgb = cgraypixels[y*width+x];
                    int a = rgb & 0xff000000;
                    int r = (rgb >> 16) & 0xff;
                    int g = (rgb >> 8) & 0xff;
                    int b = rgb & 0xff;
                    int rgb2 = csrcpixels[y*width+x];
                    int r2 = (rgb2 >> 16) & 0xff;
                    int g2 = (rgb2 >> 8) & 0xff;
                    int b2 = rgb2 & 0xff;
                    if ( r > 0 ) {
                        r = Min(MAX_VALUE, Max(0,(int)(r*strength) + r2));
                        g = Min(MAX_VALUE, Max(0,(int)(g*strength) + g2));
                        b = Min(MAX_VALUE, Max(0,(int)(b*strength) + b2));
                    } else {
                        r = r2;
                        g = g2;
                        b = b2;
                    }
                    rgb = a | (r << 16) | (g << 8) | b;
                    csrcpixels[y*width+x] = rgb;
                }
            }
    jintArray result = (*env)->NewIntArray(env,size);
    (*env)->SetIntArrayRegion(env,result, 0, size, csrcpixels);
    (*env)->ReleaseIntArrayElements(env,srcpixels, csrcpixels, 0);
    (*env)->ReleaseIntArrayElements(env,graypixels, cgraypixels, 0);
    return result;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_SunyunJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height){
	int pixR=0;
	int pixG=0;
	int pixB=0;
	int pixColor=0;
    int newR=0;
	int newG=0;
	int newB=0;
	int centerX=width/2;
	int centerY=height/2;
	int radius =Min(centerX,centerY);
	float strength=200;//鍏夌収寮哄害100~150
	jint *cpixels;
	cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
    if (cpixels == NULL) {
    	return 0; /* exception occurred */
	}
	int pos=0;
	int i=0,k=0;
	for(i=1;i<height;i++)
	{
	   for(k=1;k<width;k++)
	{
	pos=i*width +k;
	pixColor=cpixels[pos];
	pixR=((pixColor&0x00FF0000) >> 16);
	pixG=((pixColor&0x0000FF00) >> 8);
	pixB=pixColor&0x000000FF;
	newR=pixR;
	newG=pixG;
	newB=pixB;
 //璁＄畻褰撳墠鐐瑰埌鍏夌収涓績鐨勮窛绂伙紝骞抽潰搴ф爣绯讳腑姹備袱鐐逛箣闂寸殑璺濈
	 long distance=(centerY-i)*(centerY-i)+(centerX-k)*(centerX-k);
	 if(distance<radius*radius)
	 {
	 //鎸夌収璺濈澶у皬璁＄畻澧炲姞鐨勫厜鐓у�
	 int result=(int) (strength*(1.0-sqrt(distance)/radius));
	 newR=pixR +result;
	 newG=pixG +result;
	 newB=pixB +result;
	 }
	 newR=Min(MAX_VALUE, Max(0,newR));
	 newG=Min(MAX_VALUE, Max(0,newG));
	 newB=Min(MAX_VALUE, Max(0,newB));
	 cpixels[pos]=MAX_VALUE<<24|newR<<16|newG<<8|newB;
	 }
	 }
	int size=width * height;
	jintArray result = (*env)->NewIntArray(env,size);
	(*env)->SetIntArrayRegion(env,result, 0, size, cpixels);
	(*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
	return result;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_SunzhaoJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height){
	int pixR=0;
	int pixG=0;
	int pixB=0;
	int pixColor=0;
    int newR=0;
	int newG=0;
	int newB=0;
	int centerX=0;
	int centerY=0;
	int radius =sqrt(width*width+height*height);
	float strength=200;//鍏夌収寮哄害100~150
	jint *cpixels;
	cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
    if (cpixels == NULL) {
    	return 0; /* exception occurred */
	}
	int pos=0;
	int i=0,k=0;
	for(i=1;i<height;i++)
	{
	   for(k=1;k<width;k++)
	{
	pos=i*width +k;
	pixColor=cpixels[pos];
	pixR=((pixColor&0x00FF0000) >> 16);
	pixG=((pixColor&0x0000FF00) >> 8);
	pixB=pixColor&0x000000FF;
	newR=pixR;
	newG=pixG;
	newB=pixB;
 //璁＄畻褰撳墠鐐瑰埌鍏夌収涓績鐨勮窛绂伙紝骞抽潰搴ф爣绯讳腑姹備袱鐐逛箣闂寸殑璺濈
	 long distance=(centerY-i)*(centerY-i)+(centerX-k)*(centerX-k);
	 //鎸夌収璺濈澶у皬璁＄畻澧炲姞鐨勫厜鐓у�
	 int result=(int) (strength*(1.0-sqrt(distance)/radius));
	 newR=pixR +result;
	 newG=pixG +result;
	 newB=pixB +result;
	 newR=Min(MAX_VALUE, Max(0,newR));
	 newG=Min(MAX_VALUE, Max(0,newG));
	 newB=Min(MAX_VALUE, Max(0,newB));
	 cpixels[pos]=MAX_VALUE<<24|newR<<16|newG<<8|newB;
	 }
	 }
	int size=width * height;
	jintArray result = (*env)->NewIntArray(env,size);
	(*env)->SetIntArrayRegion(env,result, 0, size, cpixels);
	(*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
	return result;
}
#define UNDEF  0xffff
void rgb2yuv(int rgb[],double yuv[]){
	double  Y = 0.299*rgb[0] + 0.587*rgb[1]+ 0.114*rgb[2];
	double U = (1 / 2.032)*(rgb[2]-Y);
	double V = (1 / 1.140)*(rgb[0]-Y);
    yuv[0]=Y;
	yuv[1]=U;
	yuv[2]=V;
}
void yuv2rgb(double yuv[],int rgb[]){
int	B = yuv[0] + 2.032*yuv[1];
int	R = yuv[0] + 1.140*yuv[2];
int	G = yuv[0] - (0.114*2.032/0.587)*yuv[1] - (0.299*1.140/0.587)*yuv[2];
rgb[0]=R;
rgb[1]=G;
rgb[2]=B;
}
void rgb2hsv(int rgb[],double hsv[]){
   int r = rgb[0], g = rgb[1], b = rgb[2];
       double      min, max, delta;
       min = r < g ? r : g;
       min = min  < b ? min  : b;
       max = r > g ? r : g;
       max = max  > b ? max  : b;
       hsv[2] = max;                                // v
       delta = max - min;
       if( max > 0.0 ) {
           hsv[1] = (delta / max);                  // s
       } else {
           // r = g = b = 0                        // s = 0, v is undefined
           hsv[1] = 0.0;
           hsv[0] = UNDEF;                            // its now undefined
           return;
       }
       if( r >= max )                           // > is bogus, just keeps compilor happy
           hsv[0] = ( g - b ) / delta;        // between yellow & magenta
       else
       if( g >= max )
           hsv[0] = 2.0 + ( b - r ) / delta;  // between cyan & yellow
       else
           hsv[0] = 4.0 + ( r - g ) / delta;  // between magenta & cyan

       hsv[0] *= 60.0;                              // degrees

       if( hsv[0] < 0.0 )
           hsv[0] += 360.0;
       return;
   }
void hsv2rgb(double hsv[],int rgb[]){
	    double      hh, p, q, t, ff;
	    long        i;
	    if(hsv[1] <= 0.0) {       // < is bogus, just shuts up warnings
	        if(hsv[0]==UNDEF) {   // in.h == NAN
	            rgb[0] = hsv[2];
	            rgb[1] = hsv[2];
	            rgb[2]= hsv[2];
	            return;
	        }
	        // error - should never happen
	        rgb[0] = 0.0;
	        rgb[1] = 0.0;
	        rgb[2] = 0.0;
	        return;
	    }
	    hh = hsv[0];
	    if(hh >= 360.0) hh = 0.0;
	    hh /= 60.0;
	    i = (long)hh;
	    ff = hh - i;
	    p = hsv[2] * (1.0 - hsv[1]);
	    q = hsv[2] * (1.0 - (hsv[1] * ff));
	    t = hsv[2] * (1.0 - (hsv[1] * (1.0 - ff)));
	    switch(i) {
	    case 0:
	        rgb[0] = hsv[2];
	        rgb[1] = t;
	        rgb[2] = p;
	        break;
	    case 1:
	        rgb[0] = q;
	        rgb[1] = hsv[2];
	        rgb[2] = p;
	        break;
	    case 2:
	        rgb[0] = p;
	        rgb[1] = hsv[2];
	        rgb[2] = t;
	        break;

	    case 3:
	        rgb[0] = p;
	        rgb[1] = q;
	        rgb[2] = hsv[2];
	        break;
	    case 4:
	        rgb[0] = t;
	        rgb[1]= p;
	        rgb[2] =hsv[2];
	        break;
	    case 5:
	    default:
	        rgb[0] = hsv[2];
	        rgb[1] = p;
	        rgb[2]= q;
	        break;
	    }
	    return;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_beautiFaceJni
  (JNIEnv *env, jclass thiz, jintArray pixels, jint x, jint y, jint w, jint h, jint flag){
	jint *cpixels;
			cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
		    if (cpixels == NULL) {
		    	return 0; /* exception occurred */
			}
		    int rgb[3]={0};
		    double hsv[3]={0};
		    int i=0,j=0;
		    //int alpha=0;
		    int r;
		    for(r=2;r<=20;r=r+2)
		              {float t1=rand()%50/50.0,t2=rand()%20/1000.0;
		                  float t;
		                  for(t=t1;t<1+t1;t=t+0.02+t2)
		    			  {     int m=x+r*cos(t*6.28);
		    					int n=y+r*sin(t*6.28);
		    					 int c=cpixels[n*w+m];
		    					 rgb[2]=c&0x000000FF;
		    					 rgb[1]=(c>>8)&0x000000FF;
		    					 rgb[0]=(c>>16)&0x000000FF;
		    					 rgb2hsv(rgb,hsv);
		    					 hsv[0]=0;
		    					 hsv2rgb(hsv,rgb);
		    					 if(rgb[0]>255)rgb[0]=255;
		    	                	if(rgb[1]>255)rgb[1]=255;
		    	            	 if(rgb[2]>255)rgb[2]=255;
		    	            	 cpixels[n*w+m]=(rgb[0] << 16) | (rgb[1] << 8) | rgb[2];
		    			  }
		              }
		            int size=w * h;
		    		jintArray result = (*env)->NewIntArray(env,size);
		    		(*env)->SetIntArrayRegion(env,result, 0, size, cpixels);
		    		(*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
		    		return result;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_ColorChangeJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height, jint flag){
	jint *cpixels;
		cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
	    if (cpixels == NULL) {
	    	return 0; /* exception occurred */
		}
	    int rgb[3]={0};
	    double hsv[3]={0};
	    int i=0,j=0;
	    int alpha=0;
	    for (j = 0; j < height; j++ ) {
	            for ( i = 0; i < width; i++ ) {
	                        int color = cpixels[j*width+i];
	                        alpha =(color>>24)&0xff;
	                        rgb[0] = (color >> 16) & 0xff;
	                        rgb[1] = (color >> 8) & 0xff;
	                        rgb[2] = color & 0xff;
	                        rgb2hsv(rgb,hsv);
	                 if(!(hsv[1]<0.24&&hsv[2]>0.7||hsv[2]<0.2))
	                 {
	                 if(flag>0&&flag<5){
                     if(flag==1){
                         if(hsv[0]>-1&&hsv[0]<21||hsv[0]>322&&hsv[0]<361)
                        	 hsv[0]=60.0;
                         else if(hsv[0]>21&&hsv[0]<78)
                        	 hsv[0]=0;
                     }
                     if(flag==2){
                    	 if(hsv[0]>-1&&hsv[0]<21||hsv[0]>322&&hsv[0]<361)
                    	     hsv[0]=240.0;
                    	 else if(hsv[0]>210&&hsv[0]<265)
                    	     hsv[0]=0;
                     }
                     if(flag==3){
                         if(hsv[0]>-1&&hsv[0]<21||hsv[0]>322&&hsv[0]<361)
                             hsv[0]=300.0;
                         else if(hsv[0]>269&&hsv[0]<320)
                             hsv[0]=0;
                     }
                     if(flag==4){
                        if(hsv[0]>21&&hsv[0]<78)
                             hsv[0]=300.0;
                        else if(hsv[0]>269&&hsv[0]<320)
                             hsv[0]=60;
                     }
                     hsv2rgb(hsv,rgb);
	                 }
	                 }
	                 cpixels[j*width+i] = (alpha<<24) | (rgb[0] << 16) | (rgb[1] << 8) | rgb[2];
	            }
	    }
		int size=width * height;
		jintArray result = (*env)->NewIntArray(env,size);
		(*env)->SetIntArrayRegion(env,result, 0, size, cpixels);
		(*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
		return result;
}
int isRGBSkin(int tr, int tg, int tb) {
    int max = Max(tr, Max(tg, tb));
    int min = Min(tr, Min(tg, tb));
    int rg = abs(tr - tg);
    if(tr > 95 && tg > 40 && tb > 20 && rg > 15 &&
            (max - min) > 15 && tr > tg && tr > tb) {
        return 1;
    } else {
        return 0;
    }
}
int isHSVSkin(int tr, int tg, int tb) {
	double hsv[3]={0};
	int rgb[3]={0};
	rgb[0]=tr;
	rgb[1]=tg;
	rgb[2]=tb;
    rgb2hsv(rgb,hsv);
    if((hsv[0] > 0.0f && hsv[0] < 50.0f ) && (hsv[1] > 0.23f && hsv[1] < 0.68f)){
        return 1;
    } else {
        return 0;
    }
}
void rgbToYcrCb(int tr, int tg, int tb,double YcrCb[]) {
    double sum = tr + tg + tb;
    double r = ((double)tr)/sum;
    double g = ((double)tg)/sum;
    double b = ((double)tb)/sum;
     YcrCb[0] = 65.481*r + 128.553*g + 24.966*b + 16.0;
     YcrCb[1] = -37.7745*r - 74.1592*g + 111.9337*b + 128.0;
    YcrCb[2] = 111.9581*r -93.7509*g -18.2072*b + 128.0;
}
int isSkin(int tr, int tg, int tb,int flag){
if(flag==1)
	return isRGBSkin(tr,tg,tb);
else
if(flag==2)
	return isHSVSkin(tr,tg,tb);
else
	return 0;
}
JNIEXPORT jintArray JNICALL Java_com_huangbin_test_diyimage_ImageUtilsJni_SkinFilterJni
  (JNIEnv *env, jclass class, jintArray pixels, jint width, jint height, jint flag){
	jint *cpixels;
		cpixels = (*env)->GetIntArrayElements(env,pixels, NULL);
	    if (cpixels == NULL) {
	    	return 0; /* exception occurred */
		}
	    int index = 0;
	    int sencha=30;
	    int row=0,col=0;
	            for(row=0; row<height; row++) {
	                int ta = 0, tr = 0, tg = 0, tb = 0;
	                for(col=0; col<width; col++) {
	                    index = row * width + col;
	                    ta = (cpixels[index] >> 24) & 0xff;
	                    tr = (cpixels[index] >> 16) & 0xff;
	                    tg = (cpixels[index] >> 8) & 0xff;
	                    tb = cpixels[index] & 0xff;
	                    if(isSkin(tr, tg, tb,1)) {
                          //  tr+=sencha;
                        //    tg+=sencha;
                        //    tb+=sencha;
	                    //	tr=Min(MAX_VALUE, Max(0,tr));
	                    //    tg=Min(MAX_VALUE, Max(0,tg));
	                   // 	tb=Min(MAX_VALUE, Max(0,tb));
	                    //    cpixels[index] = (ta << 24) | (tr << 16) | (tg << 8) | tb;
	                    	if(flag==1)
	                     cpixels[index] = middle_smooth(cpixels,width,col,row);
	                    	else if(flag==2)
	                    	{
	                    		tr+=30;
	                    		tg+=30;
	                    		tb+=30;
	                    		tr=Min(MAX_VALUE, Max(0,tr));
	                    		tg=Min(MAX_VALUE, Max(0,tg));
	                    		tb=Min(MAX_VALUE, Max(0,tb));
	                    		cpixels[index] = (ta << 24) | (tr << 16) | (tg << 8) | tb;
	                    	}
	                    	else if(flag==3)
	                    	 cpixels[index] = ave_smooth(cpixels,width,col,row);
	                    } else {
	                       // tr = tg = tb = 0;
	                    	cpixels[index] = (ta << 24) | (tr << 16) | (tg << 8) | tb;
	                    }

	                }
	            }
	            int size=width * height;
	            		jintArray result = (*env)->NewIntArray(env,size);
	            		(*env)->SetIntArrayRegion(env,result, 0, size, cpixels);
	            		(*env)->ReleaseIntArrayElements(env,pixels, cpixels, 0);
	            		return result;
}

