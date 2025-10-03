#include <windows.h>
#include <NTSecAPI.h>
#include <Sddl.h>
#include <iostream>


/*
    These two LSA (Local Security Authority) APIs allow enumeration of all logon sessions on a system, along with detailed data like:

    Username and domain
    Logon time and session ID (LUID)
    Logon type (interactive, remote, service, etc.)
    SID (Security Identifier)
    Authentication package (e.g., Kerberos, MSV1_0)

    NTSTATUS LsaEnumerateLogonSessions(
    PULONG  LogonSessionCount,
    PLUID   *LogonSessionList
    );
    typedef struct _SECURITY_LOGON_SESSION_DATA {
    ULONG     Size;
    LUID      LogonId;
    UNICODE_STRING UserName;
    UNICODE_STRING LogonDomain;
    UNICODE_STRING AuthenticationPackage;
    ULONG     LogonType;
    ULONG     Session;
    PSID      Sid;
    LARGE_INTEGER LogonTime;
    // ...
    } SECURITY_LOGON_SESSION_DATA, *PSECURITY_LOGON_SESSION_DATA;
*/

#pragma comment(lib,"Secur32.lib")

int wmain(){
    ULONG sessionCount = 0;
    PLUID sessions = nullptr;

    if(LsaEnumerateLogonSessions(&sessionCount,&sessions) != 0){
        std::wcerr << "Failed to enumerated Lsalogons sessions" << std::endl;
        return -1;
    }
    for(ULONG i=0;i<sessionCount;i++){
        PSECURITY_LOGON_SESSION_DATA pSessionData = nullptr;
        if(LsaGetLogonSessionData(&sessions[i],pSessionData) == 0 && pSessionData){
            if(pSessionData->UserName.Buffer && pSessionData->LogonDomain.Buffer){
                std::wcout << L"User: " << std::wstring(pSessionData->AuthenticationPackage.Buffer,
                pSessionData->AuthenticationPackage.Length/2) << std::endl;

                std::wcout << L"Logon Type: " << pSessionData->Logontype << std::endl;
                std::wcout << L"Logon Session" << pSessionData->Session << std::endl;

                if (pSessionData->Sid) {
                    LPWSTR sidStr;
                    if (ConvertSidToStringSid(pSessionData->Sid, &sidStr)) {
                        std::wcout << L"SID: " << sidStr << std::endl;
                        LocalFree(sidStr);
                    }
                }
                 std::wcout << L"------------------------" << std::endl;
            }
             LsaFreeReturnBuffer(pSessionData);
        }
    }
    LsaFreeReturnBuffer(sessions);
    return 0;
}
/*
Session Hijacking	    | Identify logon sessions of high-privilege users (e.g., DOMAIN\Administrator)
Token Impersonation	    | Combine with OpenProcessToken, DuplicateToken, ImpersonateLoggedOnUser
Credential Harvesting   | Locate interactive users and extract credentials from LSASS memory
Multi-user Recon        | Useful on RDP servers, terminal servers, Citrix, and VDI environments
Domain Lateral Movement	| Identify sessions of domain users for pivoting
*/



