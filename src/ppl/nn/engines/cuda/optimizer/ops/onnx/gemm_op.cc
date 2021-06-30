#include "ppl/nn/engines/cuda/optimizer/ops/onnx/gemm_op.h"

#include "ppl/nn/common/logger.h"
#include "ppl/nn/engines/cuda/kernels/onnx/gemm_kernel.h"
#include "ppl/nn/oputils/onnx/reshape_gemm.h"

using namespace std;
using namespace ppl::common;
using namespace ppl::nn::common;

namespace ppl { namespace nn { namespace cuda {

RetCode GemmOp::Init(const OptKernelOptions& options) {
    auto status = GenericLoadParam<GemmParam>(options, &param_.param);
    if (status != RC_SUCCESS) {
        LOG(ERROR) << "load param failed: " << GetRetCodeStr(status);
        return status;
    }

    // TODO (WJF)： use fp32 version temporally
    infer_type_func_ = [this](InputOutputInfo* info, datatype_t type) -> RetCode {
        // type = ppl::common::DATATYPE_FLOAT32;
        type = ppl::common::DATATYPE_FLOAT16;
        return InferDefaultType(info, type);
    };

    infer_dims_func_ = [this](InputOutputInfo* info) -> RetCode {
        return oputils::ReshapeGemm(info, &param_.param);
    };

    return RC_SUCCESS;
}

RetCode GemmOp::Finalize(const OptKernelOptions& options) {
    param_ = *((CudaGemmParam*)options.param);

    auto status = SetCommonParam(options);
    if (status != RC_SUCCESS) {
        LOG(ERROR) << "load common param failed: " << GetRetCodeStr(status);
        return status;
    }

    return RC_SUCCESS;
}

void GemmOp::CopyParam(void*& param) {
    if (param == nullptr) {
        param = new CudaGemmParam();
    }
    *(CudaGemmParam*)param = param_;
    return;
}

KernelImpl* GemmOp::CreateKernelImpl() const {
    return CreateKernelImplWithParam<GemmKernel>(&param_);
}

}}} // namespace ppl::nn::cuda