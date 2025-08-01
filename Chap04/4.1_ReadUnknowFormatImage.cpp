/**********************************************************************

  文件名: 4.1_ReadUnknowFormatImage.cpp
  Copyright (c) 张晓东, 罗火灵. All rights reserved.
  更多信息请访问: 
    http://www.vtkchina.org (VTK中国)
	http://blog.csdn.net/www_doling_net (东灵工作室) 

**********************************************************************/

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   // 必需：初始化OpenGL渲染后端
VTK_MODULE_INIT(vtkInteractionStyle);   // 必需：初始化交互模块

#include <vtkSmartPointer.h>
#include <vtkImageReader2Factory.h>
#include <vtkImageReader2.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

//测试文件：data/VTK-logo
int main ( int argc, char *argv[])
{
	//if(argc != 2)
	//{
	//	std::cout << "Required arguments: Filename" << std::endl;
	//	return EXIT_FAILURE;
	//}
	// 
	//读取图像
	vtkSmartPointer<vtkImageReader2Factory> readerFactory = vtkSmartPointer<vtkImageReader2Factory>::New();
	vtkImageReader2 * reader = readerFactory->CreateImageReader2("C:\\Users\\luhy\\Desktop\\data\\VTK-logo.png");  // 根据文件的类型生成相对应类型的Reader
	reader->SetFileName("C:\\Users\\luhy\\Desktop\\data\\VTK-logo.png");
	reader->Update();

	//显示读取的图像
	vtkSmartPointer<vtkImageViewer2> imageViewer = vtkSmartPointer<vtkImageViewer2>::New();
	imageViewer->SetInputConnection(reader->GetOutputPort());
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	imageViewer->SetupInteractor(renderWindowInteractor);
	imageViewer->Render();
	imageViewer->GetRenderer()->ResetCamera();
	imageViewer->Render();

	imageViewer->SetSize(640, 480);
	imageViewer->GetRenderWindow()->SetWindowName("ReadUnknowFormatImage");

	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}
