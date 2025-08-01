/**********************************************************************

  文件名: 5.6_IdealLowPassExample.cpp
  Copyright (c) 张晓东, 罗火灵. All rights reserved.
  更多信息请访问: 
    http://www.vtkchina.org (VTK中国)
	http://blog.csdn.net/www_doling_net (东灵工作室) 

**********************************************************************/

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   // 必需：初始化OpenGL渲染后端
VTK_MODULE_INIT(vtkInteractionStyle);   // 必需：初始化交互模块

#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkImageIdealLowPass.h>
#include <vtkImageActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkJPEGReader.h>
#include <vtkImageFFT.h>
#include <vtkImageRFFT.h>
#include <vtkImageExtractComponents.h>
#include <vtkImageCast.h>

//测试图像：../data/lena-gray.jpg
int main(int argc, char* argv[])
{
	//if (argc < 2)
	//{
	//	std::cout<<argv[0]<<" "<<"ImageFile(*.jpg)"<<std::endl;
	//	return EXIT_FAILURE;
	//}
	vtkSmartPointer<vtkJPEGReader> reader =  vtkSmartPointer<vtkJPEGReader>::New();
	reader->SetFileName("C:\\Users\\luhy\\Desktop\\data\\lena-gray.jpg");
	reader->Update();

	// 傅里叶变换
	vtkSmartPointer<vtkImageFFT> fftFilter = vtkSmartPointer<vtkImageFFT>::New();
	fftFilter->SetInputConnection(reader->GetOutputPort());
	fftFilter->Update();

	// 理想低通
	vtkSmartPointer<vtkImageIdealLowPass> lowPassFilter =  vtkSmartPointer<vtkImageIdealLowPass>::New();
	lowPassFilter->SetInputConnection(fftFilter->GetOutputPort());
	lowPassFilter->SetXCutOff(0.05); // X方向的截止频率
	lowPassFilter->SetYCutOff(0.05); // Y方向的截止频率
	lowPassFilter->Update();

	// 逆傅里叶变换
	vtkSmartPointer<vtkImageRFFT> rfftFilter = vtkSmartPointer<vtkImageRFFT>::New();
	rfftFilter->SetInputConnection(lowPassFilter->GetOutputPort());
	rfftFilter->Update();

	// 提取分量
	vtkSmartPointer<vtkImageExtractComponents> ifftExtractReal = vtkSmartPointer<vtkImageExtractComponents>::New();
	ifftExtractReal->SetInputConnection(rfftFilter->GetOutputPort());
	ifftExtractReal->SetComponents(0);

	vtkSmartPointer<vtkImageCast> castFilter = vtkSmartPointer<vtkImageCast>::New();
	castFilter->SetInputConnection(ifftExtractReal->GetOutputPort());
	castFilter->SetOutputScalarTypeToUnsignedChar();
	castFilter->Update();

	vtkSmartPointer<vtkImageActor> originalActor =
		vtkSmartPointer<vtkImageActor>::New();
	originalActor->SetInputData(reader->GetOutput());

	vtkSmartPointer<vtkImageActor> erodedActor =
		vtkSmartPointer<vtkImageActor>::New();
	erodedActor->SetInputData(castFilter->GetOutput());

	double leftViewport[4] = {0.0, 0.0, 0.5, 1.0};
	double rightViewport[4] = {0.5, 0.0, 1.0, 1.0};

	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->SetSize(640, 320);
	renderWindow->Render();
	renderWindow->SetWindowName("IdealLowPassExample");

	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);

	vtkSmartPointer<vtkRenderer> leftRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	renderWindow->AddRenderer(leftRenderer);
	leftRenderer->SetViewport(leftViewport);
	leftRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> rightRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	renderWindow->AddRenderer(rightRenderer);
	rightRenderer->SetViewport(rightViewport);
	rightRenderer->SetBackground(1.0, 1.0, 1.0);

	leftRenderer->AddActor(originalActor);
	rightRenderer->AddActor(erodedActor);

	leftRenderer->ResetCamera();
	rightRenderer->ResetCamera();

	renderWindow->Render();
	interactor->Start();

	return EXIT_SUCCESS;
}