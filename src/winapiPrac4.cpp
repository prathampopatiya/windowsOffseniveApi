#include<Windows.h>
#include<iostream>

using namespace std;

int main(){
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    cout << "Processor ARCH: " << sysinfo.wProcessorArchitecture << endl;
    cout << "Page Size: " << sysinfo.dwPageSize << "bytes" << endl;
    cout << "Number of processors: " << sysinfo.dwNumberOfProcessors << endl;

    return 0;
}