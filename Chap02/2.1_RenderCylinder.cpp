/**********************************************************************

  文件名: 2.1_RenderCylinder.cpp
  Copyright (c) 张晓东, 罗火灵. All rights reserved.
  更多信息请访问: 
    http://www.vtkchina.org (VTK中国)
	http://blog.csdn.net/www_doling_net (东灵工作室) 

**********************************************************************/

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   // 必需：初始化OpenGL渲染后端
VTK_MODULE_INIT(vtkInteractionStyle);   // 必需：初始化交互模块

#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>

int main()
{
	vtkSmartPointer<vtkCylinderSource> cylinder = vtkSmartPointer<vtkCylinderSource>::New();  // 生成圆柱体
	cylinder->SetHeight( 3.0 );
	cylinder->SetRadius( 1.0 );
	cylinder->SetResolution( 10 );  // 设置柱体横截面多边形的边数

	vtkSmartPointer<vtkPolyDataMapper> cylinderMapper =  vtkSmartPointer<vtkPolyDataMapper>::New();  // 三维转二维
	cylinderMapper->SetInputConnection( cylinder->GetOutputPort() ); 

	vtkSmartPointer<vtkActor> cylinderActor =  vtkSmartPointer<vtkActor>::New();  // 用于渲染的类
	cylinderActor->SetMapper( cylinderMapper );
	cylinderActor->GetProperty()->SetColor(1.0, 0.0, 0.0); // 设置颜色 

	vtkSmartPointer<vtkRenderer> renderer =  vtkSmartPointer<vtkRenderer>::New();  // 渲染场景
	renderer->AddActor( cylinderActor );
	renderer->SetBackground( 1.0, 1.0, 1.0 );  // 设置背景颜色

	vtkSmartPointer<vtkRenderWindow> renWin =  vtkSmartPointer<vtkRenderWindow>::New();  // 渲染窗口
	renWin->AddRenderer( renderer );
	renWin->SetSize( 640, 480 );
	renWin->Render(); // 渲染
	renWin->SetWindowName("RenderCylinder");

	vtkSmartPointer<vtkRenderWindowInteractor> iren =  vtkSmartPointer<vtkRenderWindowInteractor>::New(); // 渲染窗口交互器 监听鼠标和键盘事件 并交于InceractorStyle处理
	iren->SetRenderWindow(renWin);

	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =  vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();  // 交互器样式的一种 通过移动相机实现对物体的放大缩小等操作
	iren->SetInteractorStyle(style);

	iren->Initialize();  // 初始化交互器 
	iren->Start();  // 进入事件响应循环

	return EXIT_SUCCESS;
}