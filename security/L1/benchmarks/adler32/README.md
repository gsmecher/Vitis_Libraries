Adler32
========

To profile performance of adler32, we prepare a datapack of 268,435,456 byte messages as kernel input.
Base on U50, We have 1 kernel, each kernel has 1 PU.
Kernel utilization and throughput is shown in table below.

Executable Usage
================

* **Work Directory(Step 1)**

The steps for library download and environment setup can be found in :ref:`l1_vitis_security`. For getting the design,

```
   cd L1/benchmarks/adler32
```

* **Build kernel(Step 2)**


Run the following make command to build your XCLBIN and host binary targeting a specific device. Please be noticed that this process will take a long time, maybe couple of hours.

```
   source /opt/xilinx/Vitis/2022.2/settings64.sh
   source /opt/xilinx/xrt/setenv.sh
   export DEVICE=u50_gen3x16
   export TARGET=hw
   make run 
```

* **Run kernel(Step 3)**

To get the benchmark results, please run the following command.

```
   ./BUILD_DIR/host.exe -xclbin ./BUILD_DIR/Adler32Kernel.xclbin -data PROJECT/data/test.dat -num 16
```

Input Arguments:

```
   Usage: host.exe -[-xclbin]
          -xclbin     binary;
```

* **Example output(Step 4)**

```
   kernel has been created
   kernel start------
   kernel end------
   Execution time 724.018ms
   Write DDR Execution time 1.19501 ms
   Kernel Execution time 721.203 ms
   Read DDR Execution time 0.07055 ms
   Total Execution time 723.504 ms
```

Profiling 
=========

The Adler32 is validated on Xilinx Alveo U50 board. 
Its resource, frequency and throughput is shown as below.

      +-----------+------------+------------+----------+--------+--------+-------------+
      | Frequency |    LUT     |     REG    |   BRAM   |  URAM  |   DSP  | Throughput  |
      +-----------+------------+------------+----------+--------+--------+-------------+
      | 262 MHz   |   6,348    |   12,232   |   16     |   0    |   0    |   4.1 GB/s  |
      +-----------+------------+------------+----------+--------+--------+-------------+


