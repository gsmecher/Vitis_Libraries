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

#include "opencv2/opencv.hpp"
#include <ap_int.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <cstring>
#include <sstream>
#include "common/xf_headers.hpp"
#include "xf_3dlut_config.h"
#include "xf_opencl_wrap.hpp"

float interp1(float val1, float val2, float val, int dim) {
    float ret = val1 + val * (val2 - val1); /// dim;
    return ret;
}

float interp3(float p000,
              float p100,
              float p001,
              float p101,
              float p010,
              float p110,
              float p011,
              float p111,
              float dist_r,
              float dist_g,
              float dist_b,
              int len) {
    float a = interp1(p000, p100, dist_r, len);
    float b = interp1(p001, p101, dist_r, len);
    float c = interp1(p010, p110, dist_r, len);
    float d = interp1(p011, p111, dist_r, len);

    float e = interp1(a, b, dist_g, len);
    float f = interp1(c, d, dist_g, len);

    float g = interp1(e, f, dist_b, len);

    return g;
}

void lut3d_ref(int height, int width, int lutdim, cv::Mat& in_img, float* lut, cv::Mat& out_img) {
    struct cInfo {
        float r;
        float g;
        float b;
    };

    cInfo lutGrid[lutdim][lutdim][lutdim];

    for (int k = 0; k < lutdim; k++) {
        for (int l = 0; l < lutdim; l++) {
            for (int m = 0; m < lutdim; m++) {
                lutGrid[k][l][m].r = lut[k * lutdim * lutdim * 3 + l * lutdim * 3 + m * 3 + 0];
                lutGrid[k][l][m].g = lut[k * lutdim * lutdim * 3 + l * lutdim * 3 + m * 3 + 1];
                lutGrid[k][l][m].b = lut[k * lutdim * lutdim * 3 + l * lutdim * 3 + m * 3 + 2];
            }
        }
    }

    cv::Vec3b outPix = 0;
    float outR = 0;
    float outG = 0;
    float outB = 0;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            ap_uint<8> inPixR = in_img.at<cv::Vec3b>(i, j)[0];
            ap_uint<8> inPixG = in_img.at<cv::Vec3b>(i, j)[1];
            ap_uint<8> inPixB = in_img.at<cv::Vec3b>(i, j)[2];

            float scale_r = (int)inPixR / (float)255;
            float scale_g = (int)inPixG / (float)255;
            float scale_b = (int)inPixB / (float)255;

            float index_r = scale_r * (lutdim - 1);
            float index_g = scale_g * (lutdim - 1);
            float index_b = scale_b * (lutdim - 1);

            int index_r_int = index_r;
            int index_g_int = index_g;
            int index_b_int = index_b;

            float dist_r = index_r - index_r_int;
            float dist_g = index_g - index_g_int;
            float dist_b = index_b - index_b_int;

            if ((index_r_int == lutdim - 1) || (index_g_int == lutdim - 1) || (index_b_int == lutdim - 1)) {
                outR = lutGrid[index_b_int][index_g_int][index_r_int].r;
                outG = lutGrid[index_b_int][index_g_int][index_r_int].g;
                outB = lutGrid[index_b_int][index_g_int][index_r_int].b;
            } else {
                float interR000 = lutGrid[index_b_int][index_g_int][index_r_int].r;
                float interG000 = lutGrid[index_b_int][index_g_int][index_r_int].g;
                float interB000 = lutGrid[index_b_int][index_g_int][index_r_int].b;

                float interR100 = lutGrid[index_b_int + 1][index_g_int][index_r_int].r;
                float interG100 = lutGrid[index_b_int + 1][index_g_int][index_r_int].g;
                float interB100 = lutGrid[index_b_int + 1][index_g_int][index_r_int].b;

                float interR001 = lutGrid[index_b_int][index_g_int][index_r_int + 1].r;
                float interG001 = lutGrid[index_b_int][index_g_int][index_r_int + 1].g;
                float interB001 = lutGrid[index_b_int][index_g_int][index_r_int + 1].b;

                float interR101 = lutGrid[index_b_int + 1][index_g_int][index_r_int + 1].r;
                float interG101 = lutGrid[index_b_int + 1][index_g_int][index_r_int + 1].g;
                float interB101 = lutGrid[index_b_int + 1][index_g_int][index_r_int + 1].b;

                float interR010 = lutGrid[index_b_int][index_g_int + 1][index_r_int].r;
                float interG010 = lutGrid[index_b_int][index_g_int + 1][index_r_int].g;
                float interB010 = lutGrid[index_b_int][index_g_int + 1][index_r_int].b;

                float interR110 = lutGrid[index_b_int + 1][index_g_int + 1][index_r_int].r;
                float interG110 = lutGrid[index_b_int + 1][index_g_int + 1][index_r_int].g;
                float interB110 = lutGrid[index_b_int + 1][index_g_int + 1][index_r_int].b;

                float interR011 = lutGrid[index_b_int][index_g_int + 1][index_r_int + 1].r;
                float interG011 = lutGrid[index_b_int][index_g_int + 1][index_r_int + 1].g;
                float interB011 = lutGrid[index_b_int][index_g_int + 1][index_r_int + 1].b;

                float interR111 = lutGrid[index_b_int + 1][index_g_int + 1][index_r_int + 1].r;
                float interG111 = lutGrid[index_b_int + 1][index_g_int + 1][index_r_int + 1].g;
                float interB111 = lutGrid[index_b_int + 1][index_g_int + 1][index_r_int + 1].b;

                outR = interp3(interR000, interR100, interR001, interR101, interR010, interR110, interR011, interR111,
                               dist_r, dist_g, dist_b, lutdim - 1);
                outG = interp3(interG000, interG100, interG001, interG101, interG010, interG110, interG011, interG111,
                               dist_r, dist_g, dist_b, lutdim - 1);
                outB = interp3(interB000, interB100, interB001, interB101, interB010, interB110, interB011, interB111,
                               dist_r, dist_g, dist_b, lutdim - 1);
            }

            outPix[0] = outR * 255;
            outPix[1] = outG * 255;
            outPix[2] = outB * 255;

            out_img.at<cv::Vec3b>(i, j)[0] = (unsigned char)outPix[0];
            out_img.at<cv::Vec3b>(i, j)[1] = (unsigned char)outPix[1];
            out_img.at<cv::Vec3b>(i, j)[2] = (unsigned char)outPix[2];
        }
    }
}

