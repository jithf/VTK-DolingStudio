/**********************************************************************

  文件名: 5.2_ImageBlendExample.cpp
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
#include <vtkImageCanvasSource2D.h>
#include <vtkImageBlend.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkJPEGReader.h>
#include <vtkImageActor.h>
#include <vtkImageCast.h>
#include <vtkImageToImageStencil.h>

//测试图像：../data/lena-gray.jpg
int main(int argc, char* argv[])
{
	//if (argc < 2)
	//{
	//	std::cout<<argv[0]<<" "<<"ImageFile(*.jpg)"<<std::endl;
	//	return EXIT_FAILURE;
	//}

	vtkSmartPointer<vtkJPEGReader> reader = vtkSmartPointer<vtkJPEGReader>::New();
	reader->SetFileName ("C:\\Users\\luhy\\Desktop\\data\\lena-gray.jpg");
	reader->Update();

	// 获取图像尺寸
	int dims[3];
	reader->GetOutput()->GetDimensions(dims);
	int width = dims[0];
	int height = dims[1];

	vtkSmartPointer<vtkImageCanvasSource2D> imageSource = vtkSmartPointer<vtkImageCanvasSource2D>::New();
	imageSource->SetNumberOfScalarComponents(1);  //通道数
	imageSource->SetScalarTypeToUnsignedChar();
	imageSource->SetExtent(0, width - 1, 0, height - 1, 0, 0);  // 维度数 大小
	imageSource->SetDrawColor(0.0);
	imageSource->FillBox(0, width - 1, 0, height - 1);
	imageSource->SetDrawColor(255.0);
	imageSource->FillBox(100, 400, 100, 400);
	imageSource->Update();

	// 创建图像混合器
	vtkSmartPointer<vtkImageBlend> imageBlend =  vtkSmartPointer<vtkImageBlend>::New();
	imageBlend->AddInputData(reader->GetOutput());  // 主图像
	imageBlend->AddInputData( imageSource->GetOutput());  // 遮罩图像
	imageBlend->SetOpacity(0, 0.6);  // 透明度
	imageBlend->SetOpacity(1, 0.4);
	imageBlend->Update();

	// Create actors
	vtkSmartPointer<vtkImageActor> originalActor1 = vtkSmartPointer<vtkImageActor>::New();
	originalActor1->SetInputData(reader->GetOutput());

	vtkSmartPointer<vtkImageActor> originalActor2 = vtkSmartPointer<vtkImageActor>::New();
	originalActor2->SetInputData(imageSource->GetOutput());

	vtkSmartPointer<vtkImageActor> blendActor = vtkSmartPointer<vtkImageActor>::New();
	blendActor->SetInputData(imageBlend->GetOutput());

	// Define viewport ranges
	// (xmin, ymin, xmax, ymax)
	double leftViewport[4] = {0.0, 0.0, 0.33, 1.0};
	double midViewport[4] = {0.33, 0.0, 0.66, 1.0};
	double rightViewport[4] = {0.66, 0.0, 1.0, 1.0};

	// Setup renderers
	vtkSmartPointer<vtkRenderer> originalRenderer1 = vtkSmartPointer<vtkRenderer>::New();
	originalRenderer1->SetViewport(leftViewport);
	originalRenderer1->AddActor(originalActor1);
	originalRenderer1->ResetCamera();
	originalRenderer1->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> originalRenderer2 = vtkSmartPointer<vtkRenderer>::New();
	originalRenderer2->SetViewport(midViewport);
	originalRenderer2->AddActor(originalActor2);
	originalRenderer2->ResetCamera();
	originalRenderer2->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> blendRenderer = vtkSmartPointer<vtkRenderer>::New();
	blendRenderer->SetViewport(rightViewport);
	blendRenderer->AddActor(blendActor);
	blendRenderer->ResetCamera();
	blendRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(originalRenderer1);
	renderWindow->AddRenderer(originalRenderer2);
	renderWindow->AddRenderer(blendRenderer);
	renderWindow->SetSize( 640, 320 );
	renderWindow->Render();
	renderWindow->SetWindowName("ImageBlendExample");

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
	renderWindowInteractor->SetInteractorStyle(style);
	renderWindowInteractor->SetRenderWindow(renderWindow);

	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}