/**********************************************************************

  文件名: 10.3_AlgorithmFilterTest.cpp
  Copyright (c) 张晓东, 罗火灵. All rights reserved.
  更多信息请访问: 
    http://www.vtkchina.org (VTK中国)
	http://blog.csdn.net/www_doling_net (东灵工作室) 

**********************************************************************/

#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkPassThrough.h>
#include "vtkTestDataObject.h"
#include "vtkAlgorithmFilter.h"

int main(int argc, char *argv[])
{
	vtkSmartPointer<vtkTestDataObject> input =  vtkSmartPointer<vtkTestDataObject>::New();
	input->SetValue(10);
	int v = input->GetValue();

	vtkSmartPointer<vtkAlgorithmFilter> filter = vtkAlgorithmFilter::New();
	filter->SetFactor(2);

	// 数据对象不能直接作为生产者，需要通过 vtkPassThroughFilter 等适配器
	// 这是关键改进：数据对象需通过生产者端口连接
	vtkSmartPointer<vtkPassThrough> passThrough = vtkSmartPointer<vtkPassThrough>::New();
	passThrough->SetInputData(input);
	passThrough->Update();
	// 验证passThrough->GetOutput()
	vtkTestDataObject* outputOfPassThrough = vtkTestDataObject::SafeDownCast(passThrough->GetOutput());
	if (outputOfPassThrough)
	{
		std::cout << "PassThrough output value: " << outputOfPassThrough->GetValue() << std::endl;
	}
	else
	{
		std::cout << "PassThrough output is not vtkTestDataObject!" << std::endl;
	}

	// 正确连接过滤器管道
	filter->SetInputConnection(passThrough->GetOutputPort());
	filter->Update();

	vtkTestDataObject* output = filter->GetOutput();
	std::cout << "Output value: " << output->GetValue() << std::endl;
	std::cout << "Input  value: " << input->GetValue() << std::endl;

	system("pause");

	return EXIT_SUCCESS;
}