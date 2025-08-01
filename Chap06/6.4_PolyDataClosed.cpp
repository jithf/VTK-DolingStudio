/**********************************************************************

  �ļ���: 6.4_PolyDataClosed.cpp
  Copyright (c) ������, �޻���. All rights reserved.
  ������Ϣ�����: 
    http://www.vtkchina.org (VTK�й�)
	http://blog.csdn.net/www_doling_net (���鹤����) 

**********************************************************************/

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   // ���裺��ʼ��OpenGL��Ⱦ���
VTK_MODULE_INIT(vtkInteractionStyle);   // ���裺��ʼ������ģ��

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


//������������
void GenerateData(vtkSmartPointer<vtkPolyData> input)
{
	// ����
	vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();  
	sphereSource->Update();

	// Id��������
	vtkSmartPointer<vtkIdTypeArray> ids = vtkSmartPointer<vtkIdTypeArray>::New();
	ids->SetNumberOfComponents(1);
	ids->InsertNextValue(2);
	ids->InsertNextValue(10);

	// ѡ��ڵ�
	vtkSmartPointer<vtkSelectionNode> selectionNode = vtkSmartPointer<vtkSelectionNode>::New();
	selectionNode->SetFieldType(vtkSelectionNode::CELL);  // ѡ��Ԫ��
	selectionNode->SetContentType(vtkSelectionNode::INDICES);  //ѡ���������
	selectionNode->SetSelectionList(ids);  // ѡ����Щ���� ѡ���б�
	selectionNode->GetProperties()->Set(vtkSelectionNode::INVERSE(), 1);  // ����ѡ�� ѡ���б���Ľڵ�

	// ѡ��
	vtkSmartPointer<vtkSelection> selection = vtkSmartPointer<vtkSelection>::New();
	selection->AddNode(selectionNode);

	// ִ��ѡ��
	vtkSmartPointer<vtkExtractSelection> extractSelection = vtkSmartPointer<vtkExtractSelection>::New();
	extractSelection->SetInputData(0, sphereSource->GetOutput());
	extractSelection->SetInputData(1, selection);
	extractSelection->Update();

	// ��ȡѡ�����еı�������
	vtkSmartPointer<vtkDataSetSurfaceFilter> surfaceFilter = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
	surfaceFilter->SetInputConnection(extractSelection->GetOutputPort());
	surfaceFilter->Update();

	input->ShallowCopy(surfaceFilter->GetOutput());  // ǳ����
}

int main(int argc, char *argv[])
{
	vtkSmartPointer<vtkPolyData> input = vtkSmartPointer<vtkPolyData>::New();
	GenerateData(input);

	vtkSmartPointer<vtkFeatureEdges> featureEdges = vtkSmartPointer<vtkFeatureEdges>::New();
	featureEdges->SetInputData(input);
	featureEdges->BoundaryEdgesOn();  //�߽��
	featureEdges->FeatureEdgesOff();  //������
	featureEdges->ManifoldEdgesOff();  //���α�
	featureEdges->NonManifoldEdgesOff();  //�����α�
	featureEdges->Update();

	int numberOfOpenEdges = featureEdges->GetOutput()->GetNumberOfCells();  // ���������
	if(numberOfOpenEdges) 
	{
		std::cout<<"������ģ�Ͳ��Ƿ�յ�..."<<std::endl;
	}
	else
	{
		std::cout<<"������ģ���Ƿ�յ�..."<<std::endl;
		return EXIT_SUCCESS;
	}

	// ���׶�
	vtkSmartPointer<vtkFillHolesFilter> fillHolesFilter = vtkSmartPointer<vtkFillHolesFilter>::New();
	fillHolesFilter->SetInputData(input);
	fillHolesFilter->Update();

	// ���㷨����
	vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
	normals->SetInputConnection(fillHolesFilter->GetOutputPort());
	normals->ConsistencyOn();  // �Զ�������Ԫ���˳��
	normals->SplittingOff();  // �ر����Ե���� ���������Ե����ģ�ͷ��ѣ����ݷ����仯
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
	leftRenderer->GetActiveCamera()->Azimuth(30);  // ˮƽ��ת���������λ�ǣ�
	leftRenderer->GetActiveCamera()->Elevation(30);  // ��ֱ��ת����������ǣ�
	leftRenderer->ResetCamera();
	rightRenderer->SetActiveCamera(leftRenderer->GetActiveCamera());

	renderWindow->Render();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}

