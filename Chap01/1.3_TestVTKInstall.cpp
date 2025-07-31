/**********************************************************************

  �ļ���: 1.3_TestVTKInstall.cpp
  Copyright (c) ������, �޻���. All rights reserved.
  ������Ϣ�����: 
    http://www.vtkchina.org (VTK�й�)
	http://blog.csdn.net/www_doling_net (���鹤����) 

**********************************************************************/

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   // ���裺��ʼ��OpenGL��Ⱦ���
VTK_MODULE_INIT(vtkInteractionStyle);   // ���裺��ʼ������ģ��

#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>  // �ؼ�������������ͷ�ļ�

int main() {
    // 1. ������Ⱦ����
    vtkSmartPointer<vtkRenderWindow> renWin =  vtkSmartPointer<vtkRenderWindow>::New();
    renWin->SetWindowName("VTK Window Test");
    renWin->SetSize(640, 480);

    // 2. �����������Ⱦ�������������
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renWin->AddRenderer(renderer);

    // 3. �������������ؼ������ִ��ڴ򿪣�
    vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    iren->SetRenderWindow(renWin);  // ������Ⱦ����

    // 4. ��ʼ���������¼�ѭ��
    iren->Initialize();  // ��ʼ��������
    renWin->Render();    // �״���Ⱦ
    iren->Start();       // �����¼�ѭ�������ڱ��ִ򿪣�

    return EXIT_SUCCESS;
}
