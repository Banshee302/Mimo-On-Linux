# Mimo for Linux
*Mimo for linux is a Subsytem of Mimo that runs layered on the Linux kernel, it is very bare bones and is easy to add new Apps and Services to test on the Platform.*

To build Mimo Subsystem on Linux you will need:
**A COMPILER**: Gcc is used for the Makefile, just make sure you have the GCC for your target architecture installed.
**THE RUNTIMES**: Go, A linux box, such as WSL or a actual Linux device. And C/C++ standard library.
**ASSEMBLER**: List below:
powerpc-linux-gnu-as for PowerPC
as for ARM (Gnu-Assembler)
NASM for x86.

In future releases Assembly for low level apps will be included, such as the CpuIdentify command and more, but as the first release - these cannot run so far.

To contribute please do a fork.
