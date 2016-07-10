package com.huangbin.test;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

public class ImageAdaper extends BaseAdapter {
private LayoutInflater inflater;
private Integer[] imageIds={
		R.drawable.open,
		R.drawable.camera,
		R.drawable.exit,
};
private String[] textIds={
        "选择",
        "拍照",
        "退出",
};
	public ImageAdaper(Context c)
	{
		inflater=LayoutInflater.from(c);
	}
	@Override
	public int getCount() {
		// TODO 鑷姩鐢熸垚鐨勬柟娉曞瓨鏍�
		return imageIds.length;
	}

	@Override
	public Object getItem(int position) {
		// TODO 鑷姩鐢熸垚鐨勬柟娉曞瓨鏍�
		return position;
	}

	@Override
	public long getItemId(int position) {
		// TODO 鑷姩鐢熸垚鐨勬柟娉曞瓨鏍�
		return position;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		// TODO 鑷姩鐢熸垚鐨勬柟娉曞瓨鏍�
		View view=convertView;
		view=inflater.inflate(R.layout.home_menu_item, null);
		ImageView iView=(ImageView)view.findViewById(R.id.item_image);
		iView.setImageResource(imageIds[position]);
		TextView tView=(TextView)view.findViewById(R.id.item_text);	
		tView.setText(textIds[position]);	
		return view;
	}

}
