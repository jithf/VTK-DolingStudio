/**********************************************************************

  文件名: 7.3_vtkLODProp3DApp.cpp
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
#include <vtkPolyDataMapper.h>
#include <vtkLODProp3D.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkCallbackCommand.h>
#include <vtkProperty.h>
#include <vtkStructuredPoints.h>
#include <vtkStructuredPointsReader.h>
#include <vtkVolumeProperty.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkGPUVolumeRayCastMapper.h>

//测试：../data/mummy.128.vtk
int main (int argc, char *argv[])
{
	//if (argc < 2)
	//{
	//	std::cout<<argv[0]<<" "<<"StructuredPointsFile(*.vtk)"<<std::endl;
	//	return EXIT_FAILURE;
	//}

	vtkSmartPointer<vtkStructuredPointsReader> reader = vtkSmartPointer<vtkStructuredPointsReader>::New();
	reader->SetFileName("C:\\Users\\luhy\\Desktop\\data\\mummy.128.vtk");
	reader->Update();

	vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	volumeProperty->SetInterpolationTypeToLinear();
	volumeProperty->ShadeOn();
	volumeProperty->SetAmbient(0.4);
	volumeProperty->SetDiffuse(0.6);
	volumeProperty->SetSpecular(0.2);

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

	vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapper1 = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
	volumeMapper1->SetInputConnection(reader->GetOutputPort());
	volumeMapper1->SetBlendModeToComposite();
	volumeMapper1->SetAutoAdjustSampleDistances(0); //关闭自动调整采样距离

	vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapper2 = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
	volumeMapper2->SetInputConnection(reader->GetOutputPort());
	volumeMapper2->SetBlendModeToComposite();
	volumeMapper2->SetAutoAdjustSampleDistances(0); //关闭自动调整采样距离
	volumeMapper2->SetSampleDistance(4* volumeMapper1->GetSampleDistance());
	volumeMapper2->SetImageSampleDistance(4* volumeMapper1->GetImageSampleDistance());

	vtkSmartPointer<vtkLODProp3D> prop =  vtkSmartPointer<vtkLODProp3D>::New();
	prop->AddLOD(volumeMapper1, volumeProperty, 0.0); // 第3个参数为渲染的开始时间  若为0则是立即开始渲染
	prop->AddLOD(volumeMapper2,  volumeProperty, 0.0);

	vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();
	ren->SetBackground(1.0, 1.0, 1.0);
	ren->AddVolume( prop ); 

	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->AddRenderer(ren);
	renWin->SetSize(640, 480);
	renWin->Render();
	renWin->SetWindowName("7.3_vtkLODProp3DApp");

	vtkSmartPointer<vtkRenderWindowInteractor> iren = 
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);
	ren->ResetCamera();

	renWin->Render();
	iren->Start();

	return EXIT_SUCCESS;
}