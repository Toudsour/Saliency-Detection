#include <opencv.hpp>
#include "spams.h"
using namespace cv;
void SplitCannel(Mat& SouceImg,Matrix<double>& A,Matrix<double>& B,Matrix<double>& C)
{
	int Row=SouceImg.rows;
	int Col=SouceImg.cols;
	int Elem=Col*SouceImg.channels();
	int Num=Row*Col;
	double *Cannel=new double[Num];
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

void GetLearningDictionary(Matrix<double>& RGBDic,Matrix<double>& LabDic)
{
	double *Data;
	Data=new double[64*200];
	//double point,row,col
	RGBDic.setData(Data,64,200);
	RGBDic.setAleat();
	RGBDic.normalize();
	Data=new double [64*200];
	LabDic.setData(Data,64,200);
	LabDic.setAleat();
	LabDic.normalize();
}
template <typename T>
void lasso(const Matrix<T>& X,const Matrix<T> &D,SpMatrix<T>& spalpha,Sparam sparam)
{
	lasso(X,D,spalpha,sparam.L,sparam.constraint,\
		sparam.lambda2,sparam.mode,sparam.pos,sparam.pos,sparam.ols);
}

void ImageToCol(Matrix<double>& Input,int R,int C)
{
	int Row=Input.m();
	int Col=Input.n();
	int RowNum;
	int ColNum;
	RowNum=Row/R+(Row%R?1:0);
	ColNum=Col/C+(Col%C?1:0);
	int SourceIndex;
	int DestIndex;
	int DestRow;
	int DestCol;
	int BlockIndex;
	int BlockRow;
	int BlockCol;
	int InBlockIndex;
	int InBlockRow;
	int InBlockCol;
	int ElementNum=RowNum*ColNum*R*C;
	int OutRow=R*C;
	int OutCol=RowNum*ColNum;
	double *Data=new double[ElementNum];
	memset(Data,0,sizeof(double)*ElementNum);
	for(int i=0;i<Row;i++)
		for(int j=0;j<Col;j++)
		{
			SourceIndex=i*Col+j;
			BlockCol=j/C;
			BlockRow=i/R;
			BlockIndex=BlockRow*RowNum+BlockCol;
			DestCol=BlockIndex;
			InBlockCol=j%C;
			InBlockRow=i%R;
			InBlockIndex=InBlockCol*R+InBlockRow;
			DestRow=InBlockIndex;
			DestIndex=DestRow*OutCol+DestCol;
			Data[DestIndex]=Input[SourceIndex];
		}
	Input.clear();
	Input.setData(Data,OutRow,OutCol);
}

