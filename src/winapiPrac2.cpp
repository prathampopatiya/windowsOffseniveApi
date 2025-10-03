#include<Windows.h>
#include<iostream>

using namespace std;
int main(void){
    HANDLE hfile = CreateFileA(
        "example.txt",
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if(hfile == INVALID_HANDLE_VALUE){
        cerr << "Failed to create file. Error " << GetLastError() << endl;
        return 1;
    }
    const char *data = "Hello,WinAPI file I/O!";
    DWORD bytesWritten;
    BOOL success = WriteFile(
        hfile,
        data,
        strlen(data),
        &bytesWritten,
        NULL
    );
    if(!success){
        cerr << "Failed to write data to the file" << GetLastError() << endl;
    }else{
        cout << "Successfully Wrote " << bytesWritten << "bytes" << endl;
    }
    CloseHandle(hfile);
    return 0;
}