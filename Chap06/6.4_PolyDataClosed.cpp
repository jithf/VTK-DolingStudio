/**********************************************************************

  文件名: 6.4_PolyDataClosed.cpp
  Copyright (c) 张晓东, 罗火灵. All rights reserved.
  更多信息请访问: 
    http://www.vtkchina.org (VTK中国)
	http://blog.csdn.net/www_doling_net (东灵工作室) 

**********************************************************************/

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   // 必需：初始化OpenGL渲染后端
VTK_MODULE_INIT(vtkInteractionStyle);   // 必需：初始化交互模块

#include <vtkSmartPointer.h>
#include <vtkSelectionNode.h>
#include <vtkInformation.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPolyData.h>
#include <vtkPolyDataNormals.h>
#include <vtkPointData.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSelection.h>
#include <vtkSelectionNode.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkProperty.h>
#include <vtkIdTypeArray.h>
#include <vtkExtractSelection.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkFeatureEdges.h>
#include <vtkFillHolesFilter.h>


//生成网格数据
void GenerateData(vtkSmartPointer<vtkPolyData> input)
{
	// 球体
	vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();  
	sphereSource->Update();

	// Id类型数组
	vtkSmartPointer<vtkIdTypeArray> ids = vtkSmartPointer<vtkIdTypeArray>::New();
	ids->SetNumberOfComponents(1);
	ids->InsertNextValue(2);
	ids->InsertNextValue(10);

	// 选择节点
	vtkSmartPointer<vtkSelectionNode> selectionNode = vtkSmartPointer<vtkSelectionNode>::New();
	selectionNode->SetFieldType(vtkSelectionNode::CELL);  // 选择单元格
	selectionNode->SetContentType(vtkSelectionNode::INDICES);  //选择的是索引
	selectionNode->SetSelectionList(ids);  // 选择哪些索引 选择列表
	selectionNode->GetProperties()->Set(vtkSelectionNode::INVERSE(), 1);  // 反向选择 选择列表外的节点

	// 选择
	vtkSmartPointer<vtkSelection> selection = vtkSmartPointer<vtkSelection>::New();
	selection->AddNode(selectionNode);

	// 执行选择
	vtkSmartPointer<vtkExtractSelection> extractSelection = vtkSmartPointer<vtkExtractSelection>::New();
	extractSelection->SetInputData(0, sphereSource->GetOutput());
	extractSelection->SetInputData(1, selection);
	extractSelection->Update();

	// 提取选择结果中的表面数据
	vtkSmartPointer<vtkDataSetSurfaceFilter> surfaceFilter = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
	surfaceFilter->SetInputConnection(extractSelection->GetOutputPort());
	surfaceFilter->Update();

	input->ShallowCopy(surfaceFilter->GetOutput());  // 浅拷贝
}

int main(int argc, char *argv[])
{
	vtkSmartPointer<vtkPolyData> input = vtkSmartPointer<vtkPolyData>::New();
	GenerateData(input);

	vtkSmartPointer<vtkFeatureEdges> featureEdges = vtkSmartPointer<vtkFeatureEdges>::New();
	featureEdges->SetInputData(input);
	featureEdges->BoundaryEdgesOn();  //边界边
	featureEdges->FeatureEdgesOff();  //特征边
	featureEdges->ManifoldEdgesOff();  //流形边
	featureEdges->NonManifoldEdgesOff();  //非流形边
	featureEdges->Update();

	int numberOfOpenEdges = featureEdges->GetOutput()->GetNumberOfCells();  // 结果的数量
	if(numberOfOpenEdges) 
	{
		std::cout<<"该网格模型不是封闭的..."<<std::endl;
	}
	else
	{
		std::cout<<"该网格模型是封闭的..."<<std::endl;
		return EXIT_SUCCESS;
	}

	// 填充孔洞
	vtkSmartPointer<vtkFillHolesFilter> fillHolesFilter = vtkSmartPointer<vtkFillHolesFilter>::New();
	fillHolesFilter->SetInputData(input);
	fillHolesFilter->Update();

	// 计算法向量
	vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
	normals->SetInputConnection(fillHolesFilter->GetOutputPort());
	normals->ConsistencyOn();  // 自动调整单元点的顺序
	normals->SplittingOff();  // 关闭锐边缘处理 避免因锐边缘导致模型分裂，数据发生变化
	normals->Update();

	//////////////////////////////////////////////////////////////////////////
	double leftViewport[4] = {0.0, 0.0, 0.5, 1.0};
	double rightViewport[4] = {0.5, 0.0, 1.0, 1.0};

	vtkSmartPointer<vtkPolyDataMapper> originalMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	originalMapper->SetInputData(input);
	vtkSmartPointer<vtkProperty> backfaceProp = vtkSmartPointer<vtkProperty>::New();
	backfaceProp->SetDiffuseColor(0.89,0.81,0.34);
	vtkSmartPointer<vtkActor> originalActor = vtkSmartPointer<vtkActor>::New();
	originalActor->SetMapper(originalMapper);
	originalActor->SetBackfaceProperty(backfaceProp);
	originalActor->GetProperty()->SetDiffuseColor(1.0, 0.3882, 0.2784);

	vtkSmartPointer<vtkPolyDataMapper> edgeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	edgeMapper->SetInputData(featureEdges->GetOutput());
	vtkSmartPointer<vtkActor> edgeActor = vtkSmartPointer<vtkActor>::New();
	edgeActor->SetMapper(edgeMapper);
	edgeActor->GetProperty()->SetEdgeColor(0.,0.,1.0);
	edgeActor->GetProperty()->SetEdgeVisibility(1);
	edgeActor->GetProperty()->SetLineWidth(5);

	vtkSmartPointer<vtkPolyDataMapper> filledMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	filledMapper->SetInputData(normals->GetOutput());
	vtkSmartPointer<vtkActor> filledActor = vtkSmartPointer<vtkActor>::New();
	filledActor->SetMapper(filledMapper);
	filledActor->GetProperty()->SetDiffuseColor(1.0, 0.3882, 0.2784);

	vtkSmartPointer<vtkRenderer> leftRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	leftRenderer->SetViewport(leftViewport);
	leftRenderer->AddActor(originalActor);
	leftRenderer->AddActor(edgeActor);
	leftRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> rightRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	rightRenderer->SetViewport(rightViewport);
	rightRenderer->AddActor(filledActor);
	rightRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(leftRenderer);
	renderWindow->AddRenderer(rightRenderer);
	renderWindow->SetSize(640, 320);
	renderWindow->Render();
	renderWindow->SetWindowName("PolyDataClosed");

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);

	leftRenderer->GetActiveCamera()->SetPosition(0, -1, 0);
	leftRenderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	leftRenderer->GetActiveCamera()->SetViewUp(0, 0, 1);
	leftRenderer->GetActiveCamera()->Azimuth(30);  // 水平旋转摄像机（方位角）
	leftRenderer->GetActiveCamera()->Elevation(30);  // 垂直旋转摄像机（仰角）
	leftRenderer->ResetCamera();
	rightRenderer->SetActiveCamera(leftRenderer->GetActiveCamera());

	renderWindow->Render();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}

