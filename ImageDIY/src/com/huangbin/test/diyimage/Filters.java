package com.huangbin.test.diyimage;

import android.graphics.Bitmap;

public class Filters {
	float xscale;
    float yscale;
    float xshift;
    float yshift;
    int [] s;
 //   private String TAG = "Filters";
    public Filters(){

        //Log.e(TAG, "***********inside constructor");
    }
   
    public Bitmap Skinfilter(Bitmap bitmap) {  
        int width = bitmap.getWidth();  
        int height =bitmap.getHeight();  
        int[] inPixels = new int[width*height];  
        int[] outPixels = new int[width*height];  
        bitmap.getPixels(inPixels,0,width,0,0,width,height);
        Bitmap dst = Bitmap.createBitmap(width, height,bitmap.getConfig());
        
        int index = 0;  
        for(int row=0; row<height; row++) {  
            int ta = 0, tr = 0, tg = 0, tb = 0;  
            for(int col=0; col<width; col++) {  
                index = row * width + col;  
                ta = (inPixels[index] >> 24) & 0xff;  
                tr = (inPixels[index] >> 16) & 0xff;  
                tg = (inPixels[index] >> 8) & 0xff;  
                tb = inPixels[index] & 0xff;  
                  
                // detect skin method...   
                double sum = tr + tg + tb;  
                if (((double)tb/(double)tg<1.249) &&  
                    ((double)sum/(double)(3*tr)>0.696) &&  
                    (0.3333-(double)tb/(double)sum>0.014) &&  
                    ((double)tg/(double)(3*sum)<0.108))  
                {  
                   // tr = tg = tb = 0;  
                } else {  
                    tr = tg = tb = 0;  
                }  
                outPixels[index] = (ta << 24) | (tr << 16) | (tg << 8) | tb;  
            }  
        }  
        dst.setPixels(outPixels,0,width,0,0,width,height);
        return dst;  
    }  
    private boolean isBlack(int color){
    	int secha=20;
    	int pixR,pixB,pixG;
    	pixR=((color&0x00FF0000) >> 16);
    	pixG=((color&0x0000FF00) >> 8);
    	pixB=color&0x000000FF;
    	if(pixR<secha&&pixG<secha&&pixB<secha)
    		return true;
    	else
    		return false;
    }
    /*private boolean isWhite(int color){
    	int secha=20;
    	int pixR,pixB,pixG;
    	pixR=((color&0x00FF0000) >> 16);
    	pixG=((color&0x0000FF00) >> 8);
    	pixB=color&0x000000FF;
    	if(pixR>(255-secha)&&pixG>(255-secha)&&pixB>(255-secha))
    	   return true;
    	else
    	   return false;
    }*/
    
