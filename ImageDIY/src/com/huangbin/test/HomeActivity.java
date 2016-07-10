package com.huangbin.test;

import java.io.File;
import com.huangbin.test.diyimage.ImageDiyActivity;
import android.app.Activity;
import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.text.TextUtils;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Toast;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.GridView;

public class HomeActivity extends Activity {
	
	private GridView gridmenu;
	private final int OPEN=0;
	private final int CAMERA=1;
	private final int EXIT=2;
	private  final int CHOSE_IMG=3;
	private final int CAMERA_IMG=4;
	String dirpath=Environment.getExternalStorageDirectory().getPath()+"/DCIM/images";
	String tmpfilepath=dirpath+"/tmp.jpg";
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO 鑷姩鐢熸垚鐨勬柟娉曞瓨鏍�
		super.onCreate(savedInstanceState);
		setContentView(R.layout.home);
		setTitle(R.string.home);
		File file = new File(dirpath);
		if(!file.exists())
			file.mkdirs();
		sdScan();//扫描SD卡，刷新媒体库
		gridmenu=(GridView)findViewById(R.id.gridmenu);
		gridmenu.setAdapter(new ImageAdaper(this));
		gridmenu.setOnItemClickListener(new OnItemClickListener() {

			@Override
			public void onItemClick(AdapterView<?> arg0, View arg1, int arg2,
					long arg3) {
				// TODO 鑷姩鐢熸垚鐨勬柟娉曞瓨鏍�
				Intent intent=new Intent();
				switch(arg2)
				{
				case OPEN:
					intent.setAction(Intent.ACTION_GET_CONTENT);
					intent.setType("image/*");
					startActivityForResult(intent, CHOSE_IMG);
					break;
				case CAMERA:
					File tmpfile = new File(tmpfilepath);
					if(tmpfile.exists())
						tmpfile.delete();
					intent.setAction(MediaStore.ACTION_IMAGE_CAPTURE);			
					intent.putExtra(MediaStore.EXTRA_OUTPUT, Uri.fromFile(tmpfile));
					startActivityForResult(intent, CAMERA_IMG);
					break;
				case EXIT:
					HomeActivity.this.finish();
				default:
					break;
				}
			}
			
		});
		
	}
	public void sdScan(){ 
		/*此处部分手机权限不够会报异常*/
        //sendBroadcast(new Intent(Intent.ACTION_MEDIA_MOUNTED, Uri.parse("file://"+ Environment.getExternalStorageDirectory())));     
} 
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO 鑷姩鐢熸垚鐨勬柟娉曞瓨鏍�
		if (resultCode == RESULT_OK && null != data)
		{
			switch (requestCode)
			{
			case CHOSE_IMG:
				Uri uri = data.getData();
				if (!TextUtils.isEmpty(uri.getAuthority()))
				{
					Cursor cursor = getContentResolver().query(uri, new String[]{ MediaStore.Images.Media.DATA }, null, null, null);
					if (null == cursor)
					{
						Toast.makeText(this, R.string.no_img+"null=cursor", Toast.LENGTH_SHORT).show();
						return;
					}
					cursor.moveToFirst();
					String path = cursor.getString(cursor.getColumnIndex(MediaStore.Images.Media.DATA));
					Intent intent = new Intent(this, ImageDiyActivity.class);
					intent.putExtra("path", path);
					startActivity(intent);
				}
				else
				{
					Intent intent = new Intent(this, HomeActivity.class);
					intent.putExtra("path", uri.getPath());
					startActivity(intent);
				}
				break;
			case CAMERA_IMG:		
				File tmpfile = new File(tmpfilepath);
				if(!tmpfile.exists()){
					Toast.makeText(HomeActivity.this, "拍照返回失败", Toast.LENGTH_SHORT).show();
					return;
				}
				Intent cameraintent = new Intent(this, ImageDiyActivity.class);
				cameraintent.putExtra("path", tmpfilepath);
				startActivity(cameraintent);
	         }
	
	     }
		else{
			//Toast.makeText(HomeActivity.this, "澶辫触", Toast.LENGTH_SHORT).show();
			if(requestCode==CAMERA_IMG){
				File tmpfile = new File(tmpfilepath);
				if(!tmpfile.exists()){
					Toast.makeText(HomeActivity.this, "拍照返回失败", Toast.LENGTH_SHORT).show();
					return;
				}
				Intent intent = new Intent(this, ImageDiyActivity.class);
				intent.putExtra("path", tmpfilepath);
				startActivity(intent);
			}
			else{
				
			}
		}
	}
}
	

