/**********************************************************************

  文件名: vtkAlgorithmFilter.cpp
  Copyright (c) 张晓东, 罗火灵. All rights reserved.
  更多信息请访问: 
    http://www.vtkchina.org (VTK中国)
	http://blog.csdn.net/www_doling_net (东灵工作室) 

**********************************************************************/

#include "vtkAlgorithmFilter.h"
#include "vtkTestDataObject.h"

#include "vtkPassThrough.h"
#include "vtkCommand.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkStreamingDemandDrivenPipeline.h"

vtkStandardNewMacro(vtkAlgorithmFilter);

//----------------------------------------------------------------------------
vtkAlgorithmFilter::vtkAlgorithmFilter()
{
	Factor = 2.0;
	this->SetNumberOfInputPorts( 1 );  // 1输入
	this->SetNumberOfOutputPorts( 1 ); // 1输出
}

//----------------------------------------------------------------------------
vtkAlgorithmFilter::~vtkAlgorithmFilter()
{
}

//----------------------------------------------------------------------------
// 打印方法
void vtkAlgorithmFilter::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);
}

//----------------------------------------------------------------------------
// 核心请求处理
int vtkAlgorithmFilter::ProcessRequest(vtkInformation* request,
									   vtkInformationVector** inputVector,
									   vtkInformationVector* outputVector)
{
	// 创建输出数据对象（确保输出结构正确）
	// 例如：调用 vtkAlgorithm::Update() 或 UpdateInformation() 
	if(request->Has(vtkDemandDrivenPipeline::REQUEST_DATA_OBJECT()))
	{
		return this->RequestDataObject(request, inputVector, outputVector);
	}

	// 执行实际的数据处理（核心逻辑）
	// 例如：调用 vtkAlgorithm::Update() 或 vtkAlgorithm::GetOutput() 
	if(request->Has(vtkDemandDrivenPipeline::REQUEST_DATA()))
	{
		return this->RequestData(request, inputVector, outputVector);
	}

	// 商需要处理的数据范围（如分块数据的处理范围）
	// 例如：调用 vtkStreamingDemandDrivenPipeline::SetUpdateExtent() 
	if(request->Has(vtkStreamingDemandDrivenPipeline::REQUEST_UPDATE_EXTENT()))
	{
		return this->RequestUpdateExtent(request, inputVector, outputVector);
	}

	// 传递元数据（如数据维度、范围等）
	// 例如：首次调用 Update()  或 调用vtkAlgorithm::UpdateInformation()  时
	if(request->Has(vtkDemandDrivenPipeline::REQUEST_INFORMATION()))
	{
		return this->RequestInformation(request, inputVector, outputVector);
	}

	//在典型的管道执行流程中，请求的触发顺序为：
	//	REQUEST_DATA_OBJECT（确保输出对象存在）
	//	REQUEST_INFORMATION（传递元数据）
	//	REQUEST_UPDATE_EXTENT（协商数据范围）
	//	REQUEST_DATA（执行核心处理）

	// 上传到父类的请求处理
	return this->Superclass::ProcessRequest(request, inputVector, outputVector);
}

//----------------------------------------------------------------------------
// 输出端口数据类型
int vtkAlgorithmFilter::FillOutputPortInformation(
	int vtkNotUsed(port), vtkInformation* info)
{
	info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkTestDataObject");
	return 1;
}

//----------------------------------------------------------------------------
// 输入端口数据类型
int vtkAlgorithmFilter::FillInputPortInformation(
	int vtkNotUsed(port), vtkInformation* info)
{
	info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkTestDataObject");
	return 1;
}

//----------------------------------------------------------------------------
// 输出数据对象创建 RequestDataObject
int vtkAlgorithmFilter::RequestDataObject(vtkInformation* vtkNotUsed(request),
										  vtkInformationVector** vtkNotUsed(inputVector),
										  vtkInformationVector* outputVector )
{
	vtkInformation* outInfo = outputVector->GetInformationObject(0);
	vtkTestDataObject* output = vtkTestDataObject::SafeDownCast( outInfo->Get( vtkDataObject::DATA_OBJECT() ) );

	// 如果输出端口尚未关联数据对象，新建一个 vtkTestDataObject 并绑定到输出端口
	if ( ! output )
	{
		output = vtkTestDataObject::New();
		outInfo->Set( vtkDataObject::DATA_OBJECT(), output );
		output->FastDelete();  // FastDelete() 用于释放临时创建的对象引用  （VTK 引用计数机制，避免内存泄漏）
		output->SetInformation( outInfo );

		this->GetOutputPortInformation(0)->Set(vtkDataObject::DATA_EXTENT_TYPE(), output->GetExtentType() );
	}

	return 1;
}

//----------------------------------------------------------------------------
// 信息传递
int vtkAlgorithmFilter::RequestInformation(
	vtkInformation* vtkNotUsed(request),
	vtkInformationVector** vtkNotUsed(inputVector),
	vtkInformationVector* vtkNotUsed(outputVector))
{
	return 1;
}

