#include<iostream>
#include<Windows.h>
#include <iphlpapi.h>
using namespace std;
/*

    DWORD GetAdpatersInfo(
        PIP_ADPTER_INFO pAdapterInfo,
        PULONG pOutBufLen
    );

   typedef struct _IP_ADAPTER_INFO {
        struct _IP_ADAPTER_INFO *Next;
        DWORD ComboIndex;
        char AdapterName[MAX_ADAPTER_NAME_LENGTH + 4];
        char Description[MAX_ADAPTER_DESCRIPTION_LENGTH + 4];
        UINT AddressLength;
        BYTE Address[MAX_ADAPTER_ADDRESS_LENGTH]; // MAC
        DWORD Index;
        IP_ADDR_STRING IpAddressList;
        IP_ADDR_STRING GatewayList;
        IP_ADDR_STRING DhcpServer;
        BOOL HaveWins;
        IP_ADDR_STRING PrimaryWinsServer;
        IP_ADDR_STRING SecondaryWinsServer;
        time_t LeaseObtained;
        time_t LeaseExpires;
} IP_ADAPTER_INFO;

*/
int main(){
    IP_ADAPTER_INFO adapterinfo[16];
    DWORD size = sizeof(adapterinfo);

    if(GetAdpaterInfo(adapterinfo,&size) == ERROR_SUCCESS){
        PIP_ADAPTER_INFO adapter = adapterinfo;

        while(adapter){
            cout << "Adpater: " << adpater->Description << endl;
            cout << "IP Address: " << adapter->IpAddressList.IpAddress.String << endl;
            cout << "GateWay: " << adapter->GateWayList.IpAddress.String << endl;
            cout << "MAC ADDRESS: ";
            for (uint i = 0;i<adapter->AddresLength;i++){
                printf("%02X%s",adapter->Address[i],(i == adapter->AddressLength - 1) ? "" : "-");
            }
            cout << "\n\n";
            adapter = adapter->next;

        }
    }else{
        cerr << "GetAdapterInfo Failed." << endl;
    }
    DWORD size1 = 0;
    GetIpAddrTable(NULL,&size1,false);

    PMIB_IPADDRTABLE ipTable = (PMIB_IPADDRTABLE)malloc(size);
    if(GetIpAddrTable(ipTable,&size1,true) == NO_ERROR){
        for(int i = 0;i<ipTable->table[i];i++){
            IN_ADDR ipAddr;
            ipAddr.S_un.S_addr = ipTable.table[i].dwAddr;
            cout << "IP address: " << inet_ntoa(ipAddr) << endl;
        }
    }else{
        cerr << "GetIpAddrTable Failed." << endl;
    }
    free(ipTable);
}

