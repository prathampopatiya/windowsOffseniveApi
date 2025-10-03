#include<windows.h>
#include<iostream>
#include<wincrypt.h>

#pragma comment(lib,"Crypt32.lib")

/*
CryptUnprotectData is a Windows API that decrypts data previously encrypted with CryptProtectData using the Windows Data Protection API (DPAPI).

It is commonly used to:

    Retrieve saved credentials (e.g., browser passwords, Wi-Fi keys, app secrets)
    Decrypt configuration blobs or secrets stored by malware or legitimate software
    Extract secrets from LSA secrets, Credential Manager, Chrome, RDP, etc.

    BOOL CryptUnprotectData(
    DATA_BLOB* pDataIn,
    LPWSTR*    ppszDataDescr,
    DATA_BLOB* pOptionalEntropy,
    PVOID      pvReserved,
    CRYPTPROTECT_PROMPTSTRUCT* pPromptStruct,
    DWORD      dwFlags,
    DATA_BLOB* pDataOut
    );
    typedef struct _DATA_BLOB {
    DWORD cbData;
    BYTE* pbData;
    }   DATA_BLOB;

*/

BOOL DecryptDPAPI(const BYTE* encryptionData,DWORD dataSize){
    DATA_BLOB inBlob = {datasize,const_cast<BYTE*>(encryptedData)};
    DATA_BLOB outBlob = {};

    if(CryptUnprotectData(
        &inBlob,
        NULL,
        NULL,
        NULL,
        NULL,
        0,
        &outBlob
    )){
        std::cout << "Decrypted Data: ";
        for(DWORD i = 0;i<outBlob.cbData;i++){
            cout << static_cast<char>(outBlob.pbData[i]);
        }
        std::cout << std::endl;
        LocalFree(outBlob.pbData);
        return true;
    }else{
        printf("[-] %s CryptUnprotectData ERROR=0x%08x",__FUNCTION__,GetLastError());
        return -1;
    }
}
int main(){
    BYTE exampleData[] = {};
    DWORD exampleSize = sizeof(exampleData);

    DecryptDPAPI(exampleData,exampleSize);
    return 0;
}


