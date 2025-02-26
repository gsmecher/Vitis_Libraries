# Copyright 2019-2021 Xilinx, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# sc makefile-generator v1.0.0

############################## Help Section ##############################
.PHONY: help

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all TARGET=<hw/hw_emu/sw_emu/> PLATFORM=<FPGA platform>"
	$(ECHO) "      Command to generate the design for specified Target and Shell."
	$(ECHO) ""
	$(ECHO) "  make run TARGET=<hw/hw_emu/sw_emu/> PLATFORM=<FPGA platform>"
	$(ECHO) "      Command to run application in emulation."
	$(ECHO) ""
	$(ECHO) "  make host HOST_ARCH=<hw/hw_emu/sw_emu/>"
	$(ECHO) "      Command to build host application."
	$(ECHO) ""
	$(ECHO) "  make clean "
	$(ECHO) "      Command to remove the generated non-hardware files."
	$(ECHO) ""
	$(ECHO) "  make cleanall"
	$(ECHO) "      Command to remove all the generated files."
	$(ECHO) ""

############################## Setting up Project Variables ##############################

MK_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
XF_PROJ_ROOT ?= $(shell bash -c 'export MK_PATH=$(MK_PATH); echo $${MK_PATH%/L1/*}')
CUR_DIR := $(patsubst %/,%,$(dir $(MK_PATH)))
XFLIB_DIR = $(XF_PROJ_ROOT)

# setting devault value
TARGET ?= sw_emu
HOST_ARCH ?= x86

#setting PLATFORM
ifeq ($(PLATFORM),)
PLATFORM := $(DEVICE)
endif
ifeq ($(PLATFORM),)
PLATFORM := xilinx_u250_gen3x16_xdma_4_1_202210_1
endif

# #################### Checking if PLATFORM in whitelist ############################
PLATFORM_ALLOWLIST +=  u250 u200
PLATFORM_BLOCKLIST +=  xilinx_u250_xdma_201830_2 xilinx_u200_xdma_201830_2 zc aws-vu9p-f1

GCC_INTOOL := 8.3.0
BINUTILS_INTOOL := 2.37
include ./utils.mk
TEMP_DIR := _x_temp.$(TARGET).$(PLATFORM_NAME)
TEMP_REPORT_DIR := $(CUR_DIR)/reports/_x.$(TARGET).$(PLATFORM_NAME)
BUILD_DIR := build_dir.$(TARGET).$(PLATFORM_NAME)
BUILD_REPORT_DIR := $(CUR_DIR)/reports/_build.$(TARGET).$(PLATFORM_NAME)
EMCONFIG := $(BUILD_DIR)/emconfig.json
XCLBIN_DIR := $(CUR_DIR)/$(BUILD_DIR)
export XCL_BINDIR = $(XCLBIN_DIR)

EXE_FILE_DEPS :=
BINARY_CONTAINERS_DEPS :=
RUN_DEPS :=

# set debug switch
ifneq ($(debug),yes)
CXXFLAGS += -O3
endif

# get global setting
ifdef XILINX_SC_PFM_CONFIG
CXXFLAGS += -DXILINX_SC_PFM_CONFIG=$(XILINX_SC_PFM_CONFIG)
endif
ifdef XILINX_SC_PFM_EXT
CXXFLAGS += -DXILINX_SC_PFM_EXT=$(XILINX_SC_PFM_EXT)
endif
ifeq ($(HOST_ARCH), x86)
CXXFLAGS += -I $(XILINX_VITIS)/system_compiler/include -I $(XILINX_HLS)/include 
LDFLAGS += -L$(XILINX_XRT)/lib -L$(XILINX_VITIS)/system_compiler/lib/x86 -lvpp_acc -l$(LIB_XRT) -lxrt_coreutil  -Wl,-rpath=$(XILINX_VITIS)/system_compiler/lib/x86:$(XILINX_XRT)/lib:$(GCC_HOME)/lib64  -Wl,--enable-new-dtags -lpthread 
VPP_FLAGS += -t $(TARGET) --platform $(XPLATFORM) --temp_dir $(TEMP_DIR) --save-temps -g -I $(XILINX_VITIS)/system_compiler/include 
VPP_LDFLAGS += --optimize 2 -R 2 
else ifeq ($(HOST_ARCH), aarch64)
CXXFLAGS += 
LDFLAGS += 
VPP_FLAGS += 
VPP_LDFLAGS += 
endif
CXXFLAGS += $(EXTRA_CXXFLAGS)
VPP_FLAGS += $(EXTRA_VPP_FLAGS)

