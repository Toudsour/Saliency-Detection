#include <opencv.hpp>
#include <cstdio>
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


void GetLearningDictionary(Matrix<double>* RGBDic,Matrix<double>* LABDic)
{
	FILE * DictionFile;
	DictionFile=NULL;
	int Row,Col;
	DictionFile=fopen("DSRGB.dat","r");
	if(DictionFile!=NULL)
	{
		fscanf(DictionFile,"%d %d",&Row,&Col);
		for(int Can=0;Can<3;Can++)
		{
			double *Data=new double[Row*Col];
			for(int i=0;i<Row;i++)
				for(int j=0;j<Col;j++)
					fscanf(DictionFile,"%lf",&Data[i*Col+j]);
			RGBDic[Can].setData(Data,Row,Col);
		}
		fclose(DictionFile);
	}
	else 
		printf("NO DSRGB.dat!\n");
	DictionFile=fopen("DSLAB.dat","r");
	if(DictionFile!=NULL)
	{
		fscanf(DictionFile,"%d %d",&Row,&Col);
		for(int Can=0;Can<3;Can++)
		{
			double *Data=new double[Row*Col]; 
			for(int i=0;i<Row;i++)
				for(int j=0;j<Col;j++)
					fscanf(DictionFile,"%lf",&Data[i*Col+j]);
			LABDic[Can].setData(Data,Row,Col);
		}
		fclose(DictionFile);
	}
	else 
		printf("NO DSLAB.dat!\n");
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


//Attention: This function is correct only when it's called inverse to ImageToCol
void ColToImage(Matrix<double>& Input,int R,int C,int OutRow,int OutCol)
{
	int Row=Input.m();
	int Col=Input.n();
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
	int ColBlcokNum=OutCol/C+OutCol%C?1:0;
	int ColLen=ColBlcokNum*C;
	double *Temp=new double[Row*Col];
	double *Data=new double[OutRow*OutCol];
	for(int i=0;i<Row;i++)
		for(int j=0;j<Col;j++)
		{
			SourceIndex=i*Col+j;
			BlockIndex=j;
			BlockCol=j%ColBlcokNum;
			BlockRow=j/ColBlcokNum;
			InBlockIndex=i;
			InBlockCol=i%C;
			InBlockRow=i/C;
			DestRow=BlockRow*R+InBlockRow;
			DestCol=BlockCol*C+InBlockCol;
			DestIndex=DestRow*ColLen+DestCol;
			Temp[DestIndex]=Input[SourceIndex];
		}
		//Cut
		for(int i=0;i<OutRow;i++)
			for(int j=0;j<OutCol;j++)
			{
				SourceIndex=i*ColLen+j;
				DestIndex=i*OutCol+j;
				Data[DestIndex]=Temp[SourceIndex];
			}
		delete Temp;
		Input.clear();
		Input.setData(Data,OutRow,OutCol);
}
template<class T>
T*** GetThridMatrix(int Row,int Col,int Size,T Type)
{
	T*** Output;
	Output=new T**[Row];
	for(int i=0;i<Row;i++)
		Output[i]=new T*[Col];
	for(int i=0;i<Row;i++)
		for(int j=0;j<Col;j++)
			Output[i][j]=new T[Size];
	return Output;
}



template<class T>
void DeleteThridMatrix(int Size,int Row,int Col,T*** P)
{
	for(int i=0;i<Row;i++)
		for(int j=0;j<Col;j++)
			delete(P[i][j]);
	for(int i=0;i<Row;i++)
		delete(P[i]);
	delete(P);
}



double*** Reshape(Matrix<double>& Input,int Size,int Row,int Col)
{
	const double *Data=Input.X();
	double ***Output;
	Output=GetThridMatrix(Row,Col,Size,1.0);
	int InputRow=Input.m();
	int InputCol=Input.n();
	int PathCol=Col;
	int DestRow;
	int DestCol;
	for(int i=0;i<InputRow;i++)
		for(int j=0;j<InputCol;j++)
		{
			DestRow=j/Col;
			DestCol=j%Col;
			Output[DestRow][DestCol][i]=Data[i*InputCol+j];
		}
	return Output;
}



double Average(double *A,double *B,int Len)
{
	double Sum=0;
	for(int i=0;i<Len;i++)
		Sum+=abs(A[i]-B[i]);
	Sum/=Len;
	return Sum;
}

double FindMatrixMax(const Matrix<double>& Input)
{
	double Max=DBL_MIN;
	int Count=Input.m()*Input.n();
	for(int i=0;i<Count;i++)
		Max=max(Input[i],Max);
	return Max;
}

double FindMatrixMin(const Matrix<double>& Input)
{
	double Min=DBL_MAX;
	int Count=Input.m()*Input.n();
	for(int i=0;i<Count;i++)
		Min=min(Input[i],Min);
	return Min;
}

void MatrixSubConst(Matrix<double>& Input,double Const)
{
	int Count=Input.m()*Input.n();
	for(int i=0;i<Count;i++)
		Input[i]-=Const;
}
void MatrixDivConst(Matrix<double>& Input,double Const)
{
	int Count=Input.m()*Input.n();
	for(int i=0;i<Count;i++)
		Input[i]/=Const;
}
 