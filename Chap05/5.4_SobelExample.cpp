/**********************************************************************

  文件名: 5.4_SobelExample.cpp
  Copyright (c) 张晓东, 罗火灵. All rights reserved.
  更多信息请访问: 
    http://www.vtkchina.org (VTK中国)
	http://blog.csdn.net/www_doling_net (东灵工作室) 

**********************************************************************/

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   // 必需：初始化OpenGL渲染后端
VTK_MODULE_INIT(vtkInteractionStyle);   // 必需：初始化交互模块

#include <vtkSmartPointer.h>
#include <vtkImageMathematics.h>
#include <vtkImageData.h>
#include <vtkImageSobel2D.h>
#include <vtkImageMagnitude.h>
#include <vtkImageExtractComponents.h>
#include <vtkImageShiftScale.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkImageActor.h>
#include <vtkJPEGReader.h>

//测试图像：../data/lena-gray.jpg
int main(int argc, char* argv[])
{
	//if (argc < 2)
	//{
	//	std::cout<<argv[0]<<" "<<"ImageFile(*.jpg)"<<std::endl;
	//	return EXIT_FAILURE;
	//}

	vtkSmartPointer<vtkJPEGReader> reader =
		vtkSmartPointer<vtkJPEGReader>::New();
	reader->SetFileName("C:\\Users\\luhy\\Desktop\\data\\lena-gray.jpg");
	reader->Update();

	vtkSmartPointer<vtkImageSobel2D> sobelFilter =
		vtkSmartPointer<vtkImageSobel2D>::New();
	sobelFilter->SetInputConnection(reader->GetOutputPort());

	//提取分量  X方向梯度
	vtkSmartPointer<vtkImageExtractComponents> extractXFilter = 
		vtkSmartPointer<vtkImageExtractComponents>::New();
	extractXFilter->SetComponents(0);  
	extractXFilter->SetInputConnection(sobelFilter->GetOutputPort());
	extractXFilter->Update();

	double xRange[2];  
	extractXFilter->GetOutput()->GetScalarRange(xRange);  // 值域

	vtkSmartPointer<vtkImageMathematics> xImageAbs =
		vtkSmartPointer<vtkImageMathematics>::New();
	xImageAbs->SetOperationToAbsoluteValue();  // 取绝对值
	xImageAbs->SetInputConnection(extractXFilter->GetOutputPort());
	xImageAbs->Update();

	vtkSmartPointer<vtkImageShiftScale> xShiftScale =
		vtkSmartPointer<vtkImageShiftScale>::New();
	xShiftScale->SetOutputScalarTypeToUnsignedChar();
	xShiftScale->SetScale( 255 / xRange[1] );
	xShiftScale->SetInputConnection(xImageAbs->GetOutputPort());
	xShiftScale->Update();

	//提取分量  Y方向梯度
	vtkSmartPointer<vtkImageExtractComponents> extractYFilter = 
		vtkSmartPointer<vtkImageExtractComponents>::New();
	extractYFilter->SetComponents(1);
	extractYFilter->SetInputConnection(sobelFilter->GetOutputPort());
	extractYFilter->Update();

	double yRange[2];
	extractYFilter->GetOutput()->GetScalarRange(yRange);

	vtkSmartPointer<vtkImageMathematics> yImageAbs =
		vtkSmartPointer<vtkImageMathematics>::New();
	yImageAbs->SetOperationToAbsoluteValue();
	yImageAbs->SetInputConnection(extractYFilter->GetOutputPort());
	yImageAbs->Update();

	vtkSmartPointer<vtkImageShiftScale> yShiftScale =
		vtkSmartPointer<vtkImageShiftScale>::New();
	yShiftScale->SetOutputScalarTypeToUnsignedChar();
	yShiftScale->SetScale( 255 / yRange[1] );
	yShiftScale->SetInputConnection(yImageAbs->GetOutputPort());
	yShiftScale->Update();

	vtkSmartPointer<vtkImageActor> originalActor =
		vtkSmartPointer<vtkImageActor>::New();
	originalActor->SetInputData(reader->GetOutput());

	vtkSmartPointer<vtkImageActor> xActor =
		vtkSmartPointer<vtkImageActor>::New();
	xActor->SetInputData(xShiftScale->GetOutput());

	vtkSmartPointer<vtkImageActor> yActor =
		vtkSmartPointer<vtkImageActor>::New();
	yActor->SetInputData(yShiftScale->GetOutput());

	double originalViewport[4] = {0.0, 0.0, 0.33, 1.0};
	double xViewport[4] = {0.33, 0.0, 0.66, 1.0};
	double yViewport[4] = {0.66, 0.0, 1.0, 1.0};

	vtkSmartPointer<vtkRenderer> originalRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	originalRenderer->SetViewport(originalViewport);
	originalRenderer->AddActor(originalActor);
	originalRenderer->ResetCamera();
	originalRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> xRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	xRenderer->SetViewport(xViewport);
	xRenderer->AddActor(xActor);
	xRenderer->ResetCamera();
	xRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> yRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	yRenderer->SetViewport(yViewport);
	yRenderer->AddActor(yActor);
	yRenderer->ResetCamera();
	yRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->SetSize(1200, 300);
	renderWindow->AddRenderer(originalRenderer);
	renderWindow->AddRenderer(xRenderer);
	renderWindow->AddRenderer(yRenderer);
	renderWindow->Render();
	renderWindow->SetWindowName("SobelExample");

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style =
		vtkSmartPointer<vtkInteractorStyleImage>::New();

	renderWindowInteractor->SetInteractorStyle(style);
	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}
