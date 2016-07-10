package com.huangbin.test.diyimage;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.sql.Date;
import java.text.SimpleDateFormat;
import java.util.Locale;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.os.Environment;
import android.util.Log;

public class ImageSwitcher {

public static Bitmap readBitmapAutoSize(String filePath, int outWidth, int outHeight) {     
       
FileInputStream fs = null;   
BufferedInputStream bs = null;   
try {   
    fs = new FileInputStream(filePath);   
    bs = new BufferedInputStream(fs);   
    BitmapFactory.Options options = setBitmapOption(filePath, outWidth, outHeight);   
    return BitmapFactory.decodeStream(bs, null, options);   
} catch (Exception e) {   
    e.printStackTrace();   
} finally {   
    try {   
        bs.close();   
        fs.close();   
    } catch (Exception e) {   
        e.printStackTrace();   
    }   
}   
return null;   
}   

private static BitmapFactory.Options setBitmapOption(String file, int width, int height) {   
BitmapFactory.Options opt = new BitmapFactory.Options();   
opt.inJustDecodeBounds = true;             
BitmapFactory.decodeFile(file, opt);   
int outWidth = opt.outWidth; 
int outHeight = opt.outHeight;   
opt.inDither = false;   
opt.inPreferredConfig = Bitmap.Config.RGB_565;       
int sampleSize=1;
if((outWidth>width)||(outHeight>height)){
    sampleSize=(outWidth/width)>(outHeight/height)?(outWidth/width):(outHeight/height);
}
opt.inSampleSize = sampleSize;    
opt.inJustDecodeBounds = false;
return opt;   
}  
public static Bitmap readBitmapRealSize(String filePath){
	FileInputStream fs = null;   
	BufferedInputStream bs = null;   
	try {   
	    fs = new FileInputStream(filePath);   
	    bs = new BufferedInputStream(fs);   
	    BitmapFactory.Options opt = new BitmapFactory.Options();  
	    opt.inDither = false;   
	    opt.inPreferredConfig = Bitmap.Config.ARGB_8888;   
	    opt.inSampleSize = 1;    
	    return BitmapFactory.decodeStream(bs, null, opt);   
	} catch (Exception e) {   
	    e.printStackTrace();   
	} finally {   
	    try {   
	        bs.close();   
	        fs.close();   
	    } catch (Exception e) {   
	        e.printStackTrace();   
	    }   
	}   
	return null;  
}
public static Bitmap keepBitmapAutoSize(Bitmap bitmap,int width,int height){
	Matrix mMatrix=new Matrix();
	float scale=1.0f;
	float widthscale=width*1.0f/bitmap.getWidth();
	float heightscale=height*1.0f/bitmap.getHeight();
	if((widthscale<1.0f)||(heightscale<1.0f)){
		scale=widthscale<heightscale?widthscale:heightscale;
	}
	mMatrix.postScale(scale,scale);
	return Bitmap.createBitmap(bitmap,0,0,bitmap.getWidth(),bitmap.getHeight(),mMatrix,true);
}
public static Bitmap resizeBitmap(Bitmap bitmap,int width,int height){
	Matrix mMatrix=new Matrix();
	float widthscale=width*1.0f/bitmap.getWidth();
	float heightscale=height*1.0f/bitmap.getHeight();
	mMatrix.postScale(widthscale,heightscale);
	return Bitmap.createBitmap(bitmap,0,0,bitmap.getWidth(),bitmap.getHeight(),mMatrix,true);
}
public static String saveImage(Bitmap bitmap){
	SimpleDateFormat   formatter=new SimpleDateFormat("yyyyMMddHHmmss",Locale.CHINA);
	Date  curDate=new  Date(System.currentTimeMillis());//
	String  datestr=formatter.format(curDate);
	Log.i("save",datestr);
	String dirpath=Environment.getExternalStorageDirectory().getPath()+"/DCIM/images";
	File file = new File(dirpath);
	String filepath=file.getPath()+"/"+datestr+".jpg";
	if(!file.exists())
		file.mkdirs();
		try {
			FileOutputStream fileOutputStream = new FileOutputStream(filepath);
			bitmap.compress(Bitmap.CompressFormat.JPEG, 100, fileOutputStream);
			fileOutputStream.close();
			return filepath;
		} catch (Exception e) {
			e.printStackTrace();
			return null;
	} 
}
public static String saveImage(Bitmap bitmap,String name){
	String dirpath=Environment.getExternalStorageDirectory().getPath()+"/DCIM/images";
	File file = new File(dirpath);
	String filepath=file.getPath()+"/"+name;
	if(!file.exists())
		file.mkdirs();
	File imagefile=new File(filepath);
	if(imagefile.exists())
		imagefile.delete();
		try {
			FileOutputStream fileOutputStream = new FileOutputStream(filepath);
			bitmap.compress(Bitmap.CompressFormat.JPEG, 100, fileOutputStream);
			fileOutputStream.close();
			return filepath;
		} catch (Exception e) {
			e.printStackTrace();
			return null;
	} 
}
}
