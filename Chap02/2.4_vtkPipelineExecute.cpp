/**********************************************************************

  文件名: 2.4_vtkPipelineExecute.cpp
  Copyright (c) 张晓东, 罗火灵. All rights reserved.
  更多信息请访问: 
    http://www.vtkchina.org (VTK中国)
	http://blog.csdn.net/www_doling_net (东灵工作室) 

**********************************************************************/

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   // 必需：初始化OpenGL渲染后端
VTK_MODULE_INIT(vtkInteractionStyle);   // 必需：初始化交互模块

#include <vtkSmartPointer.h>
#include <vtkBMPReader.h>
#include <vtkImageData.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindow.h>

//测试文件：data/VTK-logo.bmp
int main(int argc, char* argv[])
{
	//if (argc < 2)
	//{
	//	std::cout<<argv[0]<<" "<<"BMP-File(*.bmp)"<<std::endl;
	//	return EXIT_FAILURE;
	//}

	vtkSmartPointer<vtkBMPReader> reader = vtkSmartPointer<vtkBMPReader>::New();
	std::cout<<"Modification Time of reader (After New()): "<<reader->GetMTime()<<std::endl;
	reader->SetFileName("C://Users//luhy//Desktop//VTK-logo.bmp");
	std::cout<<"Modification Time of reader (After SetFileName()): "<<reader->GetMTime()<<std::endl;

	vtkAlgorithmOutput* outputPort = reader->GetOutputPort();
	vtkImageData* imageData = reader->GetOutput();
	std::cout<<"Modification Time of reader (After GetOutput()): "<<reader->GetMTime()<<std::endl;
	reader->Update();
	std::cout<<"Modification Time of reader (After Update()): "<<reader->GetMTime()<<std::endl;

	int extent[6];
	imageData->GetExtent(extent);  // 获取图像数据的范围
	//-`extent[0]`：x方向的最小索引（通常为0）
	//- `extent[1]`：x方向的最大索引（宽度 - 1）
	//- `extent[2]`：y方向的最小索引（通常为0）
	//- `extent[3]`：y方向的最大索引（高度 - 1）
	//- `extent[4]`：z方向的最小索引（通常为0，对于2D图像）
	//- `extent[5]`：z方向的最大索引（深度 - 1，对于2D图像通常为0）
	std::cout<<"Extent of image: "<<extent[0]<<" "
		<<extent[1]<<" "<<extent[2]<<" "<<extent[3]<<" "
		<<extent[4]<<" "<<extent[5]<<" "<<std::endl;

	vtkSmartPointer<vtkImageViewer2> viewer =  vtkSmartPointer<vtkImageViewer2>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> interactor =  vtkSmartPointer<vtkRenderWindowInteractor>::New();
	viewer->SetupInteractor(interactor);
	viewer->SetInputConnection(outputPort);
	viewer->Render();

	viewer->SetSize(640, 480);
	viewer->GetRenderWindow()->SetWindowName("vtkPipelineExecute");

	interactor->Initialize();
	interactor->Start();

	return EXIT_SUCCESS;
}