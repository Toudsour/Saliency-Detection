#include "HeadFile.h"
char FileName[MAXPATHLEN];
Matrix<double> LabDic;
Matrix<double> RGBDic;
int main()
{
	//Get Learning Dictionary 
	GetLearningDictionary(RGBDic,LabDic);
	

	//Do Saliency Detection
	for(int FileNode=1;FileNode<=MAXPIC;FileNode++)
	{
		
		//GetPic
		Mat SourceImage;
		Mat RGBImage;
		Mat LABImage;
		sprintf(FileName,"%s%d.jpg",IMGSAIM,FileNode);  //get filename
		SourceImage=imread(FileName);						//load picture from file
		resize(SourceImage,RGBImage,Size(PICSIZE,PICSIZE),0,0,INTER_LINEAR);
		cvtColor(RGBImage,LABImage,COLOR_BGR2Lab);		//translate RGB into Lab
		imshow("Source",RGBImage);


		Matrix<double> RGBCannels[3];
		Matrix<double> LabCanenls[3];
		SpMatrix<double> SPRGB[3];
		SpMatrix<double> SPLab[3];
		Sparam sparam;
		for(int ColorSpace=1;ColorSpace<=2;ColorSpace++)
		{
			Matrix<double> *Cannels;
			Matrix<double> *Dic;
			SpMatrix<double> *Sparsecode;
			//Choose Image Color Space
			if(ColorSpace==1)
			{
				Cannels=RGBCannels;
				Dic=&RGBDic;
				Sparsecode=SPRGB;
			}
			else 
			{
				Cannels=LabCanenls;
				Dic=&LabDic;
				Sparsecode=SPLab;
			}
			

			//Split picture into different cannels and translate Mat into Matrix<T>
			SplitCannel(RGBImage,Cannels[0],Cannels[1],Cannels[2]);	//Split Picture into R,G,B cannel


			//Image into Row
			for(int i=0;i<3;i++)
				ImageToCol(Cannels[i],PATCHSIZE,PATCHSIZE);


			//Represent picture by sparse coding
			for(int Can=0;Can<3;Can++)
				lasso(Cannels[Can],*Dic,Sparsecode[Can],sparam);	
				
		}
		waitKey(0);
	}
	return 0;
}