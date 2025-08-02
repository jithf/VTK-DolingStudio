/**********************************************************************

  文件名: 7.1_VolumeRenderingApp.cpp
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
#include <vtkImageData.h>
#include <vtkStructuredPoints.h>
#include <vtkStructuredPointsReader.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkVolumeProperty.h>
#include <vtkAxesActor.h>
#include <vtkImageShiftScale.h>
#include <vtkImageCast.h>
#include <vtkFixedPointVolumeRayCastMapper.h>
#include <vtkGPUVolumeRayCastMapper.h>


//测试：../data/mummy.128.vtk
int main(int argc, char *argv[])
{
	//if (argc < 2)
	//{
	//	std::cout<<argv[0]<<" "<<"StructuredPointsFile(*.vtk)"<<std::endl;
	//	return EXIT_FAILURE;
	//}

	vtkSmartPointer<vtkStructuredPointsReader> reader = vtkSmartPointer<vtkStructuredPointsReader>::New();
	reader->SetFileName("C:\\Users\\luhy\\Desktop\\data\\mummy.128.vtk");
	reader->Update();

	// 创建GPU加速的体积映射器
	vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapper  = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
	volumeMapper->SetInputData(reader->GetOutput());
	volumeMapper->SetBlendModeToComposite();  // 使用复合混合模式 沿视线方向累积每个体素的颜色和透明度，实现半透明效

	//设置光线采样距离  光线采样距离越小，细节越精细
	//volumeMapper->SetSampleDistance(volumeMapper->GetSampleDistance()*4);
	//设置图像采样步长 图像采样步长越大，渲染速度越快，但图像可能模糊
	//volumeMapper->SetAutoAdjustSampleDistances(0); // 关闭自动采样距离调整
	//volumeMapper->SetImageSampleDistance(4); // 设置图像采样步长

	// 体绘制属性
	vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	volumeProperty->SetInterpolationTypeToLinear();  // 设置体素插值方式为线性插值
	//volumeProperty->ShadeOn();  //打开或者关闭阴影测试
	volumeProperty->SetAmbient(0.4); // Ambient（环境光）：物体受环境光影响的强度（0.4 表示 40%）。
	volumeProperty->SetDiffuse(0.6);  // Diffuse（漫反射）：物体表面向各个方向均匀反射的光线强度（0.6 表示 60%）。
	volumeProperty->SetSpecular(0.2);  // 物体表面镜面反射的强度（0.2 表示 20%），使表面有 “光泽” 效果。

	//  配置不透明度传输函数
	// vtkPiecewiseFunction：定义 “标量值 - 不透明度” 的映射关系
	vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity =  vtkSmartPointer<vtkPiecewiseFunction>::New();
	compositeOpacity->AddPoint(70,   0.00);
	compositeOpacity->AddPoint(90,   0.40);
	compositeOpacity->AddPoint(180,  0.60);
	volumeProperty->SetScalarOpacity(compositeOpacity); //设置不透明度传输函数

	//测试隐藏部分数据,对比不同的设置
	//compositeOpacity->AddPoint(120,  0.00);
	//compositeOpacity->AddPoint(180,  0.60);
	//volumeProperty->SetScalarOpacity(compositeOpacity);

	// 梯度不透明度
	// 根据体素值的梯度（变化率）控制透明度，用于增强物体边缘（如组织边界）的显示。
	vtkSmartPointer<vtkPiecewiseFunction> volumeGradientOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
	volumeGradientOpacity->AddPoint(10,  0.0);
	volumeGradientOpacity->AddPoint(90,  0.5);
	volumeGradientOpacity->AddPoint(100, 1.0);
	//volumeProperty->SetGradientOpacity(volumeGradientOpacity);//设置梯度不透明度效果对比

	// 配置颜色传输函数
	// vtkColorTransferFunction：定义 “标量值 - 颜色” 的映射关系，控制不同体素值的显示颜色
	vtkSmartPointer<vtkColorTransferFunction> color =  vtkSmartPointer<vtkColorTransferFunction>::New();
	color->AddRGBPoint(0.000,  0.00, 0.00, 0.00);
	color->AddRGBPoint(64.00,  1.00, 0.52, 0.30);
	color->AddRGBPoint(190.0,  1.00, 1.00, 1.00);
	color->AddRGBPoint(220.0,  0.20, 0.20, 0.20);
	volumeProperty->SetColor(color);

	vtkSmartPointer<vtkVolume> volume =  vtkSmartPointer<vtkVolume>::New();
	volume->SetMapper(volumeMapper); // 体绘制内容
	volume->SetProperty(volumeProperty); // 体绘制属性

	vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();
	ren->SetBackground(1.0, 1.0, 1.0);
	ren->AddVolume( volume ); 

	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->AddRenderer(ren);
	renWin->SetSize(640, 480);
	renWin->Render();
	renWin->SetWindowName("VolumeRenderingApp");

	vtkSmartPointer<vtkRenderWindowInteractor> iren = 
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);
	ren->ResetCamera();

	renWin->Render();
	iren->Start();

	return EXIT_SUCCESS;
}