int main(int argc, char** argv) {
    cv::Mat in_img, out_img_hls, out_img_ref, diff;

    if (argc != 3) {
        fprintf(stderr, "Usage: <executable> <input image> <input lut file>\n");
        return -1;
    }

    // Read input image
    in_img = cv::imread(argv[1], 1);
    if (in_img.data == NULL) {
        fprintf(stderr, "Can't open image !!\n");
        return -1;
    }

    int rows = in_img.rows;
    int cols = in_img.cols;

    int lut_dim = 33;
    int lut_size = lut_dim * lut_dim * lut_dim * 3;

    std::cout << "Input image height : " << rows << std::endl;
    std::cout << "Input image width  : " << cols << std::endl;

    out_img_hls.create(rows, cols, in_img.type());
    out_img_ref.create(rows, cols, in_img.type());
    diff.create(rows, cols, in_img.type());

    float* lut = (float*)malloc(sizeof(float) * lut_size);
    unsigned int* casted_lut = (unsigned int*)malloc(sizeof(unsigned int) * lut_size);

    std::string fileText;

    std::ifstream infile(argv[2]);

    if (infile.fail()) {
        fprintf(stderr, "ERROR: Cannot open input lut file %s\n ", argv[2]);
        return EXIT_FAILURE;
    }

    int idx = 0;
    float tmp_lut_val_flt = 0.0f;

    while (getline(infile, fileText)) {
        std::string spstr;
        std::stringstream ss(fileText);
        while (ss >> spstr) {
            if (idx > lut_size) {
                fprintf(stderr, "ERROR: Lut file size content larger than specified lut dimension\n ");
                return -1;
            }
            tmp_lut_val_flt = stof(spstr);
            lut[idx] = tmp_lut_val_flt;
            casted_lut[idx++] = *((unsigned int*)(&tmp_lut_val_flt));
        }
    }

    infile.close();

    // reference function call
    lut3d_ref(rows, cols, lut_dim, in_img, lut, out_img_ref);

    // OpenCL section:
    size_t image_out_size_bytes = in_img.rows * in_img.cols * 3 * sizeof(unsigned char);
    size_t image_in_size_bytes = in_img.rows * in_img.cols * 3 * sizeof(unsigned char);
    size_t lut_in_size_bytes = lut_dim * lut_dim * lut_dim * sizeof(float) * 3;

    cl_int err;
    std::cout << "INFO: Running OpenCL section." << std::endl;

    // Get the device:
    std::vector<cl::Device> devices = xcl::get_xil_devices();
    cl::Device device = devices[0];

    // Context, command queue and device name:
    OCL_CHECK(err, cl::Context context(device, NULL, NULL, NULL, &err));
    OCL_CHECK(err, cl::CommandQueue queue(context, device, CL_QUEUE_PROFILING_ENABLE, &err));
    OCL_CHECK(err, std::string device_name = device.getInfo<CL_DEVICE_NAME>(&err));

    std::cout << "INFO: Device found - " << device_name << std::endl;
    std::cout << "Input Image Bit Depth:" << XF_DTPIXELDEPTH(IN_TYPE, NPC1) << std::endl;
    std::cout << "Input Image Channels:" << XF_CHANNELS(IN_TYPE, NPC1) << std::endl;
    std::cout << "NPPC:" << NPC1 << std::endl;

    // Load binary:
    std::string binaryFile = xcl::find_binary_file(device_name, "krnl_3dlut");
    cl::Program::Binaries bins = xcl::import_binary_file(binaryFile);
    devices.resize(1);
    OCL_CHECK(err, cl::Program program(context, devices, bins, NULL, &err));

    // Create a kernel:
    OCL_CHECK(err, cl::Kernel kernel(program, "lut3d_accel", &err));

    // Allocate the buffers:
    OCL_CHECK(err, cl::Buffer buffer_inImage(context, CL_MEM_READ_ONLY, image_in_size_bytes, NULL, &err));
    OCL_CHECK(err, cl::Buffer buffer_inLut(context, CL_MEM_READ_ONLY, lut_in_size_bytes, NULL, &err));
    OCL_CHECK(err, cl::Buffer buffer_outImage(context, CL_MEM_WRITE_ONLY, image_out_size_bytes, NULL, &err));

    // Set kernel arguments:
    OCL_CHECK(err, err = kernel.setArg(0, buffer_inImage));
    OCL_CHECK(err, err = kernel.setArg(1, buffer_outImage));
    OCL_CHECK(err, err = kernel.setArg(2, buffer_inLut));
    OCL_CHECK(err, err = kernel.setArg(3, rows));
    OCL_CHECK(err, err = kernel.setArg(4, cols));
    OCL_CHECK(err, err = kernel.setArg(5, lut_dim));

    // Initialize the buffers:
    cl::Event event;

    OCL_CHECK(err, queue.enqueueWriteBuffer(buffer_inImage,      // buffer on the FPGA
                                            CL_TRUE,             // blocking call
                                            0,                   // buffer offset in bytes
                                            image_in_size_bytes, // Size in bytes
                                            in_img.data,         // Pointer to the data to copy
                                            nullptr, &event));

    OCL_CHECK(err, queue.enqueueWriteBuffer(buffer_inLut,      // buffer on the FPGA
                                            CL_TRUE,           // blocking call
                                            0,                 // buffer offset in bytes
                                            lut_in_size_bytes, // Size in bytes
                                            casted_lut,        // Pointer to the data to copy
                                            nullptr, &event));
    // Execute the kernel:
    OCL_CHECK(err, err = queue.enqueueTask(kernel));

    // Copy Result from Device Global Memory to Host Local Memory
    queue.enqueueReadBuffer(buffer_outImage, // This buffers data will be read
                            CL_TRUE,         // blocking call
                            0,               // offset
                            image_out_size_bytes,
                            out_img_hls.data, // Data will be stored here
                            nullptr, &event);

    // Clean up:
    queue.finish();

    // Write output image
    cv::imwrite("hls_out.png", out_img_hls);
    cv::imwrite("ref_out.png", out_img_ref);

    // Results verification:
    cv::absdiff(out_img_ref, out_img_hls, diff); // Compute absolute difference image
    cv::imwrite("diff_img.png", diff);           // Save the difference image for debugging purpose

    float err_per;
    xf::cv::analyzeDiff(diff, ERROR_THRESHOLD, err_per);

    if (err_per > 0.0f) {
        std::cerr << "Testcase failed" << std::endl;
        return 1;
    } else
        std::cout << "Testcase passed" << std::endl;
    return 0;
}
