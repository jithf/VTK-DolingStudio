/**********************************************************************

  文件名: 6.2_PolyDataNormal.cpp
  Copyright (c) 张晓东, 罗火灵. All rights reserved.
  更多信息请访问: 
    http://www.vtkchina.org (VTK中国)
	http://blog.csdn.net/www_doling_net (东灵工作室) 

**********************************************************************/

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   // 必需：初始化OpenGL渲染后端
VTK_MODULE_INIT(vtkInteractionStyle);   // 必需：初始化交互模块

#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkPolyDataReader.h>
#include <vtkPLYReader.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <vtkPolyDataNormals.h>
#include <vtkGlyph3D.h>
#include <vtkArrowSource.h>
#include <vtkSphereSource.h>
#include <vtkMaskPoints.h>
#include <vtkProperty.h>

//测试文件：../data/fran_cut.vtk
int main(int argc, char *argv[])
{
	//if(argc < 2)
	//{
	//	std::cout<<argv[0]<<" *.vtk"<<std::endl;
	//	return EXIT_FAILURE;
	//}

	vtkSmartPointer<vtkPolyDataReader> reader =  vtkSmartPointer<vtkPolyDataReader>::New();
	reader->SetFileName("C:\\Users\\luhy\\Desktop\\data\\fran_cut.vtk");
	reader->Update();

	// 计算法向量
	vtkSmartPointer<vtkPolyDataNormals> normFilter =  vtkSmartPointer<vtkPolyDataNormals>::New();
	normFilter->SetInputData(reader->GetOutput());
	normFilter->SetComputePointNormals(1);  // 计算点集法向量
	normFilter->SetComputeCellNormals(0);  // 计算单元集法向量
	normFilter->SetConsistency(1);  // 自动调整单元点的顺序
	normFilter->SetAutoOrientNormals(1);  // 自动调整法线方向 右手定则
	normFilter->SetSplitting(0);  // 关闭锐边缘处理 避免因锐边缘导致模型分裂，数据发生变化
	normFilter->Update();

	// 法向量采样
	vtkSmartPointer<vtkMaskPoints> mask =  vtkSmartPointer<vtkMaskPoints>::New();
	mask->SetInputData(normFilter->GetOutput());
	mask->SetMaximumNumberOfPoints(300);  // 点集的最大采样数量
	mask->RandomModeOn();  // 随机选择点
	mask->Update();

	// 借助Glyph图形显示法向量
	vtkSmartPointer<vtkArrowSource> arrow =  vtkSmartPointer<vtkArrowSource>::New();
	arrow->Update();
	vtkSmartPointer<vtkGlyph3D> glyph =  vtkSmartPointer<vtkGlyph3D>::New();
	glyph->SetInputData(mask->GetOutput());
	glyph->SetSourceData(arrow->GetOutput());   // 输入数据的每个点处都会显示一个Glyph图形 箭头
	glyph->SetVectorModeToUseNormal(); // 指定使用法向量控制Glyph图形
	glyph->SetScaleFactor(0.01); // Glyph图形的大小
	glyph->Update();

	vtkSmartPointer<vtkPolyDataMapper> originMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	originMapper->SetInputData(reader->GetOutput());
	vtkSmartPointer<vtkActor> originActor =  vtkSmartPointer<vtkActor>::New();
	originActor->SetMapper(originMapper);

	vtkSmartPointer<vtkPolyDataMapper> normedMapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
	normedMapper->SetInputData(normFilter->GetOutput());
	vtkSmartPointer<vtkActor> normedActor =  vtkSmartPointer<vtkActor>::New();
	normedActor->SetMapper(normedMapper);

	vtkSmartPointer<vtkPolyDataMapper> glyphMapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
	glyphMapper->SetInputData(glyph->GetOutput());
	vtkSmartPointer<vtkActor> glyphActor =  vtkSmartPointer<vtkActor>::New();
	glyphActor->SetMapper(glyphMapper);
	glyphActor->GetProperty()->SetColor(1., 0.,0.);

	double originalViewport[4] = {0.0, 0.0, 0.33, 1.0};
	double normViewport[4] = {0.33, 0.0, 0.66, 1.0};
	double glphViewport[4] = {0.66, 0.0, 1.0, 1.0};

	vtkSmartPointer<vtkRenderer> originalRenderer = vtkSmartPointer<vtkRenderer>::New();
	originalRenderer->SetViewport(originalViewport);
	originalRenderer->AddActor(originActor);
	originalRenderer->ResetCamera();
	originalRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> normedRenderer = vtkSmartPointer<vtkRenderer>::New();
	normedRenderer->SetViewport(normViewport);
	normedRenderer->AddActor(normedActor);
	normedRenderer->ResetCamera();
	normedRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> glyphRenderer = vtkSmartPointer<vtkRenderer>::New();
	glyphRenderer->SetViewport(glphViewport);
	glyphRenderer->AddActor(glyphActor);
	glyphRenderer->AddActor(normedActor);
	glyphRenderer->ResetCamera();
	glyphRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(originalRenderer);
	renderWindow->AddRenderer(normedRenderer);
	renderWindow->AddRenderer(glyphRenderer);
	renderWindow->SetSize(640, 320);
	renderWindow->Render();
	renderWindow->SetWindowName("PolyDataNormal");

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindow->Render();
	
	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}