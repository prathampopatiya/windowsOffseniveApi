#include<windows.h>
#include<iostream>
#include<stdio.h>


using namespace std;

/*  
*'*'*'*'*'*'*'**'*'*'*'*'*'*'**'*'*'*'*'*'*'**'*'*'*'*'*'*'**'*'*'*'*'*'*'**'*'*'*'*'*'*'**'*'*'*'
    LoadLibraryA and LoadLibraryW are core Windows API functions used to load a DLL 
    into the address space of the calling process. They are defined in kernel32.dll and 
    internally call LdrLoadDll from ntdll.dll.
*'*'*'*'*'*'*'**'*'*'*'*'*'*'**'*'*'*'*'*'*'**'*'*'*'*'*'*'**'*'*'*'*'*'*'**'*'*'*'*'*'*'**'*'*'*'
*/

/*
HMODULE LoadLibraryA(LPCSTR lpLibFileName);
HMODULE LoadLibraryW(LPCWSTR lpLibFileName);
*/

int main(){
    const wchar_t *dllPath = L"c:\\windows\\system32\\user32.dll";
    HMODULE hMod = LoadLibraryW(dllPath);

    if(hMod){
        wcout << L"[+] Successfully loaded " << dllPath << endl;
        wcout << L"[+] Base Address: " << hMod << endl;
        FreeLibrary(hMod);
    }else{
        printf("[-] %s LoadLibrary ERROR=0x%08x",__FUNCTION__,GetLastError());
        return -1;
    }
/*
    typedef int (WINAPI* MSGBOXW)(HWND, LPCWSTR, LPCWSTR, UINT);

    HMODULE user32 = LoadLibraryW(L"user32.dll");
    MSGBOXW myMsgBox = (MSGBOXW)GetProcAddress(user32, "MessageBoxW");
    myMsgBox(NULL, L"Dynamic call successful", L"Info", MB_OK);

    Advanced Red Team tools avoid LoadLibrary entirely using manual mapping:

    Parse PE headers
    Map sections into memory
    Fix relocations
    Resolve imports manually
    Call DllMain directly

    This avoids detection from monitoring kernel32!LoadLibrary.
    HMODULE hKernel32 = LoadLibraryW(L"kernel32.dll");
    auto pCreateRemoteThread = (LPTHREAD_START_ROUTINE)GetProcAddress(hKernel32, "CreateRemoteThread");

    if (pCreateRemoteThread) {
    // call dynamically
    }

*/
}