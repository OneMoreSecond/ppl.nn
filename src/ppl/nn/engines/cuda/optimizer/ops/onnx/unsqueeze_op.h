// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#ifndef _ST_HPC_PPL_NN_ENGINES_CUDA_OPTIMIZER_OPS_ONNX_UNSQUEEZE_OP_H_
#define _ST_HPC_PPL_NN_ENGINES_CUDA_OPTIMIZER_OPS_ONNX_UNSQUEEZE_OP_H_

#include "ppl/nn/engines/cuda/optimizer/opt_kernel.h"

#include "ppl/nn/params/onnx/unsqueeze_param.h"

namespace ppl { namespace nn { namespace cuda {

class UnsqueezeOp final : public CudaOptKernel {
public:
    UnsqueezeOp(const ir::Node* node) : CudaOptKernel(node) {}
    KernelImpl* CreateKernelImpl() const override;
    ppl::common::RetCode Init(const OptKernelOptions&) override;
    ppl::common::RetCode Finalize(const OptKernelOptions& options) override;

    void* GetParam() override {
        return (void*)&param_;
    };
    bool CompareParam(CudaOptKernel* other) override {
        if (other->GetNode()->GetType().name == "Unsqueeze") {
            ppl::nn::common::UnsqueezeParam temp = *((ppl::nn::common::UnsqueezeParam*)other->GetParam());
            if (param_.axes.size() != temp.axes.size()) {
                return false;
            }
            for (uint32_t i = 0; i < temp.axes.size(); ++i) {
                if (param_.axes[i] != temp.axes[i]) {
                    return false;
                }
            }
        }
        return true;
    }

private:
    ppl::nn::common::UnsqueezeParam param_;
};

}}} // namespace ppl::nn::cuda

#endif
