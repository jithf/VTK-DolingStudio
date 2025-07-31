/**********************************************************************

  �ļ���: 2.4_vtkPipelineExecute.cpp
  Copyright (c) ������, �޻���. All rights reserved.
  ������Ϣ�����: 
    http://www.vtkchina.org (VTK�й�)
	http://blog.csdn.net/www_doling_net (���鹤����) 

**********************************************************************/

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   // ���裺��ʼ��OpenGL��Ⱦ���
VTK_MODULE_INIT(vtkInteractionStyle);   // ���裺��ʼ������ģ��

#include <vtkSmartPointer.h>
#include <vtkBMPReader.h>
#include <vtkImageData.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindow.h>

//�����ļ���data/VTK-logo.bmp
int main(int argc, char* argv[])
{
	//if (argc < 2)
	//{
	//	std::cout<<argv[0]<<" "<<"BMP-File(*.bmp)"<<std::endl;
	//	return EXIT_FAILURE;
	//}

	vtkSmartPointer<vtkBMPReader> reader = vtkSmartPointer<vtkBMPReader>::New();
	std::cout<<"Modification Time of reader (After New()): "<<reader->GetMTime()<<std::endl;
	reader->SetFileName("C://Users//luhy//Desktop//VTK-logo.bmp");
	std::cout<<"Modification Time of reader (After SetFileName()): "<<reader->GetMTime()<<std::endl;

	vtkAlgorithmOutput* outputPort = reader->GetOutputPort();
	vtkImageData* imageData = reader->GetOutput();
	std::cout<<"Modification Time of reader (After GetOutput()): "<<reader->GetMTime()<<std::endl;
	reader->Update();
	std::cout<<"Modification Time of reader (After Update()): "<<reader->GetMTime()<<std::endl;

	int extent[6];
	imageData->GetExtent(extent);  // ��ȡͼ�����ݵķ�Χ
	//-`extent[0]`��x�������С������ͨ��Ϊ0��
	//- `extent[1]`��x����������������� - 1��
	//- `extent[2]`��y�������С������ͨ��Ϊ0��
	//- `extent[3]`��y���������������߶� - 1��
	//- `extent[4]`��z�������С������ͨ��Ϊ0������2Dͼ��
	//- `extent[5]`��z����������������� - 1������2Dͼ��ͨ��Ϊ0��
	std::cout<<"Extent of image: "<<extent[0]<<" "
		<<extent[1]<<" "<<extent[2]<<" "<<extent[3]<<" "
		<<extent[4]<<" "<<extent[5]<<" "<<std::endl;

	vtkSmartPointer<vtkImageViewer2> viewer =  vtkSmartPointer<vtkImageViewer2>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> interactor =  vtkSmartPointer<vtkRenderWindowInteractor>::New();
	viewer->SetupInteractor(interactor);
	viewer->SetInputConnection(outputPort);
	viewer->Render();

	viewer->SetSize(640, 480);
	viewer->GetRenderWindow()->SetWindowName("vtkPipelineExecute");

	interactor->Initialize();
	interactor->Start();

	return EXIT_SUCCESS;
}