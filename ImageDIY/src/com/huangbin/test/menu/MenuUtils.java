package com.huangbin.test.menu;

import java.util.ArrayList;
import java.util.List;

import com.huangbin.test.R;

public class MenuUtils {
	public static final int MENU_TEXT=1;
	public static final int MENU_DODO=2;
	public static final int MENU_HUIDU=3;
	public static final int MENU_XUANRUAN=4;
	public static final int MENU_LVJING=5;
	public static final int MENU_BIANJI=6;
	public static final int MENU_BIANKUANG=7;
	public static final int MENU_ZUHE=8;
	public static final int MENU_LEFT=9;
	public static final int MENU_RIGHT=10;
	public static final int MENU_LR=11;	
	public static final int MENU_UP=12;
	public static final int MENU_SHUIYIN=13;
	public static final int MENU_HUAIJIU=14;
	public static final int MENU_MOHU=15;
	public static final int MENU_RUIHUA=16;
	public static final int MENU_FUDIAO=17;
	public static final int MENU_DIPIAN=18;
	public static final int MENU_GUANGZHAO=19;
	public static final int MENU_GUANGYUN=20;
	public static final int MENU_TIAOWEN=21;
	public static final int MENU_TIAOSE=22;
	public static final int MENU_YUANJIAO=23;
	public static final int MENU_EXIT=24;
	public static final int MENU_SMAL=25;
	public static final int MENU_BIG=26;
	public static final int MENU_TEXTSIZE=27;
	public static final int MENU_TEXTCOLOR=28;
	public static final int MENU_OK=29;
	public static final int MENU_CANCEL=30;
	public static final int MENU_CUXI=31;
	public static final int MENU_ERASER=32;
	public static final int MENU_PAINTCOLOR=33;
	public static final int NONE_IMAGE=0;
	public static final int MENU_RESIZE=34;
	public static final int MENU_XUANZHUAN=35;
	public static final int MENU_DAOZHI=36;
	public static final int MENU_MAIN=37;
	public static final int MENU_TEXTCANCEL=38;
	public static final int MENU_DODOCANCEL=39;
	public static final int MENU_MIROOR=40;
	public static final int MENU_SURSAVE=41;
	public static final int MENU_TEXTOK=42;
	public static final int MENU_CAIJIAN=43;
	public static final int MENU_BOXING=44;
	public static final int MENU_HAHA=45;
	public static final int MENU_LUOXUAN=46;
	public static final int MENU_QIUMIAN=47;
	public static final int MENU_ZHIFANGTU=48;
	public static final int MENU_SHUIWEN=49;
	public static final int MENU_BOLI=50;
	public static final int MENU_PENGZHANG=51;
	public static final int MENU_FUSHI=52;
	public static final int MENU_LIGHT=53;
	public static final int MENU_FANGHUABI=54;
	public static final int MENU_MOVESMOOTH=55;
	public static final int MENU_RESIZESMOOTH=56;
	public static final int MENU_NOISE=57;
	public static final int MENU_GAUSENOISE=58;
	public static final int MENU_PIPERNOISE=59;
	public static final int MENU_DAOYING=60;
	public static final int MENU_MASAIKE=61;
	public static final int MENU_SUMIAO=62;
	public static final int MENU_BANTIAOSE=63;
	public static final int MENU_LOMO=64;
	public static final int MENU_LVBO=65;
	public static final int MENU_AVELVBO=66;
	public static final int MENU_MIDDLELVBO=67;
	public static final int MENU_SHUANGBIANLVBO=68;
	public static final int MENU_MEAN=69;
	public static final int MENU_BIANYUANRUIHUA=70;
	public static final int MENU_TIDURUIHUA=71;
	public static final int MENU_LAPU=72;
	public static final int MENU_GRAYLINE=73;
	public static final int MENU_GRAYTIDU=74;
	public static final int MENU_GRAYWINDOW=75;
	public static final int MENU_GRAYFAZHI=76;
	public static final int MENU_BIANYUAN=77;
	public static final int MENU_TUOYUAN=78;
	public static final int MENU_LINE=79;
	public static final int MENU_TIDU=80;
	public static final int MENU_RADIAL=81;
	public static final int MENU_GAUSESMOOTH=87;
	public static final int MENU_MIHONG=82;
	public static final int MENU_ONE=83;
	public static final int MENU_TWO=84;
	public static final int MENU_THREE=85;
	public static final int MENU_FOUR=86;
	//public static final int MENU_POISSON=87;
	public static final int MENU_GRAYZHIFANGTU=89;
	public static final int MENU_GRAYDIPIAN=90;
	public static final int MENU_EXPLOR=91;
	public static final int MENU_YOUHUA=94;
	public static final int MENU_BLEND=95;
	public static final int MENU_BLEND1=96;
	public static final int MENU_BLEND2=97;
	public static final int MENU_BLEND3=98;
	public static final int MENU_BLEND4=99;
	public static final int MENU_BLEND5=100;
	public static final int MENU_BLEND6=101;
	public static final int MENU_CHANMIX=102;
	public static final int MENU_MASK=103;
	public static final int MENU_SOLAR=104;
	public static final int MENU_POSTER=105;
	public static final int MENU_BOX=106;
	public static final int MENU_BUMP=107;
	public static final int MENU_WATER=108;
	public static final int MENU_COLORCHANGE=109;
	public static final int MENU_SUNZHAO=110;
	public static final int MENU_SUNYUN=111;
	public static final int MENU_COLOR1=112;
	public static final int MENU_COLOR2=113;
	public static final int MENU_COLOR3=114;
	public static final int MENU_COLOR4=115;
	public static final int MENU_NIUQU=116;
	public static final int MENU_PAO=117;
	public static final int MENU_LIUXING=118;
	public static final int MENU_FISH=119;
	public static final int MENU_MEIXUE=120;
	public static final int MENU_GOUTU=122;
	public static final int MENU_BIANXING=123;
	public static final int MENU_SHOUSHEN=124;
	public static final int MENU_AOTU=125;
	public static final int MENU_AO=126;
	public static final int MENU_TU=127;
	public static final int MENU_AUTUCANCEL=128;
	public static final int MENU_QULV=129;
	public static final int MENU_AOTUCHOSE=130;
	public static final int MENU_GRAB=131;
	public static final int MENU_GRABCHOSE=132;
	public static final int MENU_FORE=133;
	public static final int MENU_BACK=134;
	public static final int MENU_GRABCUT=135;
	public static final int MENU_GCOMPOSITE=136;
	public static final int MENU_GVISUAL=137;
	public static final int MENU_GRESIZE=138;
	public static final int MENU_AUTO=139;
	public static final int MENU_ATIQU=140;
	public static final int MENU_ACOLOR=141;
	public static final int MENU_AINPAINT=142;
	public static final int MENU_ACOMPOSITE=143;
	public static final int MENU_ARESIZE=144;
	public static final int MENU_AVISUAL=145;
	public static final int MENU_VISUAL=146;
	public static final int MENU_EYESCALE=147;
	public static final int MENU_AGREEN=148;
	public static final int MENU_ARED=149;
	public static final int MENU_QUBAN=150;
	public static final int MENU_MEIBAI=151;
	public static final int MENU_QUZHOU=152;
	/**
	 * 
	 * @return
	 */
	public static List<MenuInfo> getMenuList(){	
		    List<MenuInfo> list=new ArrayList<MenuInfo>();
		    list.add(new MenuInfo(MENU_MEIXUE,"构图",R.drawable.meixue));
		    list.add(new MenuInfo(MENU_TEXT,"文字",R.drawable.text));
		    list.add(new MenuInfo(MENU_DODO,"涂鸦",R.drawable.texiao));
		    list.add(new MenuInfo(MENU_LVJING,"滤镜",R.drawable.lvjing));
			list.add(new MenuInfo(MENU_BIANJI,"编辑",R.drawable.caijian));
			list.add(new MenuInfo(MENU_BIANKUANG,"边框",R.drawable.biankuang));
			list.add(new MenuInfo(MENU_TIAOSE,"调色",R.drawable.tiaose));
			//list.add(new MenuInfo(MENU_XUANRUAN,"渲染",R.drawable.xuanran));
			list.add(new MenuInfo(MENU_BIANXING,"人脸",R.drawable.xuanran));
		    return list;
	}
	public static List<MenuInfo> getBianjiMenuList(){	
	    List<MenuInfo> list=new ArrayList<MenuInfo>();
	    list.add(new MenuInfo(MENU_RESIZE,"缩放",R.drawable.thumb));
	    list.add(new MenuInfo(MENU_XUANZHUAN,"旋转",R.drawable.xuanzhuan));
	    list.add(new MenuInfo(MENU_MIROOR,"镜像",R.drawable.fanzhuan));
	    list.add(new MenuInfo(MENU_CAIJIAN,"裁剪",R.drawable.caijian));
	    return list;
}
	public static List<MenuInfo> getBianXingMenuList(){	
	    List<MenuInfo> list=new ArrayList<MenuInfo>();
	    list.add(new MenuInfo(MENU_AOTU,"凹凸",NONE_IMAGE));
	    list.add(new MenuInfo(MENU_SHOUSHEN,"瘦脸",NONE_IMAGE));
	    list.add(new MenuInfo(MENU_QUBAN,"去斑",NONE_IMAGE));
	    list.add(new MenuInfo(MENU_MEIBAI,"美白",NONE_IMAGE));
	    list.add(new MenuInfo(MENU_QUZHOU,"去皱",NONE_IMAGE));
	    //list.add(new MenuInfo(MENU_EYESCALE,"眼睛放大",NONE_IMAGE));
	    return list;
}
	public static List<MenuInfo> getAoTuMenuList(){	
	    List<MenuInfo> list=new ArrayList<MenuInfo>();
	    list.add(new MenuInfo(MENU_AOTUCHOSE,"选择",NONE_IMAGE));
	    list.add(new MenuInfo(MENU_TU,"凸起",NONE_IMAGE));
	    list.add(new MenuInfo(MENU_AO,"挤压",NONE_IMAGE));
	    list.add(new MenuInfo(MENU_QULV,"曲率",NONE_IMAGE));
	    list.add(new MenuInfo(MENU_AUTUCANCEL,"取消",NONE_IMAGE));
	    return list;
}
	public static List<MenuInfo> getGrabMenuList(){	
	    List<MenuInfo> list=new ArrayList<MenuInfo>();
	    list.add(new MenuInfo(MENU_GRABCHOSE,"选择",NONE_IMAGE));
	    list.add(new MenuInfo(MENU_FORE,"主体",NONE_IMAGE));
	    list.add(new MenuInfo(MENU_BACK,"背景",NONE_IMAGE));
	    list.add(new MenuInfo(MENU_GRABCUT,"提取",NONE_IMAGE));
	    list.add(new MenuInfo(MENU_GCOMPOSITE,"重构",NONE_IMAGE));
	    list.add(new MenuInfo(MENU_GRESIZE,"resize",NONE_IMAGE));
	  //  list.add(new MenuInfo(MENU_GVISUAL,"visual",NONE_IMAGE));
	    return list;
}
	public static List<MenuInfo> getAutoMenuList(){	
	    List<MenuInfo> list=new ArrayList<MenuInfo>();
	    //list.add(new MenuInfo(MENU_ACOLOR,"颜色",NONE_IMAGE));
	    list.add(new MenuInfo(MENU_AINPAINT,"修复",NONE_IMAGE));
	    list.add(new MenuInfo(MENU_ATIQU,"提取",NONE_IMAGE));
	    list.add(new MenuInfo(MENU_ACOMPOSITE,"重构",NONE_IMAGE));
	    list.add(new MenuInfo(MENU_ARESIZE,"resize",NONE_IMAGE));
	   // list.add(new MenuInfo(MENU_AVISUAL,"visual",NONE_IMAGE));
	    return list;
}
	public static List<MenuInfo> getColorChangeMenuList(){	
	    List<MenuInfo> list=new ArrayList<MenuInfo>();
	    list.add(new MenuInfo(MENU_COLOR1,"红蓝",NONE_IMAGE));
	    list.add(new MenuInfo(MENU_COLOR2,"红黄",NONE_IMAGE));
	    list.add(new MenuInfo(MENU_COLOR3,"红紫",NONE_IMAGE));
	    list.add(new MenuInfo(MENU_COLOR4,"黄紫",NONE_IMAGE));
	    return list;
}
	public static List<MenuInfo> getTeXiaoMenuList(){
		List<MenuInfo> list=new ArrayList<MenuInfo>();
		list.add(new MenuInfo(MENU_MOHU,"模糊",NONE_IMAGE));
		list.add(new MenuInfo(MENU_NOISE,"噪声",NONE_IMAGE));
		list.add(new MenuInfo(MENU_LVBO,"滤波",NONE_IMAGE));
		list.add(new MenuInfo(MENU_RUIHUA,"锐化",NONE_IMAGE));
		list.add(new MenuInfo(MENU_NIUQU,"扭曲",NONE_IMAGE));
		list.add(new MenuInfo(MENU_FISH,"鱼眼",NONE_IMAGE));
		list.add(new MenuInfo(MENU_ZHIFANGTU,"直方图",NONE_IMAGE));
		list.add(new MenuInfo(MENU_COLORCHANGE,"颜色转换",NONE_IMAGE));
		list.add(new MenuInfo(MENU_LOMO,"LOMO",NONE_IMAGE));
		list.add(new MenuInfo(MENU_MIHONG,"霓虹",NONE_IMAGE));
		list.add(new MenuInfo(MENU_SUMIAO,"素描",NONE_IMAGE));
		list.add(new MenuInfo(MENU_FUDIAO,"浮雕",NONE_IMAGE));
		list.add(new MenuInfo(MENU_DIPIAN,"底片",NONE_IMAGE));
		list.add(new MenuInfo(MENU_SUNZHAO,"光照",NONE_IMAGE));
		list.add(new MenuInfo(MENU_SUNYUN,"光晕",NONE_IMAGE));
		list.add(new MenuInfo(MENU_HUAIJIU,"怀旧",NONE_IMAGE));
		list.add(new MenuInfo(MENU_MASAIKE,"马赛克",NONE_IMAGE));
		list.add(new MenuInfo(MENU_EXPLOR,"曝光",NONE_IMAGE));
		list.add(new MenuInfo(MENU_YOUHUA,"油画",NONE_IMAGE));
		list.add(new MenuInfo(MENU_BLEND,"叠加",NONE_IMAGE));
		list.add(new MenuInfo(MENU_SOLAR,"日晒",NONE_IMAGE));
		list.add(new MenuInfo(MENU_PAO,"气泡",NONE_IMAGE));
		list.add(new MenuInfo(MENU_LIUXING,"流星",NONE_IMAGE));
		list.add(new MenuInfo(MENU_CHANMIX,"ColorMix",NONE_IMAGE));
		list.add(new MenuInfo(MENU_DAOYING,"倒影",NONE_IMAGE));
		list.add(new MenuInfo(MENU_HUIDU,"灰度",NONE_IMAGE));
		list.add(new MenuInfo(MENU_POSTER,"Posterize",NONE_IMAGE));
		list.add(new MenuInfo(MENU_BUMP,"Bump",NONE_IMAGE));
		return list;
	}
	public static List<MenuInfo> getTextMenuList(){
		List<MenuInfo> list=new ArrayList<MenuInfo>();
		list.add(new MenuInfo(MENU_TEXTSIZE,"大小",NONE_IMAGE));
		list.add(new MenuInfo(MENU_TEXTCOLOR,"颜色",NONE_IMAGE));
		list.add(new MenuInfo(MENU_TEXTCANCEL,"清除",NONE_IMAGE));
		list.add(new MenuInfo(MENU_TEXTOK,"确定",NONE_IMAGE));
		list.add(new MenuInfo(MENU_SURSAVE,"保存",NONE_IMAGE));
		return list;
	}
	public static List<MenuInfo> getAngulationMenuList(){
		List<MenuInfo> list=new ArrayList<MenuInfo>();
		list.add(new MenuInfo(MENU_SHUIWEN,"水纹",NONE_IMAGE));
		list.add(new MenuInfo(MENU_BOXING,"波形",NONE_IMAGE));
		list.add(new MenuInfo(MENU_HAHA,"哈哈镜",NONE_IMAGE));
		list.add(new MenuInfo(MENU_LUOXUAN,"螺旋",NONE_IMAGE));
		list.add(new MenuInfo(MENU_QIUMIAN,"球面",NONE_IMAGE));
		return list;
	}
	public static List<MenuInfo> getBlendingMenuList(){
		List<MenuInfo> list=new ArrayList<MenuInfo>();
		list.add(new MenuInfo(MENU_BLEND1,"Multiply",NONE_IMAGE));
		list.add(new MenuInfo(MENU_BLEND2,"Dissolve",NONE_IMAGE));
		list.add(new MenuInfo(MENU_BLEND3,"Screen",NONE_IMAGE));
		list.add(new MenuInfo(MENU_BLEND4,"Overlay",NONE_IMAGE));
		list.add(new MenuInfo(MENU_BLEND5,"SoftLight",NONE_IMAGE));
		list.add(new MenuInfo(MENU_BLEND6,"HardLight",NONE_IMAGE));
		return list;
	}
	public static List<MenuInfo> getDodoMenuList(){
		List<MenuInfo> list=new ArrayList<MenuInfo>();
		list.add(new MenuInfo(MENU_CUXI,"粗细",NONE_IMAGE));
		list.add(new MenuInfo(MENU_PAINTCOLOR,"颜色",NONE_IMAGE));
		list.add(new MenuInfo(MENU_ERASER,"橡皮擦",NONE_IMAGE));
		list.add(new MenuInfo(MENU_SURSAVE,"保存",NONE_IMAGE));
		list.add(new MenuInfo(MENU_DODOCANCEL,"清屏",NONE_IMAGE));
		//list.add(new MenuInfo(MENU_EXIT,"杩斿洖",NONE_IMAGE));
		return list;
	}
	public static List<MenuInfo> getHuiDuMenuList(){
		List<MenuInfo> list=new ArrayList<MenuInfo>();
		list.add(new MenuInfo(MENU_BIANYUAN,"边缘检测",NONE_IMAGE));
		list.add(new MenuInfo(MENU_GRAYZHIFANGTU,"直方图",NONE_IMAGE));
		list.add(new MenuInfo(MENU_GRAYFAZHI,"阀值",NONE_IMAGE));
		list.add(new MenuInfo(MENU_GRAYWINDOW,"窗口",NONE_IMAGE));
		list.add(new MenuInfo(MENU_GRAYLINE,"线性",NONE_IMAGE));
		list.add(new MenuInfo(MENU_GRAYDIPIAN,"底片",NONE_IMAGE));
		//list.add(new MenuInfo(MENU_GRAYTIDU,"姊害",NONE_IMAGE));
		return list;
	}
	public static List<MenuInfo> getaddFrameMenuList(){
		List<MenuInfo> list=new ArrayList<MenuInfo>();
		list.add(new MenuInfo(MENU_ONE,"相框1",R.drawable.frame_1));
		list.add(new MenuInfo(MENU_TWO,"相框2",R.drawable.frame2));
		list.add(new MenuInfo(MENU_THREE,"相框3",R.drawable.frame3));
		list.add(new MenuInfo(MENU_FOUR,"相框4",R.drawable.frame4));
		return list;
	}
	public static List<MenuInfo> getXuanRanMenuList(){
		List<MenuInfo> list=new ArrayList<MenuInfo>();
		list.add(new MenuInfo(MENU_LINE,"线性",NONE_IMAGE));
		list.add(new MenuInfo(MENU_TIDU,"梯度",NONE_IMAGE));
		list.add(new MenuInfo(MENU_RADIAL,"环形",NONE_IMAGE));
		list.add(new MenuInfo(MENU_TUOYUAN,"椭圆",NONE_IMAGE));
		return list;
	}
	public static List<MenuInfo> getRuiHuaMenuList(){
		List<MenuInfo> list=new ArrayList<MenuInfo>();
		list.add(new MenuInfo(MENU_BIANYUANRUIHUA,"边缘",NONE_IMAGE));
		list.add(new MenuInfo(MENU_TIDURUIHUA,"梯度",NONE_IMAGE));
		list.add(new MenuInfo(MENU_LAPU,"拉普拉斯",NONE_IMAGE));
		return list;
	}
	public static List<MenuInfo> getNoiseMenuList(){
		List<MenuInfo> list=new ArrayList<MenuInfo>();
		list.add(new MenuInfo(MENU_PIPERNOISE,"椒盐噪声",NONE_IMAGE));
		list.add(new MenuInfo(MENU_GAUSENOISE,"高斯噪声",NONE_IMAGE));
		//list.add(new MenuInfo(MENU_POISSON,"娉婃澗鍣０",NONE_IMAGE));
		return list;
	}
	public static List<MenuInfo> getMoHuMenuList(){
		List<MenuInfo> list=new ArrayList<MenuInfo>();
		list.add(new MenuInfo(MENU_BOX,"Box Blur",NONE_IMAGE));
		list.add(new MenuInfo(MENU_MOVESMOOTH,"移动模糊",NONE_IMAGE));
		list.add(new MenuInfo(MENU_RESIZESMOOTH,"缩放模糊",NONE_IMAGE));
		return list;
	}
	public static List<MenuInfo> getLvBoMenuList(){
		List<MenuInfo> list=new ArrayList<MenuInfo>();
		list.add(new MenuInfo(MENU_GAUSESMOOTH,"高斯",NONE_IMAGE));
		list.add(new MenuInfo(MENU_AVELVBO,"均值",NONE_IMAGE));
		list.add(new MenuInfo(MENU_MIDDLELVBO,"中值",NONE_IMAGE));
		//list.add(new MenuInfo(MENU_SHUANGBIANLVBO,"鍙岃竟",NONE_IMAGE));
		//list.add(new MenuInfo(MENU_MEAN,"Mean Shift",NONE_IMAGE));
		return list;
	}
	public static List<MenuInfo> getXuanZhuanMenuList(){	
	    List<MenuInfo> list=new ArrayList<MenuInfo>();
	    list.add(new MenuInfo(MENU_LEFT,"向左",R.drawable.rotate_left));
	    list.add(new MenuInfo(MENU_RIGHT,"向右",R.drawable.rotate_right));
	    return list;
	}
	public static List<MenuInfo> getMeiXueMenuList(){	
	    List<MenuInfo> list=new ArrayList<MenuInfo>();
	    list.add(new MenuInfo(MENU_AUTO,"Auto",NONE_IMAGE));
	    list.add(new MenuInfo(MENU_GRAB,"Grab",NONE_IMAGE));
	    list.add(new MenuInfo(MENU_VISUAL,"Visual",NONE_IMAGE));
	    return list;
	}
	public static List<MenuInfo> getFanZhuanMenuList(){	
	    List<MenuInfo> list=new ArrayList<MenuInfo>();
	    list.add(new MenuInfo(MENU_LR,"左右",R.drawable.left_right));
	    list.add(new MenuInfo(MENU_UP,"上下",R.drawable.up_down));
	    return list;
	}
	public static List<MenuInfo> getAColorMenuList(){	
	    List<MenuInfo> list=new ArrayList<MenuInfo>();
	    list.add(new MenuInfo(MENU_AGREEN,"绿色",NONE_IMAGE));
	    list.add(new MenuInfo(MENU_ARED,"红色",NONE_IMAGE));
	    return list;
	}
}
