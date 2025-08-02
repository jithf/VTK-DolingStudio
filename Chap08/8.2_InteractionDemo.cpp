/**********************************************************************

  文件名: 8.2_InteractionDemo.cpp
  Copyright (c) 张晓东, 罗火灵. All rights reserved.
  更多信息请访问: 
    http://www.vtkchina.org (VTK中国)
	http://blog.csdn.net/www_doling_net (东灵工作室) 

**********************************************************************/

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   // 必需：初始化OpenGL渲染后端
VTK_MODULE_INIT(vtkInteractionStyle);   // 必需：初始化交互模块
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2); // 确保正确初始化体积渲染模块

#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkJPEGReader.h>
#include <vtkImageActor.h>

//测试：../data/VTKBook-TestImage.jpg
int main(int argc, char* argv[])
{
	//if ( argc < 2 )
	//{
	//	std::cout<<"please load a *.jpg image."<<std::endl;
	//	return EXIT_FAILURE;
	//}

	char* fileName = "C:\\Users\\luhy\\Desktop\\data\\VTKBook-TestImage.jpg";
	vtkSmartPointer<vtkJPEGReader> reader =  vtkSmartPointer<vtkJPEGReader>::New();
	reader->SetFileName(fileName);
	reader->Update();

	vtkSmartPointer<vtkImageActor> imageActor =  vtkSmartPointer<vtkImageActor>::New();
	imageActor->SetInputData( reader->GetOutput() );

	vtkSmartPointer<vtkRenderer> renderer =  vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor( imageActor );
	renderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renWin =  vtkSmartPointer<vtkRenderWindow>::New();
	renWin->AddRenderer( renderer );
	renWin->SetSize( 640, 480 );
	renWin->Render();
	renWin->SetWindowName("InteractionDemo");

	vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);

	vtkSmartPointer<vtkInteractorStyleImage> style =  vtkSmartPointer<vtkInteractorStyleImage>::New();
	iren->SetInteractorStyle(style);
	iren->Initialize();
	iren->Start();

	return EXIT_SUCCESS;
}
