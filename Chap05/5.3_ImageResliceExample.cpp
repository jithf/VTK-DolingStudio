/**********************************************************************

  �ļ���: 5.3_ImageResliceExample.cpp
  Copyright (c) ������, �޻���. All rights reserved.
  ������Ϣ�����: 
    http://www.vtkchina.org (VTK�й�)
	http://blog.csdn.net/www_doling_net (���鹤����) 

**********************************************************************/

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   // ���裺��ʼ��OpenGL��Ⱦ���
VTK_MODULE_INIT(vtkInteractionStyle);   // ���裺��ʼ������ģ��

#include <vtkSmartPointer.h>
#include <vtkMatrix4x4.h>
#include <vtkImageReslice.h>
#include <vtkImageActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkImageData.h>
#include <vtkMetaImageReader.h>
#include <vtkImageCast.h>
#include <vtkLookupTable.h>
#include <vtkImageMapToColors.h>

//����ͼ��..\\data\\brain.mhd
int main(int argc, char* argv[])
{
	//if (argc < 2)
	//{
	//	std::cout<<argv[0]<<" "<<"ImageFile(*.mhd)"<<std::endl;
	//	return EXIT_FAILURE;
	//}

	vtkSmartPointer<vtkMetaImageReader> reader = vtkSmartPointer<vtkMetaImageReader>::New();
	reader->SetFileName("C:\\Users\\luhy\\Desktop\\data\\brain.mhd");
	reader->Update();

	int extent[6];
	double spacing[3];
	double origin[3];

	reader->GetOutput()->GetExtent(extent);   //ģ�ͳߴ�
	reader->GetOutput()->GetSpacing(spacing); //���ؼ��
	reader->GetOutput()->GetOrigin(origin); // ԭ��

	double center[3];  // ģ������
	center[0] = origin[0] + spacing[0] * 0.5 * (extent[0] + extent[1]);
	center[1] = origin[1] + spacing[1] * 0.5 * (extent[2] + extent[3]);
	center[2] = origin[2] + spacing[2] * 0.5 * (extent[4] + extent[5]);

	// ����ı任���� ǰ3�зֱ�ΪX\Y\Z����ʸ�� ��4��Ϊ��������ϵԭ��juzheng
    // ͨ���޸���������ϵԭ�㣬���Եõ���ͬλ�õ�����ͼ��
	// ���������ʱ��Ҳ����ʹ������ƽ�棬����������ƽ�棬���Ǳ����ͼ���ڲ���
	static double axialElements[16] = { 
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1 
	};

	// ��Ƭ
	vtkSmartPointer<vtkMatrix4x4> resliceAxes = vtkSmartPointer<vtkMatrix4x4>::New();
	resliceAxes->DeepCopy(axialElements);   // ���
	// �����е���Ԫ�ر�ʾ��������ϵ��ͼ������ϵһ�£��Ҿ���ͼ�����ĵ�center
	resliceAxes->SetElement(0, 3, center[0]);
	resliceAxes->SetElement(1, 3, center[1]);
	resliceAxes->SetElement(2, 3, center[2]);

	//��ɫӳ��
	vtkSmartPointer<vtkImageReslice> reslice = vtkSmartPointer<vtkImageReslice>::New();
	reslice->SetInputConnection(reader->GetOutputPort());
	reslice->SetOutputDimensionality(2);    //ָ�����ά������2
	reslice->SetResliceAxes(resliceAxes);  // ���ñ任����
	reslice->SetInterpolationModeToLinear();  // ָ��������ȡ�Ĳ�ֵ��ʽΪ���Բ�ֵ

	vtkSmartPointer<vtkLookupTable> colorTable = vtkSmartPointer<vtkLookupTable>::New();
	colorTable->SetRange(0, 1000);   // ����ֵ�ķ�Χ
	colorTable->SetValueRange(0.0, 1.0);  // HSV��ɫ�ռ��е�V�����ȣ���Χ
	colorTable->SetSaturationRange(0.0, 0.0);  // HSV��ɫ�ռ��е�S�����Ͷȣ���Χ
	colorTable->SetRampToLinear();  //  ������ɫӳ��Ľ��䷽ʽΪ���Բ�ֵ
	colorTable->Build();  // ������ɫӳ���

	vtkSmartPointer<vtkImageMapToColors> colorMap = vtkSmartPointer<vtkImageMapToColors>::New();
	colorMap->SetLookupTable(colorTable);
	colorMap->SetInputConnection(reslice->GetOutputPort());
	colorMap->Update();

	vtkSmartPointer<vtkImageActor> imgActor = vtkSmartPointer<vtkImageActor>::New();
	imgActor->SetInputData(colorMap->GetOutput());

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(imgActor);
	renderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	renderWindow->Render();
	renderWindow->SetSize(640, 480);
	renderWindow->SetWindowName("ImageResliceExample");

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> imagestyle = vtkSmartPointer<vtkInteractorStyleImage>::New();
	renderWindowInteractor->SetInteractorStyle(imagestyle);
	renderWindowInteractor->SetRenderWindow(renderWindow);

	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}
