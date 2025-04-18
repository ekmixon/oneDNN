/*******************************************************************************
* Copyright 2019-2021 Intel Corporation
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

#include "gpu/gpu_impl_list.hpp"

#include "gpu/ocl/cross_engine_reorder.hpp"
#include "gpu/ocl/custom_reorder.hpp"
#include "gpu/ocl/generic_reorder.hpp"
#include "gpu/ocl/ref_reorder.hpp"
#include "gpu/ocl/rnn/rnn_reorders.hpp"

namespace dnnl {
namespace impl {
namespace gpu {

namespace {

using namespace dnnl::impl::data_type;

#define INSTANCE(...) \
    impl_list_item_t( \
            impl_list_item_t::reorder_type_deduction_helper_t<__VA_ARGS__>())

// clang-format off
const impl_list_item_t reorder_impl_list[] = {
        INSTANCE(ocl::rnn_weights_reorder_t::pd_t),
        INSTANCE(ocl::cross_engine_reorder_t::pd_t),
        INSTANCE(ocl::custom_reorder_t::pd_t), // for specific tensor shapes
        
        // Disable generic reorder - it introduced regressions in reorder and
        // concat tests that slipped by precommit and were found in nightly.
        //INSTANCE(ocl::generic_reorder_t::pd_t),// fast and quite generic 

        INSTANCE(ocl::ref_reorder_t::pd_t),    // slow but fits every use case
        nullptr,
};
// clang-format on

} // namespace

const impl_list_item_t *gpu_impl_list_t::get_reorder_implementation_list(
        const memory_desc_t *, const memory_desc_t *) {
    return reorder_impl_list;
}

} // namespace gpu
} // namespace impl
} // namespace dnnl
