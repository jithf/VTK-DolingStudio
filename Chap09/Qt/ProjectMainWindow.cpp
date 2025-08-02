/**********************************************************************

  文件名: ProjectMainWindow.cpp
  Copyright (c) 张晓东, 罗火灵. All rights reserved.
  更多信息请访问: 
    http://www.vtkchina.org (VTK中国)
	http://blog.csdn.net/www_doling_net (东灵工作室) 

**********************************************************************/

#include "ProjectMainWindow.h"
#include <QFileDialog>
#include <QDir>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkImageViewer2.h>
#include <vtkJPEGReader.h>
#include <vtkImageActor.h>
#include <vtkCommand.h>

ProjectMainWindow::ProjectMainWindow(QWidget* parent)
	: QMainWindow(parent), ui(new Ui::ProjectMainWindow)
{
	ui->setupUi(this);

	// ===  配置 OpenGL 上下文 ===
	QSurfaceFormat format;
	format.setVersion(3, 3);  // 使用兼容的 OpenGL 版本
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setSamples(0);     // 禁用多重采样
	QSurfaceFormat::setDefaultFormat(format);

	// ===  初始化 VTK 组件 ===
	m_pImageViewer  = vtkSmartPointer< vtkImageViewer2 >::New();
	m_pRenderder      = vtkSmartPointer< vtkRenderer >::New();
	m_pRenderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
	m_pRenderWindow->SetMultiSamples(0);  // 禁用多重采样防止帧缓冲错误

	// 设置m_QVTKWidget的渲染器
	ui->m_QVTKWidget->setRenderWindow(m_pRenderWindow);
	m_pRenderWindow->AddRenderer(m_pRenderder);

	// === 3. 初始化交互器 ===
	// 注意：使用 QVTKOpenGLNativeWidget 内部的交互器，不要自己创建
	vtkRenderWindowInteractor* interactor = ui->m_QVTKWidget->interactor();
	interactor->SetRenderWindow(m_pRenderWindow);

	//连接打开的信号与相应的槽
	connect(ui->m_OpenAction, SIGNAL( triggered() ), this, SLOT( onOpenSlot() ) ); 

	m_Connections = vtkEventQtSlotConnect::New();
	m_Connections->Connect(interactor, vtkCommand::MouseMoveEvent, this, SLOT(updateCoords(vtkObject*)));
}

ProjectMainWindow::~ProjectMainWindow()
{
}

void ProjectMainWindow::onOpenSlot()
{
	QString filter;
	filter = "JPEG image file (*.jpg *.jpeg)";

	QDir dir;
	QString fileName = QFileDialog::getOpenFileName( this, QString(tr("打开图像")), dir.absolutePath() , filter );
	if ( fileName.isEmpty() == true ) return;

	// 支持带中文路径的读取
	QByteArray ba = fileName.toLocal8Bit();
	const char *fileName_str = ba.data();

	// 用vtkJPEGReader读取JPG图像
	vtkSmartPointer<vtkJPEGReader> reader = vtkSmartPointer<vtkJPEGReader>::New();
	reader->SetFileName(fileName_str);
	reader->Update();

	// 将reader的输出作为m_pImageViewer的输入，并设置m_pImageViewer与渲染器m_pRenderer的关联
	m_pImageViewer->SetInputData(reader->GetOutput());
	m_pImageViewer->UpdateDisplayExtent();
	m_pImageViewer->SetRenderWindow(m_pRenderWindow);
	m_pImageViewer->SetRenderer(m_pRenderder);
	m_pImageViewer->SetupInteractor(ui->m_QVTKWidget->interactor());
	m_pImageViewer->SetSliceOrientationToXY(); //默认就是这个方向的
	m_pImageViewer->GetImageActor()->InterpolateOff();   // 禁用插值以获得清晰图像
	m_pImageViewer->UpdateDisplayExtent(); // 更新显示范围
	m_pRenderder->ResetCamera();
	m_pRenderder->DrawOn();
	m_pRenderWindow->Render();
}

void ProjectMainWindow::updateCoords(vtkObject* obj)
{
	// 获取交互器
	vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::SafeDownCast(obj);

	// 获取鼠标的当前位置
	int event_pos[2];
	iren->GetEventPosition(event_pos);

	QString str;
	str.sprintf("x=%d : y=%d", event_pos[0], event_pos[1]);
	ui->m_StatusBar->showMessage(str);
}
