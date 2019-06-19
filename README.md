# TPU Systolic Array Simulator

******  ******  *    *       * * *  *****  *     *
  *     *    *  *    *       *        *    * * * *
  *     ******  *    *  ***  * * *    *    *  *  *
  *     *       *    *           *    *    *     *
  *     *       ******       * * *  *****  *     *

AUTHOR: PRABAL BASU (prabalb@aggiemail.usu.edu)
 
This is a TPU-style systolic array functional/detailed simulator.
Currently, only one layer of operation is supported.

Source files: pstream.h, SystolicArray.hpp, SystolicArray.cpp, main.cpp

To know about pstream.h, read the second answer in:
https://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c-using-posix

Makefile: makefile

How to compile: make

How to generate debug build: make -f makefile_debug

How to run: ./runTPU -h
Example: ./runTPU -a activation.txt -b bias.txt -w weight.txt -l lut.txt -o output.txt -g green_tpu_stats.txt -d 8

-d -> dimension of the weight, activation and bias matrices. d = 8 means 8x8 matrix.
      Also, the systolic array dimension is same as d.
-a -> text file containing the activation matrix.
-b -> text file containing the bias matrix.
-w -> text file containing the weight matrix.
-l -> input/output text file that will be read and/or written with the last simulation's Error LUT for GreenTPU
-g -> text file that will contain the GreenTPU statistics.
-o -> text file that will contain the output matrix obtained by multiplying the activation 
      and weight matrices through the systolic array.

The simulator also outputs the GreenTPU stats in the file green_tpu_stats.txt

Detailed simulation
===================
For each MAC operation in any cycle, the sta_tool can be invoked
to get a realistic circuit-level delay from the MAC netlist and
and from inputs in two consecutive cycles. This detailed simulation
can be guided from a prior knowledge of input data-driven delay pattern.

Tackling timing errors
======================
By default, timing errors are tackled using TE-Drop [1]. The simulator
also has an implementation of our GreenTPU technique that offers a
better resiliency against PV-induced timing errors at NTC.

[1] Zhang, J. and others ThUnderVolt: Enabling Aggressive
Voltage Underscaling and Timing Error Resilience for Energy
Efficient Deep Neural Network Accelerators. arXiv preprint
arXiv:1802.03806 (2018).


If you use this tool in your work, please use the following BibTeX for citation.

@inproceedings{pandey2019greentpu,
  title={GreenTPU: Improving Timing Error Resilience of a Near-Threshold Tensor Processing Unit},
  author={Pandey, Pramesh and Basu, Prabal and Chakraborty, Koushik and Roy, Sanghamitra},
  booktitle={Proceedings of the 56th Annual Design Automation Conference 2019},
  pages={173},
  year={2019},
  organization={ACM}
}
