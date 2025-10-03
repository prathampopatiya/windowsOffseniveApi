#include<windows.h>
#include<iostream>

using namespace std;
/* BOOL GetUserName(
    LPWSTR lpBuffer,
    LPDWORD pcbBuffer                    
    );
    BOOLEAN GetUserNameEx(
        EXTENDED_NAME_FORMAT NameFormat,
        LPWSTR lpNameBuffer,
        PULONG nSize
    );
    NameFormatTypes:- NameSamCompatible(DOMAIN\USERNAME),NameDisplay(FULL NAME FROM AD),NameUserPrincipal(user@domain.com)
*/
int main(){
    WCHAR username[256];
    DWORD size = sizeof(username) / sizeof(WCHAR);

    if(GetUserNameW(username,&size)){
        wcout << L"Current User: " << username << endl;
    }else{
        wcerr << L"Failed to getuserInfo. " << endl;
    }
    if(GetUserNameEx(NameSamCompatible,username,&size)){
        wcout << L"Full identity (SAM): " << username << endl;
    }else{
        wcerr << L"Failed to get full username" << endl;
    }
    return 0;
}