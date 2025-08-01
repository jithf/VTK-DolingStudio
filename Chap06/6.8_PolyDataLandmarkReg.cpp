/**********************************************************************

  文件名: 6.8_PolyDataLandmarkReg.cpp
  Copyright (c) 张晓东, 罗火灵. All rights reserved.
  更多信息请访问: 
    http://www.vtkchina.org (VTK中国)
	http://blog.csdn.net/www_doling_net (东灵工作室) 

**********************************************************************/

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   // 必需：初始化OpenGL渲染后端
VTK_MODULE_INIT(vtkInteractionStyle);   // 必需：初始化交互模块

#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <vtkLandmarkTransform.h>
#include <vtkMatrix4x4.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkAxesActor.h>

int main(int, char *[])
{
	//源点集
	vtkSmartPointer<vtkPoints> sourcePoints = vtkSmartPointer<vtkPoints>::New();
	double sourcePoint1[3] = {0.5, 0.0, 0.0};
	sourcePoints->InsertNextPoint(sourcePoint1);
	double sourcePoint2[3] = {0.0, 0.5, 0.0};
	sourcePoints->InsertNextPoint(sourcePoint2);
	double sourcePoint3[3] = {0.0, 0.0, 0.5};
	sourcePoints->InsertNextPoint(sourcePoint3);

	//目标点集
	vtkSmartPointer<vtkPoints> targetPoints = vtkSmartPointer<vtkPoints>::New();
	double targetPoint1[3] = {0.0, 0.0, 0.55};
	targetPoints->InsertNextPoint(targetPoint1);
	double targetPoint2[3] = {0.0, 0.55, 0.0};
	targetPoints->InsertNextPoint(targetPoint2);
	double targetPoint3[3] = {-0.55, 0.0, 0.0};
	targetPoints->InsertNextPoint(targetPoint3);

	//点云配准
	vtkSmartPointer<vtkLandmarkTransform> landmarkTransform =  vtkSmartPointer<vtkLandmarkTransform>::New();
	landmarkTransform->SetSourceLandmarks(sourcePoints);  //源点集
	landmarkTransform->SetTargetLandmarks(targetPoints);   //目标点集
	landmarkTransform->SetModeToRigidBody();  //配准变换类型为刚体变换 即只有平移和旋转
	landmarkTransform->Update(); 

	//显示源点集
	vtkSmartPointer<vtkPolyData> source = vtkSmartPointer<vtkPolyData>::New();
	source->SetPoints(sourcePoints);
	vtkSmartPointer<vtkVertexGlyphFilter> sourceGlyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
	sourceGlyphFilter->SetInputData(source);
	sourceGlyphFilter->Update();
	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter->SetInputData(sourceGlyphFilter->GetOutput());
	transformFilter->SetTransform(landmarkTransform);
	transformFilter->Update();
	vtkSmartPointer<vtkPolyDataMapper> sourceMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	sourceMapper->SetInputConnection(sourceGlyphFilter->GetOutputPort());
	vtkSmartPointer<vtkActor> sourceActor = vtkSmartPointer<vtkActor>::New();
	sourceActor->SetMapper(sourceMapper);
	sourceActor->GetProperty()->SetColor(1, 1, 0);
	sourceActor->GetProperty()->SetPointSize(5);

	//显示目标点集
	vtkSmartPointer<vtkPolyData> target = vtkSmartPointer<vtkPolyData>::New();
	target->SetPoints(targetPoints);
	vtkSmartPointer<vtkVertexGlyphFilter> targetGlyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
	targetGlyphFilter->SetInputData(target);
	targetGlyphFilter->Update();
	vtkSmartPointer<vtkPolyDataMapper> targetMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	targetMapper->SetInputConnection(targetGlyphFilter->GetOutputPort());
	vtkSmartPointer<vtkActor> targetActor = vtkSmartPointer<vtkActor>::New();
	targetActor->SetMapper(targetMapper);
	targetActor->GetProperty()->SetColor(1,0,0);
	targetActor->GetProperty()->SetPointSize(5);

	//显示配准后的点集
	vtkSmartPointer<vtkPolyDataMapper> solutionMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	solutionMapper->SetInputConnection(transformFilter->GetOutputPort());
	vtkSmartPointer<vtkActor> solutionActor = vtkSmartPointer<vtkActor>::New();
	solutionActor->SetMapper(solutionMapper);
	solutionActor->GetProperty()->SetColor(0,0,1);
	solutionActor->GetProperty()->SetPointSize(5);

	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();

	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	renderer->AddActor(sourceActor);
	renderer->AddActor(targetActor);
	renderer->AddActor(solutionActor);

	vtkSmartPointer<vtkAxesActor> axes =
		vtkSmartPointer<vtkAxesActor>::New();
	axes->SetScale(30);
	renderer->AddActor(axes);
	renderer->SetBackground(.3, .6, .3);

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);

	renderWindow->SetSize(640, 480);
	renderWindow->Render();
	renderWindow->SetWindowName("PolyDataLandmarkReg");
	renderWindow->Render();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}