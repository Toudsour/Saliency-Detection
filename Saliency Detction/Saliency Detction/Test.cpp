#include<opencv.hpp>
#include<imgproc.hpp>
using namespace cv;
int main()
{
	Mat srcImage = imread("F://1.jpg");
	imshow("SrcP",srcImage);
	Mat element = getStructuringElement(MORPH_RECT,Size(4,4));
	Mat dstImage;
	erode(srcImage,dstImage,element);
	imshow("DstImage",dstImage);
	waitKey(0);	
	return 0;
}