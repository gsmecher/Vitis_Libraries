# Vitis Vision Library
The Vitis Vision library is a set of 90+ kernels, optimized for Xilinx™ FPGAs, AI Engine™, and SoCs, based on the OpenCV computer vision library. The kernels in the Vitis Vision library are optimized and supported in the Xilinx Vitis™ Tool Suite.

## HARDWARE and SOFTWARE REQUIREMENTS
The Vitis Vision library is designed to work with Zynq™, Zynq Ultrascale+™, VCK190™, and Alveo™ FPGAs. The library has been verified on zcu102, zcu104, vck190, U50, and U200 boards.

### Prerequisites

* Valid installation of [Vitis™ 2022.2](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Installing-the-Vitis-Software-Platform) or later version and the corresponding licenses.
* Xilinx® Runtime ([XRT](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Installing-Xilinx-Runtime-and-Platforms)) must be installed. XRT provides software interface to Xilinx FPGAs.
* Install [OpenCV-4.4.0]((https://github.com/opencv/opencv/tree/4.4.0)) x86 libraries(with compatible libjpeg.so). x86 libs have to be used for

		a) L1 flow irrespective of target FPGA device being PCIe or embedded.
		b) L2/L3 flow when the target device is PCIe based
		c) L2/L3 flow when performing software emulation for an embedded platform.
		
	For L2/L3 flow targeting embedded platforms (for hardware emulationa and hardware build), aarch32/aarch64 version OpenCV shipped within their *sysroot* should be used.	
