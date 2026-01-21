#include "hgonnxdetect.h"
#include "onnxruntime_cxx_api.h"

namespace HGMACHINE {

HGOnnxDetect::HGOnnxDetect()
{

}
HGOnnxDetect::~HGOnnxDetect()
{

}

bool HGOnnxDetect::detect(cv::Mat& frame,const cv::Rect& roi)
{
    // // set the session options
    // Ort::SessionOptions sessionOptions;
    // sessionOptions.SetIntraOpNumThreads(1);
    // sessionOptions.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_EXTENDED);

    // // path to the onnx model file
    // std::string modelPath = "/mnt/extra/projects/Git/mydeepseektest/best.onnx";

    // printf("create the onnx runtime session...\n");
    // // create the onnx runtime session
    // Ort::Env env(ORT_LOGGING_LEVEL_WARNING,"ONNXRuntime");
    // printf("create the onnx runtime session1111111...\n");
    // Ort::Session onnxSession(env,modelPath.c_str(),sessionOptions);

    // printf("onnx session created successfully\n");

    // Ort::AllocatorWithDefaultOptions allocator;

    // // Get  input and ouput count
    // size_t inputCount=onnxSession.GetInputCount();
    // const char* inputName=onnxSession.GetInputName(0,allocator);
    // auto inputTypeInfo=onnxSession.GetInputTypeInfo(0);
    // auto tensorInfo=inputTypeInfo.GetTensorTypeAndShapeInfo();
    // auto inputDims=tensorInfo.GetShape();

    // std::cout<<"Model input name: "<<inputName<<std::endl;

    // //
    // cv::Mat image;
    // cv::cvtColor(frame,image,cv::COLOR_BGR2RGB);
    // cv::resize(image,image,cv::Size(inputDims[3],inputDims[2]));
    // image.convertTo(image,CV_32FC3,1.0/255.0);

    // // from HWC to CHW
    // std::vector<float> inputTensorValues(inputDims[1] * inputDims[2] * inputDims[3]);
    // std::vector<cv::Mat> channels(inputDims[1]);
    // for (size_t i=0;i<inputDims[1];++i){
    //     channels[i]=cv::Mat(inputDims[2],inputDims[3],CV_32F,inputTensorValues.data()+i*inputDims[2]*inputDims[3]);
    // }
    // cv::split(image,channels);

    // // create input tensor
    // Ort::MemoryInfo memoryInfo=Ort::MemoryInfo::CreateCpu(OrtArenaAllocator,OrtMemTypeDefault);
    // Ort::Value inputTensor=Ort::Value::CreateTensor<float>(
    //     memoryInfo,inputTensorValues.data(),inputTensorValues.size(),
    //     inputDims.data(),inputDims.size());

    // // Run the model
    // const char* outputName=onnxSession.GetOutputName(0,allocator);
    // std::vector<const char*> inputNames={inputName};
    // std::vector<const char*> outputNames={outputName};
    // auto outputTensors=onnxSession.Run(
    //     Ort::RunOptions{nullptr},inputNames.data(),
    //     &inputTensor,1,outputNames.data(),1);
    
    // // print the result
    // float* outputData=outputTensors[0].GetTensorMutableData<float>();
    // auto outputTypeInfo=outputTensors[0].GetTensorTypeAndShapeInfo();
    // auto outputShape=outputTypeInfo.GetShape();

    // std::cout<<"Output shape: ";
    // for (auto s : outputShape) std::cout<<s<<" ";
    // std::cout<<std::endl;

    // std::cout<<"First 10 output values: ";
    // for (int i=0;i<std::min<size_t>(10,outputTypeInfo.GetElementCount());++i){
    //     std::cout<<outputData[i]<<" ";
    // }
    // std::cout<<std::endl;
    
    return 0;
}

}
