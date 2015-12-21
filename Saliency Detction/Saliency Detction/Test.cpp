#include "HeadFile.h"
#include "PicPath.h"
char FileName[MAXPATHLEN];

int main()
{
	//GetPic 
	for(int FileNode=1;FileNode<=MAXPIC;FileNode++)
	{
		sprintf(FileName,"%s%d.jpg",IMGSAIM,FileNode);
		printf("%s\n",FileName);
		Mat srcImage=imread(FileName);
		imshow("Pic",srcImage);
	}
	return 0;
}