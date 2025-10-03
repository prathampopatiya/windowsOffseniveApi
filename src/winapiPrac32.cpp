#include<windows.h>
#include<iostream>
#include<stdio.h>

using namespace std;

int main(){
    HMODULE hMod = LoadLibrary(L"user32.dll");
    if(!hMod){
        printf("%s LoadLibrary ERROR=0x%08x",__FUNCTION__,GetLastError());
        return -1;
    }
    typedef int (WINAPI *MsgBoxFunc)(HWND,LPCSTR,LPCSTR,UINT);
    MsgBoxFunc OrdinalBoxA = (MsgBoxFunc)GetProcAddress(hMod,(LPCSTR)2150);
    if(!OrdinalboxA){
        printf("Failed to get function by ordinal\n");
        CloseHandle(hMod);
        return -1;
    }
    OrdinalBoxA(NULL,"Hello World","text message boxA",MB_OK|MB_ICONINFORMATION);
    FreeLibrary(hMod);
    return 0;
}