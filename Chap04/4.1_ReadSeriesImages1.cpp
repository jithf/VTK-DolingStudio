/**********************************************************************

  �ļ���: 4.1_ReadSeriesImages1.cpp
  Copyright (c) ������, �޻���. All rights reserved.
  ������Ϣ�����: 
    http://www.vtkchina.org (VTK�й�)
	http://blog.csdn.net/www_doling_net (���鹤����) 

**********************************************************************/

#include <vtkSmartPointer.h>
#include <vtkJPEGReader.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkStringArray.h>
#include <vtkRenderWindow.h>


int main()
{
	//����ͼ�����е��ļ�������
	vtkSmartPointer<vtkStringArray > fileArray = vtkSmartPointer< vtkStringArray >::New();
	char fileName[128];
	for(int i = 1; i < 100; i++)
	{
		sprintf(fileName, "C:\\Users\\luhy\\Desktop\\data\\Head\\head%03d.jpg", i);
		std::string fileStr(fileName);
		fileArray->InsertNextValue(fileStr);	
	}

	//��ȡJPG����ͼ��
	vtkSmartPointer<vtkJPEGReader> reader = vtkSmartPointer<vtkJPEGReader>::New();
	reader->SetFileNames(fileArray);

	//��ʾ��ȡ��JPGͼ��
	vtkSmartPointer<vtkImageViewer2> imageViewer = vtkSmartPointer<vtkImageViewer2>::New();
	imageViewer->SetInputConnection(reader->GetOutputPort());

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
	renderWindowInteractor->SetInteractorStyle(style);
	imageViewer->SetupInteractor(renderWindowInteractor);

	imageViewer->SetSlice(50); //Ĭ����ʾ��50����Ƭ(����50��)
	//imageViewer->SetSliceOrientationToXY();  // ��ʾ�ķ��� ��ʾXYƽ��
	//imageViewer->SetSliceOrientationToYZ();
	imageViewer->SetSliceOrientationToXZ();
	imageViewer->Render();

	imageViewer->GetRenderer()->SetBackground(1.0, 1.0, 1.0);
	imageViewer->SetSize(640, 480);
	imageViewer->GetRenderWindow()->SetWindowName("ReadSeriesImages1");

	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}