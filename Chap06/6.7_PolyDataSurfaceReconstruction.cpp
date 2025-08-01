/**********************************************************************

  文件名: 6.7_PolyDataSurfaceReconstruction.cpp
  Copyright (c) 张晓东, 罗火灵. All rights reserved.
  更多信息请访问: 
    http://www.vtkchina.org (VTK中国)
	http://blog.csdn.net/www_doling_net (东灵工作室) 

**********************************************************************/

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   // 必需：初始化OpenGL渲染后端
VTK_MODULE_INIT(vtkInteractionStyle);   // 必需：初始化交互模块

#include <vtkSmartPointer.h>
#include <vtkSurfaceReconstructionFilter.h>
#include <vtkProgrammableSource.h>
#include <vtkContourFilter.h>
#include <vtkReverseSense.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkPolyData.h>
#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>
#include <vtkPolydataReader.h>
#include <vtkVertexGlyphFilter.h>

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

	// 点集
	vtkSmartPointer<vtkPolyData> points =  vtkSmartPointer<vtkPolyData>::New();
	points->SetPoints(reader->GetOutput()->GetPoints());
	
	// 点云重建
	vtkSmartPointer<vtkSurfaceReconstructionFilter> surf =  vtkSmartPointer<vtkSurfaceReconstructionFilter>::New();
	surf->SetInputData(points);
	surf->SetNeighborhoodSize(20); // 领域点的个数
	surf->SetSampleSpacing(0.005); // 网格间隔
	surf->Update();

	// 等值面提取
	vtkSmartPointer<vtkContourFilter> contour =  vtkSmartPointer<vtkContourFilter>::New();
	contour->SetInputConnection(surf->GetOutputPort());
	contour->SetValue(0, 0.0); //提起值为0.0的等值面
	contour->Update();

	double leftViewport[4] = {0.0, 0.0, 0.5, 1.0};
	double rightViewport[4] = {0.5, 0.0, 1.0, 1.0};

	// 给点集加图标
	vtkSmartPointer<vtkVertexGlyphFilter> vertexGlyphFilter =
		vtkSmartPointer<vtkVertexGlyphFilter>::New();
	vertexGlyphFilter->AddInputData(points);
	vertexGlyphFilter->Update();

	vtkSmartPointer<vtkPolyDataMapper> vertexMapper = 
		vtkSmartPointer<vtkPolyDataMapper>::New();
	vertexMapper->SetInputData(vertexGlyphFilter->GetOutput());
	vertexMapper->ScalarVisibilityOff();
	vtkSmartPointer<vtkActor> vertexActor = 
		vtkSmartPointer<vtkActor>::New();
	vertexActor->SetMapper(vertexMapper);
	vertexActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
	vtkSmartPointer<vtkRenderer> vertexRenderer = 
		vtkSmartPointer<vtkRenderer>::New();
	vertexRenderer->AddActor(vertexActor);
	vertexRenderer->SetViewport(leftViewport);
	vertexRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkPolyDataMapper> surfMapper = 
		vtkSmartPointer<vtkPolyDataMapper>::New();
	surfMapper->SetInputData(contour->GetOutput());
	surfMapper->ScalarVisibilityOff();
	vtkSmartPointer<vtkActor> surfActor = 
		vtkSmartPointer<vtkActor>::New();
	surfActor->SetMapper(surfMapper);
	surfActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
	vtkSmartPointer<vtkRenderer> surfRenderer = 
		vtkSmartPointer<vtkRenderer>::New();
	surfRenderer->AddActor(surfActor);
	surfRenderer->SetViewport(rightViewport);
	surfRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renWin = 
		vtkSmartPointer<vtkRenderWindow>::New();
	renWin->AddRenderer(surfRenderer);
	renWin->AddRenderer(vertexRenderer);
	renWin->SetSize(640, 320);
	renWin->Render();
	renWin->SetWindowName("PolyDataSurfaceReconstruction");

	vtkSmartPointer<vtkRenderWindowInteractor> iren = 
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);

	renWin->Render();
	iren->Start();

	return EXIT_SUCCESS;
}