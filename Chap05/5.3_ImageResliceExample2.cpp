/**********************************************************************

  文件名: 5.3_ImageResliceExample2.cpp
  Copyright (c) 张晓东, 罗火灵. All rights reserved.
  更多信息请访问: 
    http://www.vtkchina.org (VTK中国)
	http://blog.csdn.net/www_doling_net (东灵工作室) 

**********************************************************************/

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   // 必需：初始化OpenGL渲染后端
VTK_MODULE_INIT(vtkInteractionStyle);   // 必需：初始化交互模块

#include <vtkSmartPointer.h>
#include <vtkImageReader2.h>
#include <vtkMatrix4x4.h>
#include <vtkImageReslice.h>
#include <vtkLookupTable.h>
#include <vtkImageMapToColors.h>
#include <vtkImageActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkCommand.h>
#include <vtkImageData.h>
#include <vtkMetaImageReader.h>
#include <vtkImageCast.h>

class vtkImageInteractionCallback : public vtkCommand
{
public:
	//单例
	static vtkImageInteractionCallback *New()
	{
		return new vtkImageInteractionCallback; 
	}

	vtkImageInteractionCallback() 
	{
		this->Slicing = 0;
		this->ImageReslice = 0;
		this->Interactor = 0; 
	}

	void SetImageReslice(vtkImageReslice *reslice) 
	{
		this->ImageReslice = reslice;   // 切片信息  
	}

	vtkImageReslice *GetImageReslice() 
	{
		return this->ImageReslice;
	}

	void SetInteractor(vtkRenderWindowInteractor *interactor) 
	{
		this->Interactor = interactor;  // 负责取切面后刷新视图
	}

	vtkRenderWindowInteractor *GetInteractor() 
	{
		return this->Interactor; 
	}

	void SetColorMap(vtkImageMapToColors* colorMap)
	{
		this->colorMap = colorMap;  // 负责取切面后刷新视图
	}

	vtkImageMapToColors* GetColorMap()
	{
		return this->colorMap;
	}

	// 重写函数Execute
	virtual void Execute(vtkObject *, unsigned long event, void *)
	{
		vtkRenderWindowInteractor *interactor = this->GetInteractor();

		int lastPos[2];
		interactor->GetLastEventPosition(lastPos);
		int currPos[2];
		interactor->GetEventPosition(currPos);

		if (event == vtkCommand::LeftButtonPressEvent)
		{
			this->Slicing = 1;
		}
		else if (event == vtkCommand::LeftButtonReleaseEvent)
		{
			this->Slicing = 0;
		}
		else if (event == vtkCommand::MouseMoveEvent)
		{
			if (this->Slicing)
			{
				vtkImageReslice *reslice = this->ImageReslice;

				// Increment slice position by deltaY of mouse  鼠标在Y轴上移动短距离
				int deltaY = lastPos[1] - currPos[1];

				reslice->Update();
				double sliceSpacing = reslice->GetOutput()->GetSpacing()[2];  // 切边在Y轴方向上的间隔
				vtkMatrix4x4 *matrix = reslice->GetResliceAxes();  // 切片的变换矩阵
				// move the center point that we are slicing through
				double point[4];
				double center[4];  // 新的切片中心
				point[0] = 0.0;
				point[1] = 0.0;
				point[2] = sliceSpacing * deltaY;
				point[3] = 1.0;
				matrix->MultiplyPoint(point, center);  // 计算新中心 齐次坐标变换
				matrix->SetElement(0, 3, center[0]);
				matrix->SetElement(1, 3, center[1]);
				matrix->SetElement(2, 3, center[2]);
				reslice->SetResliceAxes(matrix);
				reslice->Update();
				this->colorMap->Update();

				//printf("%f %f %f\n", center[0], center[1], center[2]);

				interactor->Render();
			}
			else
			{
				vtkInteractorStyle *style = vtkInteractorStyle::SafeDownCast( interactor->GetInteractorStyle());
				if (style)
				{
					style->OnMouseMove();
				}
			}
		}
	}

private:
	int Slicing;
	vtkImageReslice *ImageReslice;
	vtkRenderWindowInteractor *Interactor;
	vtkImageMapToColors* colorMap;
};

int main()
{
	vtkSmartPointer<vtkMetaImageReader> reader = vtkSmartPointer<vtkMetaImageReader>::New();
	reader->SetFileName ("C:\\Users\\luhy\\Desktop\\data\\brain.mhd" );
	reader->Update();

	int extent[6];
	double spacing[3];
	double origin[3];

	reader->GetOutput()->GetExtent(extent);   //模型尺寸
	reader->GetOutput()->GetSpacing(spacing); //体素间隔
	reader->GetOutput()->GetOrigin(origin); // 原点

	double center[3];
	center[0] = origin[0] + spacing[0] * 0.5 * (extent[0] + extent[1]);
	center[1] = origin[1] + spacing[1] * 0.5 * (extent[2] + extent[3]);
	center[2] = origin[2] + spacing[2] * 0.5 * (extent[4] + extent[5]);

	static double axialElements[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1 
	};

	vtkSmartPointer<vtkMatrix4x4> resliceAxes = vtkSmartPointer<vtkMatrix4x4>::New();
	resliceAxes->DeepCopy(axialElements);
	resliceAxes->SetElement(0, 3, center[0]);
	resliceAxes->SetElement(1, 3, center[1]);
	resliceAxes->SetElement(2, 3, center[2]);

	vtkSmartPointer<vtkImageReslice> reslice = vtkSmartPointer<vtkImageReslice>::New();
	reslice->SetInputConnection(reader->GetOutputPort());
	reslice->SetOutputDimensionality(2);
	reslice->SetResliceAxes(resliceAxes);
	reslice->SetInterpolationModeToLinear();

	vtkSmartPointer<vtkLookupTable> colorTable = vtkSmartPointer<vtkLookupTable>::New();
	colorTable->SetRange(0, 1000); 
	colorTable->SetValueRange(0.0, 1.0);
	colorTable->SetSaturationRange(0.0, 0.0);
	colorTable->SetRampToLinear();
	colorTable->Build();

	vtkSmartPointer<vtkImageMapToColors> colorMap = vtkSmartPointer<vtkImageMapToColors>::New();
	colorMap->SetLookupTable(colorTable);
	colorMap->SetInputConnection(reslice->GetOutputPort());
	colorMap->Update();

	vtkSmartPointer<vtkImageActor> imgActor = vtkSmartPointer<vtkImageActor>::New();
	imgActor->SetInputData(colorMap->GetOutput());

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(imgActor);
	renderer->SetBackground(.4, .5, .6);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->SetSize(500, 500);
	renderWindow->AddRenderer(renderer);

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> imagestyle = vtkSmartPointer<vtkInteractorStyleImage>::New();
	renderWindowInteractor->SetInteractorStyle(imagestyle);
	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();

	vtkSmartPointer<vtkImageInteractionCallback> callback = vtkSmartPointer<vtkImageInteractionCallback>::New();
	callback->SetImageReslice(reslice);  // 设置切片
	callback->SetInteractor(renderWindowInteractor);
	callback->SetColorMap(colorMap);

	imagestyle->AddObserver(vtkCommand::MouseMoveEvent, callback);
	imagestyle->AddObserver(vtkCommand::LeftButtonPressEvent, callback);
	imagestyle->AddObserver(vtkCommand::LeftButtonReleaseEvent, callback);

	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}
