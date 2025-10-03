#include<Windows.h>
#include<iostream>

using namespace std;

int main(){
    LPVOID mem = VirtualAlloc(NULL,
        1024,
        MEM_COMMIT|MEM_RESERVE,
        PAGE_READWRITE
    );
    if(!mem){
        cerr << "Failed to allocate memmory. Error" << GetLastError() << endl;
        return 1;
    }
    strcpy_s((char *)mem,1024,"Testing memory location");
    DWORD oldprotect;
    if(VirtualProtect(mem,1024,PAGE_READONLY,&oldprotect)){
        cout << "Memory protection changed" << endl;
    }else{
        cerr << "Failed to change Memory protection. Error" << endl;
    }
    VirtualFree(mem,0,MEM_RELEASE);
    return  0;
}