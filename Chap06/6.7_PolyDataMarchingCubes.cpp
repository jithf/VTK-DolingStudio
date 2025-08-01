/**********************************************************************

  �ļ���: 6.7_PolyDataMarchingCubes.cpp
  Copyright (c) ������, �޻���. All rights reserved.
  ������Ϣ�����: 
    http://www.vtkchina.org (VTK�й�)
	http://blog.csdn.net/www_doling_net (���鹤����) 

**********************************************************************/

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   // ���裺��ʼ��OpenGL��Ⱦ���
VTK_MODULE_INIT(vtkInteractionStyle);   // ���裺��ʼ������ģ��

#include <vtkSmartPointer.h>
#include <vtkMarchingCubes.h>
#include <vtkContourFilter.h>
#include <vtkVoxelModeller.h>
#include <vtkSphereSource.h>
#include <vtkImageData.h>
#include <vtkMetaImageReader.h>
#include <vtkInteractorStyleImage.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkImageActor.h>
#include <vtkProperty.h>

//���ԣ�../data/HeadMRVolume.mhd 200
int main(int argc, char *argv[])
{
	//if(argc < 3)
	//{
	//	std::cout<<argv[0]<<" *.mhd isoValue"<<std::endl;
	//	return EXIT_FAILURE;
	//}

	vtkSmartPointer<vtkMetaImageReader> reader = vtkSmartPointer<vtkMetaImageReader>::New();
	reader->SetFileName("C:\\Users\\luhy\\Desktop\\data\\HeadMRVolume.mhd");
	reader->Update();

	double isoValue = 200;

	// ��ֵ����ȡ
	vtkSmartPointer<vtkMarchingCubes> surface =  vtkSmartPointer<vtkMarchingCubes>::New();
	surface->SetInputData(reader->GetOutput());
	surface->ComputeNormalsOn(); // ���㷨���� �����Ⱦ����
	surface->SetValue(0, isoValue);  // ������ֵ ������index ,��ֵ
	//surface->GenerateValues(5, 150,200);  // ��150-200����5����ֵ����ֵ
	surface->Update();

	/*vtkSmartPointer<vtkContourFilter> surface = 
	vtkSmartPointer<vtkContourFilter>::New();
	surface->SetInput(reader->GetOutput());
	surface->ComputeNormalsOn();
	surface->SetValue(0, isoValue);*/

	vtkSmartPointer<vtkPolyDataMapper> surfMapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
	surfMapper->SetInputConnection(surface->GetOutputPort());
	surfMapper->ScalarVisibilityOff();  // �رձ������ݿ��ӻ�, ��ʱģ�͵���ɫ�����������vtkActor������ָ��
	vtkSmartPointer<vtkActor> surfActor = vtkSmartPointer<vtkActor>::New();
	surfActor->SetMapper(surfMapper);
	surfActor->GetProperty()->SetColor(1.0, 0.0, 0.0);

	vtkSmartPointer<vtkRenderer> renderer = 
		vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(surfActor);
	renderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow = 
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	renderWindow->SetSize(640, 480);
	renderWindow->Render();
	renderWindow->SetWindowName("PolyDataMarchingCubes");
	renderWindow->Render();

	vtkSmartPointer<vtkRenderWindowInteractor> interactor = 
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);

	renderWindow->Render();
	interactor->Start();

	return EXIT_SUCCESS;
}