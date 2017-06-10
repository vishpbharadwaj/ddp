DDP: Dual Decode Parallel, identifies idle hardware in ALU and utilizes it (simulation).

Outputs the frequency of the change in mnemonics as given by the ASM parser program (previous Git)

Has two threads Pipe1 and Pipe2. Pipe1 is 5 stage full pipeline and pipe2 is fetch and decode only and if the x86 mnemonics are different then with help of extra ckts the idle hardware will be utilized.

To build:
-> make -f makefile

To run:
This is run under cpuset(cset) to get accurate readings

if cset is not required comment out the "set affinity" line in the file.

If cset is required to isolate the threads to a particular core to get better readings
-> cset shield -c 1-7 /* cor 0 runs linux and 1 to 7 is free for user program */
-> cset --exec ./ddpsim [asmparser file] 
	/* asm parsed file is the output of the ASM parser program at https://github.com/vishpbharadwaj/asmparser.git */
