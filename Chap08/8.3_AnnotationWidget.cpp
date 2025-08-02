/**********************************************************************

  文件名: 8.3_AnnotationWidget.cpp
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
#include <vtkActor.h>
#include <vtkLookupTable.h>
#include <vtkUnstructuredGridReader.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDataSetMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkScalarBarActor.h>
#include <vtkScalarBarWidget.h>
#include <vtkTextActor.h>
#include <vtkTextWidget.h>
#include <vtkTextRepresentation.h>
#include <vtkTextProperty.h>
#include <vtkCaptionRepresentation.h>
#include <vtkCaptionWidget.h>
#include <vtkCaptionActor2D.h>
#include <vtkBalloonRepresentation.h> 
#include <vtkBalloonWidget.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkAxesActor.h>

//测试：../data/scalarBarWidgetTestData.vtk
int main(int argc, char* argv[])
{
	//if ( argc < 2 )
	//{
	//	std::cout<<argv[0]<<" "<<"scalarBarWidgetTestData.vtk(*.vtk)"<<std::endl;
	//	return EXIT_FAILURE;
	//}
	char* fileName = "C:\\Users\\luhy\\Desktop\\data\\scalarBarWidgetTestData.vtk";

	vtkSmartPointer< vtkUnstructuredGridReader > reader = vtkSmartPointer< vtkUnstructuredGridReader >::New();
	reader->SetFileName(fileName);
	reader->Update() ;

	// 颜色映射表
	vtkSmartPointer< vtkLookupTable > lut = vtkSmartPointer< vtkLookupTable >::New();
	lut->Build();

	vtkSmartPointer< vtkDataSetMapper > mapper = vtkSmartPointer< vtkDataSetMapper >::New();
	mapper->SetInputData(reader->GetOutput());
	mapper->SetScalarRange(reader->GetOutput()->GetScalarRange());  // 设置标量范围
	mapper->SetLookupTable(lut); // 颜色映射

	vtkSmartPointer< vtkActor > actor = vtkSmartPointer< vtkActor >::New();
	actor->SetMapper(mapper);

	vtkSmartPointer< vtkRenderer > renderer = vtkSmartPointer< vtkRenderer >::New();
	renderer->AddActor(actor);
	renderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer< vtkRenderWindow > renderWindow = vtkSmartPointer< vtkRenderWindow >::New();
	renderWindow->AddRenderer(renderer);
	renderWindow->Render();
	renderWindow->SetWindowName("AnnotationWidget");
	renderWindow->SetSize(400, 400);

	vtkSmartPointer< vtkRenderWindowInteractor > interactor = vtkSmartPointer< vtkRenderWindowInteractor >::New();
	interactor->SetRenderWindow(renderWindow);

	//////////////////////////////////////////////////////////////////////////

	// vtkScalarBarWidget 标量条
	vtkSmartPointer< vtkScalarBarActor > scalarBarActor = vtkSmartPointer< vtkScalarBarActor >::New();
	scalarBarActor->SetOrientationToHorizontal();  // 竖直方向
	scalarBarActor->SetLookupTable(lut);

	vtkSmartPointer< vtkScalarBarWidget > scalarBarWidget = vtkSmartPointer< vtkScalarBarWidget >::New();
	scalarBarWidget->SetInteractor(interactor);
	scalarBarWidget->SetScalarBarActor(scalarBarActor);
	scalarBarWidget->On();

	//////////////////////////////////////////////////////////////////////////
	
	// vtkTextWidget  文本
	vtkSmartPointer<vtkTextActor> textActor = vtkSmartPointer<vtkTextActor>::New();
	textActor->SetInput("VTK Widgets");
	textActor->GetTextProperty()->SetColor( 0.0, 1.0, 0.0 );

	vtkSmartPointer<vtkTextWidget> textWidget = vtkSmartPointer<vtkTextWidget>::New();
	vtkSmartPointer<vtkTextRepresentation> textRepresentation = vtkSmartPointer<vtkTextRepresentation>::New();  // 创建文本表示几何体
	textRepresentation->GetPositionCoordinate()->SetValue( .15, .15 );  // 定义文本位置
	textRepresentation->GetPosition2Coordinate()->SetValue( .7, .2 );  // 定义文本大小

	textWidget ->SetRepresentation( textRepresentation );
	textWidget->SetInteractor(interactor);
	textWidget->SetTextActor(textActor);
	textWidget->SelectableOff(); // 不可选中
	textWidget->On();

	//////////////////////////////////////////////////////////////////////////
	
	// vtkBalloonWidget  鼠标悬停消息
	vtkSmartPointer<vtkBalloonRepresentation> balloonRep = vtkSmartPointer<vtkBalloonRepresentation>::New();
	balloonRep->SetBalloonLayoutToImageRight();  // 指定气泡提示相对于交互对象（或鼠标位置）的显示位置

	vtkSmartPointer<vtkBalloonWidget> balloonWidget = vtkSmartPointer<vtkBalloonWidget>::New();
	balloonWidget->SetInteractor(interactor);
	balloonWidget->SetRepresentation(balloonRep);
	balloonWidget->AddBalloon(actor, "This is a widget example",NULL);
	balloonWidget->On();

	//////////////////////////////////////////////////////////////////////////
	
	// vtkOrientationMarkerWidget 方向指示
	vtkSmartPointer<vtkAxesActor> iconActor = vtkSmartPointer<vtkAxesActor>::New();
	vtkSmartPointer<vtkOrientationMarkerWidget> orientationWidget  = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
	orientationWidget ->SetOutlineColor( 0.9300, 0.5700, 0.1300 );
	orientationWidget ->SetOrientationMarker( iconActor );
	orientationWidget ->SetInteractor( interactor );
	orientationWidget ->SetViewport( 0.0, 0.0, 0.2, 0.2 );
	orientationWidget ->SetEnabled( 1 );
	orientationWidget ->InteractiveOn();

	////////////////////////////////////////////////////////////////////////////
	
	//// vtkCaptionWidget  带线框和箭头的文本信息
	//vtkSmartPointer<vtkCaptionRepresentation> captionRepresentation = vtkSmartPointer<vtkCaptionRepresentation>::New();
	//captionRepresentation->GetCaptionActor2D()->SetCaption("Caption Widget");  // 文本
	//captionRepresentation->GetCaptionActor2D()->GetTextActor()->GetTextProperty()->SetFontSize(20);  // 字体大小

	//double pos[3] = {.5,0,0};
	//captionRepresentation->SetAnchorPosition(pos);  // 锚点位置

	//vtkSmartPointer<vtkCaptionWidget> captionWidget =  vtkSmartPointer<vtkCaptionWidget>::New();
	//captionWidget->SetInteractor(interactor);
	//captionWidget->SetRepresentation(captionRepresentation);
	//captionWidget->On();

	////////////////////////////////////////////////////////////////////////////

	renderWindow->Render();
	interactor->Initialize();
	interactor->Start();

	return EXIT_SUCCESS;
}
