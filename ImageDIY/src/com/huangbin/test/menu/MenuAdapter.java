package com.huangbin.test.menu;

import java.util.List;

import com.huangbin.test.R;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

public class MenuAdapter extends BaseAdapter {

	private final List<MenuInfo> list;
	private final LayoutInflater inflater;
	public MenuAdapter(Context context,List<MenuInfo> list){
		this.list=list;
		inflater=LayoutInflater.from(context);
	}
	
	public int getCount() {
		// TODO Auto-generated method stub
		return list.size();
	}

	public Object getItem(int arg0) {
		// TODO Auto-generated method stub
		return list.get(arg0);
	}

	public long getItemId(int arg0) {
		// TODO Auto-generated method stub
		return arg0;
	}

	public View getView(int arg0, View arg1, ViewGroup arg2) {
		// TODO Auto-generated method stub
		View view=arg1;
		MenuInfo mInfo=list.get(arg0);
		if (view==null) {
			if(mInfo.imgsrc==MenuUtils.NONE_IMAGE){
				view=inflater.inflate(R.layout.noimage_menu_item, null);
			}
			else{
				view=inflater.inflate(R.layout.menu_item, null);
				ImageView iView=(ImageView)view.findViewById(R.id.item_image);
				iView.setImageResource(mInfo.imgsrc);		
				//iView.setAlpha(90);			
			}
		}				
		TextView tView=(TextView)view.findViewById(R.id.item_text);	
		tView.setText(mInfo.title);		
		return view;
	}

}
