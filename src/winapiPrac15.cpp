#include<Windows.h>
#include<iostream>

using namespace std;

int main(void){
    HMODULE hKernel32 = LoadLibraryA("kernel32.dll");
    if(!hKernel32){
        cerr << "Failed to load the library" << endl;
        return 1;
    } 
    FARPROC funcAddr = GetProcAddress(hKernel32,"Beep");
    if(funcAddr){
        cout << "Beep's func address " << funcAddr << endl; 
    }else{
        cerr << "GetProcAddress Failed. Error " << GetLastError() << endl;
    }
    FreeLibrary(hKernel32);
    return 0;
}