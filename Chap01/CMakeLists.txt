#**********************************************************************
#
#  Copyright (c) ÕÅÏþ¶«, ÂÞ»ðÁé. All rights reserved.
#  ¸ü¶àÐÅÏ¢Çë·ÃÎÊ: 
#    http://www.vtkchina.org (VTKÖÐ¹ú)
#    http://blog.csdn.net/www_doling_net (¶«Áé¹¤×÷ÊÒ) 
#
#**********************************************************************

CMAKE_MINIMUM_REQUIRED(VERSION 4.1)  
PROJECT( Chap01 )

FIND_PACKAGE( VTK REQUIRED )

#SET( VTK_DIR "D:/VTK/VTK9.3.1-src/VTK-9.3.1/VTK-9.3.1-prefix")

INCLUDE( ${VTK_USE_FILE} )  # VTK_USE_FILE变量在VTKConfig.cmake中定义

ADD_EXECUTABLE( 1.3_TestVTKInstall 1.3_TestVTKInstall.cpp)
TARGET_LINK_LIBRARIES( 1.3_TestVTKInstall vtkRendering vtkCommon ) #链接库

#-----------------------------------------------------------------------------------------------------------------------------------
# Construct a list of paths containing runtime directories for project applications on Windows.
# Create two batch files which correctly set up the environment for the application and for Visual Studio.
# 生成用于启动 Visual Studio 的批处理脚本，并确保这些脚本适用于不同的构建类型（Debug 和 Release）
IF(WIN32)  # 如果是windows系统
  SET(PROJECT_RUNTIME_PATH "${VTK_DIR}/bin/@VS_BUILD_TYPE@") # 设置PROJECT_RUNTIME_PATH  @VS_BUILD_TYPE@ 是一个占位符
  INCLUDE(CreateWindowsBatchScript.cmake) # CreateWindowsBatchScript.cmake 脚本通常定义了一个用于创建 Windows 批处理脚本的函数
  SET(VS_SOLUTION_FILE "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.sln")
  FOREACH(VS_BUILD_TYPE Debug Release)
    CreateWindowsBatchScript("${CMAKE_SOURCE_DIR}/StartVS.bat.in"  # 输入模板文件的路径
      ${PROJECT_BINARY_DIR}/StartVS_${VS_BUILD_TYPE}.bat  # 指定生成的批处理脚本文件的输出路径
      ${VS_BUILD_TYPE})   # 生成一个批处理脚本文件
  ENDFOREACH()
ENDIF(WIN32)
