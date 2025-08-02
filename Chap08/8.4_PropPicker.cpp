/**********************************************************************

  文件名: 8.4_PropPicker.cpp
  Copyright (c) 张晓东, 罗火灵. All rights reserved.
  更多信息请访问: 
    http://www.vtkchina.org (VTK中国)
	http://blog.csdn.net/www_doling_net (东灵工作室) 

**********************************************************************/

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   // 必需：初始化OpenGL渲染后端
VTK_MODULE_INIT(vtkInteractionStyle);   // 必需：初始化交互模块
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2); // 确保正确初始化体积渲染模块

#include <vtkSmartPointer.h>
#include <vtkMath.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkSphereSource.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkObjectFactory.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkPropPicker.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

// Handle mouse events
class PropPickerInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
	static PropPickerInteractorStyle* New();
	
	vtkTypeMacro(PropPickerInteractorStyle, vtkInteractorStyleTrackballCamera);  // 这是 VTK 宏，用于实现类型信息和运行时类型检查

	PropPickerInteractorStyle()
	{
		LastPickedActor = NULL;
		LastPickedProperty = vtkProperty::New();
	}

	virtual ~PropPickerInteractorStyle()
	{
		LastPickedProperty->Delete();
	}

	virtual void OnLeftButtonDown()
	{
		int* clickPos = this->GetInteractor()->GetEventPosition();  // 屏幕像素坐标

		// Pick from this location.
		vtkSmartPointer<vtkPropPicker>  picker = vtkSmartPointer<vtkPropPicker>::New();
		picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());  // 拾取

		double* pos = picker->GetPickPosition();
		
		// If we picked something before, reset its property
		if (this->LastPickedActor)
		{
			this->LastPickedActor->GetProperty()->DeepCopy(this->LastPickedProperty);
		}

		this->LastPickedActor = picker->GetActor();

		// 本次拾取到东西
		if (this->LastPickedActor)
		{
			// Save the property of the picked actor so that we can restore it next time
			this->LastPickedProperty->DeepCopy(this->LastPickedActor->GetProperty());
			// Highlight the picked actor by changing its properties
			this->LastPickedActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
			this->LastPickedActor->GetProperty()->SetDiffuse(1.0);
			this->LastPickedActor->GetProperty()->SetSpecular(0.0);
		}

		// Forward events
		vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
	}

private:
	vtkActor    *LastPickedActor;
	vtkProperty *LastPickedProperty;
};

vtkStandardNewMacro(PropPickerInteractorStyle);  // vtkStandardNewMacro 宏的作用就是自动生成这个 New() 方法的实现代码
 
int main(int argc, char *argv[])
{
	//int numberOfSpheres = 10;
	//if (argc > 1)
	//{
	//	numberOfSpheres = atoi(argv[1]);
	//}
	// A renderer and render window
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->Render();
	renderWindow->SetWindowName("PropPicker");
	renderWindow->AddRenderer ( renderer );

	// An interactor
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow ( renderWindow );

	// Set the custom type to use for interaction.
	vtkSmartPointer<PropPickerInteractorStyle> style = vtkSmartPointer<PropPickerInteractorStyle>::New();
	style->SetDefaultRenderer(renderer);
	renderWindowInteractor->SetInteractorStyle( style );

	int  numberOfSpheres = 10;
	for (int i = 0; i < numberOfSpheres; ++i)
	{
		vtkSmartPointer<vtkSphereSource> source = vtkSmartPointer<vtkSphereSource>::New();
		double x, y, z, radius;
		x = vtkMath::Random(-5,5);
		y = vtkMath::Random(-5,5);
		z = vtkMath::Random(-5,5);
		radius = vtkMath::Random(.5, 1.0);
		source->SetRadius(radius);
		source->SetCenter(x, y, z);
		source->SetPhiResolution(11);
		source->SetThetaResolution(21);
		vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputConnection ( source->GetOutputPort());
		vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper ( mapper );
		double r, g, b;
		r = vtkMath::Random(.4, 1.0);
		g = vtkMath::Random(.4, 1.0);
		b = vtkMath::Random(.4, 1.0);
		actor->GetProperty()->SetDiffuseColor(r, g, b);
		actor->GetProperty()->SetDiffuse(.8);
		actor->GetProperty()->SetSpecular(.5);
		actor->GetProperty()->SetSpecularColor(1.0,1.0,1.0);
		actor->GetProperty()->SetSpecularPower(30.0);
		renderer->AddActor ( actor );
	}

	renderer->SetBackground ( 1.0, 1.0, 1.0 );

	// Render and interact
	renderWindow->Render();
	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}