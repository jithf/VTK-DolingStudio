/**********************************************************************

  文件名: 1.3_TestVTKInstall.cpp
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
#include <vtkRenderWindowInteractor.h>  // 关键：包含交互器头文件

int main() {
    // 1. 创建渲染窗口
    vtkSmartPointer<vtkRenderWindow> renWin =  vtkSmartPointer<vtkRenderWindow>::New();
    renWin->SetWindowName("VTK Window Test");
    renWin->SetSize(640, 480);

    // 2. 创建并添加渲染器（必需组件）
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renWin->AddRenderer(renderer);

    // 3. 创建交互器（关键：保持窗口打开）
    vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    iren->SetRenderWindow(renWin);  // 关联渲染窗口

    // 4. 初始化并启动事件循环
    iren->Initialize();  // 初始化交互器
    renWin->Render();    // 首次渲染
    iren->Start();       // 启动事件循环（窗口保持打开）

    return EXIT_SUCCESS;
}
