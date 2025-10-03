#include<windows.h>
#include<iostream>
#include<stdio.h>
#include<Lm.h>

#pragma comment(lib,"Netapi32.lib")


using namespace std;
/*
These two functions, part of the NetAPI32 library, are used to enumerate user accounts and retrieve detailed information about them, such as:

    Usernames
    Account flags (enabled, disabled, locked)
    Last logon time
    Privilege level (Administrator, Standard user, Guest)
    Group membership and password policies

These APIs are essential for:

    Post-exploitation enumeration
    Privilege escalation mapping
    Target discovery during lateral movement
    Domain reconnaissance

    NET_API_STATUS NetUserEnum(
    LPCWSTR servername,
    DWORD level,
    DWORD filter,
    LPBYTE *bufptr,
    DWORD prefmaxlen,
    LPDWORD entriesread,
    LPDWORD totalentries,
    LPDWORD resume_handle
    ); -- Enumerates user on a computer or a domain.

    NET_API_STATUS NetUserGetInfo(
    LPCWSTR servername,
    LPCWSTR username,
    DWORD level,
    LPBYTE *bufptr
    ); 

    typedef struct _USER_INFO_0 {
    LPWSTR usri0_name;
    } USER_INFO_0;

    typedef struct _USER_INFO_1 {
    LPWSTR usri1_name;
    LPWSTR usri1_password;
    DWORD  usri1_password_age;
    DWORD  usri1_priv;
    LPWSTR usri1_home_dir;
    LPWSTR usri1_comment;
    DWORD  usri1_flags;
    LPWSTR usri1_script_path;
    } USER_INFO_1;

*/
int main(){
    LPUSER_INFO_1 userInfobuf = NULL;
    DWORD entrieread = 0,totalentries = 0;

    NET_API_STATUS status = NetUserEnum(
        NULL,
        1,
        FILTER_NORMAL_USER,
        (LPBYTE*)&userIntoBuf,
        MAX_PREFERRED_LENGTH,
        &entriesread,
        &totalentries,
        NULL
    );
    if(status == NERR_Success && userInfoBuf != NULL){
        for(DWORD i = 0;i<totalentries;i++){
            wcout << L"Username: " << pBuf[i].usri1_name << std::endl;
            wcout << L"Privilege: " << (pBuf[i].usri1_priv == USER_PRIV_ADMIN ? L"Administrator" : L"Standard User") << std::endl;
            wcout << L"Flags: " << pBuf[i].usri1_flags << std::endl;
            wcout << L"Comment: " << (pBuf[i].usri1_comment ? pBuf[i].usri1_comment : L"(none)") << std::endl;
            wcout << std::endl;
        }else{
            printf("[-] %s NetUserEnum ERROR=0x%08x",__FUNCTION__,GetLastError());
        }
    }
    return 0;
}

