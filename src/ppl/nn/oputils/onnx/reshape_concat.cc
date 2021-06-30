#include "ppl/nn/oputils/onnx/reshape_concat.h"
#include "ppl/nn/common/logger.h"
#include "ppl/nn/runtime/tensor_impl.h"
using namespace ppl::common;
using namespace ppl::nn::common;

namespace ppl { namespace nn { namespace oputils {

RetCode ReshapeConcat(InputOutputInfo* info, const void* arg) {
    auto param = (const ConcatParam*)arg;
    const TensorShape& in_shape0 = info->GetInput<TensorImpl>(0)->GetShape();
    uint32_t fixed_axis =
        param->axis >= 0 ? param->axis : param->axis + info->GetInput<TensorImpl>(0)->GetShape().GetDimCount();

    std::vector<int64_t> output_dim(in_shape0.GetDimCount());
    for (uint32_t i = 0; i < in_shape0.GetDimCount(); ++i) {
        if (i == fixed_axis) {
            output_dim[i] = 0;
            for (uint32_t j = 0; j < info->GetInputCount(); ++j) {
                output_dim[i] += info->GetInput<TensorImpl>(j)->GetShape().GetDim(i);
            }
        } else {
            for (uint32_t j = 1; j < info->GetInputCount(); ++j) {
                if (info->GetInput<TensorImpl>(j)->GetShape().GetDim(i) != in_shape0.GetDim(i)) {
                    LOG(ERROR) << "input shape not match.";
                    return RC_INVALID_VALUE;
                }
            }
            output_dim[i] = in_shape0.GetDim(i);
        }
    }

    info->GetOutput<TensorImpl>(0)->GetShape().Reshape(output_dim);

    return RC_SUCCESS;
}

}}} // namespace ppl::nn::oputils