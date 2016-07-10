LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
OpenCV_INSTALL_MODULES:=on
OPENCV_CAMERA_MODULES:=off
OPENCV_LIB_TYPE:=STATIC
ifeq ("$(wildcard $(OPENCV_MK_PATH))","")
include G:\Android\OpenCV-2.4.9-android-sdk\sdk\native\jni\OpenCV.mk
else  
include $(OPENCV_MK_PATH)  
endif 
LOCAL_MODULE    := ImageUtilsJni
LOCAL_SRC_FILES := ImageUtilsJni.c opencv.cpp inpainting.cpp ms.cpp msImageProcessor.cpp PictureHandler.cpp RAList.cpp rlist.cpp \
Saliency.cpp SalientRegionDetector.cpp Composition.cpp ImageHandler.cpp ImageSeamOperations.cpp grabcut.cpp ImageProcessor.cpp LineDetect.cpp
include $(BUILD_SHARED_LIBRARY)