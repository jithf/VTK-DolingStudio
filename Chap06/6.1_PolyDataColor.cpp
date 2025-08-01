/**********************************************************************

  文件名: 6.1_PolyDataColor.cpp
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
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkLookupTable.h>
#include <vtkTransformTextureCoords.h>
#include <iostream>

int main(int argc, char *argv[])
{
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	points->InsertNextPoint(0.0, 0.0, 0.0);
	points->InsertNextPoint(1.0, 0.0, 0.0);
	points->InsertNextPoint(1.0, 1.0, 0.0);
	points->InsertNextPoint(0.0, 1.0, 0.0);
	points->InsertNextPoint(2.0, 0.0, 0.0);

	vtkSmartPointer<vtkPolygon> polygon = vtkSmartPointer<vtkPolygon>::New();
	polygon->GetPointIds()->SetNumberOfIds(4);
	polygon->GetPointIds()->SetId(0, 0);
	polygon->GetPointIds()->SetId(1, 1);
	polygon->GetPointIds()->SetId(2, 2);
	polygon->GetPointIds()->SetId(3, 3);

	vtkSmartPointer<vtkTriangle> trianle = vtkSmartPointer<vtkTriangle>::New();
	trianle->GetPointIds()->SetId(0, 1);
	trianle->GetPointIds()->SetId(1, 2);
	trianle->GetPointIds()->SetId(2, 4);

	vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
	cells->InsertNextCell(polygon);
	cells->InsertNextCell(trianle);

	vtkSmartPointer<vtkPolyData> polygonPolyData = vtkSmartPointer<vtkPolyData>::New();
	polygonPolyData->SetPoints(points);
	polygonPolyData->SetPolys(cells);

	constexpr std::array<unsigned char, 3> red = { 255, 0, 0 };
	constexpr std::array<unsigned char, 3> green = { 0, 255, 0 };
	constexpr std::array<unsigned char, 3> blue = { 0, 0, 255 };

	// 属性数据
	vtkSmartPointer<vtkUnsignedCharArray> pointColors = vtkSmartPointer<vtkUnsignedCharArray>::New();
	pointColors->SetNumberOfComponents(3);  // 元组的大小
	pointColors->SetNumberOfTuples(5); // 预分配5个元组的内存
	pointColors->SetTypedTuple(0, red.data());   // 索引0: 红
	pointColors->SetTypedTuple(1, green.data()); // 索引1: 绿
	pointColors->SetTypedTuple(2, blue.data());  // 索引2: 蓝
	pointColors->SetTypedTuple(3, green.data()); // 索引3: 绿
	pointColors->SetTypedTuple(4, red.data());   // 索引4: 红
	polygonPolyData->GetPointData()->SetScalars(pointColors);  // 点集的属性 将创建的`pointColors`数组设置为单元数据的标量属性

	vtkSmartPointer<vtkUnsignedCharArray> cellColors = vtkSmartPointer<vtkUnsignedCharArray>::New();
	cellColors->SetNumberOfComponents(3);
	cellColors->SetNumberOfTuples(2); // 预分配5个元组的内存
	cellColors->SetTypedTuple(0, red.data());   // 索引0: 红
	cellColors->SetTypedTuple(1, green.data()); // 索引1: 绿
	polygonPolyData->GetCellData()->SetScalars(cellColors);  // 单元集的属性 将创建的`cellColors`数组设置为单元数据的标量属性

	vtkSmartPointer<vtkIntArray> pointfield = vtkSmartPointer<vtkIntArray>::New();
	pointfield->SetName("Field");
	pointfield->SetNumberOfComponents(3);
	pointfield->InsertNextTuple3(1,0,0);
	pointfield->InsertNextTuple3(2,0,0);
	pointfield->InsertNextTuple3(3,0,0);
	pointfield->InsertNextTuple3(4,0,0);
	pointfield->InsertNextTuple3(5,0,0);
	polygonPolyData->GetPointData()->AddArray(pointfield);  // 将创建的数组添加到点属性中

	// 颜色映射表
	vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
	lut->SetNumberOfTableValues(10);  // 设置查找表大小
	lut->Build();
	lut->SetTableValue(0     , 0     , 0     , 0, 1);   // 参数依次为index,r,g,b,a
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
	mapper->SetInputData(polygonPolyData);
	//mapper->SetScalarModeToUseCellData();
	//mapper->SetScalarModeToUsePointData();
	mapper->SetScalarModeToUsePointFieldData();  mapper->ColorByArrayComponent("Field", 1);  // 这两句要结合使用   ColorByArrayComponent指定用哪个组分
	//mapper->SetScalarModeToUsePointFieldData(); mapper->SelectColorArray("Field");  //   这两句要结合使用
	//mapper->SetScalarRange(1,5);  // 设置标量映射的范围
	//mapper->SetLookupTable(lut);

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(actor);
	renderer->SetBackground(1.0,1.0,1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	renderWindow->SetSize( 640, 480 );
	renderWindow->Render();
	renderWindow->SetWindowName("PolyDataColor");

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);

	renderWindow->Render();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}