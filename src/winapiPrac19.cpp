#include<windows.h>
#include<iostream>

using namespace std;

int main(){
    if(IsDebuggerPresent()){
        cout << "Debugger Detected. Exiting ... " << endl;
        return 1;
    }else{
        cout << "No deugger detected. Continuing Exection" << endl;
    }
    return 0;
}
