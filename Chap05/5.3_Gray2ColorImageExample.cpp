/**********************************************************************

  文件名: 5.3_Gray2ColorImageExample.cpp
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
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkImageActor.h>
#include <vtkJPEGReader.h>
#include <vtkImageLuminance.h>
#include <vtkLookupTable.h>
#include <vtkImageMapToColors.h>

//测试图像：../data/lena-gray.jpg
int main(int argc, char* argv[])
{
	//if (argc < 2)
	//{
	//	std::cout<<argv[0]<<" "<<"ImageFile(*.jpg)"<<std::endl;
	//	return EXIT_FAILURE;
	//}

	vtkSmartPointer<vtkJPEGReader> reader = vtkSmartPointer<vtkJPEGReader>::New();
	reader->SetFileName ( "C:\\Users\\luhy\\Desktop\\data\\lena-gray.jpg" );

	vtkSmartPointer<vtkLookupTable> colorTable =  vtkSmartPointer<vtkLookupTable>::New(); 
	colorTable->SetRange( 0.0, 255.0 );  // 需要映射的标量数据的范围
	colorTable->SetHueRange( 0.1, 0.5 );  // HSV-Hue值
	colorTable->SetValueRange( 0.6, 1.0 );  // HSV-Value值
	colorTable->Build();  // 生成颜色表

	vtkSmartPointer<vtkImageMapToColors> colorMap =  vtkSmartPointer<vtkImageMapToColors>::New();
	colorMap->SetInputConnection( reader->GetOutputPort() );
	colorMap->SetLookupTable( colorTable );
	colorMap->Update();

	vtkSmartPointer<vtkImageActor> originalActor = vtkSmartPointer<vtkImageActor>::New();
	originalActor->SetInputData(reader->GetOutput());

	vtkSmartPointer<vtkImageActor> shiftscaleActor = vtkSmartPointer<vtkImageActor>::New();
	shiftscaleActor->SetInputData(colorMap->GetOutput());

	double originalViewport[4] = {0.0, 0.0, 0.5, 1.0};
	double shiftscaleViewport[4] = {0.5, 0.0, 1.0, 1.0};

	vtkSmartPointer<vtkRenderer> originalRenderer = vtkSmartPointer<vtkRenderer>::New();
	originalRenderer->SetViewport(originalViewport);
	originalRenderer->AddActor(originalActor);
	originalRenderer->ResetCamera();
	originalRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> shiftscaleRenderer = vtkSmartPointer<vtkRenderer>::New();
	shiftscaleRenderer->SetViewport(shiftscaleViewport);
	shiftscaleRenderer->AddActor(shiftscaleActor);
	shiftscaleRenderer->ResetCamera();
	shiftscaleRenderer->SetBackground(1.0, 1.0, 0.8);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(originalRenderer);
	renderWindow->AddRenderer(shiftscaleRenderer);
	renderWindow->SetSize(900, 300);
	renderWindow->Render();
	renderWindow->SetWindowName("Gray2ColorImageExample");

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
	renderWindowInteractor->SetInteractorStyle(style);
	renderWindowInteractor->SetRenderWindow(renderWindow);

	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}
