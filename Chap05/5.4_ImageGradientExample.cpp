/**********************************************************************

  �ļ���: 5.4_ImageGradientExample.cpp
  Copyright (c) ������, �޻���. All rights reserved.
  ������Ϣ�����: 
    http://www.vtkchina.org (VTK�й�)
	http://blog.csdn.net/www_doling_net (���鹤����) 

**********************************************************************/

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   // ���裺��ʼ��OpenGL��Ⱦ���
VTK_MODULE_INIT(vtkInteractionStyle);   // ���裺��ʼ������ģ��

#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkImageActor.h>
#include <vtkJPEGReader.h>
#include <vtkImageGradient.h>
#include <vtkImageMagnitude.h>
#include <vtkImageCast.h>
#include <vtkImageShiftScale.h>

//����ͼ��../data/lena-gray.jpg
int main(int argc, char* argv[])
{
	//if (argc < 2)
	//{
	//	std::cout<<argv[0]<<" "<<"ImageFile(*.jpg)"<<std::endl;
	//	return EXIT_FAILURE;
	//}

	vtkSmartPointer<vtkJPEGReader> reader =
		vtkSmartPointer<vtkJPEGReader>::New();
	reader->SetFileName("C:\\Users\\luhy\\Desktop\\data\\lena-gray.jpg");
	reader->Update();

	vtkSmartPointer<vtkImageGradient> gradientFilter =
		vtkSmartPointer<vtkImageGradient>::New();
	gradientFilter->SetInputConnection(reader->GetOutputPort());
	gradientFilter->SetDimensionality(2);  // Ҫ�����ͼ��ά��

	vtkSmartPointer<vtkImageMagnitude> magnitudeFilter =
		vtkSmartPointer<vtkImageMagnitude>::New();  // �����ݶ�ʸ����ģ��2��������������ʾ
	magnitudeFilter->SetInputConnection(gradientFilter->GetOutputPort());
	magnitudeFilter->Update();

	double range[2];
	magnitudeFilter->GetOutput()->GetScalarRange(range);  // ֵ���С

	vtkSmartPointer<vtkImageShiftScale> ShiftScale =
		vtkSmartPointer<vtkImageShiftScale>::New();
	ShiftScale->SetOutputScalarTypeToUnsignedChar();
	ShiftScale->SetScale( 255.0 / range[1] );
	ShiftScale->SetInputConnection(magnitudeFilter->GetOutputPort());
	ShiftScale->Update();

	vtkSmartPointer<vtkImageActor> originalActor =
		vtkSmartPointer<vtkImageActor>::New();
	originalActor->SetInputData(reader->GetOutput());

	vtkSmartPointer<vtkImageActor> gradActor =
		vtkSmartPointer<vtkImageActor>::New();
	gradActor->SetInputData(ShiftScale->GetOutput());

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
	gradRenderer->AddActor(gradActor);
	gradRenderer->ResetCamera();
	gradRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(originalRenderer);
	renderWindow->AddRenderer(gradRenderer);
	renderWindow->SetSize( 640, 320 );
	renderWindow->Render();
	renderWindow->SetWindowName("ImageGradientExample");

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
