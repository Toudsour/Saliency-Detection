#include "HeadFile.h"
#include "PicPath.h"
char FileName[MAXPATHLEN];
Mat RGBImage;
Mat LABImage;
int main()
{
	for(int FileNode=1;FileNode<=MAXPIC;FileNode++)
	{
		//GetPic
		sprintf(FileName,"%s%d.jpg",IMGSAIM,FileNode);
		RGBImage=imread(FileName);
		//imshow("RGB",RGBImage);
		cvtColor(RGBImage,LABImage,COLOR_BGR2Lab);
		//imshow("LAB",LABImage);
		
		waitKey(0);
	}
	return 0;
}