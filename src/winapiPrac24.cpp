#include<windows.h>
#include<winsvc.h>
#include<iostream>

using namespace std;

/*
    BOOL EnumServicesStatusEx(
        SC_HANDLE hSCManager(handle from OpenSCManager()),
        SC_ENUM_TYPE InfoLevel(set to SC_ENUM_PROCESS_INFO to retrive process related info),
        DWORD dwServiceType( Use SERVICE_WIN32 for normal services),
        DWORD dwServiceState(Use SERVICE_STATE_ALL or SERVICE_ACTIVE),
        LPBYTE lpServices(: Buffer that receives the data (array of ENUM_SERVICE_STATUS_PROCESS)),
        DWORD cbBufSize,
        LPDWORD pcbBytesNeeded,
        LPDWORD lpServicesReturned,
        LPDWORD lpResumeHandle,
        LPCWSTR pszGroupName
    );
    typedef struct _ENUM_SERVICE_STATUS_PROCESSW {
        LPWSTR lpServiceName;
        LPWSTR lpDisplayName;
        SERVICE_STATUS_PROCESS ServiceStatusProcess;
}  ENUM_SERVICE_STATUS_PROCESSW;

*/
// List All Services anf their state

int main(){
    SC_HANDLE hScManager = OpenSCManager(NULL,NULL,SC_MANAGER_ENUMERATE_SERVICE);
    if(!hSCManager){
        cerr << "OpenSCManager failed." << endl;
        return 1;
    }

    DWORD bytesNeeded = 0,servicesReturned=0,resumeHandle = 0;
    EnumServicesStatusEx(
        hSCManager,
        SC_ENUM_PROCESS_INFO,
        SERVICES_WIN32,
        SERVICE_STATE_ALL,
        NULL,
        0,
        &bytesNeeded,
        &servicesReturned,
        &resumeHandle,
        NULL
    );

    LPBYTE buffer = new BYTE[bytesNeeded];
    if(EnumServicesStatusEx(
        hSCManager,
        SC_ENUM_PROCESS_INFO,
        SERVICES_WIN32,
        SERVICE_STATE_ALL,
        buffer,
        bytesNeeded,
        &bytesNeeded,
        &servicesReturned,
        &resumeHandle,
        NULL
    )){
        LPENUM_SERVICE_STATUS_PROCESS services = (LPENUM_SERVICE_STATUS_PROCESS)buffer;
        for (int i = 0;i<servicesReturned;i++){
            wcout << L"Service Name: " << services[i].lpServiceName << endl;
            wcout << L"Service display name: " << services[i].lpServiceDisplayName << endl;
            wcout << L"State: " <<;
            switch(services[i].ServicesStatusProcess.dwCurrentState){
                case SERVICE_RUNNING : wcout << L"Running: "; break;
                case SERVICE_STOPPED : wcout << L"Stopped: "; break;
                case SERVICE_PAUSED : wcout << L"Paused: "; break;
                default : wcout << L"Others" ; break;

            }
            wcout << L" | PID " << services[i].ServicesStatusProcess.dwProcessId << endl;
            wcout << endl;

        }
    }else{
        cerr << "EnumServicesStatusEx failed." << endl;
    }
    delete[] buffer;
    CloseServiceHandle(hSCManager);
    return 0;
}
