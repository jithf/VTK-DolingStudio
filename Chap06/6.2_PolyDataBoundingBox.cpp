/**********************************************************************

  �ļ���: 6.2_PolyDataBoundingBox.cpp
  Copyright (c) ������, �޻���. All rights reserved.
  ������Ϣ�����: 
    http://www.vtkchina.org (VTK�й�)
	http://blog.csdn.net/www_doling_net (���鹤����) 

**********************************************************************/

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   // ���裺��ʼ��OpenGL��Ⱦ���
VTK_MODULE_INIT(vtkInteractionStyle);   // ���裺��ʼ������ģ��

#include <vtkSphereSource.h>
#include <vtkProperty.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkOutlineFilter.h>

int main(int, char *[])
{
	vtkSmartPointer<vtkSphereSource> sphereSource =  vtkSmartPointer<vtkSphereSource>::New();
	sphereSource->SetCenter(0.0, 0.0, 0.0);
	sphereSource->SetRadius(5.0);
	sphereSource->Update();

	vtkPolyData* sphere = sphereSource->GetOutput();
	vtkSmartPointer<vtkPolyDataMapper> mapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(sphere);

	vtkSmartPointer<vtkActor> actor =  vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	vtkSmartPointer<vtkOutlineFilter> outline  = vtkSmartPointer<vtkOutlineFilter>::New();
	outline->SetInputData(sphere);
	outline->Update();

	vtkSmartPointer<vtkPolyDataMapper> outlineMapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
	outlineMapper->SetInputData(outline->GetOutput());

	vtkSmartPointer<vtkActor> outlineActor =  vtkSmartPointer<vtkActor>::New();
	outlineActor->SetMapper(outlineMapper);
	outlineActor->GetProperty()->SetColor(1,0,0);

	vtkSmartPointer<vtkRenderer> renderer = 
		vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(actor);
	renderer->AddActor(outlineActor);
	renderer->SetBackground(1,1,1); 

	vtkSmartPointer<vtkRenderWindow> renderWindow = 
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	renderWindow->SetSize( 640, 480 );
	renderWindow->Render();
	renderWindow->SetWindowName("PolyDataBoundingBox");

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = 
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);

	renderWindow->Render();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}