//----------------------------------------------------------------------------
// 更新范围协商
// 用于协商输入数据的处理范围：设置 EXACT_EXTENT 为 1，
// 告诉上游过滤器 “必须提供精确范围的数据”（而非更大范围），
// 确保只处理必要的数据，提高效率
int vtkAlgorithmFilter::RequestUpdateExtent(
	vtkInformation* vtkNotUsed(request),
	vtkInformationVector** inputVector,
	vtkInformationVector* vtkNotUsed(outputVector))
{
	int numInputPorts = this->GetNumberOfInputPorts();
	for (int i=0; i<numInputPorts; i++)
	{
		int numInputConnections = this->GetNumberOfInputConnections(i);
		for (int j=0; j<numInputConnections; j++)
		{
			vtkInformation* inputInfo = inputVector[i]->GetInformationObject(j);
			inputInfo->Set(vtkStreamingDemandDrivenPipeline::EXACT_EXTENT(), 1);
		}
	}
	return 1;
}

//----------------------------------------------------------------------------
// 核心数据处理
int vtkAlgorithmFilter::RequestData(
									vtkInformation* vtkNotUsed(request),
									vtkInformationVector **inputVector,
									vtkInformationVector* outputVector )
{
	// 获取输出数据对象（已有代码）
	vtkInformation* outInfo = outputVector->GetInformationObject(0);
	vtkTestDataObject* output = vtkTestDataObject::SafeDownCast(outInfo->Get( vtkDataObject::DATA_OBJECT() ) );

	// 获取输入数据对象（核心步骤）
	// 从输入向量中获取第0个端口的第0个连接的信息
	vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
	// 从信息中提取vtkTestDataObject对象（使用SafeDownCast确保类型安全）
	//vtkTestDataObject* input = vtkTestDataObject::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
	// 获取输入数据对象 (此时是passThrough的输出)
	vtkDataObject* inputDO = inInfo->Get(vtkDataObject::DATA_OBJECT());
	if (!inputDO) {
		vtkErrorMacro("输入数据对象为空");
		return 0;
	}
	// 安全向下转型到具体类型
	vtkTestDataObject* input = vtkTestDataObject::SafeDownCast(inputDO);
	if (!input) {
		vtkErrorMacro("无法获取原始vtkTestDataObject输入");
	} 

	// 判空处理（避免空指针错误）
	if (!input || !output)
	{
		vtkErrorMacro("输入或输出数据对象为空！");
		return 0; // 失败返回0
	}

	// 4. 获取input中的值（假设vtkTestDataObject有GetValue()方法）
	double inputValue = input->GetValue(); // 获取输入值（例如你设置的10）
	vtkDebugMacro("获取到的输入值为: " << inputValue); // 调试输出
	std::cout << "获取到的输入值为: " << inputValue << std::endl;
	
	// 复制输入数据到输出，然后将输出值增加Factor（2.0）
	output->ShallowCopy(input);
	output->SetValue(output->GetValue() + this->GetFactor());
	double outputValue = output->GetValue(); // 获取输入值（例如你设置的10）
	vtkDebugMacro("获取到的输出值为: " << outputValue); // 调试输出
	std::cout << "获取到的输出值为: " << outputValue << std::endl;

	return 1;
}

//----------------------------------------------------------------------------
//  输入输出便捷方法
vtkTestDataObject* vtkAlgorithmFilter::GetOutput()
{
	return this->GetOutput(0);
}

//----------------------------------------------------------------------------
vtkTestDataObject* vtkAlgorithmFilter::GetOutput(int port)
{
	return vtkTestDataObject::SafeDownCast(this->GetOutputDataObject(port));
}

//----------------------------------------------------------------------------
void vtkAlgorithmFilter::SetOutput(vtkDataObject* d)
{
	this->GetExecutive()->SetOutputData(0, d);
}

//----------------------------------------------------------------------------
vtkDataObject* vtkAlgorithmFilter::GetInput()
{
	return this->GetInput(0);
}

//----------------------------------------------------------------------------
vtkDataObject* vtkAlgorithmFilter::GetInput(int port)
{
	return this->GetExecutive()->GetInputData(port, 0);
}

//----------------------------------------------------------------------------
void vtkAlgorithmFilter::SetInput(vtkPassThrough* input)
{
	this->SetInput(0, input);
}

//----------------------------------------------------------------------------
void vtkAlgorithmFilter::SetInput(int index, vtkPassThrough* input)
{
	if(input)
	{
		this->SetInputConnection(index, input->GetOutputPort());
	}
	else
	{
		this->SetInputConnection(index, 0);
	}
}

//----------------------------------------------------------------------------
void vtkAlgorithmFilter::AddInput(vtkPassThrough* input)
{
	this->AddInput(0, input);
}

//----------------------------------------------------------------------------
void vtkAlgorithmFilter::AddInput(int index, vtkPassThrough* input)
{
	if(input)
	{
		this->AddInputConnection(index, input->GetOutputPort());
	}
}
