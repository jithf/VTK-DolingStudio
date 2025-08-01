/**********************************************************************

  �ļ���: 4.1_ReadUnknowFormatImage.cpp
  Copyright (c) ������, �޻���. All rights reserved.
  ������Ϣ�����: 
    http://www.vtkchina.org (VTK�й�)
	http://blog.csdn.net/www_doling_net (���鹤����) 

**********************************************************************/

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   // ���裺��ʼ��OpenGL��Ⱦ���
VTK_MODULE_INIT(vtkInteractionStyle);   // ���裺��ʼ������ģ��

#include <vtkSmartPointer.h>
#include <vtkImageReader2Factory.h>
#include <vtkImageReader2.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

//�����ļ���data/VTK-logo
int main ( int argc, char *argv[])
{
	//if(argc != 2)
	//{
	//	std::cout << "Required arguments: Filename" << std::endl;
	//	return EXIT_FAILURE;
	//}
	// 
	//��ȡͼ��
	vtkSmartPointer<vtkImageReader2Factory> readerFactory = vtkSmartPointer<vtkImageReader2Factory>::New();
	vtkImageReader2 * reader = readerFactory->CreateImageReader2("C:\\Users\\luhy\\Desktop\\data\\VTK-logo.png");  // �����ļ��������������Ӧ���͵�Reader
	reader->SetFileName("C:\\Users\\luhy\\Desktop\\data\\VTK-logo.png");
	reader->Update();

	//��ʾ��ȡ��ͼ��
	vtkSmartPointer<vtkImageViewer2> imageViewer = vtkSmartPointer<vtkImageViewer2>::New();
	imageViewer->SetInputConnection(reader->GetOutputPort());
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	imageViewer->SetupInteractor(renderWindowInteractor);
	imageViewer->Render();
	imageViewer->GetRenderer()->ResetCamera();
	imageViewer->Render();

	imageViewer->SetSize(640, 480);
	imageViewer->GetRenderWindow()->SetWindowName("ReadUnknowFormatImage");

	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}
