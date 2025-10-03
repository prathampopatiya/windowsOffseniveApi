#include<iostream>
#include<Windows.h>

using namespace std;

int main(){
    LPVOID mem = VirtualAlloc(
        NULL,
        1024,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE
    );
    if(mem == NULL){
        cerr << "Not enough space" << endl;
        return 1;
    }
    printf("Memory allocated at %p\n",mem);
    strcpy_s((char *)mem,1024,"This is dynamic memory via WinAPI\n");

    cout << "Memory Content: " << (char *)mem << endl;
    VirtualFree(mem,0,MEM_RELEASE);
    return 0;
}