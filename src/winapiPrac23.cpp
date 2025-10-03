#include<windows.h>
#include<Lm.h>
#include<iostream>

#pragma comment(lib,"Netapi32.lib")
using namespace std;
/*
    NET_API_STATUS NetSessionEnum(
        LPCWSTR servname(target machine),
        LPCWSTR UncClientName(filtering clients),
        LPCWSTR username(filterring users),
        DWORD level(10,502),
        LBYTE *bufptr(pointer to session buffer),
        DWORD prefmaxlen,
        LPDWORD entreisread,
        LPDWORD totalentries,
        LPDWORD resume_handle
    );
    When using level 10 this structure is returned
    typdef struct _SESSION_INFO_10{
        LPWSTR sessi10_cname;
        LPWSTR sessi10_username;
        DWORD sessi10_time;
        DWORD sessi10_idle_time;
    } SESSION_INFO_10;

*/

int main(){
    LPSESSION_INFO_10 pBuf = NULL;
    DWORD entriesread = 0;
    DWORD totalentries = 0;

    NET_API_STATUS status = NetSessionEnum(
        NULL,
        NULL,
        NULL,
        10,
        (LPBYTE*)&pBuf,
        MAX_PREFERRED_LENGTH,
        &entriesread,
        &totalentries,
        NULL
    );
    if(status == NERR_Success && pBuf != NULL){
        for(DWORD i= 0;i< entriesread;++i){
            wcout << L"Client: " << pBuf[i].sesi10_cname << endl;
            wcout << L"User: " << pBuf[i].sesi10_username << endl;
            wcout << L"Active time: " << pBuf[i].sesi10_time << L" sec" << endl;
            wcout << L"Idle time: " << pBuf[i].sesi10_idle_time << L" sec " << endl;
            wcout << endl;
        }
    }else{
        wcerr << L"NetSessionEnum failed with error: " << status << endl;
    }
    if(pBuf != NULL) NetApiBufferFree(pBuf);
    return 0;
}
