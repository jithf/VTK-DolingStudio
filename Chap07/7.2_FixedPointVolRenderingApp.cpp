/**********************************************************************

  文件名: 7.2_FixedPointVolRenderingApp.cpp
  Copyright (c) 张晓东, 罗火灵. All rights reserved.
  更多信息请访问: 
    http://www.vtkchina.org (VTK中国)
	http://blog.csdn.net/www_doling_net (东灵工作室) 

**********************************************************************/

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   // 必需：初始化OpenGL渲染后端
VTK_MODULE_INIT(vtkInteractionStyle);   // 必需：初始化交互模块
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2); // 确保正确初始化体积渲染模块
VTK_MODULE_INIT(vtkRenderingFreeType);  // 初始化文本渲染相关功能

#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkStructuredPoints.h>
#include <vtkStructuredPointsReader.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkVolumeProperty.h>
#include <vtkAxesActor.h>
#include <vtkImageShiftScale.h>
#include <vtkImageCast.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkPiecewiseFunction.h>
#include <vtkMetaImageReader.h>
#include <vtkOutlineFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkContourValues.h>

//测试：../data/mummy.128.vtk
int main(int argc, char *argv[])
{
	//if (argc < 2)
	//{
	//	std::cout<<argv[0]<<" "<<"StructuredPointsFile(*.vtk)"<<std::endl;
	//	return EXIT_FAILURE;
	//}

	vtkSmartPointer<vtkStructuredPointsReader> reader =
		vtkSmartPointer<vtkStructuredPointsReader>::New();
	reader->SetFileName("C:\\Users\\luhy\\Desktop\\data\\mummy.128.vtk");
	reader->Update();

	// VTK-9.X 已禁用以下函数
	//vtkSmartPointer<vtkVolumeRayCastIsosurfaceFunction> rayCastFun = 
	//	vtkSmartPointer<vtkVolumeRayCastIsosurfaceFunction>::New();
	//rayCastFun->SetIsoValue(100);

	//vtkSmartPointer<vtkVolumeRayCastMapper> volumeMapper = 
	//	vtkSmartPointer<vtkVolumeRayCastMapper>::New();
	//volumeMapper->SetInput(reader->GetOutput());
	//volumeMapper->SetVolumeRayCastFunction(rayCastFun);//必须设置，否则出错

	// 使用GPU加速渲染器（替代已弃用的CPU渲染器）
	vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapper = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
	volumeMapper->SetInputConnection(reader->GetOutputPort());
	// 使用等值面体绘制
	volumeMapper->SetBlendModeToIsoSurface();

	vtkSmartPointer<vtkVolumeProperty> volumeProperty = 
		vtkSmartPointer<vtkVolumeProperty>::New();
	volumeProperty->SetInterpolationTypeToLinear();
	volumeProperty->ShadeOn();
	volumeProperty->SetAmbient(0.4);
	volumeProperty->SetDiffuse(0.6);
	volumeProperty->SetSpecular(0.2);

	//  配置等值面阈值  非常重要！
	vtkSmartPointer<vtkContourValues> contourValues = volumeProperty->GetIsoSurfaceValues();
	contourValues->SetValue(0, 100.0);  // 第1个等值面，值为100.0
	contourValues->SetValue(1, 200.0);  // 第2个等值面，值为200.0
	contourValues->SetValue(2, 300.0);  // 第3个等值面，值为300.0

	vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = 
		vtkSmartPointer<vtkPiecewiseFunction>::New();
	compositeOpacity->AddPoint(70,   0.00);
	compositeOpacity->AddPoint(90,   0.40);
	compositeOpacity->AddPoint(180,  0.60);
	volumeProperty->SetScalarOpacity(compositeOpacity);

	vtkSmartPointer<vtkColorTransferFunction> color = 
		vtkSmartPointer<vtkColorTransferFunction>::New();
	color->AddRGBPoint(0.000,  0.00, 0.00, 0.00);
	color->AddRGBPoint(64.00,  1.00, 0.52, 0.30);
	color->AddRGBPoint(190.0,  1.00, 1.00, 1.00);
	color->AddRGBPoint(220.0,  0.20, 0.20, 0.20);
	volumeProperty->SetColor(color);

	vtkSmartPointer<vtkVolume> volume = 
		vtkSmartPointer<vtkVolume>::New();
	volume->SetMapper(volumeMapper);
	volume->SetProperty(volumeProperty);

	vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();
	ren->SetBackground(0.5f,0.5f,1.0f);
	ren->AddVolume( volume ); 

	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->AddRenderer(ren);
	renWin->SetSize(800,600);

	vtkSmartPointer<vtkRenderWindowInteractor> iren = 
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);
	ren->ResetCamera();

	renWin->Render();
	iren->Start();

	return EXIT_SUCCESS;
}