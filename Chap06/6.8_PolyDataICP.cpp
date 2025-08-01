/**********************************************************************

  文件名: 6.8_PolyDataICP.cpp
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
#include <vtkIterativeClosestPointTransform.h>
#include <vtkPolyDataReader.h>
#include <vtkTransform.h>

//测试文件：../data/fran_cut.vtk
int main(int argc, char * argv[])
{
	//if(argc < 2)
	//{
	//	std::cout<<argv[0]<<" *.vtk"<<std::endl;
	//	return EXIT_FAILURE;
	//}

	vtkSmartPointer<vtkPolyDataReader> reader =  vtkSmartPointer<vtkPolyDataReader>::New();
	reader->SetFileName("C:\\Users\\luhy\\Desktop\\data\\fran_cut.vtk");
	reader->Update();
	vtkSmartPointer<vtkPolyData> original  =  reader->GetOutput();

	// 变换矩阵
	vtkSmartPointer<vtkTransform> translation = vtkSmartPointer<vtkTransform>::New();
	translation->Translate(0.2, 0.0, 0.0);
	translation->RotateX(30);
	// 对点集进行变换
	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter1 = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter1->SetInputData(reader->GetOutput());
	transformFilter1->SetTransform(translation);
	transformFilter1->Update();

	//源点集
	vtkSmartPointer<vtkPolyData> source = vtkSmartPointer<vtkPolyData>::New();
	source->SetPoints(original->GetPoints());
	vtkSmartPointer<vtkVertexGlyphFilter> sourceGlyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
	sourceGlyphFilter->SetInputData(source);
	sourceGlyphFilter->Update();
	vtkSmartPointer<vtkPolyDataMapper> sourceMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	sourceMapper->SetInputConnection(sourceGlyphFilter->GetOutputPort());
	vtkSmartPointer<vtkActor> sourceActor = vtkSmartPointer<vtkActor>::New();
	sourceActor->SetMapper(sourceMapper);
	sourceActor->GetProperty()->SetColor(0, 1, 0);
	sourceActor->GetProperty()->SetPointSize(3);

	//目标点集
	vtkSmartPointer<vtkPolyData> target = vtkSmartPointer<vtkPolyData>::New();
	target->SetPoints(transformFilter1->GetOutput()->GetPoints());
	vtkSmartPointer<vtkVertexGlyphFilter> targetGlyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
	targetGlyphFilter->SetInputData(target);
	targetGlyphFilter->Update();
	vtkSmartPointer<vtkPolyDataMapper> targetMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	targetMapper->SetInputConnection(targetGlyphFilter->GetOutputPort());
	vtkSmartPointer<vtkActor> targetActor = vtkSmartPointer<vtkActor>::New();
	targetActor->SetMapper(targetMapper);
	targetActor->GetProperty()->SetColor(1, 0, 0);
	targetActor->GetProperty()->SetPointSize(3);

	// ICP 点云配准
	vtkSmartPointer<vtkIterativeClosestPointTransform> icpTransform =  vtkSmartPointer<vtkIterativeClosestPointTransform>::New();
	icpTransform->SetSource(sourceGlyphFilter->GetOutput());
	icpTransform->SetTarget(targetGlyphFilter->GetOutput());
	icpTransform->GetLandmarkTransform()->SetModeToRigidBody();  //配准变换类型为刚体变换 即只有平移和旋转
	icpTransform->SetMaximumNumberOfIterations(20); // 最大迭代次数
	icpTransform->StartByMatchingCentroidsOn();  // 配准之前先计算两 个点集重心，并平移源点集使得两者重心重合
	icpTransform->Modified(); // 通知 VTK 流水线，该对象的参数已修改，需要重新计算
	icpTransform->Update();  // 计算变换矩阵

	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter2 = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter2->SetInputData(sourceGlyphFilter->GetOutput());
	transformFilter2->SetTransform(icpTransform);  // 进行配准变换
	transformFilter2->Update();
	vtkSmartPointer<vtkPolyDataMapper> solutionMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	solutionMapper->SetInputConnection(transformFilter2->GetOutputPort());
	vtkSmartPointer<vtkActor> solutionActor = vtkSmartPointer<vtkActor>::New();
	solutionActor->SetMapper(solutionMapper);
	solutionActor->GetProperty()->SetColor(0,0,1);
	solutionActor->GetProperty()->SetPointSize(3);

	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	renderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	renderer->AddActor(sourceActor);
	renderer->AddActor(targetActor);
	renderer->AddActor(solutionActor);

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);

	renderWindow->SetSize(640, 480);
	renderWindow->Render();
	renderWindow->SetWindowName("PolyDataICP");
	renderWindow->Render();
	renderWindow->Render();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}