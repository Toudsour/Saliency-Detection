#include "HeadFile.h"
#include "PicPath.h"
char FileName[MAXPATHLEN];
Mat RGBImage;
Mat LABImage;
void SplitCancle(Mat& SouceImg,Matrix<double>& A,Matrix<double>& B,Matrix<double>& C)
{
	int Row=SouceImg.rows;
	int Col=SouceImg.cols;
	int Elem=Col*SouceImg.channels();
	int Num=Row*Col;
	double *Cannel=new double[Num];
	cout<<Row<<" "<<Col<<endl;
	for(int Color=0;Color<3;Color++)
	{
		int Count=0;
		for(int i=0;i<Row;i++)
		{
			uchar* Data=SouceImg.ptr<uchar>(i);
			for(int j=0;j<Elem;j++)
			{
				if(j%3==Color)
					Cannel[Count++]=(double)Data[j]/256;
			}
		}
		switch(Color)
		{
			case 0:
				A.setData(Cannel,Row,Col);
			case 1:
				B.setData(Cannel,Row,Col);
			case 2:
				C.setData(Cannel,Row,Col);
		}
	}
}
int main()
{
	for(int FileNode=1;FileNode<=MAXPIC;FileNode++)
	{
		sprintf(FileName,"%s%d.jpg",IMGSAIM,FileNode);
		RGBImage=imread(FileName);
		cvtColor(RGBImage,LABImage,COLOR_BGR2Lab);
		imshow("Source",RGBImage);
		Matrix<double> RGB_R;
		Matrix<double> RGB_G;
		Matrix<double> RGB_B;
		SplitCancle(RGBImage,RGB_R,RGB_G,RGB_B);
		waitKey(0);
	}
	return 0;
}