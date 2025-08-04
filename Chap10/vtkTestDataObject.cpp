/**********************************************************************

  文件名: vtkTestDataObject.cpp
  Copyright (c) 张晓东, 罗火灵. All rights reserved.
  更多信息请访问: 
    http://www.vtkchina.org (VTK中国)
	http://blog.csdn.net/www_doling_net (东灵工作室) 

**********************************************************************/

#include "vtkTestDataObject.h"
#include "vtkObjectFactory.h"

vtkStandardNewMacro(vtkTestDataObject);

vtkTestDataObject::vtkTestDataObject()
{
	this->Value = 0.0;
}

vtkTestDataObject::~vtkTestDataObject()
{
}

void vtkTestDataObject::PrintSelf( ostream& os, vtkIndent indent )
{
	this->Superclass::PrintSelf( os, indent );
}

// 浅拷贝
void vtkTestDataObject::ShallowCopy(vtkDataObject* src)
{
	// 调用父类方法
	this->Superclass::ShallowCopy(src);

	// 复制自定义数据
	vtkTestDataObject* source = vtkTestDataObject::SafeDownCast(src);
	if (source)
	{
		this->Value = source->Value; // 确保复制值
	}
}

void vtkTestDataObject::DeepCopy(vtkDataObject* src)
{
	// 调用父类方法
	this->Superclass::DeepCopy(src);

	// 复制自定义数据
	vtkTestDataObject* source = vtkTestDataObject::SafeDownCast(src);
	if (source)
	{
		this->Value = source->Value; // 确保复制值
	}
}