ifeq ($(TARGET),sw)
  $(error Error: The sw target is not supported anymore. Please use sw_emu instead)
else ifeq ($(TARGET),sw_emu)
  LIB_XRT  := xrt_swemu
  HOST_PREAMBLE := XCL_EMULATION_MODE=sw_emu
else ifeq ($(TARGET),hw_emu)
  LIB_XRT  := xrt_hwemu
  HOST_PREAMBLE := XCL_EMULATION_MODE=hw_emu
  ifneq (,$(findstring -g,$(EXTRA_VPPFLAGS) $(CXXFLAGS)))
    # for sourcing pre/post xsim scripts
    ifneq ($(XILINX_SC_HW_EMU),0)
      HOST_PREAMBLE += XILINX_SC_HW_EMU=1 XILINX_SC_BUILD_DIR=$(PWD)/$(BUILD_DIR)
    endif
  endif
else ifeq ($(TARGET),hw)
  LIB_XRT  := xrt_core
endif

########################## Setting up Host Variables ##########################

#Inclue Required Host Source Files
HOST_SRCS += $(CUR_DIR)/host/main.cpp 
CXXFLAGS +=  -I $(XFLIB_DIR)/L1/include -I $(CUR_DIR)/kernel -I $(XFLIB_DIR)/ext/xcl2
CXXFLAGS += -O3

EXE_NAME := host.exe
REL_HOST_SRCS := $(shell realpath --relative-to $(CUR_DIR) $(HOST_SRCS))
EXE_OBJS := $(addprefix $(TEMP_DIR)/, $(addsuffix .o,$(basename $(shell realpath --relative-to $(CUR_DIR) $(HOST_SRCS) | sed "s|\.\.\/||g"))))
EXE_FILE := $(BUILD_DIR)/$(EXE_NAME)
EXE_FILE_DEPS := $(EXE_OBJS) 
MAKEDEPEND = $(CXX) $< -MM -MP -MF $(basename $@).d -MT $@  $(CXXFLAGS)

HOST_ARGS := -len 1024 -num 16 -gld $(XFLIB_DIR)/L1/benchmarks/aes256CbcDecrypt_sc/data/gld.dat
ifneq ($(HOST_ARCH), x86)
PKG_HOST_ARGS = $(foreach args,$(HOST_ARGS),$(subst $(dir $(patsubst %/,%,$(args))),,$(args)))
endif

########################## Kernel compiler global settings ##########################
VPP_FLAGS +=  -I $(XFLIB_DIR)/L1/include -I $(CUR_DIR)/kernel -I $(CUR_DIR)/host -I $(XFLIB_DIR)/L1/include

######################### binary container global settings ##########################
VPP_FLAGS_hls_kernel += --hls.clock 300000000:hls_kernel
ifneq ($(HOST_ARCH), x86)
VPP_LDFLAGS_hls_kernel += --clock.defaultFreqHz 300000000
else
VPP_LDFLAGS_hls_kernel += --kernel_frequency 300
endif

ifeq ($(HOST_ARCH), x86)
BINARY_CONTAINERS_TMP := $(BUILD_DIR)/$(TARGET).o
BINARY_CONTAINERS := $(BUILD_DIR)/$(TARGET).xclbin
ifeq ($(TARGET),sw_emu)
  BINARY_CONTAINERS_TMP :=
endif
else
# placeholder for non_x86
endif

.SECONDEXPANSION:
# ################ Setting Rules for Binary Containers (Building Kernels) ################
ACC_SRCS_hls_kernel += $(CUR_DIR)/kernel/aes256CbcDecrypt_acc.cpp
ACC_OBJS_hls_kernel  := $(addprefix $(TEMP_DIR)/, $(addsuffix .o,$(basename $(shell realpath --relative-to $(CUR_DIR)  $(ACC_SRCS_hls_kernel) | sed "s|\.\.\/||g"))))
$(ACC_OBJS_hls_kernel): $(TEMP_DIR)/%.o : %.cpp $$(@D)/.f
	@echo "--> Making $@ from: $?"
	$(MAKEDEPEND)
	$(VPP) $(VPP_FLAGS) $(VPP_FLAGS_hls_kernel) -o $@  -c $(shell find $(ACC_SRCS_hls_kernel) -name  $(subst .o,.cpp,$(notdir $@)) )
