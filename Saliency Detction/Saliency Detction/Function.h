#include <opencv.hpp>
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

