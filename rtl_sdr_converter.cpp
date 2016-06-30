// Tegan Lamoureux
// tegan@teux.me
// June 8, 2016

// Converts raw output of rtl_sdr program into useable data (.cfile) for gqrx
// and other SDR software.
//
// Nitty gritty: Takes the raw I/Q output of the rtl-sdr program, which is
//               unsigned 8 bit interleaved I/Q samples (0 - 255), and
//               converts them to an interleaved, 32 bit floating point,
//               normalized (between -1.0 and +1.0) format.

#include <iostream>
#include <cstring>
#include <fstream>

// TODO: - Update to handle pipe input/output (for compatibility with fifo), and
//         POSIX switches and convention.

using namespace std;


const int BUFF_SIZE = 2; // Input buffer size. Currently 2 bytes (1 I/Q sample).


int main(int argc, char *argv[])
{
    ifstream infile;   // Source file stream.
    ofstream outfile;  // Destination file stream.

    char * raw_data = new char[BUFF_SIZE]; // My temp read buffer.
    float i = 0.0f, q = 0.0f; // My result containers (post-conversion).
    bool skip_conversion = false; // If true, skip conversion.

    if(argc != 3) // Check for proper usage, display error and exit if not.
    {
        cout << "Usage: " << argv[0] << " <input_file> <output_file>" << endl;
        return 1;
    }

    infile.open(argv[2]); // Test open output file.

    if(infile && !skip_conversion) // Check if file exsists already.
    {
        char warning_input;

        cout << "Warning, file already exsists. Overwrite? (y/n): ";
        cin >> warning_input;

        if(tolower(warning_input) != 'y') // Only proceed if 'y' or 'Y'.
            skip_conversion = true;

        infile.close();
    }

    // Open my input and output files in binary mode.
    infile.open(argv[1], ios::binary);
    outfile.open(argv[2], ios::binary | ios::trunc);

    if(infile && outfile && !skip_conversion)
    {
        // Read two bytes at a time (the in-phase and quadrature component of
        // a single sample, I and Q).
        while(infile.read(raw_data, BUFF_SIZE))
        {
            // Conversion steps:
            // 1. Deinterleave. Read two bytes (one sample) at a time.
            // 2. Convert from 8-bit unsigned value (0 to 255) to a floating
            //    point value 0.0f to 255.0f.
            // 3. Subtract 127.5f to move range from 0 to 255 down to -127.5f to
            //    +127.5f.
            // 4. Multiply by 1/128 to normalize values between -1.0 and +1.0.

            i = (((unsigned char)raw_data[0]) - 127.5f) * 0.0078125f;
            q = (((unsigned char)raw_data[1]) - 127.5f) * 0.0078125f;

            // Write 32 bits of each component, interleaved again I/Q/I/Q....
            outfile.write((char *)&i, 4);
            outfile.write((char *)&q, 4);
        }
    }
    else if(!infile)
        cout << "Input file error." << endl;
    else if(skip_conversion)
        cout << "Conversion skipped." << endl;

    // Clean up allocated memory and close files.
    if(raw_data)
        delete [] raw_data;

    if(infile)
        infile.close();
    if(outfile)
        outfile.close();

    return 0;
}
