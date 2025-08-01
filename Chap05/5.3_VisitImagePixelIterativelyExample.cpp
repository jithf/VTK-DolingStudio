/**********************************************************************

  文件名: 5.3_VisitImagePixelIterativelyExample.cpp
  Copyright (c) 张晓东, 罗火灵. All rights reserved.
  更多信息请访问: 
    http://www.vtkchina.org (VTK中国)
	http://blog.csdn.net/www_doling_net (东灵工作室) 

**********************************************************************/

#include <vtkSmartPointer.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkBMPReader.h>
#include <vtkImageData.h>
#include <vtkImageIterator.h>

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

	int subRegion[6] = {0,300, 0, 300, 0, 0};  // 迭代范围
	vtkImageIterator<unsigned char> it(reader->GetOutput(), subRegion);  // 迭代器

	while(!it.IsAtEnd())
	{
		unsigned char *inSI = it.BeginSpan();  // 元组开始位置
		unsigned char *inSIEnd = it.EndSpan();  // 元组结束位置

		while(inSI != inSIEnd)
		{
			*inSI = 255-*inSI;
			++inSI;
		}
		it.NextSpan();
	}

	vtkSmartPointer<vtkImageViewer2> imageViewer = vtkSmartPointer<vtkImageViewer2>::New();
	imageViewer->SetInputConnection(reader->GetOutputPort());

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	imageViewer->SetupInteractor(renderWindowInteractor);
	imageViewer->Render();
	imageViewer->GetRenderer()->ResetCamera();
	imageViewer->Render();
	imageViewer->GetRenderer()->SetBackground(1.0, 1.0, 1.0);
	imageViewer->SetSize(640, 480);
	imageViewer->GetRenderWindow()->SetWindowName("VisitImagePixelIterativelyExample");

	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}
