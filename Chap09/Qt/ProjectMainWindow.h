/**********************************************************************

  文件名: ProjectMainWindow.h
  Copyright (c) 张晓东, 罗火灵. All rights reserved.
  更多信息请访问: 
    http://www.vtkchina.org (VTK中国)
	http://blog.csdn.net/www_doling_net (东灵工作室) 

**********************************************************************/

#ifndef Project_MainWindow_H
#define Project_MainWindow_H

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);       // 更新为现代OpenGL渲染后端
VTK_MODULE_INIT(vtkInteractionStyle);       // 保留交互样式支持

#include <QMainWindow>
#include <vtkSmartPointer.h>
#include <QVTKOpenGLNativeWidget.h>
#include <QMainWindow>
#include <vtkSmartPointer.h>
#include <vtkImageViewer2.h>
#include <vtkRenderer.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include "ui_ProjectMainWindow.h" // 假设UI文件生成的头文件

class vtkImageViewer2;
class vtkRenderer;
class vtkEventQtSlotConnect;
class vtkObject;
class vtkCommand;

namespace Ui {
	class ProjectMainWindow;
}

class ProjectMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit ProjectMainWindow(QWidget* parent = nullptr);
	~ProjectMainWindow() override;

private slots:
	//响应打开图像文件的槽函数
	void onOpenSlot();
	//响应鼠标移动的消息，实时输出鼠标的当前位置
	void updateCoords(vtkObject* obj);

private:
	vtkSmartPointer< vtkImageViewer2 >           m_pImageViewer;
	vtkSmartPointer< vtkRenderer >                   m_pRenderder;
	vtkSmartPointer<vtkGenericOpenGLRenderWindow>           m_pRenderWindow;
	vtkSmartPointer < vtkEventQtSlotConnect> m_Connections;

	Ui::ProjectMainWindow* ui;
};

#endif
