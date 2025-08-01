/**********************************************************************

  �ļ���: 5.4_CannyExample.cpp
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
#include <vtkImageShiftScale.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkImageActor.h>
#include <vtkJPEGReader.h>
#include <vtkImageCast.h>
#include <vtkImageGaussianSmooth.h>
#include <vtkImageGradient.h>
#include <vtkImageMagnitude.h>
#include <vtkImageNonMaximumSuppression.h>
#include <vtkImageConstantPad.h>
#include <vtkImageToStructuredPoints.h>
#include <vtkLinkEdgels.h>
#include <vtkThreshold.h>
#include <vtkGeometryFilter.h>
#include <vtkSubPixelPositionEdgels.h>
#include <vtkCamera.h>
#include <vtkProperty.h>
#include <vtkStripper.h>
#include <vtkPolyDataMapper.h>

//����ͼ��../data/lena-gray.jpg
int main(int argc, char* argv[])
{
	//if (argc < 2)
	//{
	//	std::cout<<argv[0]<<" "<<"ImageFile(*.jpg)"<<std::endl;
	//	return EXIT_FAILURE;
	//}

	vtkSmartPointer<vtkJPEGReader> reader = vtkSmartPointer<vtkJPEGReader>::New();
	reader->SetFileName("C:\\Users\\luhy\\Desktop\\data\\lena-gray.jpg");
	reader->Update();

	//תFloat����
	vtkSmartPointer<vtkImageCast> ic = vtkSmartPointer<vtkImageCast>::New();
	ic->SetOutputScalarTypeToFloat();
	ic->SetInputConnection(reader->GetOutputPort());
	ic->Update();

	//��˹ƽ��
	vtkSmartPointer<vtkImageGaussianSmooth> gs = vtkSmartPointer<vtkImageGaussianSmooth>::New();
	gs->SetInputConnection(ic->GetOutputPort());
	gs->SetDimensionality(2); // ����ά����
	gs->SetRadiusFactors(1, 1, 0); //�˴�С
	gs->Update();
	
	// �����ݶ�
	vtkSmartPointer<vtkImageGradient> imgGradient = vtkSmartPointer<vtkImageGradient>::New();
	imgGradient->SetInputConnection(gs->GetOutputPort());
	imgGradient->SetDimensionality(2);
	imgGradient->Update();

	// �����ݶȵ�ģ
	vtkSmartPointer<vtkImageMagnitude> imgMagnitude = vtkSmartPointer<vtkImageMagnitude>::New();
	imgMagnitude->SetInputConnection(imgGradient->GetOutputPort());
	imgMagnitude->Update();

	// �Ǽ���ֵ����
	vtkSmartPointer<vtkImageNonMaximumSuppression> nonMax = vtkSmartPointer<vtkImageNonMaximumSuppression>::New();
	nonMax->SetMagnitudeInputData(imgMagnitude->GetOutput());
	nonMax->SetVectorInputData(imgGradient->GetOutput());
	nonMax->SetDimensionality(2);  // ����ά����
	nonMax->Update();

	//����ͼ���С
	vtkSmartPointer<vtkImageConstantPad> pad = vtkSmartPointer<vtkImageConstantPad>::New();
	pad->SetInputConnection(imgGradient->GetOutputPort());
	pad->SetOutputNumberOfScalarComponents(3);  // ���ݶ�ͼ�����ص�����޸�Ϊ3
	pad->SetConstant(0);  // ���ӵĿռ�����0
	pad->Update();

	// �� vtkImageData ��ʽת��Ϊ����㼯
	vtkSmartPointer<vtkImageToStructuredPoints> i2sp1 = vtkSmartPointer<vtkImageToStructuredPoints>::New();
	i2sp1->SetInputConnection(nonMax->GetOutputPort());
	i2sp1->SetVectorInputData(pad->GetOutput());
	i2sp1->Update();

	//  ���ݵ�����ڹ�ϵ�������ӳ�����������(Polyline)
	vtkSmartPointer<vtkLinkEdgels> imgLink = vtkSmartPointer<vtkLinkEdgels>::New();
	imgLink->SetInputData(i2sp1->GetOutput());
	imgLink->SetGradientThreshold(2);  // С�ڸ���ֵ�ĵ㲻��
	imgLink->Update();

	// ɾѡ����
	vtkSmartPointer<vtkThreshold> thresholdEdgels = vtkSmartPointer<vtkThreshold>::New();
	thresholdEdgels->SetInputConnection(imgLink->GetOutputPort());
	thresholdEdgels->SetUpperThreshold (10);
	thresholdEdgels->AllScalarsOff();  // �������е�������ֵ�󣬵�Ԫ������
	thresholdEdgels->Update();

	// ������תΪ��������
	vtkSmartPointer<vtkGeometryFilter> gf = vtkSmartPointer<vtkGeometryFilter>::New();
	gf->SetInputConnection(thresholdEdgels->GetOutputPort());
	gf->Update();

	// ͼ������תΪ�ṹ���ĵ㼯
	vtkSmartPointer<vtkImageToStructuredPoints> i2sp = vtkSmartPointer<vtkImageToStructuredPoints>::New();
	i2sp->SetInputConnection(imgMagnitude->GetOutputPort());
	i2sp->SetVectorInputData(pad->GetOutput());
	i2sp->Update();

	//�����ݶȶ�ͼ���Ե����λ�õ���
	vtkSmartPointer<vtkSubPixelPositionEdgels> spe = vtkSmartPointer<vtkSubPixelPositionEdgels>::New();
	spe->SetInputConnection(gf->GetOutputPort());
	spe->SetGradMapsData(i2sp->GetStructuredPointsOutput());

	// ������Ķ���Ρ������λ����߶����������������������߶�
	vtkSmartPointer<vtkStripper> strip = vtkSmartPointer<vtkStripper>::New();
	strip->SetInputConnection(spe->GetOutputPort());

	// Mapper
	vtkSmartPointer<vtkPolyDataMapper> dsm = vtkSmartPointer<vtkPolyDataMapper>::New();
	dsm->SetInputConnection(strip->GetOutputPort());
	dsm->ScalarVisibilityOff();

	vtkSmartPointer<vtkActor> planeActor = vtkSmartPointer<vtkActor>::New();
	planeActor->SetMapper(dsm);
	planeActor->GetProperty()->SetAmbient(1.0);
	planeActor->GetProperty()->SetDiffuse(0.0);
	planeActor->GetProperty()->SetColor(1.0, 0.0, 0.0);

	vtkSmartPointer<vtkImageActor> originalActor = vtkSmartPointer<vtkImageActor>::New();
	originalActor->SetInputData(reader->GetOutput());

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
	gradRenderer->AddActor(planeActor);
	gradRenderer->ResetCamera();
	gradRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->SetSize(900, 300);
	renderWindow->AddRenderer(originalRenderer);
	renderWindow->AddRenderer(gradRenderer);
	renderWindow->Render();
	renderWindow->SetWindowName("CannyExample");

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