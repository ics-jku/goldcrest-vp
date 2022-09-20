# Goldcrest-VP

Goldcrest - The smallest bird native to Europe


Goldcrest is a microcoded RISC-V Virtual Prototype (VP) which implements the RV32I base instruction set on top of SUBLEQ microcode.
There are two papers based on Goldcrest which you can find [here](https://www.ics.jku.at/files/2022ISVLSI_ExplorationPlatform_RISC-V_OISC.pdf) and  [here](https://www.ics.jku.at/files/2022FDL_FormalVerificationSUBLEQMicrocode.pdf).

## SUBLEQ Microcode
Yes, RISC-V is a Reduced Instruction Set Computer (RISC) architecture, however we can go even further and reduce the number of instruction down to only one.
This kind of instruction set is called an One Instruction Set Computer (OISC) architecture of which SUBLEQ is probably the most famous.
The SUBLEQ instruction, which is shown to be Turing complete, performs a merged subtract-and-branch computation.
While not much is needed to implement an OISC core (in RTL or in SW), it is extremely hard writing software with only one instruction.
We tried to solve this problem by combining RISC-V and SUBLEQ to get the simplicity of SUBLEQ and the large ecosystem of RISC-V.

### SUBLEQ Examples
In SUBLEQ, we can only subtract and branch. Naturally, the question arises how we can implement more operations with just this instruction.

Let us take the example of addition. Adding two registers requires two instructions and a double negation.
For example, if we want to add the registers `SRC1` and `SRC2` we first have to negate `SRC1` by subtracting `SRC1` from a third temporary register `TMP0` which contains 0. Next, we can subtract `TMP0` from `SRC2` which will have the effect of `SRC2 - (-SRC1)`.

Each SUBLEQ instruction consists of three arguments A, B, and C.
First, A is subtracted from B and the result is compared to 0. If the result is ≤ 0 C is added to the program counter. Therefore, having C=1 makes sure that no jump is performed, and the following instruction is executed always.
Oh, and we do not have to write SUBLEQ in every instruction since we only have one instruction anyway ;)

```
SRC1 TMP0 1
TMP0 SRC2 1
```

### SUBLEQ Microcode
The SUBLEQ microcode in `microcode` can be "compiled" to header files using the Emacs package in `subleq-mode`.

## Installation
To install Goldcrest you need `a c++ compiler`, `cmake`, `boost-iostreams`, `boost-log`, and `boost-program-options`.

For example on Ubuntu you can install all dependencies with `sudo apt install g++ cmake libboost-all-dev`.

And on Fedora run `sudo dnf install g++ cmake boost boost-devel`.

Then, to build and install run:
```
cd goldcrest-vp
mkdir release
cd release
cmake -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install
```

## Example Programs
A range of example programs can be found in the `sw` directory.

If you want to compile the test programs a RV32I toolchain must be on your PATH.

## Publications
If you want to learn more about SUBLEQ and RISC-V check out our publications about the Goldcrest VP [1] and about how we formally verified the SUBLEQ microcode [2]!

[1] [Lucas Klemmer and Daniel Große. An exploration platform for microcoded RISC-V cores leveraging the one instruction set computer principle. In ISVLSI, 2022.](https://www.ics.jku.at/files/2022ISVLSI_ExplorationPlatform_RISC-V_OISC.pdf)

[2] [Lucas Klemmer, Sonja Gurtner, and Daniel Große. Formal verification of SUBLEQ microcode implementing the RV32I ISA. In FDL, 2022.](https://www.ics.jku.at/files/2022FDL_FormalVerificationSUBLEQMicrocode.pdf)