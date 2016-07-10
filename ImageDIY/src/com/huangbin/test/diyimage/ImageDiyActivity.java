package com.huangbin.test.diyimage;

import java.util.List;

import com.huangbin.test.R;
import com.huangbin.test.menu.MenuAdapter;
import com.huangbin.test.menu.MenuInfo;
import com.huangbin.test.menu.MenuUtils;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.ColorMatrix;
import android.graphics.ColorMatrixColorFilter;
import android.graphics.Paint;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.widget.AdapterView;
import android.widget.EditText;
import android.widget.GridView;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.SeekBar;
import android.widget.ImageView.ScaleType;
import android.widget.PopupWindow;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.AdapterView.OnItemClickListener;

public class ImageDiyActivity extends Activity implements OnSeekBarChangeListener {
	
//	private ImageButton btn_cancel;
//	private ImageButton btn_store;
	private ImageView imageview;
	private PopupWindow menu;
	private ProgressDialog mProgress;
	private MenuAdapter menuAdapter;
	private List<MenuInfo> menulists;
	private GridView gridview;
	private View view;
	private Bitmap bitmap;
	private Bitmap tmpbitmap;
	private Bitmap maskbitmap;
	//private Bitmap meixuebmp;
	private int max_width;
	private int max_height;
	private String path;
	AlertDialog dialog;
	Dialog mdialog;
	private int setuppos;
	private ImageSurface sv;
	private final int TEXTSIZE=1;
	private final int TEXTCOLOR=2;
	private final int PAINTSIZE=3;
	private final int IMG_OK=4;
	private final int GAUSE_NOISE=5;
	private final int POISSON=6;
	private final int MOVE_SMOOTH=7;
	private final int RESIZE_SMOOTH=8;
	private final int SHUIWEN=9;
	private final int HAHA=10;
	private final int LUOXUAN=11;
	private final int QIUMIAN=12;
	private final int FISH=13;
	private final int PINGJIA=14;
	private final int GRABCUT=15;
	private final int GRABCOMPOSITE=16;
	private final int AINPAINT=17;
	private final int ATIQU=18;
	private final int ACOMPOSITE=19;
	private final int GRESIZE=20;
	private final int ARESIZE=21;
	private final int VISUAL=22;
	private boolean dismissflag=true;
	private Filters filter;
	private int lastSeekBar=0;
	private TextView image_score=null;
	private String imagescore=null;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO 鑷姩鐢熸垚鐨勬柟娉曞瓨鏍�
		super.onCreate(savedInstanceState);
		setContentView(R.layout.imagelayout);
		setTitle("图片处理");
		setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
	//	btn_cancel=(ImageButton)findViewById(R.id.btn_cancel);
	//	btn_store=(ImageButton)findViewById(R.id.btn_store);
		DisplayMetrics metric = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(metric);
        max_width = metric.widthPixels;     // 灞忓箷瀹藉害锛堝儚绱狅級
        max_height = metric.heightPixels-120;   // 灞忓箷楂樺害锛堝儚绱狅級
        Log.i("save",Integer.toString(max_height));  
		imageview=(ImageView)findViewById(R.id.imageview);
		
