/*******************************************************************************
* Copyright 2021 Arm Ltd. and affiliates
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#include "cpu/aarch64/acl_softmax.hpp"

namespace dnnl {
namespace impl {
namespace cpu {
namespace aarch64 {

template <impl::data_type_t data_type>
status_t acl_softmax_fwd_t<data_type>::execute_forward(
        const exec_ctx_t &ctx) const {

    // Lock here is needed because resource_mapper does not support
    // concurrent multithreaded access.
    std::lock_guard<std::mutex> _lock {this->mtx};

    auto src = CTX_IN_MEM(const data_t *, DNNL_ARG_SRC);
    auto dst = CTX_OUT_MEM(data_t *, DNNL_ARG_DST);

    // Retrieve primitive resource and configured Compute Library objects
    auto *acl_resource
            = ctx.get_resource_mapper()->get<acl_softmax_resource_t>(this);
    acl_softmax_obj_t &acl_obj = acl_resource->get_acl_obj();

    acl_obj.src_tensor.allocator()->import_memory(const_cast<data_t *>(src));
    acl_obj.dst_tensor.allocator()->import_memory(dst);

    acl_obj.softmax.run();

    acl_obj.src_tensor.allocator()->free();
    acl_obj.dst_tensor.allocator()->free();

    return status::success;
}

template struct acl_softmax_fwd_t<data_type::f32>;

} // namespace aarch64
} // namespace cpu
} // namespace impl
} // namespace dnnl
