/*
 * Copyright 2021 Xilinx, Inc.
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
 */

#pragma once

#include "kernels.hpp"

namespace us {
namespace L2 {

class ones : public adf::graph {
   public:
    ones() {
        // Kernel definition
        onesKernel = adf::kernel::create(L1::ones<float, LENGTH, INCREMENT_VECTOR, SIMD_DEPTH>);

        // input and output port
        output_ones = adf::output_plio::create(adf::plio_32_bits, "data/output.txt");

        // result
        adf::connect<adf::window<WIN_SIZE_MATRIX> > output_ones_q(onesKernel.out[0], output_ones.in[0]);

        // source kernel
        adf::source(onesKernel) = "ones/ones.cpp";

        // Setting kernel ratio
        adf::runtime<adf::ratio>(onesKernel) = KERNEL_RATIO;
    }

    adf::output_plio output_ones;

   private:
    // Kernel declaration
    adf::kernel onesKernel;
};
} // namespace L2
} // namespace us
