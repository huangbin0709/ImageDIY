package com.huangbin.test.diyimage;

import java.util.Vector;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PaintFlagsDrawFilter;
import android.graphics.Point;
import android.graphics.Rect;
import android.graphics.Bitmap.Config;
import android.graphics.Paint.Cap;
import android.graphics.Paint.Style;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.WindowManager;
import android.view.SurfaceView;

public class ImageSurface extends SurfaceView implements SurfaceHolder.Callback {

	private SurfaceHolder holder=null; 
	private boolean isBitmapInitial;
	private Bitmap bitmap;
	private int max_width;
	private int max_height;
	private Point location;
	private MainLoop mainloop;
	private Vector<Float> xList = new Vector<Float>();
	private Vector<Float> yList = new Vector<Float>();
	private Vector<Integer> colorList;
	private Vector<Integer> strokeWidthList;
	private Vector<Integer> endList;
	private int color = Color.GREEN;
	private int strokeWidth = 5;
	String[] text = {" "};
	private int textColor = Color.GREEN;
	private int textSize= 30;
	float[] textWindow;
	float[] textStartMovePos = new float[2];
	float[] windowStartMovePos = new float[2];
	float tempX;
	float tempY;
	float tempXX;
	float tempYY;
	float tmp0;
	float tmp1;
	float tmp2;
	float tmp3;
	int windowheight=100;
	int windowwidth=100;
	boolean textMove = false;
	float[] window;
	boolean windowMove=false;
	int windowresize=0;
	int distance=5;
	private boolean eraser = false;
	private int State;
	private boolean isTextOk = false;
	private boolean isClipOk = false;
	PaintFlagsDrawFilter mSetfil;
	public static final int TEXT=1;
	public static final int DODO=2;
	public static final int CLIP=4;
	public static final int LASHEN=5;
	public static final int FANGDA=6;
	public static final int FISHEYE=7;
	public static final int GRAB=8;
	public static final int AUTO=9;
	public static final int FACERED=10;
	public static final int OTHERS=3;
	private int resizeflag=0;
	private float qulv=0.1f;
	private boolean isChoseOk=false;
	private boolean isGrabChoseOk=false;
	private Filters filter;
	private ChangeBitmapThread bitmapchange;
	private int EventDown=1;//CV_EVENT_LBUTTONDOWN=1,CV_EVENT_RBUTTONDOWN=2
	private int EventUp=4;//CV_EVENT_LBUTTONUP=4,CV_EVENT_RBUTTONUP=5
	private int EventMove=0;//CV_EVENT_MOUSEMOVE=0
	private int flags=0;
	public void setGrabChoseState(boolean state){
		isGrabChoseOk=state;
	}
	public void setState(int state){
		State = state;
	}
	public void setChoseState(boolean state){
		isChoseOk = state;
		resizeflag=1;
	}
	public void setQuLv(float value){
		qulv=value;
	}
	public int getState(){
		return State;
	}
    public void setGrabFore(){
    	setColor(Color.RED);
    	if(!isGrabChoseOk)
    	{
    		setGrabRect();
    		isGrabChoseOk=true;
    		clear();
    	}
    	flags=16;
    }
    public void setGrabBack(){
    	setColor(Color.GREEN);
    	if(!isGrabChoseOk)
    	{
    		setGrabRect();
    		isGrabChoseOk=true;
    		clear();
    	}
    	flags=8;
    	
    }
	public void doodleInitial(){
		colorList = new Vector<Integer>();
		strokeWidthList = new Vector<Integer>();
		endList = new Vector<Integer>();
	}
	//璁剧疆鐢荤瑪棰滆壊
	public void setColor(int c){
		color = c;
	}
	//璁剧疆鐢荤瑪绮楃粏
	public void setStrokeWidth(int c){
		strokeWidth = c;
	}
	//璁剧疆姗＄毊鎿�
	public void setEraser(boolean c){
		eraser = c;
	}
	//娓呯┖灞忓箷
	public void clear(){
		xList.clear();
		yList.clear();
		colorList.clear();
		strokeWidthList.clear();
		endList.clear();
	}
	//text鏂规硶
	public void setText(String s){
		text = s.split("\n");
	}
			//鏂囧瓧棰滆壊
	public void setTextColor(int color){
		textColor = color;
	}
			//鏂囧瓧澶у皬
	public void setTextSize(int size){
		textSize = size;
	}
	public void setTextOk(boolean state){
		isTextOk=state;
	}
	public void setClipOk(boolean state){
		isClipOk=state;
	}
	public ImageSurface(Context context, AttributeSet attrs) {
		super(context, attrs);
		// TODO 鑷姩鐢熸垚鐨勬瀯閫犲嚱鏁板瓨鏍�
		 holder=getHolder();
		 holder.addCallback(this);
		 WindowManager wm = (WindowManager)context.getSystemService(Context.WINDOW_SERVICE);
		 max_width = wm.getDefaultDisplay().getWidth();     // 灞忓箷瀹藉害锛堝儚绱狅級
	     max_height = wm.getDefaultDisplay().getHeight()-120;   // 灞忓箷楂樺害锛堝儚绱狅級
	     textWindow = new float[4];
		 textWindow[0] = wm.getDefaultDisplay().getWidth()/2 -110;
		 textWindow[1] = wm.getDefaultDisplay().getHeight()/2 - 180;
		 textWindow[2] = textWindow[0] + 200;
		 textWindow[3] = textWindow[1] + 50;
		 window = new float[4];
		 windowwidth=100;
		 windowheight=100;
		 window[0] = wm.getDefaultDisplay().getWidth()/2 -110;
		 window[1] = wm.getDefaultDisplay().getHeight()/2 - 180;
		 window[2] = window[0] + windowwidth;
		 window[3] = window[1] + windowheight;
	     bitmap=Bitmap.createBitmap(max_width, max_height, Bitmap.Config.ARGB_8888);
	     location=new Point();
	     mSetfil= new PaintFlagsDrawFilter(0, Paint.FILTER_BITMAP_FLAG);
	     filter=new Filters();
	 	}
	
