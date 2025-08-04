/**********************************************************************

  文件名: 10.1_ClassTest.cpp
  Copyright (c) 张晓东, 罗火灵. All rights reserved.
  更多信息请访问: 
    http://www.vtkchina.org (VTK中国)
	http://blog.csdn.net/www_doling_net (东灵工作室) 

**********************************************************************/

#include "vtkAutoInit.h" 
VTK_MODULE_INIT(vtkRenderingOpenGL2);     // VTK was built with vtkRenderingOpenGL2
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);    // Build with vtkTextActor

#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include "vtkTestClass.h"

int main(int argc, char *argv[])
{
	vtkSmartPointer<vtkTestClass> test = vtkSmartPointer<vtkTestClass>::New();
	test->Print(std::cout);

	test->FlagOn();  // 将Flag变量设为true
	test->SetPosition(10, 10, 10);
	test->SetSpeed(1000);
	test->Print(std::cout);

	system("pause");

	return EXIT_SUCCESS;
}