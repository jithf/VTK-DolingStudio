/**********************************************************************

  �ļ���: 6.6_PolyDataDecimation.cpp
  Copyright (c) ������, �޻���. All rights reserved.
  ������Ϣ�����: 
    http://www.vtkchina.org (VTK�й�)
	http://blog.csdn.net/www_doling_net (���鹤����) 

**********************************************************************/

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   // ���裺��ʼ��OpenGL��Ⱦ���
VTK_MODULE_INIT(vtkInteractionStyle);   // ���裺��ʼ������ģ��

#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkDecimatePro.h>
#include <vtkQuadricDecimation.h>
#include <vtkQuadricClustering.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkPolydataReader.h>
#include <vtkCamera.h>

//�����ļ���../data/fran_cut.vtk
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

	std::cout << "��ȡǰ��" << std::endl << "------------" << std::endl;
	std::cout << "ģ�͵���Ϊ�� " << original->GetNumberOfPoints() << std::endl;
	std::cout << "ģ������Ϊ�� " << original->GetNumberOfPolys() << std::endl;

	// ģ�ͳ�ȡ
	vtkSmartPointer<vtkDecimatePro> decimate = vtkSmartPointer<vtkDecimatePro>::New();
	decimate->SetInputData(original);
	decimate->SetTargetReduction(.80);  // ��ȡ����  80%�ĵ�Ԫ���ݽ���ɾ��
	decimate->Update();

	vtkSmartPointer<vtkPolyData> decimated = decimate->GetOutput();
	std::cout << "��ȡ��" << std::endl << "------------" << std::endl;
	std::cout << "ģ�͵���Ϊ��" << decimated->GetNumberOfPoints()<< std::endl;
	std::cout << "ģ������Ϊ��" << decimated->GetNumberOfPolys()<< std::endl;

	vtkSmartPointer<vtkPolyDataMapper> origianlMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	origianlMapper->SetInputData(original);

	vtkSmartPointer<vtkActor> origianlActor =
		vtkSmartPointer<vtkActor>::New();
	origianlActor->SetMapper(origianlMapper);

	vtkSmartPointer<vtkPolyDataMapper> decimatedMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	decimatedMapper->SetInputData(decimated);

	vtkSmartPointer<vtkActor> decimatedActor =
		vtkSmartPointer<vtkActor>::New();
	decimatedActor->SetMapper(decimatedMapper);

	double leftViewport[4] = {0.0, 0.0, 0.5, 1.0};
	double rightViewport[4] = {0.5, 0.0, 1.0, 1.0};

	vtkSmartPointer<vtkRenderer> leftRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	leftRenderer->SetViewport(leftViewport);
	leftRenderer->AddActor(origianlActor);
	leftRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> rightRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	rightRenderer->SetViewport(rightViewport);
	rightRenderer->AddActor(decimatedActor);
	rightRenderer->SetBackground(1.0, 1.0, 1.0);

	leftRenderer->GetActiveCamera()->SetPosition(0, -1, 0);
	leftRenderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	leftRenderer->GetActiveCamera()->SetViewUp(0, 0, 1);
	leftRenderer->GetActiveCamera()->Azimuth(30);
	leftRenderer->GetActiveCamera()->Elevation(30);
	leftRenderer->ResetCamera();
	rightRenderer->SetActiveCamera(leftRenderer->GetActiveCamera());

	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(leftRenderer);
	renderWindow->AddRenderer(rightRenderer);
	renderWindow->SetSize(640, 320);
	renderWindow->Render();
	renderWindow->SetWindowName("PolyDataDecimation");

	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);

	renderWindow->Render();
	interactor->Start();

	return EXIT_SUCCESS;
}