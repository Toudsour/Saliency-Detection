#Saliecy Detection
##Aim
	将图片分成 Lab 和 RGB 两个色彩域对于每个信道利用 Loacl 和 Global 的显著性图像，
	最终标准化并合成最终的显著性图像。
	
##Configuration
**以VS2012为例：**
*（欢迎探索更多环境）*

- 下载OpneCV3.0版本[Link:]
- 配置OpenCV[Link:]
- 下载安装**Intel Parallel Studio XE 2015**[Link]
- VS->项目->(项目名)属性页->配置属性->*Intell Performance Libraries*->*Use Intel MKL*->**Parallel**(Others maybe Ok.)
- 添加SPAMS工具箱的头文件函数
- 配置完毕

##File Table
|文件名|类型|意义|
|-----|---|---|
|Opencv|.hpp|OpenCV主文件|
|imgproc|.hpp|OpenCV组件|
|spams|.h|SPAMS工具箱头文件|
|HeadFile|.h|管理头文件|
|Setting|.h|一些设置的常量（如：图集地址，图片resize后大小等等）|
|DataStruct|.h|自定义的结构|
|Function|.h|一些常用的函数|
|Core|.h|计算Local,Global显著性|
|Saliency|.cpp|主函数|

##Function Table
**Function.h:**

|函数名|参数|返回值|作用|
|-----|---|-----|----|
|SplitCannel|Mat&(原图像),Matrix& A,B,C(三个信道)|void|将Mat格式的图像的三个信道分离出来转成Matrix格式|
|GetLearningDictionary|Matrix\*,Matrix\*|void|从文件中读取字典|
|lasso|Matrix&,Matrix&,SpMatrix&,Sparam|void|对SPAMS里lasso的重载|
|ImagetoCol|Matrix&,int(行),int(列)|void|将大举证分割成R*C的小矩阵，并将小矩阵转换成列|
|ColToImage|Matrix&,int（小矩阵行）,int（小矩阵列）,int(输出矩阵行),int（输出矩阵列）|void|ImagetoCal函数的逆操作，将列转换成小矩阵并裁减拼成的大矩阵|
|GetThridMatrix|int,int,int|T\*\*\*|生成一个三维矩阵，并返回指针|
|DeleteThridMatrix|int,int,int,T\*\*\*|void|释放一个三维矩阵|
|Reshape|Matrix&,int,int,int|void|讲列转换成二维矩阵|
|Average|doubel\*,double\*,int|doubel|求两个向量每个元素差的平均值|
|FindMatrixMax|const Matrix&|double|求一个矩阵中最大的元素|
|FindMatrixMin|const Matrix&|double|求一个矩阵中最小的元素|
|MatrixSubConst|Matrix&,double|void|矩阵减去一个值|
|MatrixDivConst|Matrix&,double|void|矩阵除以一个值|


**Setting.h**

|常量值|含义|
|-----|---|
|IMGSAIM|AIM图集的路径|
|MAXPATHPIC|最大路径长度|
|MAXPIC|最大图片数量|
|MAXSTEP|SPAMS工具箱最大迭代次数|
|PATCHSIZE|分割后PATCH的大小|
|PICSIZE|图片大小|
|PATCHLEN|分割后PATCH的数量|
|WINDOWSIZE|Local计算中窗口大小|


**Core.h**

|函数名|参数|返回值|作用|
|-----|---|-----|----|

##Tips

**看不懂的请在GITHUB上私信**
