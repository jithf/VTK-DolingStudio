/**********************************************************************

  文件名: 6.2_PolyDataMassProperty.cpp
  Copyright (c) 张晓东, 罗火灵. All rights reserved.
  更多信息请访问: 
    http://www.vtkchina.org (VTK中国)
	http://blog.csdn.net/www_doling_net (东灵工作室) 

**********************************************************************/

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   // 必需：初始化OpenGL渲染后端
VTK_MODULE_INIT(vtkInteractionStyle);   // 必需：初始化交互模块

#include <vtkSmartPointer.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkScalarBarActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyData.h>
#include <vtkCubeSource.h>
#include <vtkProperty.h>
#include <vtkTriangleFilter.h>
#include <vtkMassProperties.h>

int main(int argc, char *argv[])
{
	vtkSmartPointer<vtkCubeSource> cubeSource =  vtkSmartPointer<vtkCubeSource>::New();
	cubeSource->Update();

	// 多边形网格数据转为三角形网格数据
	vtkSmartPointer<vtkTriangleFilter> triFilter =  vtkSmartPointer<vtkTriangleFilter>::New(); 
	triFilter->SetInputData(cubeSource->GetOutput());
	triFilter->Update();

	// 三角形网格数据的表面积和体积的计算
	vtkSmartPointer<vtkMassProperties> massProp =  vtkSmartPointer<vtkMassProperties>::New();
	massProp->SetInputData(triFilter->GetOutput());
	float vol = massProp->GetVolume();  // 体积
	float area= massProp->GetSurfaceArea();  // 表面积
	float maxArea = massProp->GetMaxCellArea();  // 最大三角面
	float minArea = massProp->GetMinCellArea();  // // 最小三角面

	std::cout<<"Volume      :"<<vol<<std::endl;
	std::cout<<"Surface Area:"<<area<<std::endl;
	std::cout<<"Max Area    :"<<maxArea<<std::endl;
	std::cout<<"Min Area    :"<<minArea<<std::endl;

	vtkSmartPointer<vtkPolyDataMapper> mapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(cubeSource->GetOutput());
	//mapper->SetInputData(triFilter->GetOutput());

	vtkSmartPointer<vtkActor> actor =  vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(0,1,0);
	actor->GetProperty()->SetEdgeColor(1,0,0);
	actor->GetProperty()->SetEdgeVisibility(1);

	vtkSmartPointer<vtkRenderer> renderer =  vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(actor);
	renderer->SetBackground(1.0,1.0,1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow =  vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	renderWindow->SetSize( 640, 480 );
	renderWindow->Render();
	renderWindow->SetWindowName("PolyDataMassProperty");

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =  vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);

	renderWindow->Render();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}