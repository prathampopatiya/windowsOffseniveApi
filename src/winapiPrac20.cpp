#include<windows.h>
#include<iostream>
#include<string>
#include<vector>

using namespace std;

vector<wstring> suspiciousTitle = {
    L"OllyDbg",L"x64Dbg",L"IDA",L"Immunity Debugger",L"WinDbg"
};
BOOL CALLBACK EnumWindowsCallBack(HWND hwnd,LPARAM lparam){
    wchar_t title[256];
    GetWindowTextW(hwnd,title,sizeof(title)/sizeof(wchar_t));

    for(const auto &suspect : suspiciousTitle){
        if(wcsstr(title,suspect.c_str())){
            wcout << L"[!] Suspicious Window Detected found: " << title << endl;
        }
    }
    return TRUE;
}
int main(){
    EnumWindows(EnumWindowsCallBack,0);
    return 0;
}
