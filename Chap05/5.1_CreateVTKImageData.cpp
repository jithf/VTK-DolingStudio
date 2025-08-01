/**********************************************************************

  文件名: 5.1_CreateVTKImageData.cpp
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
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkImageActor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderWindow.h>

int main()
{
	// 创建图像数据
	vtkSmartPointer<vtkImageData> img =  vtkSmartPointer<vtkImageData>::New();
	img->SetDimensions(16,16,1);  // 设置维度数
	img->AllocateScalars(VTK_UNSIGNED_CHAR, 1);  // 分配内存 元素的类型和标量组件数量

	unsigned char *ptr = (unsigned char*)img->GetScalarPointer();
	for(int i=0; i<16*16*1; i++)
	{
		*ptr ++ =i%256; 
	}

	vtkSmartPointer<vtkImageActor> redActor = vtkSmartPointer<vtkImageActor>::New();
	redActor->SetInputData(img);

	double redViewport[4] = {0.0, 0.0, 1.0, 1.0};
	vtkSmartPointer<vtkRenderer> redRenderer = vtkSmartPointer<vtkRenderer>::New();
	redRenderer->SetViewport(redViewport);
	redRenderer->AddActor(redActor);
	redRenderer->ResetCamera();
	redRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(redRenderer);
	renderWindow->SetSize( 640, 480 );
	renderWindow->Render();
	renderWindow->SetWindowName("CreateVTKImageData");

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
	renderWindowInteractor->SetInteractorStyle(style);

	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}
