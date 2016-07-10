package com.huangbin.test.diyimage;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.ColorMatrix;
import android.graphics.ColorMatrixColorFilter;
import android.graphics.LinearGradient;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.graphics.PorterDuffXfermode;
import android.graphics.Shader.TileMode;
import android.util.Log;

public class ImageUtils {
	
	public static Bitmap resizeImage(Bitmap bitmap,float scale){
		Matrix mMatrix=new Matrix();
		mMatrix.postScale(scale,scale);
		return Bitmap.createBitmap(bitmap,0,0,bitmap.getWidth(),bitmap.getHeight(),mMatrix,true);
	}
	public static Bitmap SeamCarveImage(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height=bitmap.getHeight();
		final ImageSeamOperations s = new ImageSeamOperations(bitmap);
	    int nw = 220;//(int)(width*0.8f);
	    int nh = 160;//(int)(height*0.75);
	    final int fnw = s.getImg().getWidth() - nw;
	    final int fnh = s.getImg().getHeight() - nh;
	//    s.findOptimalHorizontalSeam();
	   if (fnw >=0 && fnw <s.getImg().getWidth() && fnh >=0 && fnh <s.getImg().getHeight()) {
	                int h = fnh, w=fnw;
	                for (int i = 0; i< Math.max(fnh, fnw); i++) {
	                    if (h>0) {
	                        s.carveHorizontalSeam();
	                        h--;
	                    }
	                    if (w>0) {
	                        s.carveVerticalSeam();
	                        w--;
	                    }
	                }
	    }
	    return s.getImg();//ImageSwitcher.keepBitmapAutoSize(s.getImg(), width, height);
	}
	public static Bitmap ReflectionImage(Bitmap bitmap) {       
        Bitmap bmp = Bitmap.createBitmap(bitmap.getWidth(), bitmap.getHeight()+bitmap.getHeight()/2,Bitmap.Config.ARGB_8888); 
        Canvas canvas = new Canvas(bmp); 
        canvas.drawBitmap(bitmap, 0, 0, null) ;  
        Matrix mMatrix = new Matrix(); 
        mMatrix.preScale(1, -1);   
        Bitmap tmpbitmap  = Bitmap.createBitmap(bitmap, 0, 0, bitmap.getWidth(), bitmap.getHeight(), mMatrix, true) ; 
        //
        canvas.drawBitmap(tmpbitmap, 0, bitmap.getHeight(), null) ; 
        Paint mPaint = new Paint() ; 
        //
        LinearGradient shader = new LinearGradient(0,bitmap.getHeight(), 0, bmp.getHeight(), 0xa0ffffff, 0x30ffffff, TileMode.CLAMP);   
         mPaint.setShader(shader);   
         mPaint.setXfermode(new PorterDuffXfermode(PorterDuff.Mode.DST_IN));   
         canvas.drawRect(0, bitmap.getHeight(), bitmap.getWidth(), bmp.getHeight(), mPaint);   
        return bmp; 
    }
	public static Bitmap rotateBitmap(Bitmap bitmap,float degree){
		Matrix mMatrix=new Matrix();
		mMatrix.postRotate(degree);
		return Bitmap.createBitmap(bitmap,0,0,bitmap.getWidth(),bitmap.getHeight(),mMatrix,true);
	}
	public static Bitmap toFilm(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.toFilmJni(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap beautiFace(Bitmap bitmap,int x,int y,int flag){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.beautiFaceJni(pixels,x,y,width,height,flag);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap Img(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.ImgFun(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap cvSmooth(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height=bitmap.getHeight();
		setSourceImage(bitmap);
		ImageUtilsJni.cvSmooth();
		Bitmap bmp=getSourceImage(width, height);
		return bmp;
	}
	public static Bitmap SkinFilter(Bitmap bitmap,int flag){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.SkinFilterJni(pixels, width, height, flag);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap lineChangeImage(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.lineChangeJni(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap sunYunImage(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.SunyunJni(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap sunZhaoImage(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.SunzhaoJni(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap ColorChangeImage(Bitmap bitmap,int flag){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.ColorChangeJni(pixels, width, height, flag);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap sharpenImage(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.sharpenImageJni(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap bianYuanSharpenImage(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.bianYuanSharpJni(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap exposureImage(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.exposureJni(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap guass_SmoothImage(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.gauss_SmoothJni(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap middle_SmoothImage(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.middle_SmoothJni(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap move_SmoothImage(Bitmap bitmap,float distance,float zoom,int angel){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.move_SmoothJni(pixels, width, height, distance, zoom, angel);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap ave_SmoothImage(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.ave_SmoothJni(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap histogramFilter(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.histogramFilterJni(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap StrokeAreasImage(Bitmap bitmap,int size){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.StrokeAreasJni(pixels, width, height, size);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap polishImage(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.polishJni(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap daoZhiImage(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(height,width,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.polishJni(pixels, width, height);
		bmp.setPixels(pixels,0,height,0,0,height,width);
		return bmp;
	}
	public static Bitmap GrayImage(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.toGray(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap LightFilterGray(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.LightFilterGrayJni(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap LightFilterBlend(Bitmap bitmap,Bitmap graybmp){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		int[] graypixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		graybmp.getPixels(graypixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.LightFilterBlendJni(pixels, graypixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap LightFilter(Bitmap bitmap){
		Bitmap bmp=LightFilterGray(bitmap);
		bmp=move_SmoothImage(bmp, 20.0f, 0.4f, 120);
		bmp=LightFilterBlend(bitmap,bmp);
		return bmp;
	}
	public static Bitmap magicMiroor(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.magicMirrorJni(pixels,width,height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap swirlImage(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.swirlJni(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap GrayFaZhiImage(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.grayFaZhiJni(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap windowImage(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.windowJni(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap sketchImage(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.sketchImageJni(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap angulationImage(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.angulationJni(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap fuDiaoImage(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.fuDiaoJni(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap angulationBallImage(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.angulationBallJni(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap bianYuan(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.bianYuanJni(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap miHongImage(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.miHongJni(pixels, width, height,0);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap tiDuSharpImage(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.miHongJni(pixels, width, height,1);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap mosaicImage(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.mosaicJni(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap addSaltAndPepperNoise(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.addSaltAndPepperNoiseJni(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap addGaussNoise(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.addGauseNoiseJni(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap addPoissonNoise(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.addPoissonNoiseJni(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap ChannelMixFilter(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.ChannelMixFilterJni(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap MaskFilter(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.MaskFilterJni(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap SolarizeFilter(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.SolarizeFilterJni(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap PosterizeFilter(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.PosterizeFilterJni(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap BoxBlurFilter(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.BoxBlurFilterJni(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap BumpFilter(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.BumpFilterJni(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static boolean setSourceImage(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		int[] pixels=new int[width*height];
		boolean state=false;
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		state=ImageUtilsJni.setSourceImageJni(pixels, width, height);
		return state;
	}
	public static boolean setMaskImage(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		int[] pixels=new int[width*height];
		boolean state=false;
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		state=ImageUtilsJni.setMaskImageJni(pixels, width, height);
		return state;
	}
	public static Bitmap getSourceImage(int width,int height){
		int[] imageData=ImageUtilsJni.getSourceImageJni();
		Bitmap bmp=null;
		if(imageData!=null)
		bmp=Bitmap.createBitmap(imageData, 0, width,width, height, Bitmap.Config.ARGB_8888);
		else
		bmp=null;
		return bmp;
	}
	public static Bitmap WaterFilter(Bitmap bitmap){
		int width=bitmap.getWidth();
		int height =bitmap.getHeight();
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] pixels=new int[width*height];
		bitmap.getPixels(pixels,0,width,0,0,width,height);
		pixels=ImageUtilsJni.WaterFilterJni(pixels, width, height);
		bmp.setPixels(pixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap addFrame(Bitmap dst,Bitmap frame,int flag){
		int width=dst.getWidth();
		int height =dst.getHeight();
		frame=ImageSwitcher.resizeBitmap(frame, width, height);
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] dstpixels=new int[width*height];
		int[] framepixels=new int[width*height];
		dst.getPixels(dstpixels,0,width,0,0,width,height);
		frame.getPixels(framepixels,0,width,0,0,width,height);
		dstpixels=ImageUtilsJni.addFrameJni(dstpixels, framepixels, width, height,flag);
		if(dstpixels==null)
			Log.i("frame", "dstpixels null");
		bmp.setPixels(dstpixels,0,width,0,0,width,height);
		return bmp;
	}
	public static Bitmap blendImage(Bitmap dst,Bitmap frame,int flag){
		int width=dst.getWidth();
		int height =dst.getHeight();
		frame=ImageSwitcher.resizeBitmap(frame, width, height);
		Bitmap bmp=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
		int[] dstpixels=new int[width*height];
		int[] framepixels=new int[width*height];
		dst.getPixels(dstpixels,0,width,0,0,width,height);
		frame.getPixels(framepixels,0,width,0,0,width,height);
		dstpixels=ImageUtilsJni.blendingJni(dstpixels, framepixels, width, height,flag);
		if(dstpixels==null)
			Log.i("frame", "dstpixels null");
		bmp.setPixels(dstpixels,0,width,0,0,width,height);
		return bmp;
	}

	public static Bitmap lomoImage(Bitmap bitmap){
		Bitmap tmpbmp=Bitmap.createBitmap(bitmap.getWidth(), bitmap.getHeight(), Bitmap.Config.ARGB_8888);
		Canvas canvas=new Canvas(tmpbmp);//寰楀埌鐢荤瑪瀵硅薄
		Paint  paint =new Paint(); //鏂板缓paint
		ColorMatrix cm=new ColorMatrix();
		float[] lomoMatrix=new float[]{1.7f,0.1f,0.1f,0,-73.1f,0,1.7f,0.1f,0,-73.1f,0,0.1f,1.6f,0,-73.1f,0,0,0,1,0};
		cm.set(lomoMatrix);
		paint.setColorFilter(new ColorMatrixColorFilter(cm));
		canvas.drawBitmap(bitmap, 0, 0, paint);
		return tmpbmp;
	}
	public static Bitmap oldImage(Bitmap bitmap){
		Bitmap tmpbmp=Bitmap.createBitmap(bitmap.getWidth(), bitmap.getHeight(), Bitmap.Config.ARGB_8888);
		Canvas canvas=new Canvas(tmpbmp);//寰楀埌鐢荤瑪瀵硅薄
		Paint  paint =new Paint(); //鏂板缓paint
		ColorMatrix cm=new ColorMatrix();
		float[] lomoMatrix=new float[]{0.393f,0.769f,0.189f,0,0,0.349f,0.686f,0.168f,0,0,0.272f,0.534f,0.131f,0,0,0,0,0,1.0f,0};
		cm.set(lomoMatrix);
		paint.setColorFilter(new ColorMatrixColorFilter(cm));
		canvas.drawBitmap(bitmap, 0, 0, paint);
		return tmpbmp;
	}
	public static Bitmap reverseImage(Bitmap bitmap,int flag){
		float[] mMatrixValue=null;
		switch(flag){
		case 0://姘村钩鍙嶈浆
		mMatrixValue=new float[]{-1f,0f,0f,0f,1f,0f,0f,0f,1f};
		break;
		case 1://鍨傜洿鍙嶈浆
		mMatrixValue=new float[]{1f,0f,0f,0f,-1f,0f,0f,0f,1f};
		break;
		}
		
		 if(mMatrixValue !=null){
		 Matrix mMatrix=new Matrix();
		 mMatrix.setValues(mMatrixValue);
		 return Bitmap.createBitmap(bitmap,0,0,bitmap.getWidth(),bitmap.getHeight(),mMatrix,true);
		 }
		 return bitmap;
	}
}
