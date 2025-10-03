#include<Windows.h>

int main(){
    MessageBoxA(
        NULL,
        "Hello From WinAPI!",
        "MessageBox Here",
        MB_OK | MB_ICONINFORMATION
    );
    return 0;
}