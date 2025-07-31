/**********************************************************************

  文件名: 2.3_Viewport.cpp
  Copyright (c) 张晓东, 罗火灵. All rights reserved.
  更多信息请访问: 
    http://www.vtkchina.org (VTK中国)
	http://blog.csdn.net/www_doling_net (东灵工作室) 

**********************************************************************/

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   // 必需：初始化OpenGL渲染后端
VTK_MODULE_INIT(vtkInteractionStyle);   // 必需：初始化交互模块

#include <vtkConeSource.h>
#include <vtkCubeSource.h>
#include <vtkCylinderSource.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkActor.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>

int main()
{
	vtkSmartPointer<vtkConeSource> cone = vtkSmartPointer<vtkConeSource>::New();
	vtkSmartPointer<vtkCubeSource> cube = vtkSmartPointer<vtkCubeSource>::New();
	vtkSmartPointer<vtkCylinderSource> cylinder = vtkSmartPointer<vtkCylinderSource>::New();
	vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();

	vtkSmartPointer<vtkPolyDataMapper> coneMapper = 	vtkSmartPointer<vtkPolyDataMapper>::New();
	coneMapper->SetInputConnection(cone->GetOutputPort());
	vtkSmartPointer<vtkPolyDataMapper> cubeMapper = 	vtkSmartPointer<vtkPolyDataMapper>::New();
	cubeMapper->SetInputConnection(cube->GetOutputPort());
	vtkSmartPointer<vtkPolyDataMapper> cylinderMapper = 	vtkSmartPointer<vtkPolyDataMapper>::New();
	cylinderMapper->SetInputConnection(cylinder ->GetOutputPort());
	vtkSmartPointer<vtkPolyDataMapper> sphereMapper = 	vtkSmartPointer<vtkPolyDataMapper>::New();
	sphereMapper->SetInputConnection(sphere->GetOutputPort());

	vtkSmartPointer<vtkActor> coneActor = vtkSmartPointer<vtkActor>::New();
	coneActor->SetMapper(coneMapper);
	vtkSmartPointer<vtkActor> cubeActor = vtkSmartPointer<vtkActor>::New();
	cubeActor->SetMapper(cubeMapper);
	vtkSmartPointer<vtkActor> cylinderActor = vtkSmartPointer<vtkActor>::New();
	cylinderActor->SetMapper(cylinderMapper);
	vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
	sphereActor->SetMapper(sphereMapper);

	vtkSmartPointer<vtkRenderer> renderer1 = vtkSmartPointer<vtkRenderer>::New();
	renderer1->AddActor(coneActor);
	renderer1->SetBackground(1.0,0.0,0.0);
	renderer1->SetViewport(0.0,0.0,0.5,0.5);    // 用于设置渲染器在渲染窗口中的相对位置和大小, 四个参数分别代表视口的左下角坐标和右上角坐标
	vtkSmartPointer<vtkRenderer> renderer2 = vtkSmartPointer<vtkRenderer>::New();
	renderer2->AddActor(cubeActor);
	renderer2->SetBackground(0.0,1.0,0.0);
	renderer2->SetViewport(0.5,0.0,1.0,0.5);
	vtkSmartPointer<vtkRenderer> renderer3 = vtkSmartPointer<vtkRenderer>::New();
	renderer3->AddActor(cylinderActor);
	renderer3->SetBackground(0.0,0.0,1.0);
	renderer3->SetViewport(0.0,0.5,0.5,1.0);
	vtkSmartPointer<vtkRenderer> renderer4 = vtkSmartPointer<vtkRenderer>::New();
	renderer4->AddActor(sphereActor);
	renderer4->SetBackground(1.0,1.0,0.0);
	renderer4->SetViewport(0.5,0.5,1.0,1.0);

	vtkSmartPointer<vtkRenderWindow> renWin=vtkSmartPointer<vtkRenderWindow>::New();
	renWin->AddRenderer(renderer1);
	renWin->AddRenderer(renderer2);
	renWin->AddRenderer(renderer3);
	renWin->AddRenderer(renderer4);
	renWin->SetSize( 640, 480 );
	renWin->Render();
	renWin->SetWindowName("Viewport");

	vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renWin);

	renWin->Render();

	interactor->Initialize();
	interactor->Start();

	return EXIT_SUCCESS;
}