    public Bitmap ReflectionFilter(Bitmap bitmap,boolean IsHorizontal){
    	int height = bitmap.getHeight();
        int width = bitmap.getWidth();
        float Offset=0.5f;
        int start;
        int limit;
        Bitmap dst = Bitmap.createBitmap(width, height,bitmap.getConfig());
        if (IsHorizontal)
        {
            int y_offset = (int) (Offset * height);
            if (Offset > 0.5f) {
                start = y_offset - (height - y_offset);
                limit = y_offset;
            }
            else{
                start = y_offset;
                limit = y_offset + y_offset;
            }
            if (start < 0) {
                start = 0;
            }
            for (int y = start; (y < limit) && (y < height); y++)
            {
                int y_pos = (-y + (2 * y_offset)) - 1;
                y_pos = (y_pos < 0) ? 0 : (y_pos >= height ? height - 1 : y_pos) ;    
                for (int x = 0; x < width; x++)
                {
                    s=getARGB(bitmap, x, y);
                    int color = ((s[1]&0x0ff)<<16)|((s[2]&0x0ff)<<8)|(s[3]&0x0ff); 
                    dst.setPixel(x, y, color); 
                    dst.setPixel(x, y_pos, color);                  
                }
            }
        }
        else {
	        int x_offset = (int) (Offset * width);
	        if (Offset > 0.5f){
	            start = x_offset - (width - x_offset);
	            limit = x_offset;
	        }
	        else {
	        	start = x_offset;
	            limit = x_offset + x_offset;
	        }
	        if (start < 0){
	        	start = 0;
	        }
	        for (int x = start; (x < limit) && (x < width); x++)
	        {
	            int x_pos = (-x + (2 * x_offset)) - 1;
	            x_pos = x_pos < 0 ? 0 : (x_pos >= width ? width - 1:  x_pos);
	            for (int y = 0; y < height; y++)
	            {
	            	 s=getARGB(bitmap, x, y);
	                 int color = ((s[1]&0x0ff)<<16)|((s[2]&0x0ff)<<8)|(s[3]&0x0ff);                   
	                 dst.setPixel(x_pos, y, color); 
	                 dst.setPixel(x, y, color); 
	            }
	        }
        }
        return dst;
        
    }
    public Bitmap fisheyescale(Bitmap bitmap,int left,int top,int right,int bottom,float w,boolean IsHorizontal,boolean scale){
    	int width = bitmap.getWidth(); //image bounds
        int height =bitmap.getHeight();
        float radius;
        if(IsHorizontal)
        	radius=(right-left)>(bottom-top)?((right-left)/2):((bottom-top)/2); 
        else
            radius=(right-left)>(bottom-top)?((bottom-top)/2):((right-left)/2); 
        
        double ss=radius/Math.log(w*radius+1);
    	Bitmap dst = Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
    	int[] pixels=new int[width*height];
        bitmap.getPixels(pixels,0,width,0,0,width,height);
        dst.setPixels(pixels,0,width,0,0,width,height);
    	 float centerX=(left+right)/2; //center of distortion
         float centerY=(top+bottom)/2;
         for(int j=0;j<height;j++){
             for(int i=0;i<width;i++){
             	double distance = ((centerX - i) * (centerX - i) + (centerY - j) * (centerY - j));
     			if (distance<radius*radius)
     			{
     				// 图像放大效果
     				float ix=i-centerX;
     				float jy=centerY-j;
     				double r=(float)Math.sqrt(ix*ix+jy*jy);
     				double degree=Math.atan2(jy, ix);
     				if(scale)
     				r=(Math.exp(r/ss)-1)/w;
     				else
     				r=ss*Math.log(1+w*r);
     				float src_x=(float)(r*Math.cos(degree))+centerX;
     				float src_y=centerY-(float)(r*Math.sin(degree));
     				sampleImage(bitmap, src_x, src_y);
     				int color = ((s[0]&0x0ff)<<24)|((s[1]&0x0ff)<<16)|((s[2]&0x0ff)<<8)|(s[3]&0x0ff);
     				dst.setPixel(i, j, color);
     			}
             }
     	} 
         return dst;
    }
    public Bitmap zoomSwirl(Bitmap bitmap,int left,int top,int right,int bottom,float scale){
        int width = bitmap.getWidth(); //image bounds
        int height =bitmap.getHeight();
        float radius=(right-left)>(bottom-top)?((bottom-top)/2):((right-left)/2);    	
    	Bitmap dst = Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
    	int[] pixels=new int[width*height];
        bitmap.getPixels(pixels,0,width,0,0,width,height);
        dst.setPixels(pixels,0,width,0,0,width,height);
    	 float centerX=(left+right)/2; //center of distortion
         float centerY=(top+bottom)/2;
    	for(int j=0;j<height;j++){
            for(int i=0;i<width;i++){
            	double distance = ((centerX - i) * (centerX - i) + (centerY - j) * (centerY - j));
    			if (distance<radius*radius)
    			{
    				// 图像放大效果
    				float src_x = (float)(i-centerX)/scale+centerX;
    				float src_y = (float)(j-centerY)/scale+centerY;
    				sampleImage(bitmap, src_x, src_y);
    				int color = ((s[0]&0x0ff)<<24)|((s[1]&0x0ff)<<16)|((s[2]&0x0ff)<<8)|(s[3]&0x0ff);
    				dst.setPixel(i, j, color);
    			}
            }
    	} 
        return dst;
    }
    public Bitmap lashen(Bitmap bitmap,int centerx,int centery,int mx,int my,float r,float strength){
    	int width = bitmap.getWidth(); //image bounds
        int height =bitmap.getHeight();
        double rMc=(mx-centerx)*(mx-centerx)+(my-centery)*(my-centery);
    	Bitmap dst = Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
    	int[] pixels=new int[width*height];
        bitmap.getPixels(pixels,0,width,0,0,width,height);
        dst.setPixels(pixels,0,width,0,0,width,height);
    	for(int j=0;j<dst.getHeight();j++){
            for(int i=0;i<dst.getWidth();i++){
            	double distance = ((centerx - i) * (centerx - i) + (centery - j) * (centery - j));
    			if (distance < r*r)
    			{
    				// 图像放大效果
    				double rXc=(i-centerx)*(i-centerx)+(j-centery)*(j-centery);
    				double k=((r*r-rXc)/(r*r-rXc+strength*rMc))*((r*r-rXc)/(r*r-rXc+strength*rMc));
    				float src_x =(float)(i-k*(mx-centerx));
    				float src_y = (float)(j-k*(my-centery));
    				sampleImage(bitmap, src_x, src_y);
    				int color = ((s[0]&0x0ff)<<24)|((s[1]&0x0ff)<<16)|((s[2]&0x0ff)<<8)|(s[3]&0x0ff);
    				dst.setPixel(i, j, color);
    			}  			
            }
    	}   
        return dst;	
    }
    public Bitmap barrel (Bitmap input, float k){
       // Log.e(TAG, "***********inside barrel method ");
        float centerX=input.getWidth()/2; //center of distortion
        float centerY=input.getHeight()/2;

        int width = input.getWidth(); //image bounds
        int height = input.getHeight();

        Bitmap dst = Bitmap.createBitmap(width, height,Bitmap.Config.ARGB_8888); //output pic
       // Log.e(TAG, "***********dst bitmap created ");
          xshift = calc_shift(0,centerX-1,centerX,k);
          float newcenterX = width-centerX;
          float xshift_2 = calc_shift(0,newcenterX-1,newcenterX,k);

          yshift = calc_shift(0,centerY-1,centerY,k);
          float newcenterY = height-centerY;
          float yshift_2 = calc_shift(0,newcenterY-1,newcenterY,k);

          xscale = (width-xshift-xshift_2)/width;
          yscale = (height-yshift-yshift_2)/height;
      //    Log.e(TAG, "***********about to loop through bm");
          for(int j=0;j<dst.getHeight();j++){
              for(int i=0;i<dst.getWidth();i++){
                float x = getRadialX((float)i,(float)j,centerX,centerY,k);
                float y = getRadialY((float)i,(float)j,centerX,centerY,k);
                sampleImage(input,x,y);
                int color = ((s[0]&0x0ff)<<24)|((s[1]&0x0ff)<<16)|((s[2]&0x0ff)<<8)|(s[3]&0x0ff);
                dst.setPixel(i, j, color);
              }
            }
        return dst;
    }

