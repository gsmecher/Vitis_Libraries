.. 
   Copyright 2019 Xilinx, Inc.
  
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
  
       http://www.apache.org/licenses/LICENSE-2.0
  
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

.. meta::
   :keywords: Vitis, Database, Vitis Database Library, Alveo
   :description: Vitis Database library licensing, software and hardware requirements.
   :xlnxdocumentclass: Document
   :xlnxdocumenttype: Tutorials

.. _overview:

.. toctree::
   :hidden:

Requirements
------------

FPGA Accelerator Card
~~~~~~~~~~~~~~~~~~~~~

The Database Library relies heavily on HBM,
so that high-level APIs (L2 and L3) are mostly targeting Alveo cards with HBM. For example,

* `Alveo U55C <https://www.xilinx.com/products/boards-and-kits/alveo/u55c.html>`_
* `Alveo U50 <https://www.xilinx.com/products/boards-and-kits/alveo/u50.html>`_
* `Alveo U280 <https://www.xilinx.com/products/boards-and-kits/alveo/u280.html>`_

The low-level L1 modules are actually device-agonostic, so that they can be instantiated into a Vitis project
targeting any platform, as long as the resources budget can be met.

Software Platform
~~~~~~~~~~~~~~~~~

Supported operating systems are RHEL/CentOS 7.4, 7.5 and Ubuntu 16.04.4 LTS, 18.04.1 LTS.

*GCC 5.0 or above* is required for C++11/C++14 support.
With CentOS/RHEL 7.4 and 7.5, C++11/C++14 should be enabled via
`devtoolset-6 <https://www.softwarecollections.org/en/scls/rhscl/devtoolset-6/>`_.

Development Tools
~~~~~~~~~~~~~~~~~

This library is designed to work with Vitis 2022.2,
and a matching version of XRT should be installed for both compilation and run.


Dependency
~~~~~~~~~~

This library depends on the Vitis Utility Library, which is assumed to be placed in the
same path as this library with name ``utils``. Hence the directory is organized as follows.

.. code-block:: shell

    /cloned/path/database # This library, which contains L1, L2, etc.
    /cloned/path/utils # The Vitis Utility Library, which contains its L1.

