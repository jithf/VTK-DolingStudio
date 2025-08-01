/**********************************************************************

  文件名: 5.4_CannyExample.cpp
  Copyright (c) 张晓东, 罗火灵. All rights reserved.
  更多信息请访问: 
    http://www.vtkchina.org (VTK中国)
	http://blog.csdn.net/www_doling_net (东灵工作室) 

**********************************************************************/

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   // 必需：初始化OpenGL渲染后端
VTK_MODULE_INIT(vtkInteractionStyle);   // 必需：初始化交互模块

#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkImageShiftScale.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkImageActor.h>
#include <vtkJPEGReader.h>
#include <vtkImageCast.h>
#include <vtkImageGaussianSmooth.h>
#include <vtkImageGradient.h>
#include <vtkImageMagnitude.h>
#include <vtkImageNonMaximumSuppression.h>
#include <vtkImageConstantPad.h>
#include <vtkImageToStructuredPoints.h>
#include <vtkLinkEdgels.h>
#include <vtkThreshold.h>
#include <vtkGeometryFilter.h>
#include <vtkSubPixelPositionEdgels.h>
#include <vtkCamera.h>
#include <vtkProperty.h>
#include <vtkStripper.h>
#include <vtkPolyDataMapper.h>

//测试图像：../data/lena-gray.jpg
int main(int argc, char* argv[])
{
	//if (argc < 2)
	//{
	//	std::cout<<argv[0]<<" "<<"ImageFile(*.jpg)"<<std::endl;
	//	return EXIT_FAILURE;
	//}

	vtkSmartPointer<vtkJPEGReader> reader = vtkSmartPointer<vtkJPEGReader>::New();
	reader->SetFileName("C:\\Users\\luhy\\Desktop\\data\\lena-gray.jpg");
	reader->Update();

	//转Float类型
	vtkSmartPointer<vtkImageCast> ic = vtkSmartPointer<vtkImageCast>::New();
	ic->SetOutputScalarTypeToFloat();
	ic->SetInputConnection(reader->GetOutputPort());
	ic->Update();

	//高斯平滑
	vtkSmartPointer<vtkImageGaussianSmooth> gs = vtkSmartPointer<vtkImageGaussianSmooth>::New();
	gs->SetInputConnection(ic->GetOutputPort());
	gs->SetDimensionality(2); // 计算维度数
	gs->SetRadiusFactors(1, 1, 0); //核大小
	gs->Update();
	
	// 计算梯度
	vtkSmartPointer<vtkImageGradient> imgGradient = vtkSmartPointer<vtkImageGradient>::New();
	imgGradient->SetInputConnection(gs->GetOutputPort());
	imgGradient->SetDimensionality(2);
	imgGradient->Update();

	// 计算梯度的模
	vtkSmartPointer<vtkImageMagnitude> imgMagnitude = vtkSmartPointer<vtkImageMagnitude>::New();
	imgMagnitude->SetInputConnection(imgGradient->GetOutputPort());
	imgMagnitude->Update();

	// 非极大值抑制
	vtkSmartPointer<vtkImageNonMaximumSuppression> nonMax = vtkSmartPointer<vtkImageNonMaximumSuppression>::New();
	nonMax->SetMagnitudeInputData(imgMagnitude->GetOutput());
	nonMax->SetVectorInputData(imgGradient->GetOutput());
	nonMax->SetDimensionality(2);  // 计算维度数
	nonMax->Update();

	//增加图像大小
	vtkSmartPointer<vtkImageConstantPad> pad = vtkSmartPointer<vtkImageConstantPad>::New();
	pad->SetInputConnection(imgGradient->GetOutputPort());
	pad->SetOutputNumberOfScalarComponents(3);  // 将梯度图像像素的组分修改为3
	pad->SetConstant(0);  // 增加的空间中填0
	pad->Update();

	// 将 vtkImageData 格式转换为规则点集
	vtkSmartPointer<vtkImageToStructuredPoints> i2sp1 = vtkSmartPointer<vtkImageToStructuredPoints>::New();
	i2sp1->SetInputConnection(nonMax->GetOutputPort());
	i2sp1->SetVectorInputData(pad->GetOutput());
	i2sp1->Update();

	//  根据点的相邻关系将点连接成连续的折线(Polyline)
	vtkSmartPointer<vtkLinkEdgels> imgLink = vtkSmartPointer<vtkLinkEdgels>::New();
	imgLink->SetInputData(i2sp1->GetOutput());
	imgLink->SetGradientThreshold(2);  // 小于该阈值的点不算
	imgLink->Update();

	// 删选折线
	vtkSmartPointer<vtkThreshold> thresholdEdgels = vtkSmartPointer<vtkThreshold>::New();
	thresholdEdgels->SetInputConnection(imgLink->GetOutputPort());
	thresholdEdgels->SetUpperThreshold (10);
	thresholdEdgels->AllScalarsOff();  // 并非所有点满足阈值后，单元才满足
	thresholdEdgels->Update();

	// 将数据转为几何数据
	vtkSmartPointer<vtkGeometryFilter> gf = vtkSmartPointer<vtkGeometryFilter>::New();
	gf->SetInputConnection(thresholdEdgels->GetOutputPort());
	gf->Update();

	// 图像数据转为结构化的点集
	vtkSmartPointer<vtkImageToStructuredPoints> i2sp = vtkSmartPointer<vtkImageToStructuredPoints>::New();
	i2sp->SetInputConnection(imgMagnitude->GetOutputPort());
	i2sp->SetVectorInputData(pad->GetOutput());
	i2sp->Update();

	//根据梯度对图像边缘进行位置调整
	vtkSmartPointer<vtkSubPixelPositionEdgels> spe = vtkSmartPointer<vtkSubPixelPositionEdgels>::New();
	spe->SetInputConnection(gf->GetOutputPort());
	spe->SetGradMapsData(i2sp->GetStructuredPointsOutput());

	// 将输入的多边形、三角形或者线段生成三角形条带或者折线段
	vtkSmartPointer<vtkStripper> strip = vtkSmartPointer<vtkStripper>::New();
	strip->SetInputConnection(spe->GetOutputPort());

	// Mapper
	vtkSmartPointer<vtkPolyDataMapper> dsm = vtkSmartPointer<vtkPolyDataMapper>::New();
	dsm->SetInputConnection(strip->GetOutputPort());
	dsm->ScalarVisibilityOff();

	vtkSmartPointer<vtkActor> planeActor = vtkSmartPointer<vtkActor>::New();
	planeActor->SetMapper(dsm);
	planeActor->GetProperty()->SetAmbient(1.0);
	planeActor->GetProperty()->SetDiffuse(0.0);
	planeActor->GetProperty()->SetColor(1.0, 0.0, 0.0);

	vtkSmartPointer<vtkImageActor> originalActor = vtkSmartPointer<vtkImageActor>::New();
	originalActor->SetInputData(reader->GetOutput());

	double originalViewport[4] = {0.0, 0.0, 0.5, 1.0};
	double gradviewport[4] = {0.5, 0.0, 1.0, 1.0};

	vtkSmartPointer<vtkRenderer> originalRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	originalRenderer->SetViewport(originalViewport);
	originalRenderer->AddActor(originalActor);
	originalRenderer->ResetCamera();
	originalRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> gradRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	gradRenderer->SetViewport(gradviewport);
	gradRenderer->AddActor(planeActor);
	gradRenderer->ResetCamera();
	gradRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->SetSize(900, 300);
	renderWindow->AddRenderer(originalRenderer);
	renderWindow->AddRenderer(gradRenderer);
	renderWindow->Render();
	renderWindow->SetWindowName("CannyExample");

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style =
		vtkSmartPointer<vtkInteractorStyleImage>::New();

	renderWindowInteractor->SetInteractorStyle(style);
	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}