    void sampleImage(Bitmap arr, float idx0, float idx1)
    {
        s = new int [4];
      if(idx0<0 || idx1<0 || idx0>(arr.getWidth()-1) || idx1>(arr.getHeight()-1)){
        s[0]=0;
        s[1]=0;
        s[2]=0;
        s[3]=0;
        return;
      }

      float idx0_fl=(float) Math.floor(idx0);
      float idx0_cl=(float) Math.ceil(idx0);
      float idx1_fl=(float) Math.floor(idx1);
      float idx1_cl=(float) Math.ceil(idx1);

      int [] s1 = getARGB(arr,(int)idx0_fl,(int)idx1_fl);
      int [] s2 = getARGB(arr,(int)idx0_fl,(int)idx1_cl);
      int [] s3 = getARGB(arr,(int)idx0_cl,(int)idx1_cl);
      int [] s4 = getARGB(arr,(int)idx0_cl,(int)idx1_fl);

      float x = idx0 - idx0_fl;
      float y = idx1 - idx1_fl;

      s[0]= (int) (s1[0]*(1-x)*(1-y) + s2[0]*(1-x)*y + s3[0]*x*y + s4[0]*x*(1-y));
      s[1]= (int) (s1[1]*(1-x)*(1-y) + s2[1]*(1-x)*y + s3[1]*x*y + s4[1]*x*(1-y));
      s[2]= (int) (s1[2]*(1-x)*(1-y) + s2[2]*(1-x)*y + s3[2]*x*y + s4[2]*x*(1-y));
      s[3]= (int) (s1[3]*(1-x)*(1-y) + s2[3]*(1-x)*y + s3[3]*x*y + s4[3]*x*(1-y));
    }

    int [] getARGB(Bitmap buf,int x, int y){
        int rgb = buf.getPixel(x, y); // Returns by default ARGB.
        int [] scalar = new int[4];
        scalar[0] = (rgb >> 24) & 0xFF;
        scalar[1] = (rgb >> 16) & 0xFF;
        scalar[2] = (rgb >> 8) & 0xFF;
        scalar[3] = (rgb >> 0) & 0xFF;
        return scalar;
    }

    float getRadialX(float x,float y,float cx,float cy,float k){
      x = (x*xscale+xshift);
      y = (y*yscale+yshift);
      float res = x+((x-cx)*k*((x-cx)*(x-cx)+(y-cy)*(y-cy)));
      return res;
    }

    float getRadialY(float x,float y,float cx,float cy,float k){
      x = (x*xscale+xshift);
      y = (y*yscale+yshift);
      float res = y+((y-cy)*k*((x-cx)*(x-cx)+(y-cy)*(y-cy)));
      return res;
    }

    float thresh = 1;

    float calc_shift(float x1,float x2,float cx,float k){
      float x3 = (float)(x1+(x2-x1)*0.5);
      float res1 = x1+((x1-cx)*k*((x1-cx)*(x1-cx)));
      float res3 = x3+((x3-cx)*k*((x3-cx)*(x3-cx)));

      if(res1>-thresh && res1 < thresh)
        return x1;
      if(res3<0){
        return calc_shift(x3,x2,cx,k);
      }
      else{
        return calc_shift(x1,x3,cx,k);
      }
    }
}