	public Bitmap getBitmap(){
		Bitmap temp = Bitmap.createBitmap(bitmap.getWidth(), bitmap.getHeight(), Config.ARGB_8888);
		Canvas c = new Canvas(temp);
		Log.i("setbitmap", "getBitmap doDraw");
		doDraw(c, location.x, location.y);
		c.save(Canvas.ALL_SAVE_FLAG);
		c.restore();
		Log.i("setbitmap", "getBitmap restore");
		return temp;
	}
	public Bitmap getClipBitmap(){
		Bitmap temp = Bitmap.createBitmap(windowwidth, windowheight, Config.ARGB_8888);
		Canvas c = new Canvas(temp);
		Rect cliprect=new Rect((int)window[0], (int)window[1], (int)window[2], (int)window[3]);
		Rect dstrect=new Rect(0, 0, cliprect.width(), cliprect.height());
		Paint paint=new Paint();
		c.drawBitmap(bitmap, cliprect, dstrect, paint);
		return temp;
	}
	public Bitmap getFishEyeBitmap(boolean isTu){
		Bitmap bmp=filter.fisheyescale(bitmap, (int)window[0], (int)window[1], (int)window[2], (int)window[3],qulv,false,isTu);
		return bmp;
	}
	public void setGrabRect(){
		ImageUtilsJni.setGrabRect((int)window[0], (int)window[1], (int)window[2], (int)window[3]);
		
	}
	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width,
			int height) {
		// TODO 鑷姩鐢熸垚鐨勬柟娉曞瓨鏍�
		
	}
	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		// TODO 鑷姩鐢熸垚鐨勬柟娉曞瓨鏍�
		mainloop = new MainLoop();
		mainloop.setRunning(true);
		mainloop.start();
		bitmapchange=new ChangeBitmapThread();
		bitmapchange.setRunning(true);
		bitmapchange.start();
	}
	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		// TODO 鑷姩鐢熸垚鐨勬柟娉曞瓨鏍�
		mainloop.setRunning(false);
		bitmapchange.setRunning(false);
		try {
			mainloop.join();
			bitmapchange.join();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
	public void StopThread(){
		mainloop.setRunning(false);
		bitmapchange.setRunning(false);
		try {
			mainloop.join();
			bitmapchange.join();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	public void StartThread(){
		mainloop = new MainLoop();
		mainloop.setRunning(true);
		mainloop.start();
		bitmapchange=new ChangeBitmapThread();
		bitmapchange.setRunning(true);
		bitmapchange.start();
	}
	private void noImgDraw(Canvas canvas){
		super.onDraw(canvas);
		Log.i("setbitmap", "noimgdraw");
		Paint paint=new Paint();
		canvas.drawColor(Color.BLACK);
		canvas.drawBitmap(bitmap, 0, 0, paint);
	}
	public void doDraw(Canvas canvas, float x, float y) {
		//Log.i("setbitmap", "doDraw");
		super.onDraw(canvas);
		canvas.setDrawFilter( mSetfil );
		Paint p=new Paint(); 
		p.setAntiAlias(true);
		p.setStyle(Style.STROKE);
		p.setStrokeCap(Cap.ROUND);
	    canvas.drawBitmap(bitmap, x, y, p);			
		if(State == TEXT){
			   
			   p.setColor(Color.WHITE);
			   p.setStrokeWidth(5);
			   
			   if(!isTextOk)
				   canvas.drawRect(textWindow[0]+x-location.x, textWindow[1]+y-location.y, textWindow[2]+x-location.x, textWindow[3]+y-location.y, p);
			   
			   p.setStrokeWidth(0);
			   
			   int maxWidth = 0;
			   p.setColor(textColor);			   
			   p.setTextSize(textSize);
				   
			   for(int i = 0; i < text.length; i++){
				   canvas.drawText(text[i], textWindow[0]+10+x-location.x, textSize*(i+1)+textWindow[1]+y-location.y, p);
				   if(text[i].length() > maxWidth)
					   maxWidth = text[i].length();
			   }			   
			   
			   //鏂囧瓧妗嗗ぇ灏�
			   textWindow[2] = textWindow[0] + textSize*maxWidth + 20;
			   textWindow[3] = textWindow[1] + textSize*text.length + 10;		   
		   }
		if(State == CLIP||State == FISHEYE||State==GRAB){
			   p.setColor(Color.YELLOW);
			   p.setStrokeWidth(5);
			   if(!isClipOk&&!isChoseOk)
				   canvas.drawRect(window[0]+x-location.x, window[1]+y-location.y, window[2]+x-location.x, window[3]+y-location.y, p);
			   window[2] = window[0] + windowwidth;
			   window[3] = window[1] + windowheight;
		   }
		if(State == DODO||State == AUTO||(State==GRAB&&isGrabChoseOk)){

			   if(!xList.isEmpty()&&!yList.isEmpty())
				   for(int i = 0; i < xList.size() - 1; i++){
					   if(endList.elementAt(i) == 0)
						   continue;
					   p.setColor(colorList.elementAt(i));
					   p.setStrokeWidth(strokeWidthList.elementAt(i));
					   canvas.drawLine(xList.elementAt(i)+x-location.x, yList.elementAt(i)+y-location.y, xList.elementAt(i+1)+x-location.x, yList.elementAt(i+1)+y-location.y, p);
				   }
	    }
			}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		// TODO 鑷姩鐢熸垚鐨勬柟娉曞瓨鏍�
		if(event.getAction()==MotionEvent.ACTION_DOWN){
			if(State==LASHEN){
				//Log.i("frame", "down");
				textStartMovePos[0]=event.getX();
				textStartMovePos[1] = event.getY();
				tempX=textStartMovePos[0];
				tempY=textStartMovePos[1];
				//resizeflag=1;
			//	Log.i("frame", String.valueOf(resizeflag));
			}
			/*if(State==FANGDA){
				textStartMovePos[0]=event.getX();
				textStartMovePos[1] = event.getY();
				resizeflag=1;
			}
			if(State==FACERED){
				textStartMovePos[0]=event.getX();
				textStartMovePos[1] = event.getY();
				tempX=textStartMovePos[0];
				tempY=textStartMovePos[1];
				StopThread();
				Bitmap bmp=getBitmap();
				bmp=ImageUtils.beautiFace(bmp, (int)textStartMovePos[0], (int)textStartMovePos[1], 1);
				setBitmap(bmp);
				StartThread();
			}*/
			if(State==GRAB&&isGrabChoseOk){
				ImageUtilsJni.mouseClickJni(EventDown, (int)event.getX(),(int)event.getY(), flags);
			}
			if(State == TEXT){
				if(event.getX() > textWindow[0] && event.getX() < textWindow[2]
						   && event.getY() > textWindow[1] && event.getY() < textWindow[3]){
					textStartMovePos[0] = event.getX();
					textStartMovePos[1] = event.getY();
					tempX = textWindow[0];
					tempY = textWindow[1];
					textMove = true;
					//Log.i("text", "settextmove");
				}
			}
			if(State == CLIP||State == FISHEYE||(State==GRAB&&!isGrabChoseOk)){
				if(event.getX() > window[0]+distance && event.getX() < window[2]-distance
						   && event.getY() > window[1]+distance && event.getY() < window[3]-distance){
					windowStartMovePos[0] = event.getX();
					windowStartMovePos[1] = event.getY();
					tempXX = window[0];
					tempYY= window[1];
					windowMove = true;
					//Log.i("text", "settextmove");
				}
				if(Math.abs(event.getX()-window[0])<distance||Math.abs(event.getX()-window[2])<distance||Math.abs(event.getY()-window[1])<distance||Math.abs(event.getY()-window[3])<distance){
					windowStartMovePos[0] = event.getX();
					windowStartMovePos[1] = event.getY();
					tmp0=window[0];
					tmp1=window[1];
					tmp2=window[2];
					tmp3=window[3];
					if(Math.abs(event.getX()-window[0])<distance)
						windowresize=1;
					else if(Math.abs(event.getX()-window[2])<distance)
						windowresize=2;
					else if(Math.abs(event.getY()-window[1])<distance)
						windowresize=3;
					else 
						windowresize=4;
						
				}
			}
		}
		if(event.getAction()==MotionEvent.ACTION_MOVE ){
			//Log.i("text", "actionmove");
			if(State == TEXT){
				if(textMove){
					
					   //Log.i("text", "textmove");
					   textWindow[0] = event.getX() - textStartMovePos[0] + tempX;
					   textWindow[1] = event.getY() - textStartMovePos[1] + tempY;
					
				}
			}
			if(State==GRAB&&isGrabChoseOk){
				ImageUtilsJni.mouseClickJni(EventMove, (int)event.getX(),(int)event.getY(), flags);
			}
			if(State==LASHEN){
		         if(Math.sqrt((event.getX()-textStartMovePos[0])*(event.getX()-textStartMovePos[0])+(event.getY()-textStartMovePos[1])*(event.getY()-textStartMovePos[1]))>3){
		        	// if(!hasdone){
		        		 textWindow[2]=event.getX();
		        		 textWindow[3]=event.getY();
			        	 resizeflag=1;
			        	// Log.i("frame", "should do");
			        	// hasdone=true;
		        	// }
		        	 
		         }
			}
			/*if(State==FACERED){
		         if(Math.sqrt((event.getX()-textStartMovePos[0])*(event.getX()-textStartMovePos[0])+(event.getY()-textStartMovePos[1])*(event.getY()-textStartMovePos[1]))>3){
		        	 textStartMovePos[0]=event.getX();
						textStartMovePos[1] = event.getY();
						//resizeflag=1;
						StopThread();
						Bitmap bmp=getBitmap();
						bmp=ImageUtils.beautiFace(bmp, (int)textStartMovePos[0], (int)textStartMovePos[1], 1);
						setBitmap(bmp);
						StartThread();
		        	 
		         }
			}*/
			if(State == CLIP||State == FISHEYE||(State==GRAB&&!isGrabChoseOk)){
				if(event.getX() > location.x && event.getX() < (location.x+bitmap.getWidth()) &&
						event.getY() > location.y && event.getY() < (location.y+bitmap.getHeight())){
				if(windowMove){
					
					   //Log.i("text", "textmove");
					   window[0] = event.getX() - windowStartMovePos[0] + tempXX;
					   window[1] = event.getY() - windowStartMovePos[1] + tempYY;
					
				}
				if(windowresize>0){
					   float xchange=event.getX() - windowStartMovePos[0];
					   float ychange=event.getY() - windowStartMovePos[1];
					if(windowresize==1)
					{  
						window[0] =tmp0+xchange;
					    windowwidth=(int)(window[2]-window[0]);
					}
					if(windowresize==2)
					{
						window[2]=tmp2+xchange;
						windowwidth=(int)(window[2]-window[0]);
					}
					if(windowresize==3)
					{
						window[1]=tmp1+ychange;
						windowheight=(int)(window[3]-window[1]);
					}
					if(windowresize==4)
					{
						window[3]=tmp3+ychange;
						windowheight=(int)(window[3]-window[1]);
					}
				}
			}
			}
			if(event.getX() > location.x && event.getX() < (location.x+bitmap.getWidth()) &&
					event.getY() > location.y && event.getY() < (location.y+bitmap.getHeight())){
			if(!eraser){//doodle涓嬭褰曡建杩�
			if(xList.isEmpty() && yList.isEmpty()){
				xList.add(event.getX());
				yList.add(event.getY());
				if((State == DODO||State == AUTO||(State==GRAB&&isGrabChoseOk))&& colorList.isEmpty() && endList.isEmpty() && strokeWidthList.isEmpty()){
					colorList.add(color);
					endList.add(1);
					strokeWidthList.add(strokeWidth);
				}
			}
			else if(Math.abs(xList.lastElement()-event.getX()) > (strokeWidth) || Math.abs(yList.lastElement()-event.getY()) > (strokeWidth)){
				xList.add(event.getX());
				yList.add(event.getY());
				if(State == DODO||State == AUTO||(State==GRAB&&isGrabChoseOk)){
				colorList.add(color);
				endList.add(1);
				strokeWidthList.add(strokeWidth);
				}
			}
			}else{
				for(int i = 0; i < xList.size(); i++){
					if(xList.elementAt(i) < (event.getX() + strokeWidth) &&
							xList.elementAt(i) > (event.getX() - strokeWidth) &&
							yList.elementAt(i) < (event.getY() + strokeWidth) &&
							yList.elementAt(i) > (event.getY() - strokeWidth)){
						if(i > 0)
						endList.set(i-1, 0);
						xList.remove(i);
						yList.remove(i);
						colorList.remove(i);
						strokeWidthList.remove(i);
						endList.remove(i);
					}
				}
			}
			}else{
				if(State == DODO||State == AUTO||(State==GRAB&&isGrabChoseOk))
					if(!endList.isEmpty())
						endList.set(endList.size()-1, 0);
			}
		}
		if(event.getAction()==MotionEvent.ACTION_UP ){
			textMove = false;
			windowMove = false;
			windowresize=0;
			if(State==GRAB&&isGrabChoseOk){
				ImageUtilsJni.mouseClickJni(EventUp, (int)event.getX(),(int)event.getY(), flags);
			}
			if(event.getX() > location.x && event.getX() < (location.x+bitmap.getWidth()) &&
				event.getY() > location.y && event.getY() < (location.y+bitmap.getHeight())){
			
			if(!eraser){
			xList.add(event.getX());
			yList.add(event.getY());
			if(State == DODO||State == AUTO||(State==GRAB&&isGrabChoseOk)){
			colorList.add(color);
			endList.add(0);
			strokeWidthList.add(strokeWidth);
			}
			}
			}
			else{
				if(State == DODO||State == AUTO||(State==GRAB&&isGrabChoseOk))
					if(!endList.isEmpty())
						endList.set(endList.size()-1, 0);
			}
		}
		return true;
	}

	public void setBitmap(Bitmap bmp){
		bitmap = bmp.copy(Bitmap.Config.ARGB_8888, false);
//		mBitmapShader = new BitmapShader(bitmap, TileMode.REPEAT, TileMode.MIRROR);
		isBitmapInitial = true;
		//Log.i("setbitmap", "setbitmap");
		
	}
	Bitmap getCopyBitmap(int x,int y,int R){
	Bitmap temp = Bitmap.createBitmap(windowwidth, windowheight, Config.ARGB_8888);
	Canvas c = new Canvas(temp);
	Rect cliprect=new Rect(x-R, y-R, x+R, y+R);
	Rect dstrect=new Rect(0, 0, cliprect.width(), cliprect.height());
	Paint paint=new Paint();
	c.drawBitmap(bitmap, cliprect, dstrect, paint);
	return temp;
	}
	class ChangeBitmapThread extends Thread{
		private boolean isBitmapThreadRun;
		public void setRunning(boolean r){
			isBitmapThreadRun = r;
		}
		public void run() {
			 // TODO Auto-generated method stub
			while(isBitmapThreadRun){
				//Log.d("setbitmap", "start---run");
			   try{
				  // Log.i("frame", "run");
				   if(resizeflag>0){
					  // Log.i("frame","doing");
					//  int R=10;
					//   if(isChoseOk)
					//	   bitmap=getFishEyeBitmap();
					textStartMovePos[0]=textWindow[2];
					textStartMovePos[1]=textWindow[3];
					if(State==LASHEN)
					bitmap=filter.lashen(bitmap, (int)tempX, (int)tempY, (int)textWindow[2], (int)textWindow[3],30,10.0f);
					/*if(State==FACERED)
						{
						Bitmap bmp=getBitmap();
						bmp=ImageUtils.beautiFace(bmp, (int)textStartMovePos[0], (int)textStartMovePos[1], 1);
						setBitmap(bmp);
						}*/
					/*else if(State==FANGDA)
					{ bitmap=filter.zoomSwirl(bitmap,(int)textStartMovePos[0]-R,(int)textStartMovePos[1]-R,(int)textStartMovePos[0]+R,(int)textStartMovePos[1]+R,1.05f);
					   Log.i("frame","done");}*/
					   resizeflag=0;
				   }
			       // resizeflag=0;
			     }catch(Exception e){
			    	 Log.i("setbitmap", "exception");
			   }finally{
			   }
			}
		}		
	}
	class MainLoop extends Thread{
		private boolean isRun;
		public void setRunning(boolean r){
			isRun = r;
		}
		public void run() {
			 // TODO Auto-generated method stub
			Canvas c = null;
			while(isRun){
				//Log.d("setbitmap", "start---run");
			   try{
				   
			     if(isBitmapInitial){
			    	 Rect rect = new Rect(location.x, location.y, location.x+bitmap.getWidth(), location.y+bitmap.getHeight());
			    	// Rect rect = new Rect(0, 0, max_width, max_height);
			    	 c=holder.lockCanvas(rect);
			    	 location.x = c.getWidth()/2-bitmap.getWidth()/2;
			    	 location.y = c.getHeight()/2-bitmap.getHeight()/2;
			    	// Log.i("frame","runing");
			    	 doDraw(c, location.x, location.y);
			    	 //sleep(100);
			    	 //doDraw(c, 0, 0);
			    	 //matrix.setTranslate(center.x, center.y);
			     }
			     else
			     { 
			    	 Rect rect = new Rect(location.x, location.y, max_width, max_height);
			    	 c=holder.lockCanvas(rect);
			    	 noImgDraw(c);

			     }

			   
			     }catch(Exception e){
			    	 Log.i("setbitmap", "exception");
			   }finally{
				   
				   holder.unlockCanvasAndPost(c);
			   }
			}
		}
	}

}
