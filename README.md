# libtfmpvalue

A cmake overlay for TFMpvalue C++ library

This repo serves as a wrapper for the incredibly useful [TFM-Pvalue C++ program](http://bioinfo.lifl.fr/tfm-pvalue/tfm-pvalue.php). It allows users to determine score thresholds for a given transcription factor position frequency matrix associated with a specific p-value. Naturally, it can also perform the reverse, quickly calculating an accurate p-value from a score for a given motif matrix.

``libpytfmpval`` allows this functionality to be easily exploited via a cmake overlay.
This overlay does several things : 
  - core sources are built as a (static) library
  - package command-lines can be compiled via dedicated targets
  - tests from the original packages are implemented as cmake tests

Original sources can be found at http://bioinfo.lifl.fr/tfm-pvalue/tfm-pvalue.php.

**This project has been archived and is provided as-is, with no additional support or development.**

Installation
---------------

To use the library, add these lines to your `CMakeLists.txt` file : 

Contribute
---------------

Any and all contributions are welcome. 


Reference
--------------

| Efficient and accurate P-value computation for Position Weight Matrices
| H. Touzet and J.S. Varré
| *Algorithms for Molecular Biology 2007, 2:15*

License
-----------

This project is licensed under the GPL2 license, as the original sources. 
You are free to use, modify, and distribute it as you see fit.
The program is provided as is, with no guarantees.