		imageview.setMaxHeight(max_height);
		imageview.setMaxWidth(max_width);
		imageview.setAdjustViewBounds(true);
		image_score=(TextView)findViewById(R.id.image_score);
		Intent intent = getIntent();
		path = intent.getStringExtra("path");
		if (null == path)
		{
			Toast.makeText(this, R.string.no_img, Toast.LENGTH_SHORT).show();
			finish();
		}
		filter=new Filters();
		bitmap = ImageSwitcher.readBitmapAutoSize(path,max_width,max_height);
		//bitmap=ImageSwitcher.readBitmapRealSize(path);
		bitmap = ImageSwitcher.keepBitmapAutoSize(bitmap, max_width, max_height);
		imageview.setImageBitmap(bitmap);
		
	}
	private void storeImage(){
		if((path=ImageSwitcher.saveImage(bitmap))!=null){
			dialog=new AlertDialog.Builder(this).setTitle("保存成功")
		    		 .setIcon(R.drawable.save_ok)
		    		 .create();
		    		 dialog.show();
		}
		else{
			dialog=new AlertDialog.Builder(this).setTitle("保存失败")
		    		 .setIcon(R.drawable.save_wrong)
		    		 .setMessage("保存失败")
		    		 .create();
		    		 dialog.show();
		}
		    new Thread(){
		    	public void run()
		    	{
				try{
					sleep(1000);
				}
				catch(Exception e)
				{
					e.printStackTrace();
				}
				finally
				{
					dialog.dismiss();
				}
				
			    }
		        }.start();
	}
	public void onClick(View v) {
		switch(v.getId())
		{
		case R.id.btn_cancel:
			    bitmap = ImageSwitcher.readBitmapAutoSize(path,max_width,max_height);
			    bitmap = ImageSwitcher.keepBitmapAutoSize(bitmap, max_width, max_height);
		      //  hideSaveButton();
		        imageview.setScaleType(ScaleType.CENTER_INSIDE);
		        imageview.setImageBitmap(bitmap);
			    break;
		case R.id.btn_store:
			storeImage();
			break;
		case R.id.btn_menu:
			initMenu(MenuUtils.MENU_MAIN,true);
			startMenu(MenuUtils.MENU_MAIN, 0);
			break;
		case R.id.btn_exit:
			 finish();
			break;
		}
	}
	private void startNoChildMenu(int flag,int surflag){
		switch(flag){
		case MenuUtils.MENU_RESIZE:
			imageview.setScaleType(ScaleType.CENTER);
			tmpbitmap=bitmap.copy(Bitmap.Config.ARGB_8888, false);
			break;
		case MenuUtils.MENU_TIAOSE:
			tmpbitmap=bitmap.copy(Bitmap.Config.ARGB_8888, false);
			break;
		}
		if(surflag>0)
			menu.showAtLocation(ImageDiyActivity.this.findViewById(R.id.imageedit), Gravity.BOTTOM, 0, 0);
			else
			menu.showAtLocation(ImageDiyActivity.this.findViewById(R.id.imagelayout), Gravity.BOTTOM, 0, 0);
	}
	private void startMenu(int flag,int surflag){
		tmpbitmap=bitmap.copy(Bitmap.Config.RGB_565, false);
		image_score.setVisibility(View.INVISIBLE);
		switch(flag){
		case MenuUtils.MENU_MAIN:
			menulists = MenuUtils.getMenuList();
			break;
		case MenuUtils.MENU_BIANJI:
			menulists = MenuUtils.getBianjiMenuList();
			break;
		case MenuUtils.MENU_TEXT:
			menulists = MenuUtils.getTextMenuList();
			break;
		case MenuUtils.MENU_DODO:
			menulists = MenuUtils.getDodoMenuList();
			break;
		case MenuUtils.MENU_LVJING:
			menulists = MenuUtils.getTeXiaoMenuList();
			break;
		case MenuUtils.MENU_XUANZHUAN:
			menulists = MenuUtils.getXuanZhuanMenuList();
			break;
		case MenuUtils.MENU_MIROOR:
			menulists = MenuUtils.getFanZhuanMenuList();
			break;
		case MenuUtils.MENU_XUANRUAN:
			menulists = MenuUtils.getXuanRanMenuList();
			break;
		case MenuUtils.MENU_HUIDU:
			menulists = MenuUtils.getHuiDuMenuList();
			break;
		case MenuUtils.MENU_RUIHUA:
			menulists = MenuUtils.getRuiHuaMenuList();
			break;
		case MenuUtils.MENU_MOHU:
			menulists = MenuUtils.getMoHuMenuList();
			break;
		case MenuUtils.MENU_LVBO:
			menulists = MenuUtils.getLvBoMenuList();
			break;
		case MenuUtils.MENU_NOISE:
			menulists = MenuUtils.getNoiseMenuList();
			break;
		case MenuUtils.MENU_BIANKUANG:
			menulists = MenuUtils.getaddFrameMenuList();
			break;
		case MenuUtils.MENU_COLORCHANGE:
			menulists = MenuUtils.getColorChangeMenuList();
			break;
		case MenuUtils.MENU_BLEND:
			menulists = MenuUtils.getBlendingMenuList();
			break;
		case MenuUtils.MENU_NIUQU:
			menulists = MenuUtils.getAngulationMenuList();
			break;
		case MenuUtils.MENU_MEIXUE:
			menulists = MenuUtils.getMeiXueMenuList();
			break;
		case MenuUtils.MENU_BIANXING:
			menulists = MenuUtils.getBianXingMenuList();
			break;
		case MenuUtils.MENU_AOTU:
			menulists = MenuUtils.getAoTuMenuList();
			break;
		case MenuUtils.MENU_GRAB:
			menulists = MenuUtils.getGrabMenuList();
			break;
		case MenuUtils.MENU_AUTO:
			menulists = MenuUtils.getAutoMenuList();
			break;
		case MenuUtils.MENU_ACOLOR:
			menulists = MenuUtils.getAColorMenuList();
			break;
		}
		menuAdapter = new MenuAdapter(ImageDiyActivity.this, menulists);
		gridview.setAdapter(menuAdapter);
		if(surflag>0)
		menu.showAtLocation(ImageDiyActivity.this.findViewById(R.id.imageedit), Gravity.BOTTOM, 0, 0);
		else
		menu.showAtLocation(ImageDiyActivity.this.findViewById(R.id.imagelayout), Gravity.BOTTOM, 0, 0);
	}
	private void initMenu(int flag,boolean disflag) {
		dismissflag=disflag;
		gridview=(GridView)View.inflate(this, R.layout.gridview_menu, null);
		menu = new PopupWindow(gridview, LayoutParams.FILL_PARENT, LayoutParams.WRAP_CONTENT);
		menu.setBackgroundDrawable(getResources().getDrawable(R.drawable.popup));
		menu.setFocusable(true);
		menu.setAnimationStyle(R.style.menushow);
		menu.update();
		gridview.setFocusableInTouchMode(true);
		gridview.setOnKeyListener(new android.view.View.OnKeyListener() {
			public boolean onKey(View v, int keyCode, KeyEvent event) {
				// TODO Auto-generated method stub
				if ((keyCode == KeyEvent.KEYCODE_MENU) && (menu.isShowing())) {
					menu.dismiss();  
					return true;
				}
				return false;
			}

		});
		gridview.setOnItemClickListener(new OnItemClickListener() {
			public void onItemClick(AdapterView<?> arg0, View arg1, int arg2, long arg3) {
				// TODO Auto-generated method stub
				MenuInfo mInfo = menulists.get(arg2);
				if(dismissflag){
					menu.dismiss();
				}
				//showSaveButton();
				switch (mInfo.menuId) {
				case MenuUtils.MENU_AOTU:
					initImageSurface(ImageSurface.FISHEYE);
					initMenu(MenuUtils.MENU_AOTU,true);
					startMenu(MenuUtils.MENU_AOTU, 1);
					break;
				case MenuUtils.MENU_GRAB:
					initImageSurface(ImageSurface.GRAB);
					sv.setStrokeWidth(2);
					ImageUtils.setSourceImage(bitmap);
					ImageUtilsJni.GrabInit();
					initMenu(MenuUtils.MENU_GRAB,true);
					startMenu(MenuUtils.MENU_GRAB, 1);
					break;
				case MenuUtils.MENU_AUTO:
					initImageSurface(ImageSurface.AUTO);
					sv.setStrokeWidth(5);
					sv.setColor(Color.GREEN);
					initMenu(MenuUtils.MENU_AUTO,true);
					startMenu(MenuUtils.MENU_AUTO, 1);
					break;
				case MenuUtils.MENU_AINPAINT:
					maskbitmap=sv.getBitmap();
					sv.StopThread();
					sv.clear();
					showProgress();
					new MyThread(AINPAINT).start();
					break;
				case MenuUtils.MENU_ACOMPOSITE:
					maskbitmap=sv.getBitmap();
					sv.StopThread();
					sv.clear();
					showProgress();
					new MyThread(ACOMPOSITE).start();				
					break;
				case MenuUtils.MENU_ARESIZE:
					maskbitmap=sv.getBitmap();
					sv.StopThread();
					sv.clear();
					showProgress();
					new MyThread(ARESIZE).start();				
					break;
				case MenuUtils.MENU_ATIQU:
					sv.StopThread();
					sv.clear();
					showProgress();
					new MyThread(ATIQU).start();				
					break;
				case MenuUtils.MENU_AVISUAL:
					ImageUtils.setSourceImage(bitmap);
					sv.StopThread();
					showProgress();
					new MyThread(VISUAL).start();				
					break;
				case MenuUtils.MENU_FORE:
					sv.setGrabFore();
					break;
				case MenuUtils.MENU_BACK:
					sv.setGrabBack();
					break;
				case MenuUtils.MENU_GRABCUT:					
					sv.clear();
					sv.StopThread();
					showProgress();
					new MyThread(GRABCUT).start();				
					break;				
				case MenuUtils.MENU_GCOMPOSITE:					
					ImageUtils.setSourceImage(bitmap);					
					sv.StopThread();
					showProgress();
					new MyThread(GRABCOMPOSITE).start();				
					break;
				case MenuUtils.MENU_GRESIZE:					
					ImageUtils.setSourceImage(bitmap);
					sv.StopThread();
					showProgress();
					new MyThread(GRESIZE).start();				
					break;
				case MenuUtils.MENU_GVISUAL:					
					ImageUtils.setSourceImage(bitmap);
					sv.StopThread();
					showProgress();
					new MyThread(VISUAL).start();				
					break;
				case MenuUtils.MENU_VISUAL:					
					ImageUtils.setSourceImage(bitmap);
					//sv.StopThread();
					showProgress();
					new MyThread(VISUAL).start();				
					break;
				case MenuUtils.MENU_SHOUSHEN:
					initImageSurface(ImageSurface.LASHEN);
					break;
				case MenuUtils.MENU_EYESCALE:
					initImageSurface(ImageSurface.FANGDA);
					break;
				case MenuUtils.MENU_MEIBAI:
					bitmap=ImageUtils.SkinFilter(bitmap, 2);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_QUBAN:
					bitmap=ImageUtils.SkinFilter(bitmap, 1);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_QUZHOU:
					bitmap=ImageUtils.SkinFilter(bitmap, 3);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_AO:
					sv.setChoseState(true);
					sv.setBitmap(sv.getFishEyeBitmap(false));
					break;
				case MenuUtils.MENU_TU:
					sv.setChoseState(true);
					sv.setBitmap(sv.getFishEyeBitmap(true));
					break;
				case MenuUtils.MENU_AUTUCANCEL:
					sv.setBitmap(bitmap);
					break;
				case MenuUtils.MENU_AOTUCHOSE:
					sv.setChoseState(false);
					break;
				case MenuUtils.MENU_TEXT:
					initImageSurface(ImageSurface.TEXT);
					initMenu(MenuUtils.MENU_TEXT,true);
					startMenu(MenuUtils.MENU_TEXT, 1);
					break;
				case MenuUtils.MENU_DODO:
					initImageSurface(ImageSurface.DODO);
					initMenu(MenuUtils.MENU_DODO,true);
					startMenu(MenuUtils.MENU_DODO, 1);
					break;
				case MenuUtils.MENU_HUIDU:
					bitmap=ImageUtils.GrayImage(bitmap);
					imageview.setImageBitmap(bitmap);
					initMenu(MenuUtils.MENU_HUIDU,false);
					startMenu(MenuUtils.MENU_HUIDU,0);
					break;
				case MenuUtils.MENU_LVBO:
					menu.dismiss();
					initMenu(MenuUtils.MENU_LVBO,false);
					startMenu(MenuUtils.MENU_LVBO,0);
					break;
				case MenuUtils.MENU_BIANKUANG:
					menu.dismiss();
					initMenu(MenuUtils.MENU_BIANKUANG,false);
					startMenu(MenuUtils.MENU_BIANKUANG,0);
					break;
				case MenuUtils.MENU_MOHU:
					menu.dismiss();
					initMenu(MenuUtils.MENU_MOHU,false);
					startMenu(MenuUtils.MENU_MOHU,0);
					break;
				case MenuUtils.MENU_RUIHUA:
					menu.dismiss();
					initMenu(MenuUtils.MENU_RUIHUA,false);
					startMenu(MenuUtils.MENU_RUIHUA,0);
					break;
				case MenuUtils.MENU_NOISE:
					menu.dismiss();
					initMenu(MenuUtils.MENU_NOISE,false);
					startMenu(MenuUtils.MENU_NOISE,0);
					break;
				case MenuUtils.MENU_XUANRUAN:
					initImageSurface(ImageSurface.OTHERS);
					initMenu(MenuUtils.MENU_XUANRUAN,false);
					startMenu(MenuUtils.MENU_XUANRUAN,1);
				    break;
				case MenuUtils.MENU_LVJING:
                     initMenu(MenuUtils.MENU_LVJING,false);
                     startMenu(MenuUtils.MENU_LVJING, 0);
					    break;
				case MenuUtils.MENU_BIANJI:
					initMenu(MenuUtils.MENU_BIANJI,true);
					startMenu(MenuUtils.MENU_BIANJI, 0);
					    break;
				case MenuUtils.MENU_MEIXUE:
					initMenu(MenuUtils.MENU_MEIXUE,true);
					startMenu(MenuUtils.MENU_MEIXUE, 0);
					    break;
				case MenuUtils.MENU_BIANXING:
					initMenu(MenuUtils.MENU_BIANXING,true);
					startMenu(MenuUtils.MENU_BIANXING, 0);
					    break;
				case MenuUtils.MENU_TIAOSE:
					initTiaoSeMenu();
					startNoChildMenu(MenuUtils.MENU_TIAOSE, 0);
					break;
				case MenuUtils.MENU_LEFT:
					bitmap=ImageUtils.rotateBitmap(bitmap, -90.0f);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_RIGHT:
					bitmap=ImageUtils.rotateBitmap(bitmap, 90.0f);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_RESIZE:
					initSuoFangMenu(1);
					startNoChildMenu(MenuUtils.MENU_RESIZE, 0);
					break;
				case MenuUtils.MENU_QULV:
					initSuoFangMenu(2);
					startNoChildMenu(MenuUtils.MENU_QULV, 1);
					break;
				case MenuUtils.MENU_XUANZHUAN:
					initMenu(MenuUtils.MENU_XUANZHUAN,false);
					startMenu(MenuUtils.MENU_XUANZHUAN, 0);
					break;
				case MenuUtils.MENU_CAIJIAN:
					initImageSurface(ImageSurface.CLIP);
					break;
				case MenuUtils.MENU_TEXTSIZE:
					//Toast.makeText(ImageDiyActivity.this, "澶у皬", Toast.LENGTH_SHORT).show();
					setTextSize();
					break;
				case MenuUtils.MENU_TEXTCOLOR:
					//Log.i("menu", "textcolor");
					setTextColor();
					break;
				case MenuUtils.MENU_TEXTCANCEL:
					sv.setText("");
					break;
				case MenuUtils.MENU_CUXI:
					sv.setEraser(false);
					setPaintSize();
					break;
				case MenuUtils.MENU_PAINTCOLOR:
					sv.setEraser(false);
					setTextColor();
					break;
				case MenuUtils.MENU_DODOCANCEL:
					sv.clear();
					break;
				case MenuUtils.MENU_ERASER:
					sv.setEraser(true);
					break;
				case MenuUtils.MENU_TEXTOK:
					sv.setTextOk(true);
					break;
				case MenuUtils.MENU_SURSAVE:
					bitmap=sv.getBitmap();
					storeImage();
					break;
				case MenuUtils.MENU_MIROOR:
					initMenu(MenuUtils.MENU_MIROOR, false);
					startMenu(MenuUtils.MENU_MIROOR, 0);
					break;
				case MenuUtils.MENU_LR:
					bitmap=ImageUtils.reverseImage(bitmap, 0);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_UP:
					bitmap=ImageUtils.reverseImage(bitmap, 1);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_HUAIJIU:
					bitmap=ImageUtils.oldImage(tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_LOMO:
					bitmap=ImageUtils.lomoImage(tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_SUMIAO:
					bitmap=ImageUtils.GrayImage(tmpbitmap);
					bitmap=ImageUtils.sketchImage(bitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_MIHONG:
					bitmap=ImageUtils.miHongImage(tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_DIPIAN:
					bitmap=ImageUtils.toFilm(tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_BOXING:
					bitmap=ImageUtils.angulationImage(tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_QIUMIAN:
					showProgress();
					new MyThread(QIUMIAN).start();
					break;
				case MenuUtils.MENU_HAHA:
					showProgress();
					new MyThread(HAHA).start();							
					break;
				case MenuUtils.MENU_LUOXUAN:
					showProgress();
					new MyThread(LUOXUAN).start();				
					break;
				case MenuUtils.MENU_BIANYUAN:
					bitmap=ImageUtils.bianYuan(tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_FUDIAO:
					bitmap=ImageUtils.fuDiaoImage(tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_GRAYWINDOW:
					bitmap=ImageUtils.windowImage(tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_GRAYFAZHI:
					bitmap=ImageUtils.GrayFaZhiImage(tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_GRAYLINE:
					bitmap=ImageUtils.lineChangeImage(tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_GRAYDIPIAN:
					bitmap=ImageUtils.toFilm(tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_GRAYZHIFANGTU:
					bitmap=ImageUtils.histogramFilter(tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_ONE:				
					bitmap=addFrame(tmpbitmap, 1);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_TWO:				
					bitmap=addFrame(tmpbitmap, 2);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_THREE:				
					bitmap=addFrame(tmpbitmap, 3);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_FOUR:				
					bitmap=addFrame(tmpbitmap, 4);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_BIANYUANRUIHUA:
					bitmap=ImageUtils.bianYuanSharpenImage(tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_LAPU:
					bitmap=ImageUtils.sharpenImage(tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_TIDURUIHUA:
					bitmap=ImageUtils.tiDuSharpImage(tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_DAOYING:
					bitmap=ImageUtils.ReflectionImage(tmpbitmap);
					bitmap=ImageSwitcher.keepBitmapAutoSize(bitmap, tmpbitmap.getWidth(),tmpbitmap.getHeight());
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_PIPERNOISE:
					bitmap=ImageUtils.addSaltAndPepperNoise(tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_GAUSENOISE:
					showProgress();
					new MyThread(GAUSE_NOISE).start();
					break;
				case MenuUtils.MENU_MIDDLELVBO:
					//bitmap=ImageUtils.middle_SmoothImage(tmpbitmap);
					bitmap=ImageUtils.cvSmooth(tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_AVELVBO:
					bitmap=ImageUtils.ave_SmoothImage(tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_GAUSESMOOTH:
					bitmap=ImageUtils.guass_SmoothImage(tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_MOVESMOOTH:
					showProgress();
					new MyThread(MOVE_SMOOTH).start();					
					break;
				case MenuUtils.MENU_RESIZESMOOTH:
					showProgress();
					new MyThread(RESIZE_SMOOTH).start();									
					break;
				case MenuUtils.MENU_ZHIFANGTU:
					bitmap=ImageUtils.histogramFilter(tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_EXPLOR:
					bitmap=ImageUtils.exposureImage(tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_YOUHUA:
					bitmap=ImageUtils.polishImage(tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_MASAIKE:
					bitmap=ImageUtils.mosaicImage(tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_BLEND:
					menu.dismiss();
					initMenu(MenuUtils.MENU_BLEND,false);
					startMenu(MenuUtils.MENU_BLEND, 0);
					break;
				case MenuUtils.MENU_COLORCHANGE:
					menu.dismiss();
					initMenu(MenuUtils.MENU_COLORCHANGE,false);
					startMenu(MenuUtils.MENU_COLORCHANGE, 0);
					break;
				case MenuUtils.MENU_NIUQU:
					menu.dismiss();
					initMenu(MenuUtils.MENU_NIUQU,false);
					startMenu(MenuUtils.MENU_NIUQU, 0);
					break;
				case MenuUtils.MENU_BLEND1:
					bitmap=blending(1,tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_BLEND2:
					bitmap=blending(2,tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_BLEND3:
					bitmap=blending(3,tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_BLEND4:
					bitmap=blending(4,tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_BLEND5:
					bitmap=blending(5,tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_BLEND6:
					bitmap=blending(6,tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_COLOR1:
					bitmap=ImageUtils.ColorChangeImage(tmpbitmap, 2);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_COLOR2:
					bitmap=ImageUtils.ColorChangeImage(tmpbitmap, 1);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_COLOR3:
					bitmap=ImageUtils.ColorChangeImage(tmpbitmap, 3);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_COLOR4:
					bitmap=ImageUtils.ColorChangeImage(tmpbitmap, 4);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_SOLAR:
					bitmap=ImageUtils.SolarizeFilter(tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_SUNZHAO:
					bitmap=ImageUtils.sunZhaoImage(tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_SUNYUN:
					bitmap=ImageUtils.sunYunImage(tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_CHANMIX:
					bitmap=ImageUtils.ChannelMixFilter(tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_MASK:
					bitmap=ImageUtils.MaskFilter(tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_POSTER:
					bitmap=ImageUtils.PosterizeFilter(tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_BUMP:
					bitmap=ImageUtils.BumpFilter(tmpbitmap);
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_PAO:
					Bitmap tmpframe=((BitmapDrawable)getResources().getDrawable(R.drawable.pao)).getBitmap();
					bitmap=ImageUtils.blendImage(tmpbitmap, tmpframe,3);
					tmpframe.recycle();	
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_LIUXING:
					Bitmap tmpframe2=((BitmapDrawable)getResources().getDrawable(R.drawable.start2)).getBitmap();
					bitmap=ImageUtils.blendImage(tmpbitmap, tmpframe2,3);
					tmpframe2.recycle();	
					imageview.setImageBitmap(bitmap);
					break;
				case MenuUtils.MENU_BOX:
					bitmap=ImageUtils.BoxBlurFilter(tmpbitmap);					
					break;
				case MenuUtils.MENU_SHUIWEN:
					showProgress();
					new MyThread(SHUIWEN).start();									
					break;
				case MenuUtils.MENU_FISH:
					showProgress();
					new MyThread(FISH).start();									
					break;
				}
			}
		});	
	}
	private Bitmap blending(int flag,Bitmap srcbmp){
		Bitmap tmpframe=((BitmapDrawable)getResources().getDrawable(R.drawable.start3)).getBitmap();
		Bitmap bmp=ImageUtils.blendImage(srcbmp, tmpframe,flag);
		return bmp;
	}
	private Bitmap addFrame(Bitmap bmp,int id){
		int frameid=0;
		int mode=0;
		switch(id){
		case 1:
			frameid=R.drawable.frame_big1;
			mode=1;
			break;
		case 2:
			frameid=R.drawable.frame2;
			mode=2;
			break;
		case 3:
			frameid=R.drawable.frame3;
			mode=2;
			break;
		case 4:
			frameid=R.drawable.framebig_4;
			mode=2;
			break;
		}
		Bitmap frame=((BitmapDrawable)getResources().getDrawable(frameid)).getBitmap();
		Bitmap dstbmp=ImageUtils.addFrame(bmp, frame,mode);
		return dstbmp;
	}
    private void initSuoFangMenu(int flag) {
    	int layoutid=0;
    	int barid=0;
    	if(flag==1){
    		layoutid= R.layout.resize;
    		barid=R.id.resizebar;
    	}
    	else{
    		layoutid= R.layout.fisheye;
    		barid=R.id.qulvbar;
    	}
		view=(View)View.inflate(this,layoutid, null);
		menu = new PopupWindow(view, LayoutParams.FILL_PARENT, LayoutParams.WRAP_CONTENT);
		menu.setBackgroundDrawable(getResources().getDrawable(R.drawable.popup));
		menu.setFocusable(true);
		menu.setAnimationStyle(R.style.menushow);
		menu.update();
		view.setFocusableInTouchMode(true);
		
		SeekBar resizeBar=(SeekBar)view.findViewById(barid);
		resizeBar.setOnSeekBarChangeListener(this);		
	}
    private void initTiaoSeMenu() {
		view=(View)View.inflate(this, R.layout.tiaose, null);
		menu = new PopupWindow(view, LayoutParams.FILL_PARENT, LayoutParams.WRAP_CONTENT);
		menu.setBackgroundDrawable(getResources().getDrawable(R.drawable.popup));
		menu.setFocusable(true);
		menu.setAnimationStyle(R.style.menushow);
		menu.update();
		view.setFocusableInTouchMode(true);
		SeekBar saturationBar=(SeekBar)view.findViewById(R.id.saturation);
		saturationBar.setOnSeekBarChangeListener(this);
		SeekBar lightnessBar=(SeekBar)view.findViewById(R.id.lightness);
		SeekBar contrastBar=(SeekBar)view.findViewById(R.id.contrast);
		lightnessBar.setOnSeekBarChangeListener(this);
		contrastBar.setOnSeekBarChangeListener(this);
	}
	private void initImageSurface(int state){
		setContentView(R.layout.imageedit);
		bitmap=ImageSwitcher.keepBitmapAutoSize(bitmap,max_width,max_height);
		sv=(ImageSurface)findViewById(R.id.sv);
		RelativeLayout.LayoutParams layout=new RelativeLayout.LayoutParams(bitmap.getWidth(), bitmap.getHeight());
		layout.addRule(RelativeLayout.CENTER_IN_PARENT);
		sv.setLayoutParams(layout);
		sv.setBitmap(bitmap);
		switch(state){
		case ImageSurface.DODO:
		case ImageSurface.GRAB:
		case ImageSurface.AUTO:
			sv.doodleInitial();
			break;
		case ImageSurface.CLIP:
			break;
		case ImageSurface.TEXT:
			sv.setTextOk(false);
		    EditText edittext=(EditText)findViewById(R.id.edittext);
		    edittext.setVisibility(View.VISIBLE);
		    edittext.addTextChangedListener(new TextWatcher() {
				@Override
				public void onTextChanged(CharSequence s, int start, int before, int count) {
					// TODO 鑷姩鐢熸垚鐨勬柟娉曞瓨鏍�
					if(s==null)
						s="";
					sv.setText(s.toString());
				}
				@Override
				public void beforeTextChanged(CharSequence s, int start, int count,
						int after) {
					// TODO 鑷姩鐢熸垚鐨勬柟娉曞瓨鏍�
				}
				@Override
				public void afterTextChanged(Editable s) {
					// TODO 鑷姩鐢熸垚鐨勬柟娉曞瓨鏍�
				}
			});
		    break;
		}
		sv.setState(state);
	}
	public void backtoImageView(){
		sv.setTextOk(true);
		sv.setChoseState(true);
		if(sv.getState()==ImageSurface.CLIP)
			bitmap=sv.getClipBitmap();
		else
		bitmap=sv.getBitmap();
		sv.setState(ImageSurface.OTHERS);
		setContentView(R.layout.imagelayout);
		imageview=(ImageView)findViewById(R.id.imageview);	
		imageview.setMaxHeight(max_height);
		imageview.setMaxWidth(max_width);
		imageview.setAdjustViewBounds(true);
		imageview.setImageBitmap(bitmap);
		//showSaveButton();
	}
    public void suronClick(View v){
    	switch(v.getId()){
    	case R.id.sur_exit:
    		backtoImageView();
    		break;
    	case R.id.sur_menu:
    		if(sv.getState()==ImageSurface.DODO){
    		initMenu(MenuUtils.MENU_DODO,true);
    		startMenu(MenuUtils.MENU_DODO, 1);
    		}else if(sv.getState()==ImageSurface.TEXT){
    			initMenu(MenuUtils.MENU_TEXT,true);
        		startMenu(MenuUtils.MENU_TEXT, 1);	
    		}else if(sv.getState()==ImageSurface.OTHERS){
    			initMenu(MenuUtils.MENU_XUANRUAN,true);
        		startMenu(MenuUtils.MENU_XUANRUAN, 1);
    		}else if(sv.getState()==ImageSurface.FISHEYE){
    			initMenu(MenuUtils.MENU_AOTU,true);
        		startMenu(MenuUtils.MENU_AOTU, 1);
    		}else if(sv.getState()==ImageSurface.GRAB){
    			initMenu(MenuUtils.MENU_GRAB,true);
        		startMenu(MenuUtils.MENU_GRAB, 1);
    		}
    		else if(sv.getState()==ImageSurface.AUTO){
    			initMenu(MenuUtils.MENU_AUTO,true);
        		startMenu(MenuUtils.MENU_AUTO, 1);
    		}
    		break;
    	}
    }
private void setTextColor(){
	mdialog = new ColorPickerDialog(this,"颜色选择", 
			new ColorPickerDialog.OnColorChangedListener() {
		
		@Override
		public void colorChanged(int color) {
			Message msg = new Message();
			msg.what = TEXTCOLOR;
			msg.arg1 = color;
			mhHandler.sendMessage(msg);
		}
	});
	mdialog.show();	
    }
   
    private void setTextSize(){
    	String color[] = new String[] { "16", "18", "24", "26", "28","30" };
    	dialog=new AlertDialog.Builder(this)
		.setTitle("文字大小")

		.setSingleChoiceItems(color, 0,
				new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog,
							int which) {
						setuppos = which;
					}
				})
        .setPositiveButton("确定",
				new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog,
							int which) {
						Message msg = new Message();
						msg.what = TEXTSIZE;
						switch (setuppos){
						case 0:
							msg.arg1=16;
							break;
						case 1:
							msg.arg1=18;
							break;
						case 2:
							msg.arg1=24;
							break;
						case 3:
							msg.arg1=26; 
							break;
						case 4:
							msg.arg1=28;
							break;
						case 5:
							msg.arg1=30;
						default:
							break;
						}
						mhHandler.sendMessage(msg);
					}
				})
		.setNegativeButton("取消",
				new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog,
							int which) {
					}
				}).create();
    	dialog.show();
    }
    private void setPaintSize(){
    	String color[] = new String[] { "2", "4", "6", "8", "10","14" };
    	dialog=new AlertDialog.Builder(this)
		.setTitle("画笔大小")

		.setSingleChoiceItems(color, 0,
				new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog,
							int which) {
						setuppos = which;
					}
				})
        .setPositiveButton("确定",
				new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog,
							int which) {
						Message msg = new Message();
						msg.what = PAINTSIZE;
						switch (setuppos){
						case 0:
							msg.arg1=2;
							break;
						case 1:
							msg.arg1=4;
							break;
						case 2:
							msg.arg1=6;
							break;
						case 3:
							msg.arg1=8; 
							break;
						case 4:
							msg.arg1=10;
							break;
						case 5:
							msg.arg1=14;
						default:
							break;
						}
						mhHandler.sendMessage(msg);
					}
				})
		.setNegativeButton("取消",
				new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog,
							int which) {
					}
				}).create();
    	dialog.show();
    }
    Handler mhHandler = new Handler() {
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case TEXTCOLOR:
				sv.setTextColor(msg.arg1);
				sv.setColor(msg.arg1);
				break;
			case TEXTSIZE:
				sv.setTextSize(msg.arg1);
				break;
			case PAINTSIZE:
				sv.setStrokeWidth(msg.arg1);
				break;
			case IMG_OK:
				imageview.setImageBitmap(bitmap);
                break;
			case GRABCUT:
				sv.setBitmap(maskbitmap);
				sv.StartThread();
				break;
			case GRABCOMPOSITE:
				sv.setBitmap(bitmap);
				sv.setChoseState(true);
				sv.StartThread();
				break;
			case GRESIZE:
				sv.setBitmap(bitmap);
				sv.setChoseState(true);
				sv.StartThread();
				break;
			case ACOMPOSITE:
			case ARESIZE:
				sv.setBitmap(bitmap);
				sv.StartThread();
				break;
			case VISUAL:
				//ImageUtils.setSourceImage(bitmap);
				//ImageUtilsJni.GrabInit();
				//sv.setBitmap(bitmap);
				//sv.StartThread();
				imageview.setImageBitmap(bitmap);
				break;
			case AINPAINT:
				sv.setBitmap(bitmap);
				sv.StartThread();
				break;
			case ATIQU:
				sv.setBitmap(maskbitmap);
				sv.StartThread();
				break;
			case PINGJIA:
				if((imagescore==null)||(imagescore==" "))
					imagescore=new String("失败");
				image_score.setVisibility(View.VISIBLE);
				image_score.setText("照片质量："+imagescore);
				break;
			}
		}
    };
	@Override
	public void onProgressChanged(SeekBar seekBar, int progress,
			boolean fromUser) {
		// TODO 鑷姩鐢熸垚鐨勬柟娉曞瓨鏍�
		if(seekBar.getId()==R.id.resizebar)
		{
		    float value=(float)progress/100;
		    if(value<=0)
			return;
		    bitmap=ImageUtils.resizeImage(tmpbitmap, value);
		    imageview.setImageBitmap(bitmap);
		    return;
		}
		if(seekBar.getId()==R.id.qulvbar)
		{
				float value=(float)progress/200;
				if(value<=0)
					return;
				sv.setQuLv(value);
		    return;
		}
		Bitmap tmpbmp=Bitmap.createBitmap(bitmap.getWidth(), bitmap.getHeight(), Bitmap.Config.ARGB_8888);
		Canvas canvas=new Canvas(tmpbmp);//寰楀埌鐢荤瑪瀵硅薄
		Paint  paint =new Paint(); //鏂板缓paint
		paint.setAntiAlias(true);
		ColorMatrix mAllMatrix=new ColorMatrix();
		ColorMatrix mLightnessMatrix=new ColorMatrix();
		ColorMatrix mSaturationMatrix=new ColorMatrix();
		ColorMatrix mHueMatrix=new ColorMatrix();
		if(lastSeekBar!=seekBar.getId())
			tmpbitmap=bitmap.copy(Bitmap.Config.ARGB_8888, false);
		switch(seekBar.getId()){
		case R.id.lightness:
			float mLumValue=progress*1.0F/127;
			mHueMatrix.reset();
			mHueMatrix.setScale(mLumValue,mLumValue,mLumValue,1);
			break;
		case R.id.contrast:
			float mHueValue=(progress-127) *1.0F/127*360;
			mLightnessMatrix.reset();//璁句负榛樿鍊�
			mLightnessMatrix.setRotate(0,mHueValue);//鎺у埗璁╃孩鑹插尯鍦ㄨ壊杞笂鏃嬭浆鐨勮搴�
			mLightnessMatrix.setRotate(1,mHueValue);//鎺у埗璁╃豢绾㈣壊鍖哄湪鑹茶疆涓婃棆杞殑瑙掑害
			mLightnessMatrix.setRotate(2,mHueValue);//鎺у埗璁╄摑鑹插尯鍦ㄨ壊杞笂鏃嬭浆鐨勮搴�
			break;
		case R.id.saturation:
			float mSaturationValue=progress*1.0F/127;
			mSaturationMatrix.reset();
			mSaturationMatrix.setSaturation(mSaturationValue);
			break;
		}
		lastSeekBar=seekBar.getId();
		mAllMatrix.reset();
		mAllMatrix.postConcat(mHueMatrix);
		mAllMatrix.postConcat(mSaturationMatrix);//鏁堟灉鍙犲姞
		mAllMatrix.postConcat(mLightnessMatrix);//鏁堟灉鍙犲姞
		paint.setColorFilter(new ColorMatrixColorFilter(mAllMatrix));//璁剧疆棰滆壊鍙樻崲鏁堟灉
		canvas.drawBitmap(tmpbitmap,0,0,paint);
		bitmap=tmpbmp.copy(Bitmap.Config.ARGB_8888, false);
		imageview.setImageBitmap(bitmap);	
	}
	@Override
	public void onStartTrackingTouch(SeekBar seekBar) {
		// TODO 鑷姩鐢熸垚鐨勬柟娉曞瓨鏍�
	}
	@Override
	public void onStopTrackingTouch(SeekBar seekBar) {
		// TODO 鑷姩鐢熸垚鐨勬柟娉曞瓨鏍�
	}
	private void showProgress()
	{
		Context context = this;
		mProgress = ProgressDialog.show(context, null, context.getResources().getString(R.string.handling));
		mProgress.show();
		Log.d("may", "show Progress");
	}
	
	/**
	 * 鍏抽棴杩涘害鏉�
	 */
	private void closeProgress()
	{
		if (null != mProgress)
		{
			mProgress.dismiss();
			mProgress = null;
		}
	}
	class MyThread extends Thread{
		private int flag;
		public MyThread(int doflag){
			super();
			flag=doflag;
		}
		public void run(){
			try{
			switch(flag){
			case GAUSE_NOISE:
				bitmap=ImageUtils.addGaussNoise(tmpbitmap);
				break;
			case AINPAINT:
				ImageUtils.setMaskImage(maskbitmap);
				ImageUtils.setSourceImage(bitmap);
				ImageUtilsJni.Inpainting();
				bitmap=ImageUtils.getSourceImage(ImageUtilsJni.getImageWidth(),ImageUtilsJni.getImageHeight());
				break;
			case ATIQU:
				ImageUtils.setSourceImage(bitmap);
				ImageUtilsJni.SalientRegionDetectorJni(5, 10);
				maskbitmap=ImageUtils.getSourceImage(ImageUtilsJni.getImageWidth(), ImageUtilsJni.getImageHeight());
				break;
			case ACOMPOSITE:
				ImageUtils.setMaskImage(maskbitmap);
				ImageUtils.setSourceImage(bitmap);
				ImageUtilsJni.reComposition();
				bitmap=ImageUtils.getSourceImage(ImageUtilsJni.getImageWidth(),ImageUtilsJni.getImageHeight());
				break;
			case ARESIZE:
				ImageUtils.setMaskImage(maskbitmap);
				ImageUtils.setSourceImage(bitmap);
				ImageUtilsJni.reCompositeAndResize();
				bitmap=ImageUtils.getSourceImage(ImageUtilsJni.getImageWidth(),ImageUtilsJni.getImageHeight());
				break;
			case GRABCUT:
				ImageUtilsJni.GrabCut();
				maskbitmap=ImageUtils.getSourceImage(ImageUtilsJni.getImageWidth(),ImageUtilsJni.getImageHeight());
				break;
			case GRABCOMPOSITE:				
			    ImageUtilsJni.GrabreComposite();			
				bitmap=ImageUtils.getSourceImage(ImageUtilsJni.getImageWidth(),ImageUtilsJni.getImageHeight());				
				break;
			case GRESIZE:				
			    ImageUtilsJni.GrabreCompositeAndResize();			
				bitmap=ImageUtils.getSourceImage(ImageUtilsJni.getImageWidth(),ImageUtilsJni.getImageHeight());				
				break;
			case VISUAL:
				ImageUtilsJni.VisualBalance();
				bitmap=ImageUtils.getSourceImage(ImageUtilsJni.getImageWidth(),ImageUtilsJni.getImageHeight());	
				bitmap = ImageSwitcher.keepBitmapAutoSize(bitmap, max_width, max_height);
				break;
			case MOVE_SMOOTH:
				bitmap=ImageUtils.move_SmoothImage(tmpbitmap, 10, 0, 0);
				break;
			case RESIZE_SMOOTH:
				bitmap=ImageUtils.move_SmoothImage(tmpbitmap, 0, 0.1f, 0);
				break;
			case SHUIWEN:
				bitmap=ImageUtils.WaterFilter(tmpbitmap);
				break;
			case HAHA:
				bitmap=ImageUtils.magicMiroor(tmpbitmap);				
				//bitmap=ImageUtils.getSourceImage(ImageUtilsJni.getImageWidth(),ImageUtilsJni.getImageHeight());
				break;
			case LUOXUAN:
				bitmap=ImageUtils.swirlImage(tmpbitmap);			
				break;
			case QIUMIAN:
				bitmap=ImageUtils.angulationBallImage(tmpbitmap);
				bitmap=ImageUtils.middle_SmoothImage(bitmap);
				break;
			case FISH:
				bitmap=filter.fisheyescale(tmpbitmap, 0, 0, tmpbitmap.getWidth(), tmpbitmap.getHeight(), 0.1f, true, true);
				break;
			case PINGJIA:
				imagescore=ImageUtilsJni.Evaluate();
				break;
			}
			Message msg = new Message();
			if(flag==PINGJIA)
				msg.what=PINGJIA;
			else if(flag==GRABCUT)
				msg.what=GRABCUT;
			else if(flag==ACOMPOSITE)
				msg.what=ACOMPOSITE;
			else if(flag==ARESIZE)
				msg.what=ARESIZE;
			else if(flag==ATIQU)
				msg.what=ATIQU;
			else if(flag==VISUAL)
				msg.what=VISUAL;
			else if(flag==AINPAINT)
				msg.what=AINPAINT;
			else if(flag==GRABCOMPOSITE)
				msg.what=GRABCOMPOSITE;
			else if(flag==GRESIZE)
				msg.what=GRESIZE;
			else
			msg.what = IMG_OK;
			mhHandler.sendMessage(msg);
			closeProgress();
			}
			catch(Exception e)
			{
				Toast.makeText(ImageDiyActivity.this,"出错", Toast.LENGTH_SHORT).show();
				e.printStackTrace();
				
			}
			finally
			{
				
			}
		}
	}
}
