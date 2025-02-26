/*
 * Copyright 2022 Xilinx, Inc.
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

#include <adf.h>
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>

#define KERNEL_RATIO 0.4 // 1.0

#define SIMD_DEPTH 4
#define LENGTH 32
#define LENGTH_FOC LENGTH
#define SPACE_DIMENSION 4
//#define WIN_SIZE 128
#define FIFO_DEPTH 8100
//#define N_SAMPLES 2048
#define INCREMENT_MATRIX (SIMD_DEPTH / SPACE_DIMENSION)
#define INCREMENT_VECTOR SIMD_DEPTH
#define SPEED_OF_SOUND 1540
#define INVERSE_SPEED_OF_SOUND 0.000649350649 //(1/SPEED_OF_SOUND)
#define F_NUMBER 2
#define PI 3.1415926536
#define PI_2 6.28318531
#define BYTE_ALIGNMENT 4
#define WIN_SIZE_VECTOR (LENGTH * BYTE_ALIGNMENT)
#define WIN_SIZE_MATRIX (LENGTH * BYTE_ALIGNMENT * SPACE_DIMENSION)
#define SAMPLING_FREQUENCY 100000000 // 0.000000083
#define POINTS_PER_ITERATION 32
#define WIN_SIZE_INTERPOLATOR (POINTS_PER_ITERATION * SIMD_DEPTH * BYTE_ALIGNMENT)

namespace us {
namespace L1 {

template <typename T, const unsigned LEN, const unsigned INCREMENT, const unsigned VECDIM>
void ones(output_window<T>* out);

template <typename T, const unsigned LEN, const unsigned INCREMENT, const unsigned VECDIM>
void outer(input_window<T>* in1, input_stream<T>* in2, output_window<T>* out);

template <typename T, const unsigned LEN, const unsigned INCREMENT, const unsigned VECDIM>
void outerStream(input_stream<T>* in1, input_stream<T>* in2, output_window<T>* out);

template <typename T, const unsigned LEN, const unsigned INCREMENT, const unsigned VECDIM>
void sumMM(input_window<T>* in1, input_window<T>* in2, output_stream<T>* out);

template <typename T, const unsigned int LEN, const unsigned int INCREMENT, const unsigned VECDIM>
void tileVApo(output_window<T>* out);

template <typename T, const unsigned int LEN, const unsigned int INCREMENT, const unsigned VECDIM>
void diffMV(input_stream<T>* in1, input_stream<T>* in2, output_window<T>* out);

template <typename T, const unsigned int LEN, const unsigned int INCREMENT, const unsigned VECDIM>
void mulMM(input_window<T>* in1, input_window<T>* in2, output_window<T>* out);

template <typename T, const unsigned int LEN, const unsigned int INCREMENT, const unsigned VECDIM>
void sum_axis_1(input_window<T>* in1, output_window<T>* out);

template <typename T, const unsigned int LEN, const unsigned int INCREMENT, const unsigned VECDIM>
void absV(input_window<T>* in1, output_stream<T>* out);

template <typename T, const unsigned int LEN, const unsigned int INCREMENT, const unsigned VECDIM>
void diffVS(input_stream<T>* in1, input_stream<T>* in2, output_window<T>* out);

template <typename T, const unsigned int LEN, const unsigned int INCREMENT, const unsigned VECDIM>
void sign(input_window<T>* in1, output_window<T>* out);

template <typename T, const unsigned int LEN, const unsigned int INCREMENT, const unsigned VECDIM>
void mulVV(input_window<T>* in1, input_window<T>* in2, output_window<T>* out);

template <typename T, const unsigned int LEN, const unsigned int INCREMENT, const unsigned VECDIM>
void norm_axis_1(input_window<T>* in1, output_window<T>* out);

template <typename T, const unsigned int LEN, const unsigned int INCREMENT, const unsigned VECDIM>
void divVSSpeedOfSound(input_window<T>* in1, output_window<T>* out);

template <typename T, const unsigned int LEN, const unsigned int INCREMENT, const unsigned VECDIM>
void sumVSStream(input_window<T>* in1, input_stream<T>* in2, output_window<T>* out);

template <typename T, const unsigned int LEN, const unsigned int INCREMENT, const unsigned VECDIM>
void diffVSStreamOut(input_window<T>* in1, input_stream<T>* in2, output_stream<T>* out);

template <typename T, const unsigned int LEN, const unsigned int INCREMENT, const unsigned VECDIM>
void mulVSStreamIn(input_window<T>* in1, input_stream<T>* in2, output_window<T>* out);

template <typename T, const unsigned int LEN, const unsigned int INCREMENT, const unsigned VECDIM>
void sumVOne(input_window<T>* in1, output_stream<T>* out);

template <typename T, const unsigned int LEN, const unsigned int INCREMENT, const unsigned VECDIM>
void sumVVStreamIn1(input_stream<T>* in1, input_window<T>* in2, output_window<T>* out);

template <typename T, const unsigned int LEN, const unsigned int INCREMENT, const unsigned VECDIM>
void diffSV(input_stream<T>* in1, input_stream<T>* in2, output_window<T>* out);

template <typename T, const unsigned int LEN, const unsigned int INCREMENT, const unsigned VECDIM>
void squareV(input_window<T>* in1, output_window<T>* out);

template <typename T, const unsigned int LEN, const unsigned int INCREMENT, const unsigned VECDIM>
void sumVV(input_window<T>* in1, input_window<T>* in2, output_window<T>* out);

template <typename T, const unsigned int LEN, const unsigned int INCREMENT, const unsigned VECDIM>
void sqrtV(input_window<T>* in1, output_stream<T>* out);

template <typename T, const unsigned int LEN, const unsigned VECDIM>
void mulVSCRSWindow(input_window<T>* in1, input_window<T>* in2, output_window<T>* out);

template <typename T, const unsigned int LEN, const unsigned VECDIM>
void mulLinSCRStreamIn(input_window<T>* in2, output_window<T>* out);

template <typename T, const unsigned int LEN, const unsigned VECDIM>
void diffOneLin(output_window<T>* out);

template <typename T, const unsigned int LEN, const unsigned VECDIM>
void diffTwoLin(output_window<T>* out);

template <typename T, const unsigned int LEN, const unsigned VECDIM>
void diffLinOne(output_window<T>* out);

template <typename T, const unsigned int LEN, const unsigned VECDIM>
void sumVVStreamOut(input_window<T>* in1, input_window<T>* in2, output_stream<T>* out);

template <typename T, const unsigned int LEN, const unsigned VECDIM>
void diffLinTwo(output_window<T>* out);

template <typename T, const unsigned int LEN, const unsigned VECDIM>
void mulVHalfInt(input_window<T>* in1, output_window<T>* out);

template <typename T, const unsigned int LEN, const unsigned VECDIM>
void mulLinHalf(output_window<T>* out);

template <typename T, const unsigned int LEN, const unsigned VECDIM>
void mulVVStream(input_window<T>* in1, input_stream<T>* in2, output_window<T>* out);

template <typename T, const unsigned int LEN, const unsigned VECDIM>
void dataMover(input_stream<T>* in1, output_window<T>* out1);

template <typename T, const unsigned int LEN, const unsigned VECDIM>
void diffThreeLin(output_window<T>* out);

template <typename T, const unsigned LEN, const unsigned INCREMENT, const unsigned VECDIM, const unsigned SCALAR>
void equalS(input_stream<T>* in1, output_window<T>* out);

template <typename T, const unsigned LEN, const unsigned INCREMENT, const unsigned VECDIM, const unsigned SCALAR>
void lessOrEqualThanS(input_window<T>* in1, output_window<T>* out);

template <typename T, const unsigned LEN, const unsigned INCREMENT, const unsigned VECDIM>
void mulVPi(input_window<T>* in1, output_window<T>* out);

template <typename T, const unsigned LEN, const unsigned INCREMENT, const unsigned VECDIM>
void mulV1e_16(input_window<T>* in1, output_window<T>* out);

template <typename T, const unsigned LEN, const unsigned INCREMENT, const unsigned VECDIM>
void mulVS(input_window<T>* in1, input_stream<T>* in2, output_window<T>* out);

template <typename T, const unsigned LEN, const unsigned INCREMENT, const unsigned VECDIM>
void mulVSStream(input_stream<T>* in1, input_stream<T>* in2, output_stream<T>* out);

template <typename T, const unsigned LEN, const unsigned INCREMENT, const unsigned VECDIM>
void mulVSWS(input_window<T>* in1, input_stream<T>* in2, output_stream<T>* out);

template <typename T, const unsigned LEN, const unsigned INCREMENT, const unsigned VECDIM>
void absVSW(input_stream<T>* in1, output_window<T>* out);

template <typename T, const unsigned LEN, const unsigned INCREMENT, const unsigned VECDIM>
void reciprocalV(input_window<T>* in1, output_window<T>* out);

template <typename T, const unsigned LEN, const unsigned INCREMENT, const unsigned VECDIM>
void cosV(input_window<T>* in1, output_window<T>* out);

template <typename T, const unsigned int LEN, const unsigned int INCREMENT, const unsigned VECDIM>
void diffOneV(input_stream<T>* in1, output_window<T>* out);

template <typename T, const unsigned LEN, const unsigned INCREMENT, const unsigned VECDIM>
void absVWS(input_window<T>* in1, output_stream<T>* out);

template <typename T, const unsigned int LEN, const unsigned int INCREMENT, const unsigned VECDIM>
void mulVHalf(input_window<T>* in1, output_window<T>* out);

template <typename T, const unsigned int LEN, const unsigned int INCREMENT, const unsigned VECDIM>
void sumVOneSW(input_stream<T>* in1, output_window<T>* out);

template <typename T, const unsigned int LEN, const unsigned int INCREMENT, const unsigned VECDIM>
void mulVVStreamOut(input_window<T>* in1, input_window<T>* in2, output_stream<T>* out);

template <typename T, const unsigned int LEN, const unsigned int INCREMENT, const unsigned VECDIM>
void diffOneVWW(input_window<T>* in1, output_window<T>* out);

template <typename T, const unsigned int LEN, const unsigned int INCREMENT, const unsigned VECDIM>
void sumVVStreamIn1(input_stream<T>* in1, input_window<T>* in2, output_window<T>* out);

} // namespace L1
} // namespace us
