/**********************************************************************

  �ļ���: 6.2_PolyDataNormal.cpp
  Copyright (c) ������, �޻���. All rights reserved.
  ������Ϣ�����: 
    http://www.vtkchina.org (VTK�й�)
	http://blog.csdn.net/www_doling_net (���鹤����) 

**********************************************************************/

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   // ���裺��ʼ��OpenGL��Ⱦ���
VTK_MODULE_INIT(vtkInteractionStyle);   // ���裺��ʼ������ģ��

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

//�����ļ���../data/fran_cut.vtk
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

	// ���㷨����
	vtkSmartPointer<vtkPolyDataNormals> normFilter =  vtkSmartPointer<vtkPolyDataNormals>::New();
	normFilter->SetInputData(reader->GetOutput());
	normFilter->SetComputePointNormals(1);  // ����㼯������
	normFilter->SetComputeCellNormals(0);  // ���㵥Ԫ��������
	normFilter->SetConsistency(1);  // �Զ�������Ԫ���˳��
	normFilter->SetAutoOrientNormals(1);  // �Զ��������߷��� ���ֶ���
	normFilter->SetSplitting(0);  // �ر����Ե���� ���������Ե����ģ�ͷ��ѣ����ݷ����仯
	normFilter->Update();

	// ����������
	vtkSmartPointer<vtkMaskPoints> mask =  vtkSmartPointer<vtkMaskPoints>::New();
	mask->SetInputData(normFilter->GetOutput());
	mask->SetMaximumNumberOfPoints(300);  // �㼯������������
	mask->RandomModeOn();  // ���ѡ���
	mask->Update();

	// ����Glyphͼ����ʾ������
	vtkSmartPointer<vtkArrowSource> arrow =  vtkSmartPointer<vtkArrowSource>::New();
	arrow->Update();
	vtkSmartPointer<vtkGlyph3D> glyph =  vtkSmartPointer<vtkGlyph3D>::New();
	glyph->SetInputData(mask->GetOutput());
	glyph->SetSourceData(arrow->GetOutput());   // �������ݵ�ÿ���㴦������ʾһ��Glyphͼ�� ��ͷ
	glyph->SetVectorModeToUseNormal(); // ָ��ʹ�÷���������Glyphͼ��
	glyph->SetScaleFactor(0.01); // Glyphͼ�εĴ�С
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