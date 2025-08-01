/**********************************************************************

  文件名: 6.5_PolyDataConnectedCompExtract.cpp
  Copyright (c) 张晓东, 罗火灵. All rights reserved.
  更多信息请访问: 
    http://www.vtkchina.org (VTK中国)
	http://blog.csdn.net/www_doling_net (东灵工作室) 

**********************************************************************/

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   // 必需：初始化OpenGL渲染后端
VTK_MODULE_INIT(vtkInteractionStyle);   // 必需：初始化交互模块

#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkConeSource.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkAppendPolyData.h>

int main(int, char *[])
{
	// 球体
	vtkSmartPointer<vtkSphereSource> sphereSource =  vtkSmartPointer<vtkSphereSource>::New();
	sphereSource->SetRadius(10);
	sphereSource->SetThetaResolution(10);
	sphereSource->SetPhiResolution(10);
	sphereSource->Update();

	// 椎体
	vtkSmartPointer<vtkConeSource> coneSource =  vtkSmartPointer<vtkConeSource>::New();
	coneSource->SetRadius(5);
	coneSource->SetHeight(10);
	coneSource->SetCenter(25,0,0);
	coneSource->Update();

	// 多边形数据集合
	vtkSmartPointer<vtkAppendPolyData> appendFilter =  vtkSmartPointer<vtkAppendPolyData>::New();
	appendFilter->AddInputData(sphereSource->GetOutput());
	appendFilter->AddInputData(coneSource->GetOutput());
	appendFilter->Update();

	//连通区域分析
	vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivityFilter =  vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
	connectivityFilter->SetInputData(appendFilter->GetOutput());
	connectivityFilter->SetExtractionModeToCellSeededRegions();  // 指定提取 “由种子单元格（Seed Cell）所属的连通区域
	connectivityFilter->AddSeed(100);  // 指定 ID 为 100 的单元格作为 “种子”。过滤器会以此为起点，递归查找所有与其连通的区域
	connectivityFilter->Update();

	vtkSmartPointer<vtkPolyDataMapper> originalMapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
	originalMapper->SetInputConnection(appendFilter->GetOutputPort());
	originalMapper->Update();
	vtkSmartPointer<vtkActor> originalActor = vtkSmartPointer<vtkActor>::New();
	originalActor->SetMapper(originalMapper);

	vtkSmartPointer<vtkPolyDataMapper> extractedMapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
	extractedMapper->SetInputConnection(connectivityFilter->GetOutputPort());
	extractedMapper->Update();
	vtkSmartPointer<vtkActor> extractedActor =  vtkSmartPointer<vtkActor>::New();
	extractedActor->SetMapper(extractedMapper);

	double leftViewport[4] = {0.0, 0.0, 0.5, 1.0};
	double rightViewport[4] = {0.5, 0.0, 1.0, 1.0};

	vtkSmartPointer<vtkRenderer> leftRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	leftRenderer->SetViewport(leftViewport);
	leftRenderer->AddActor(originalActor);
	leftRenderer->SetBackground(0.8, 0.8, 0.8);

	vtkSmartPointer<vtkRenderer> rightRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	rightRenderer->SetViewport(rightViewport);
	rightRenderer->AddActor(extractedActor);
	rightRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow = 
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(leftRenderer);
	renderWindow->AddRenderer(rightRenderer);
	renderWindow->SetSize(640, 320);
	renderWindow->Render();
	renderWindow->SetWindowName("PolyDataConnectedCompExtract");

	leftRenderer->ResetCamera();
	rightRenderer->SetActiveCamera(leftRenderer->GetActiveCamera());

	vtkSmartPointer<vtkRenderWindowInteractor> interactor = 
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);
	interactor->Initialize();
	interactor->Start();

	return EXIT_SUCCESS;
}