BINARY_CONTAINERS_DEPS  += $(ACC_OBJS_hls_kernel) 
$(BINARY_CONTAINERS_TMP) : $(BINARY_CONTAINERS_DEPS)
	@echo "--> Making $@ from: $?"
	$(VPP) $(VPP_FLAGS) $(VPP_LDFLAGS) $(VPP_LDFLAGS_hls_kernel) -o $(BINARY_CONTAINERS) -l $^
EXE_FILE_DEPS += $(BINARY_CONTAINERS_TMP)
EXE_FILE_DEPS += $(BINARY_CONTAINERS_DEPS)

############################## Setting Rules for Host (Building Host Executable) ##############################
ifeq ($(HOST_ARCH), x86)
$(TEMP_DIR)/%.o : $(REL_HOST_SRCS) $$(@D)/.f
	@echo "--> Making $@ from: $?"
	mkdir -p $(BUILD_DIR)
	$(MAKEDEPEND)
	$(CXX) -o $@ $(CXXFLAGS)  -I . -c $(shell find $(HOST_SRCS) -name $(subst .o,.cpp,$(notdir $@)) )
 
$(EXE_FILE): $(EXE_FILE_DEPS)  
	mkdir -p $(BUILD_DIR)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)
else
# place holder for arch64
endif

$(EMCONFIG):
	emconfigutil --platform $(XPLATFORM) --od $(BUILD_DIR)

%/.f:
	mkdir -p $(dir $@)
	touch $@

.PRECIOUS: %/.f

RUN_DEPS += $(EXE_FILE) $(EMCONFIG)

run: check_device  $(RUN_DEPS)
#sw_emu
ifneq (,$(filter sw_emu, $(TARGET)))
ifeq ($(HOST_ARCH), x86)
	LD_LIBRARY_PATH=$(LIBRARY_PATH):$$LD_LIBRARY_PATH \
	$(HOST_PREAMBLE) $(EXE_FILE) $(HOST_ARGS)
	
else
# place holder for arch64
endif
endif

#hw_emu
ifneq (,$(filter hw_emu, $(TARGET)))
ifeq ($(HOST_ARCH), x86)
	LD_LIBRARY_PATH=$(LIBRARY_PATH):$$LD_LIBRARY_PATH \
	$(HOST_PREAMBLE) $(EXE_FILE) $(HOST_ARGS)
	
else
# place holder for arch64
endif
endif

#hw
ifeq ($(TARGET), hw)
ifeq ($(HOST_ARCH), x86)
	LD_LIBRARY_PATH=$(LIBRARY_PATH):$$LD_LIBRARY_PATH \
	$(HOST_PREAMBLE) $(EXE_FILE) $(HOST_ARGS)
	
else
# place holder for arch64
endif
endif

############################## Setting Targets ##############################

.PHONY: all clean cleanall emconfig
emconfig: $(EMCONFIG)
ifeq ($(HOST_ARCH), x86)
all:  check_vpp check_platform check_xrt $(EXE_FILE) $(BINARY_CONTAINERS) emconfig
else
all:  check_vpp check_platform check_sysroot $(EXE_FILE) $(BINARY_CONTAINERS) emconfig sd_card
endif

.PHONY: host xclbin
ifeq ($(HOST_ARCH), x86)
host:  check_xrt $(EXE_FILE)
else
host:  check_sysroot $(EXE_FILE)
endif
xclbin: $(BINARY_CONTAINERS_TMP)

############################## Cleaning Rules ##############################
cleanh:
	-$(RMDIR) $(EXE_FILE) vitis_* TempConfig system_estimate.xtxt *.rpt .run/
	-$(RMDIR) src/*.ll _xocc_* .Xil dltmp* xmltmp* *.log *.jou *.wcfg *.wdb sample_link.ini sample_compile.ini obj*  bin* *.csv *.jpg *.jpeg *.png

cleank:
	-$(RMDIR) $(BUILD_DIR)/*.xclbin _vimage *xclbin.run_summary qemu-memory-_* emulation/ _vimage/ pl*start_simulation. sh *.xclbin
	-$(RMDIR) _x_temp.*

cleanall: cleanh cleank
	-$(RMDIR) $(BUILD_DIR)  build_dir.* emconfig.json *.html $(TEMP_DIR) $(CUR_DIR)/reports *.csv *.run_summary  $(CUR_DIR)/*.raw package_*   $(BUILD_DIR)/run_script.sh .ipcache *.str
	-$(RMDIR) $(XFLIB_DIR)/common/data/*.xe2xd* $(XFLIB_DIR)/common/data/*.orig*
	-$(RMDIR) 

clean: cleanh