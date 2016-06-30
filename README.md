# RTL_SDR to CFILE Conversion Utility

### A utility to convert the raw output of the Osmocom rtl_sdr program to the .cfile format used by SDR programs like GQRX and GNU Radio.

##### Goal:
To explore data types and interactions, while learning the processes involved in writting a command line utility. Also to create a lighter weight utility to perform the conversion process normally required when handling raw data given by the [Osmocom rtl-sdr program](http://sdr.osmocom.org/trac/wiki/rtl-sdr) when used in conjuntion with [inexpensive SDR tuners](http://www.rtl-sdr.com/) using the RTL2832U chip. Normally conversion is performed using GNU Radio signal processing blocks, which is [where I obtained the algorithm](http://sdr.osmocom.org/trac/wiki/rtl-sdr#Usingthedata) used in this utility. While very robust and effective, there's quite a bit of overhead that limits working with the data on smaller systems (such as the Raspberry Pi).

##### What it does:
Takes the raw output of the Osmocom rtl_sdr program, which is unsigned 8 bit interleaved [I/Q samples](https://en.wikipedia.org/wiki/In-phase_and_quadrature_components) (0 - 255), and converts them to a normalized (between -1.0 and +1.0), interleaved, 32 bit floating point format that's useful with programs like [GNU Radio](http://gnuradio.org/) and [GQRX](http://gqrx.dk/).

##### Building:
`g++ -o rtl_sdr_converter rtl_sdr_converter.cpp`

##### Usage:
`./rtl_sdr_converter <input_file> <output_file>`

##### ToDo List:
1. Modify the command line utility to use POSIX flags and switches, as well as the ability to handle piped input/output (for useage with fifo named pipes, for example). 
2. Expand error handling to be more robust and verbose.

##### Author:
Tegan Lamoureux, tegan@teux.me