/**********************************************************************

  文件名: 6.1_PolyDataAttribute.cpp
  Copyright (c) 张晓东, 罗火灵. All rights reserved.
  更多信息请访问: 
    http://www.vtkchina.org (VTK中国)
	http://blog.csdn.net/www_doling_net (东灵工作室) 

**********************************************************************/

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   // 必需：初始化OpenGL渲染后端
VTK_MODULE_INIT(vtkInteractionStyle);   // 必需：初始化交互模块

#include <vtkSmartPointer.h>
#include <vtkPolygon.h>
#include <vtkTriangle.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkFloatArray.h>
#include <vtkPlaneSource.h>
#include <vtkCellDataToPointData.h>
#include <vtkPointDataToCellData.h>
#include <vtkDataSet.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkLookupTable.h>

#include <iostream>

int main(int argc, char *argv[])
{
	vtkSmartPointer<vtkPlaneSource> gridSource = vtkSmartPointer<vtkPlaneSource>::New();  //平面网格数据
	gridSource->SetXResolution(3);
	gridSource->SetYResolution(3);
	gridSource->Update();
	vtkSmartPointer<vtkPolyData> grid = gridSource->GetOutput();

	vtkSmartPointer<vtkFloatArray> cellScalars = vtkSmartPointer<vtkFloatArray>::New();  // 标量
	
	vtkSmartPointer<vtkFloatArray> cellVectors = vtkSmartPointer<vtkFloatArray>::New();  // 向量
	cellVectors->SetNumberOfComponents(3);

	for (int i = 0; i < 9; i++)
	{
		cellScalars->InsertNextValue(i + 1);
		cellVectors->InsertNextTuple3(0.0, 0.0, 1.0);
	}
	grid->GetCellData()->SetScalars(cellScalars);  // 设标量属性
	grid->GetCellData()->SetVectors(cellVectors);  // 设向量属性

	vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
	lut->SetNumberOfTableValues(10);
	lut->Build();
	lut->SetTableValue(0     , 0     , 0     , 0, 1);  
	lut->SetTableValue(1, 0.8900, 0.8100, 0.3400, 1); 
	lut->SetTableValue(2, 1.0000, 0.3882, 0.2784, 1); 
	lut->SetTableValue(3, 0.9608, 0.8706, 0.7020, 1); 
	lut->SetTableValue(4, 0.9020, 0.9020, 0.9804, 1); 
	lut->SetTableValue(5, 1.0000, 0.4900, 0.2500, 1); 
	lut->SetTableValue(6, 0.5300, 0.1500, 0.3400, 1); 
	lut->SetTableValue(7, 0.9804, 0.5020, 0.4471, 1); 
	lut->SetTableValue(8, 0.7400, 0.9900, 0.7900, 1); 
	lut->SetTableValue(9, 0.2000, 0.6300, 0.7900, 1);

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(grid);//使用网格单元属性数据

	////测试单元属性数据转点属性数据
	//vtkSmartPointer<vtkCellDataToPointData> convert =  vtkSmartPointer<vtkCellDataToPointData>::New();
	//convert->SetInputData(grid);
	//convert->SetPassCellData(true);  // 是否在输出数据中保存单元属性数据
	//convert->Update();
	//mapper->SetInputData((vtkPolyData*)convert->GetOutput());

	
	mapper->SetScalarRange(0, 9); // 设标量范围  范围之外的值会被截断
	mapper->SetLookupTable(lut);

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(actor);
	renderer->SetBackground(1.0,1.0,1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	renderWindow->SetSize( 640, 480 );
	renderWindow->Render();
	renderWindow->SetWindowName("PolyDataAttribute");

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);

	renderWindow->Render();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}