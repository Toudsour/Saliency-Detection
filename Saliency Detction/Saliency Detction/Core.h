
using namespace cv;

void LocalSailency(SpMatrix<double> & SparseMatrix, Matrix<double>& Local)
{
	Matrix<double> Temp;
	SparseMatrix.toFull(Temp);
	double ***Patches;
	Patches=Reshape(Temp,200,PATCHLEN,PATCHLEN);
	Local.resize(PATCHLEN,PATCHLEN);
	for(int i=0;i<PATCHLEN;i++)
		for(int j=0;j<PATCHLEN;j++)
		{
			double Sum=0;
			for(int ShiftX=- WINDOWSIZE;ShiftX<=WINDOWSIZE;ShiftX++)
				for(int ShiftY=-WINDOWSIZE;ShiftY<=WINDOWSIZE;ShiftY++)
				{
					int PosY=ShiftY+i;
					int PosX=ShiftX+j;
					if(PosY<0||PosY>=PATCHLEN)
						continue;
					if(PosX<0||PosX>=PATCHLEN)
						continue;
					double Distance=sqrt(ShiftX*ShiftX+ShiftY*ShiftY);
					Sum+=Average(Patches[i][j],Patches[PosY][PosX],200);
				 }
			Local[i*PATCHLEN+j]=Sum;
		}
}
void Normalization(Matrix<double>& Input)
{
	MatrixSubConst(Input,FindMatrixMin(Input));
	MatrixDivConst(Input,FindMatrixMax(Input));
}