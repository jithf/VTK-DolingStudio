#**********************************************************************
#
#  Copyright (c) ÕÅÏþ¶«, ÂÞ»ðÁé. All rights reserved.
#  ¸ü¶àÐÅÏ¢Çë·ÃÎÊ: 
#    http://www.vtkchina.org (VTKÖÐ¹ú)
#	  http://blog.csdn.net/www_doling_net (¶«Áé¹¤×÷ÊÒ) 
#
#**********************************************************************

CMAKE_MINIMUM_REQUIRED( VERSION 4.1 )
PROJECT( Qt )

#----------------------------------------------------------------------------------
FIND_PACKAGE( VTK 9.3 REQUIRED ) 

set(CMAKE_PREFIX_PATH "C:/Qt/Qt5.15.2/5.15.2/msvc2019_64")
FIND_PACKAGE( Qt5 REQUIRED COMPONENTS Core Gui Widgets)

#----------------------------------------------------------------------------------
# 项目源文件
SET( PROJECT_SRCS
    main.cpp
    ProjectMainWindow.cpp
    )

# UI文件
SET( PROJECT_UIS
    ProjectMainWindow.ui
)

# 需要MOC处理的头文件
SET( PROJECT_MOC_HDRS
  ProjectMainWindow.h
)

#----------------------------------------------------------------------------------

# Qt5相关处理
QT5_WRAP_UI(PROJECT_UIS_H ${PROJECT_UIS})  # 处理UI文件
QT5_WRAP_CPP(PROJECT_MOC_SRCS ${PROJECT_MOC_HDRS})  # 处理MOC

#----------------------------------------------------------------------------------

# 包含目录
INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR} 
                     ${CMAKE_CURRENT_BINARY_DIR}  # 包含MOC和UI生成的文件
                     ${VTK_DIR} 
                   )

# 添加可执行文件
ADD_EXECUTABLE( ${PROJECT_NAME}
                ${PROJECT_SRCS} 
                ${PROJECT_UIS_H} 
                ${PROJECT_MOC_SRCS} 
              )

# 链接库
TARGET_LINK_LIBRARIES ( ${PROJECT_NAME}
  ${VTK_LIBRARIES}
  VTK::CommonCore
  VTK::RenderingOpenGL2
  VTK::InteractionStyle
  VTK::RenderingFreeType
  VTK::IOImage
  Qt5::Core
  Qt5::Gui
  Qt5::Widgets
  )

# 包含目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${VTK_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
)

#-----------------------------------------------------------------------------------------------------------------------------------
# Construct a list of paths containing runtime 
# directories for project applications on Windows
SET(PROJECT_RUNTIME_PATH 
    "${VTK_LIBRARY_DIRS}/@VS_BUILD_TYPE@;${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/@VS_BUILD_TYPE@"
    )
IF(QT4_FOUND)
  SET(PROJECT_RUNTIME_PATH "${PROJECT_RUNTIME_PATH};${QT_LIBRARY_DIR}/../bin")
ENDIF()

INCLUDE(CreateWindowsBatchScript.cmake)

# If we are under Windows, create two batch files which correctly
# set up the environment for the application and for Visual Studio
IF(WIN32)
  SET(VS_SOLUTION_FILE "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.sln")
  FOREACH(VS_BUILD_TYPE debug release)
    CreateWindowsBatchScript("${CMAKE_SOURCE_DIR}/StartVS.bat.in"
      ${PROJECT_BINARY_DIR}/StartVS_${VS_BUILD_TYPE}.bat
      ${VS_BUILD_TYPE})
  ENDFOREACH()
ENDIF(WIN32)