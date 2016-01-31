#include "HeadFile.h"
char FileName[MAXPATHLEN];
Matrix<double> LabDic[3];
Matrix<double> RGBDic[3];
int main()
{
	//Get Learning Dictionary 
	GetLearningDictionary(RGBDic,LabDic);
	//LabDic[0].print("OK?");
	

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
		Matrix<double> RGBSaliency;
		Matrix<double> LabSaliency;
		Matrix<double> FinalSaliency;
		Sparam sparam;
		for(int ColorSpace=1;ColorSpace<=2;ColorSpace++)
		{
			Matrix<double> *Cannels;
			Matrix<double> *Dic;
			SpMatrix<double> *Sparsecode;
			Matrix<double> * Saliency;
			//Choose Image Color Space
			if(ColorSpace==1)
			{
				Cannels=RGBCannels;
				Dic=RGBDic;
				Sparsecode=SPRGB;
				Saliency=&RGBSaliency;
			}
			else 
			{
				Cannels=LabCanenls;
				Dic=LabDic;
				Sparsecode=SPLab;
				Saliency=&LabSaliency;
			}
			

			//Split picture into different cannels and transform Mat into Matrix<T>
			SplitCannel(RGBImage,Cannels[0],Cannels[1],Cannels[2]);	//Split Picture into R,G,B cannel


			
			for(int Can=0;Can<3;Can++)
			{
				//Image into Row
				ImageToCol(Cannels[Can],PATCHSIZE,PATCHSIZE);


				//Represent picture by sparse coding 
				Matrix<double> Result;
				lasso(Cannels[Can],Dic[Can],Sparsecode[Can],sparam);
				Dic->mult(Sparsecode[Can],Result);
				

				//Result into Image
				ColToImage(Result,PATCHSIZE,PATCHSIZE,PICSIZE,PICSIZE);
			}


			//Get cannnel's local saliency
			for(int Can=0;Can<3;Can++)
			{
				LocalSailency(Sparsecode[Can],Cannels[Can]);
				Normalization(Cannels[Can]);
			}	


			//Composite cannels' saliency into space's saliency
			Saliency->resize(PATCHLEN,PATCHLEN);
			Saliency->setZeros();
			for(int Can=0;Can<3;Can++)
				Saliency->add(Cannels[Can]);
			Normalization(*Saliency);
		}

		//Composite Lab's and RGB's saliency into finnal sailency
		FinalSaliency.resize(PATCHLEN,PATCHLEN);
		FinalSaliency.setZeros();
		FinalSaliency.add(LabSaliency);
		FinalSaliency.add(RGBSaliency);
		Normalization(FinalSaliency);

		
		//transform saliency into image 
		Mat SaliencyImage(PATCHLEN,PATCHLEN,CV_8UC1);
		MatrixtoMat(FinalSaliency,SaliencyImage);
		imshow("Final Saliency",SaliencyImage);
		
		waitKey(0);
	}
	return 0;
}