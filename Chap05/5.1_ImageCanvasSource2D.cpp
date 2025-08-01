/**********************************************************************

  文件名: 5.1_ImageCanvasSource2D.cpp
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
#include <vtkImageCanvasSource2D.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkImageActor.h>

int main()
{
	vtkSmartPointer<vtkImageCanvasSource2D> canvas = vtkSmartPointer<vtkImageCanvasSource2D>::New();  // 空白画布
	canvas->SetScalarTypeToUnsignedChar(); // 像素类型
	canvas->SetNumberOfScalarComponents(1); //通道数
	canvas->SetExtent(0, 100, 0, 100, 0, 0);  //  画布大小 X从0到100 Y从0到100 Z从0到0
	canvas->SetDrawColor(0, 0, 0, 0);  // 画笔颜色
	canvas->FillBox(0,100,0,100);  //填充  X从0到100 Y从0到100
	canvas->SetDrawColor(255, 0, 0, 0);
	canvas->FillBox(20,40,20,40);
	canvas->Update();

	// Create actors
	vtkSmartPointer<vtkImageActor> redActor = vtkSmartPointer<vtkImageActor>::New();
	redActor->SetInputData(canvas->GetOutput());

	// Define viewport ranges
	// (xmin, ymin, xmax, ymax)
	double redViewport[4] = {0.0, 0.0, 1.0, 1.0};
	// Setup renderers
	vtkSmartPointer<vtkRenderer> redRenderer = vtkSmartPointer<vtkRenderer>::New();
	redRenderer->SetViewport(redViewport);
	redRenderer->AddActor(redActor);
	redRenderer->ResetCamera();
	redRenderer->SetBackground(1.0, 1.0, 1.0);

	// Setup render window
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(redRenderer);
	renderWindow->SetSize( 640, 480 );
	renderWindow->Render();
	renderWindow->SetWindowName("ImageCanvasSource2D");

	// Setup render window interactor
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
	renderWindowInteractor->SetInteractorStyle(style);
	renderWindowInteractor->SetRenderWindow(renderWindow);
	
	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}
