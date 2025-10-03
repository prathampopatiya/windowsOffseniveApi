#include<windows.h>
#include<iostream>
#include<stdio.h>

using namespace std;

/*
    LSTATUS RegOpenKeyExW(
        HKEY    hKey,
        LPCWSTR lpSubKey,
        DWORD   ulOptions,
        REGSAM  samDesired,
        PHKEY   phkResult
    );
    LSTATUS RegQueryValueExW(
        HKEY    hKey,
        LPCWSTR lpValueName,
        LPDWORD lpReserved,
        LPDWORD lpType,
        LPBYTE  lpData,
        LPDWORD lpcbData
    );

*/

int main(){
    HKEY hKey;
    const wchar_t* subKey = L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion";
    if(RegOpenKeyExW(HKEY_LOCAL_MACHINE,subKey,0,KEY_READ,&hKey) == ERROR_SUCCESS){
        wchar_t value[256];
        DWORD type = 0;

        if(RegQueryValueKeyExW(hKey,L"[+] ProductName",nullptr,&type,(LPBYTE)value,&value_length) == ERROR_SUCCESS){
           wcout << L"[+] Product name: " << value << endl;
           RegCloseKey(hKey);
        }else{
            printf("[-] %s RegQueryValueExW failed ERROR=0x%08x",__FUNCTION__,GetLastError());
            return -1;
        }

    }
    return 0;
}

/*
---------------------------------------------------------------------------
Common Registry Roots(HKEY)
---------------------------------------------------------------------------
    HKEY_LOCAL_MACHINE - Machine-wide settings (system, software, drivers)
    HKEY_CURRENT_USER - Per-user settings (profiles, apps, persistence)
    HKEY_CLASSES_ROOT - File associations and COM objects
    HKEY_USER - Registry for all user profiles
    HKEY_CURRENT_CONFIG	- 	Hardware profile currently used
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
Red Team Use Cases
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

    Environment recon	Read OS version, architecture, hostname
    Persistence	Check or set values in HKCU\Software\Microsoft\Windows\CurrentVersion\Run
    Credential access	Query HKLM\SYSTEM\CurrentControlSet\Control\Lsa for hashes/secrets
    Defensive evasion	Check Defender status under HKLM\Software\Microsoft\Windows Defender
    Software inventory	Enumerate installed software from HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall
*/
