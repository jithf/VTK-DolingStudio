/**********************************************************************

  文件名: 5.3_ImageAccumulateExample2.cpp
  Copyright (c) 张晓东, 罗火灵. All rights reserved.
  更多信息请访问: 
    http://www.vtkchina.org (VTK中国)
	http://blog.csdn.net/www_doling_net (东灵工作室) 

**********************************************************************/

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   // 必需：初始化OpenGL渲染后端
VTK_MODULE_INIT(vtkInteractionStyle);   // 必需：初始化交互模块

#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkImageAccumulate.h>
#include <vtkImageData.h>
#include <vtkImageExtractComponents.h>
#include <vtkBMPReader.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkXYPlotActor.h>
#include <vtkAxisActor2D.h>
#include <vtkProperty2D.h>
#include <vtkTextProperty.h>

//测试图像：../data/lena.bmp
int main(int argc, char* argv[])
{
	//if (argc < 2)
	//{
	//	std::cout<<argv[0]<<" "<<"ImageFile(*.bmp)"<<std::endl;
	//	return EXIT_FAILURE;
	//}

	vtkSmartPointer<vtkBMPReader> reader = vtkSmartPointer<vtkBMPReader>::New();
	reader->SetFileName ("C:\\Users\\luhy\\Desktop\\data\\lena.bmp");
	reader->Update();

	int numComponents = reader->GetOutput()->GetNumberOfScalarComponents();

	vtkSmartPointer<vtkXYPlotActor> plot =  vtkSmartPointer<vtkXYPlotActor>::New();
	plot->ExchangeAxesOff();
	plot->SetLabelFormat( "%g" );
	plot->SetXTitle( "Intensity" );
	plot->SetYTitle( "Frequency" );
	plot->SetXValuesToValue();
	plot->GetProperty()->SetColor(0.0, 0.0, 0.0);
	plot->GetAxisLabelTextProperty()->SetColor(0.0, 0.0, 0.0);
	plot->GetAxisTitleTextProperty()->SetColor(0.0, 0.0, 0.0);


	double colors[3][3] = {
		{ 1, 0, 0 },
		{ 0, 1, 0 },
		{ 0, 0, 1 } 
	};

	const char* labels[3] = { "Red", "Green", "Blue" };

	int xmax = 0;
	int ymax = 0;

	for( int i = 0; i < numComponents; ++i )
	{

		// 提取分量
		vtkSmartPointer<vtkImageExtractComponents> extract =  vtkSmartPointer<vtkImageExtractComponents>::New();
		extract->SetInputConnection( reader->GetOutputPort() );
		extract->SetComponents( i );
		extract->Update();

		double range[2];
		extract->GetOutput()->GetScalarRange( range );  // 分量值的范围 最小值和最大值
		int extent = static_cast<int>(range[1])-static_cast<int>(range[0])-1;

		vtkSmartPointer<vtkImageAccumulate> histogram =  vtkSmartPointer<vtkImageAccumulate>::New();
		histogram->SetInputConnection( extract->GetOutputPort() );
		histogram->SetComponentExtent( 0,extent, 0,0, 0,0);  // 每个组分的直方图的最小值和最大值
		histogram->SetComponentOrigin( range[0],0,0 ); // 统计每个组分直方图时的起始灰度值
		histogram->SetComponentSpacing( 1,0,0 );  // 直方图每个间隔代表的灰度范围
		histogram->SetIgnoreZero( 1 ); // 忽略0值
		histogram->Update();

		
		if( range[1] > xmax ) 
		{ 
			xmax = range[1];
		}

		if( histogram->GetOutput()->GetScalarRange()[1] > ymax ) 
		{
			ymax = histogram->GetOutput()->GetScalarRange()[1];
		}

		plot->AddDataSetInputConnection( histogram->GetOutputPort() );
		plot->SetPlotColor(i,colors[i]);
		plot->SetPlotLabel(i,labels[i]);
		plot->LegendOn();
	}

	plot->SetXRange( 0, xmax);
	plot->SetYRange( 0, ymax);

	vtkSmartPointer<vtkRenderer> renderer =  vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(plot);
	renderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow =  vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer( renderer );
	renderWindow->SetSize(640, 480);
	renderWindow->Render();
	renderWindow->SetWindowName("ImageAccumulateExample2");

	vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow( renderWindow );

	interactor->Initialize();
	interactor->Start(); 

	return EXIT_SUCCESS;
}
