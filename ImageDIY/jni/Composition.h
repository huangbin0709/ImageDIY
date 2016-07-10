#include "cv.h"
#include "highgui.h"
class Composition{
public:
	IplImage *root;
	IplImage *mainarea;
	IplImage *inpaint;
	IplImage *resize;
	void setRootImage(IplImage *);
	void setResizeImage(IplImage*);
	void setInpaintImage(IplImage *);
	void setMainareaImage(IplImage *);
	void reComposition(int flag);
	void reCompositionAndResize(int centerx,int centery);
	bool isBlack(uchar *);
	bool isGreen(uchar *);
	int Min(int a,int b);
	~Composition();
	Composition();

};