* libOpenCL.so must be [installed](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/OpenCL-Installable-Client-Driver-Loader) if not present.
* [Install the card](https://www.xilinx.com/support/documentation/boards_and_kits/accelerator-cards/1_9/ug1301-getting-started-guide-alveo-accelerator-cards.pdf) for which the platform is supported in Vitis 2022.2 or later versions.
* If targeting an embedded platform, [install]((https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Installing-Embedded-Platforms?tocId=hfE7LFeS8mU4dexvgPL31Q)) it and set up the [evaluation board](https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/444006775/Zynq+UltraScale+MPSoC).

##### OpenCV Installation Guidance:

It is recommended to do a fresh installation of OpenCV 4.4.0 and not use existing libs of your system, as they may or may not work with Vitis environment.

**Please make sure you update and upgrade the packages and OS libraries of your system and
have cmake version>3.5 installed before proceeding.**

The below steps can help install the basic libs required to compile and link the OpenCV calls in Vitis Vision host code.

1. create a directory "source" and clone [opencv-4.4.0](https://github.com/opencv/opencv/tree/4.4.0) into it.
2. create a directory "source_contrib" and clone [opencv-4.4.0-contrib](https://github.com/opencv/opencv_contrib/tree/4.4.0) into it.
3. create 2 more directories: *build* , *install*
4. open a bash terminal and *cd* to *build* directory
5. Run the command: *export LIBRARY_PATH=/usr/lib/x86_64-linux-gnu/*
6. Run the command: *cmake -D CMAKE_BUILD_TYPE=RELEASE
  -D CMAKE_INSTALL_PREFIX=< path-to-install-directory>
  -D CMAKE_CXX_COMPILER=< path-to-Vitis-installation-directory>/tps/lnx64/gcc-6.2.0/bin/g++
  -D OPENCV_EXTRA_MODULES_PATH=< path-to-source_contrib-directory>/modules/
  -D WITH_V4L=ON -DBUILD_TESTS=OFF -DBUILD_ZLIB=ON
  -DBUILD_JPEG=ON -DWITH_JPEG=ON -DWITH_PNG=ON
  -DBUILD_EXAMPLES=OFF -DINSTALL_C_EXAMPLES=OFF
  -DINSTALL_PYTHON_EXAMPLES=OFF -DWITH_OPENEXR=OFF
  -DBUILD_OPENEXR=OFF <path-to-source-directory>*
7. Run the command: *make all -j8*
8. Run the command: *make install*

The OpenCV includes and libs will be in the *install* directory

## Source files organization

Vitis development environment supports variety of build flows (based on the target engine (either AI Engine or Programmable Logic) and type of sources)

* **PL [HLS/RTL]** 		: Kernels targeting FPGA (PL), coded in C/C++/HDL for Vitis HLS.
* **AIE**			: Kernels targeting AI Engine programmed in C/C++ in accordance to AI Engine coding methodology.
* **PL+AIE**			: Kernels that target both FPGA (PL) and AI Engine

To facilitate this, all unit level kernels are located in *L1/include*. AIE kernels are located in *L1/include/aie*

Examples and tests in L1, L2 and L3 will use these unit level kernels in various ways for building a project.
	
The library is organized as follows :

~~~
├── L1/
│   ├── README.md
│   ├── examples/
│   ├── include/
│   ├── lib/
│   └── tests/
├── L2/
│   ├── README.md
│   ├── examples/
│   └── tests/
├── L3/
│   ├── README.md
│   ├── benchmarks/
│   ├── examples/
│   └── tests/
├── data/
├── LICENSE.txt
├── README.md
├── docs/
│   ├── images/
│   ├── include/
│   ├── Makefile
│   ├── Makefile.sphinx
│   ├── rst files
│   ├── conf.py
│   ├── conf_bhp.py
│   ├── env.csh
├── ext/
│   └── xcl2/
└── library.json
~~~

## Application development
Vitis library is organized into L1, L2 and L3 folders so as to facilitate various development stages.

**L1** :
      Makefiles and sources in L1/examples, L1/tests facilitate HLS based flow for a quick check without considering complexities of Platform, OpenCL/XRT frame work. Below list shows various tasks a user can work:

* Check the functionality of an individual kernel (C-simulation)
* Estimate resource usage, latency etc., (Synthesis)
* Run cycle accurate simulation (Co-simulation)
* Package as IP, get final resource utilization/timing details (Export RTL)
       
	**Note**:  Once RTL (or XO file after packaging IP) is generated Vivado flow can invoked for XCLBIN file generation if required.

**L2** :
       Makefiles and sources in *L2/examples* and *L2/tests* facilitate building XCLBIN file from various sources (HDL, HLS or XO files) of kernels with host code written in OpenCL/XRT frame work targeting a device. 
	   This flow supports:

* Software emulation to check the functionality
* Hardware emulation to check RTL level simulation
* Build and test on hardware

For AIE development *L2/tests/aie* , *L2/examples/aie* has Makefile(s) and sources executing below tasks:

* AIE simulation
* X86 simulation
* Hardware emulation
* Hardware build and run

**L3** :
       Makefiles and sources in *L3/examples* and *L3/tests* demonstrate applications developed involving multiple kernels in pipeline. The Makefiles provided can be used for executing tasks same as L2.

### Important

* Vitis and OpenCV-4.4.0 x86 version libs need to be installed before hand for L1 flow. Rest of the prerequisites are optional to use the L1 flow.

* Please refer to [L1 readme](https://github.com/Xilinx/Vitis_Libraries/blob/master/vision/L1/README.md) on how to setup the environment and run the L1 functions.

* All the prerequisites need to be installed before starting with L2/L3 Vitis flow.

* For embedded devices, platforms and common images have to downloaded separately from the Xilinx official [download center](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html).

* Please refer to [L2/L3 readme](https://github.com/Xilinx/Vitis_Libraries/blob/master/vision/L2/README.md) on how to setup the environment and run the L2/L3 functions.
	
* All the limitations and constraints of Vitis ([OS support, compatibility](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Installation-Requirements) etc.) are also applicable to Vitis Vison library.



## OTHER INFORMATION
Full User Guide for Vitis Vision and using OpenCV on Xilinx devices Check here:
[Xilinx Vitis Vision User Guide](https://docs.xilinx.com/r/en-US/Vitis_Libraries/vision/index.html)

## SUPPORT
For questions and to get help on this project or your own projects, visit the [Xilinx Forums](https://support.xilinx.com/s/topic/0TO2E000000YKYAWA4/vitis-acceleration-acceleration?language=en_US)

## LICENSE AND CONTRIBUTING TO THE REPOSITORY
The source for this project is licensed under the [Apache License](http://www.apache.org/licenses/LICENSE-2.0)

    Copyright 2016 - 2022 Xilinx, Inc.
    Copyright (C) 2022, Advanced Micro Devices, Inc.

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

To contribute to this project, follow the guidelines in the [Repository Contribution README][] (link yet to be updated)

## ACKNOWLEDGEMENTS
This library is written by developers at
- [Xilinx](http://www.xilinx.com)

## Changelog:

**PL additions/enhancements**:
	
    • New functions:
		    • HDR Decompanding : Compress(compand) data in a piece-wise linear (PWL) mapping to a lower bit depth
		    • Degamma : Designed to linearize the input from sensor or any pre-processing IP
		    • ISPStats : collects histogram based stats of bayer and color images
		    • ISP all-in-one pipeline : All the ISP related functions stitched in one pipeline with option to exclude unwanted functions during runtime and compile time.
		    • Multi-stream ISP : Multiple input stream ISP pipeline
    • Updates:
		    • Added new template parameter XFCVDEPTH for xf::cv::Mat class that can be used to assign custom depth to the Mat's internal hls::stream.
		    • All APIs in the library updated with newly added XFCVDEPTH parameter for xf::cv::Mat
		    • Remove deprecated __SDSVHLS__ macro from all files
		    • Replaced deprecated RESOURCE pragma with BIND_STORAGE/BIND_OP pragmas
		    • Rename NO, RO in all files to SPC (Single Pixel per Clock) and MPC (Multiple Pixels per Clock)
		    • Add missing reference functions in L1, L2, L3 testbench files
		    • Fixed Gaussian Difference incorrect implementation
		    • Fixed incorrect dst Mat assignment in xf::cv::Mat member function convertBitdepth
		    • Updated analyzeDiff in L1/include/common/xf_sw_utils.hpp to a static function
		    • Added missing "Test Passed/Failed/Finished" check in all L1/L2/L3 functions.
		    • Added 16 bit and 4 channel support, corrected B and R channel swap issue for channel extract function.
		    • Fixed a bug in BGR2HLS module of cvtcolor function
		    • Restructured L1 channel combine accel and testbench code
		    • Fixed SVM emulation and cosim hang issue
		    • Updated loop tripcounts of pyrDown, histogram, HDR extract, rgb2yuyv module in cvtColor to fix synthesis latency numbers
		    • Fix array reshape pragma in xf_sobel.hpp, xf_video_mem.hpp files
		    • Modified XFCVDEPTH values in all functions
		    • Stride support added in the preprocess kernel of L3 Defect Detection pipeline
		    • Order of kernels changed in all-in-one pipeline and renamed as all-in-one-adas
		    • Array partitions in accel file moved to kernel file in isp multistream pipeline
	
    • Lib Infra Changes:
		    • Added frequency setting in L2/L3 JSON files. 300 MHz for NPPC1 and 150MHz for NPPC8 for most cases.
		    • Updated JSON and Makefiles to use ps_on_x86 feature for software emulation targeting embedded platforms. Software emulation for embedded platforms
			no longer uses qemu but regular g++ compilation flow only.
		    • Added missing environment checks in all JSON and Makefiles


**AIE additions/enhancements:** :

    • New functions/features :
    	• Resize / Resize + Normalize
    	• Smart tiling for x86 64-bit platforms
	
    • Updates:
    	• RTL Data movers 
		- 8-bit PL / 8-bit AIE data movers
		- Multi-channel support
		- Optimized implementation
    	• Optimized smart tiling / stitching for higher performance
    	• Fix Random crashes in hardware emulation flow
    	• Miscellaneous bug fixes
	


**Known issues**

  * Vitis GUI projects on RHEL83 and CEntOS82 may fail because of a lib conflict in the
     LD_LIBRARY_PATH setting. User needs to remove ${env_var:LD_LIBRARY_PATH} from the project
      environment settings for the function to build successfully.
  * rgbir2bayer, isppipeline_rgbir PL functions are not supplied with input images
  * Software emulation for Warptransform L2 testcases doesn't work because of a known issue with platform.
  * Warptransform L1 URAM cases fail CSim because of a known HLS issue.
