/**********************************************************************

  �ļ���: 2.1_RenderCylinder.cpp
  Copyright (c) ������, �޻���. All rights reserved.
  ������Ϣ�����: 
    http://www.vtkchina.org (VTK�й�)
	http://blog.csdn.net/www_doling_net (���鹤����) 

**********************************************************************/

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   // ���裺��ʼ��OpenGL��Ⱦ���
VTK_MODULE_INIT(vtkInteractionStyle);   // ���裺��ʼ������ģ��

#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>

int main()
{
	vtkSmartPointer<vtkCylinderSource> cylinder = vtkSmartPointer<vtkCylinderSource>::New();  // ����Բ����
	cylinder->SetHeight( 3.0 );
	cylinder->SetRadius( 1.0 );
	cylinder->SetResolution( 10 );  // ���������������εı���

	vtkSmartPointer<vtkPolyDataMapper> cylinderMapper =  vtkSmartPointer<vtkPolyDataMapper>::New();  // ��άת��ά
	cylinderMapper->SetInputConnection( cylinder->GetOutputPort() ); 

	vtkSmartPointer<vtkActor> cylinderActor =  vtkSmartPointer<vtkActor>::New();  // ������Ⱦ����
	cylinderActor->SetMapper( cylinderMapper );
	cylinderActor->GetProperty()->SetColor(1.0, 0.0, 0.0); // ������ɫ 

	vtkSmartPointer<vtkRenderer> renderer =  vtkSmartPointer<vtkRenderer>::New();  // ��Ⱦ����
	renderer->AddActor( cylinderActor );
	renderer->SetBackground( 1.0, 1.0, 1.0 );  // ���ñ�����ɫ

	vtkSmartPointer<vtkRenderWindow> renWin =  vtkSmartPointer<vtkRenderWindow>::New();  // ��Ⱦ����
	renWin->AddRenderer( renderer );
	renWin->SetSize( 640, 480 );
	renWin->Render(); // ��Ⱦ
	renWin->SetWindowName("RenderCylinder");

	vtkSmartPointer<vtkRenderWindowInteractor> iren =  vtkSmartPointer<vtkRenderWindowInteractor>::New(); // ��Ⱦ���ڽ����� �������ͼ����¼� ������InceractorStyle����
	iren->SetRenderWindow(renWin);

	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =  vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();  // ��������ʽ��һ�� ͨ���ƶ����ʵ�ֶ�����ķŴ���С�Ȳ���
	iren->SetInteractorStyle(style);

	iren->Initialize();  // ��ʼ�������� 
	iren->Start();  // �����¼���Ӧѭ��

	return EXIT_SUCCESS;
}