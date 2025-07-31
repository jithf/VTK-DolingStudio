/**********************************************************************

  文件名: 2.2_TextureExample.cpp
  Copyright (c) 张晓东, 罗火灵. All rights reserved.
  更多信息请访问: 
    http://www.vtkchina.org (VTK中国)
	http://blog.csdn.net/www_doling_net (东灵工作室) 

**********************************************************************/

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   // 必需：初始化OpenGL渲染后端
VTK_MODULE_INIT(vtkInteractionStyle);   // 必需：初始化交互模块

#include <vtkSmartPointer.h>
#include <vtkJPEGReader.h>
#include <vtkTexture.h>
#include <vtkPlaneSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

//测试文件：data/texture.jpg
int main(int argc, char* argv[])
{
	//if (argc < 2)
	//{
	//	std::cout<<argv[0]<<" "<<"TextureFile(*.jpg)"<<std::endl;
	//	return EXIT_FAILURE;
	//}

	vtkSmartPointer< vtkJPEGReader > reader =  vtkSmartPointer< vtkJPEGReader >::New();  // 图片读取器
	// reader->SetFileName(argv[1]);
	reader->SetFileName("C://Users//luhy//Desktop//texture.jpg"); 
	
	vtkSmartPointer< vtkTexture > texture =  vtkSmartPointer< vtkTexture >::New();  // 纹理
	texture->SetInputConnection( reader->GetOutputPort() );
	texture->InterpolateOn();

	vtkSmartPointer< vtkPlaneSource > plane =  vtkSmartPointer< vtkPlaneSource >::New();  // 平面
	vtkSmartPointer< vtkPolyDataMapper > mapper =  vtkSmartPointer< vtkPolyDataMapper >::New();
	mapper->SetInputConnection( plane->GetOutputPort() );

	vtkSmartPointer< vtkActor > actor =  vtkSmartPointer< vtkActor >::New();
	actor->SetMapper( mapper );
	actor->SetTexture( texture );  // 设置纹理  默认位置是 (0,0,0)

    vtkSmartPointer<vtkRenderer> renderer =   vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor( actor );
    renderer->SetBackground( 1.0, 1.0, 1.0);
  
    vtkSmartPointer<vtkRenderWindow> renWin =  vtkSmartPointer<vtkRenderWindow>::New();
    renWin->AddRenderer( renderer );
	renWin->SetSize( 640, 480 );
	renWin->Render();
	renWin->SetWindowName("TextureExample");
   
    vtkSmartPointer<vtkRenderWindowInteractor> iren =   vtkSmartPointer<vtkRenderWindowInteractor>::New();
    iren->SetRenderWindow(renWin);

	iren->Initialize();
    iren->Start();
	
	return EXIT_SUCCESS;
}