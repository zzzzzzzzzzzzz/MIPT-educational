#include <fstream>
#include <iostream>
#include <sstream>
#include <errno.h>
#include <stdlib.h>
#include <thread>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;

ofstream outfile;

void CopySlice(const long unsigned int begin, const long unsigned int slice, const string& filename, const string& output) {
    ifstream infile(filename, ios::in | ios::binary);
    ofstream outfile(output, ios::out | ios::binary);
    if(!infile || !outfile) {
        cerr << "an error occured while open one of files" << endl;
        exit(errno);
    }
    infile.seekg(begin);
    outfile.seekp(begin);

    char* buffer = new char[slice];
    infile.read(buffer, slice);
    long unsigned int symNum = infile.gcount();
    if(symNum) outfile.write(buffer, symNum);
    //gcount returns amount of bytes
 
    infile.close();
    outfile.close();
    delete[] buffer;
}
 
int main(int argc, char** argv) {
    if (argc < 4) {
        cout << "program needs more arguments. Format: file | distination | number of threads" << endl;
        return 0;
    }
    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    string filename = argv[1];
    string output = argv[2];
    output = output + "result" + filename.substr(filename.find_first_of("."), string::npos);
    string threadNumStr = argv[3];
    istringstream iss (threadNumStr, istringstream::in);
    int threadNum;
    iss >> threadNum;

    ifstream infile(filename, ios::in | ios::binary);
    if(!infile) {
        cerr << "an error occured while open the input file" << endl;
        exit(errno);
    }
    // get length of file:
    infile.seekg (0, infile.end);
    long unsigned int length = infile.tellg();
    infile.close();
    long unsigned int begin = 0;
    long unsigned int slice = length / threadNum + 1;
    thread *t = new thread[threadNum - 1];

    //launch threads
    for (int i = 0; i < threadNum - 1; i++) {
        t[i] = thread(CopySlice, begin, slice, filename, output);
        begin+=slice;
    } 
    
    CopySlice(begin, slice, filename, output);

    //join threads
    for (int i = 0; i < threadNum - 1; i++) {
        t[i].join();
    } 

    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(t2 - t1).count();

    cout << duration << endl;
} 
