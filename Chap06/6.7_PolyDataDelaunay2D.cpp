/**********************************************************************

  文件名: 6.7_PolyDataDelaunay2D.cpp
  Copyright (c) 张晓东, 罗火灵. All rights reserved.
  更多信息请访问: 
    http://www.vtkchina.org (VTK中国)
	http://blog.csdn.net/www_doling_net (东灵工作室) 

**********************************************************************/

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   // 必需：初始化OpenGL渲染后端
VTK_MODULE_INIT(vtkInteractionStyle);   // 必需：初始化交互模块

#include <vtkSmartPointer.h>
#include <vtkProperty.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkDelaunay2D.h>
#include <vtkMath.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkVertexGlyphFilter.h>

int main(int, char *[])
{
	unsigned int gridSize = 10;
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	for(unsigned int x = 0; x < gridSize; x++)
	{
		for(unsigned int y = 0; y < gridSize; y++)
		{
			points->InsertNextPoint(x, y, vtkMath::Random(0.0, 3.0));  //随机点
		}
	}

	vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
	polydata->SetPoints(points);

	// 三角剖分 曲面重建
	vtkSmartPointer<vtkDelaunay2D> delaunay = vtkSmartPointer<vtkDelaunay2D>::New();
	delaunay->SetInputData(polydata);
	delaunay->Update();

	// 给点集加图标
	vtkSmartPointer<vtkVertexGlyphFilter> glyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
	glyphFilter->SetInputData(polydata);
	glyphFilter->Update();

	vtkSmartPointer<vtkPolyDataMapper> pointsMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	pointsMapper->SetInputData(glyphFilter->GetOutput());
	vtkSmartPointer<vtkActor> pointsActor = vtkSmartPointer<vtkActor>::New();
	pointsActor->SetMapper(pointsMapper);
	pointsActor->GetProperty()->SetPointSize(3);
	pointsActor->GetProperty()->SetColor(1,0,0);

	vtkSmartPointer<vtkPolyDataMapper> triangulatedMapper =vtkSmartPointer<vtkPolyDataMapper>::New();
	triangulatedMapper->SetInputData(delaunay->GetOutput());
	vtkSmartPointer<vtkActor> triangulatedActor =vtkSmartPointer<vtkActor>::New();
	triangulatedActor->SetMapper(triangulatedMapper);

	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(pointsActor);
	renderer->AddActor(triangulatedActor);
	renderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);

	renderWindow->SetSize(640, 320);
	renderWindow->Render();
	renderWindow->SetWindowName("PolyDataDelaunay2D");
	renderWindow->Render();

	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}