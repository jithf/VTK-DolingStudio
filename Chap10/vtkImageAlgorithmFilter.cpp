/**********************************************************************

  文件名: vtkImageAlgorithmFilter.cpp
  Copyright (c) 张晓东, 罗火灵. All rights reserved.
  更多信息请访问: 
    http://www.vtkchina.org (VTK中国)
	http://blog.csdn.net/www_doling_net (东灵工作室) 

**********************************************************************/

#include "vtkImageAlgorithmFilter.h"
#include "vtkImageData.h"
#include "vtkObjectFactory.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"
#include "vtkDataObject.h"
#include "vtkSmartPointer.h"

vtkStandardNewMacro(vtkImageAlgorithmFilter);

// 重写RequestData 方法
int vtkImageAlgorithmFilter::RequestData(vtkInformation *vtkNotUsed(request),
										 vtkInformationVector **inputVector,
										 vtkInformationVector *outputVector)
{
	vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
	vtkInformation *outInfo = outputVector->GetInformationObject(0);
	vtkImageData *input = vtkImageData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
	vtkImageData *output = vtkImageData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

	vtkSmartPointer<vtkImageData> image =vtkSmartPointer<vtkImageData>::New();
	image->ShallowCopy(input);
	image->SetScalarComponentFromDouble(0,0,0,0, 5.0);   // 设（0,0,0)坐标下的第0个坐标的值为5.0
	output->ShallowCopy(image);

	// 设置输出图像的范围信息（与输入保持一致）
	output->SetExtent(input->GetExtent());  // 设置当前处理范围
	//output->SetUpdateExtent(output->GetExtent());    // 设置更新范围
	//output->SetWholeExtent(output->GetExtent());     // 设置整体数据范围
	// 通过输出信息对象（outInfo）设置范围，替代原SetWholeExtent和SetUpdateExtent
	// 第三个参数 6 表示范围数组的长度（三维范围包含 6 个值：xmin, xmax, ymin, ymax, zmin, zmax）
	outInfo->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), output->GetExtent(), 6);
	outInfo->Set(vtkStreamingDemandDrivenPipeline::UPDATE_EXTENT(), output->GetExtent(), 6);
	return 1;
}