/**********************************************************************

  文件名: 4.2_Import3DS.cpp
  Copyright (c) 张晓东, 罗火灵. All rights reserved.
  更多信息请访问: 
    http://www.vtkchina.org (VTK中国)
	http://blog.csdn.net/www_doling_net (东灵工作室) 

**********************************************************************/

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   // 必需：初始化OpenGL渲染后端
VTK_MODULE_INIT(vtkInteractionStyle);   // 必需：初始化交互模块

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtk3DSImporter.h>
#include <vtkDataSet.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>

//测试文件：data/R8.3ds
int main ( int argc, char *argv[])
{
	//if(argc != 2)
	//{
	//	std::cout << "Required arguments: Filename(*.3ds)" << std::endl;
	//	return EXIT_FAILURE;
	//}

	std::string filename = "C:\\Users\\luhy\\Desktop\\data\\R8.3ds";
	std::cout << "Reading " << filename << std::endl;

	// 3DS Import
	vtkSmartPointer<vtk3DSImporter> importer = vtkSmartPointer<vtk3DSImporter>::New();
	importer->SetFileName ( filename.c_str() );
	importer->ComputeNormalsOn();  // 打开法向量的计算功能
	importer->Read();

	vtkSmartPointer<vtkRenderer> renderer = importer->GetRenderer();
	vtkSmartPointer<vtkRenderWindow> renderWindow = importer->GetRenderWindow();
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =  vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);

	renderer->GradientBackgroundOn();  // 渐变背景
	renderer->SetBackground(1.0, 1.0, 1.0);
	renderer->SetBackground2(0.0, 0.0, 0.0);
	renderWindow->Render();
	renderWindow->SetSize(640, 480);
	renderWindow->SetWindowName("Import3DS");

	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}