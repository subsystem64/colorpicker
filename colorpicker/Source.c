#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <string.h>
#include <stdio.h>

//function to convert RGB to HEX value
int hexcolor(int r, int g, int b){
    return (r << 16) | (g << 8) | b;
}
//initializes console
void console_color_init(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
}
//print ok
void print_ok(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 10);
    printf("OK\n");
}
//print error code
void print_error_code(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 207);
    printf("Error Code: %d\n", GetLastError());
    SetConsoleTextAttribute(hConsole, 15);
    getchar();
}
//print error
void print_error() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 207);
    printf("ERROR\n");
    SetConsoleTextAttribute(hConsole, 15);
    getchar();
}

int main(){

    //Variable declarations
    POINT p;
    COLORREF color;
    HDC hDC;
    BOOL b;

    while (1){
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 3);
        printf("+-+-+-+-+-+-+-+-+-\n");
        printf("   COLOR PICKER\n");
        printf("+-+-+-+-+-+-+-+-+-\n");

        //Register hotkey
        if (RegisterHotKey(NULL, 1, MOD_ALT | MOD_NOREPEAT, 0x43)){
            console_color_init();
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            printf("Copyright (c) 2021 subsystem64\n");
            printf("\n[*]Registering hotkey using MOD_NOREPEAT flag ...");
            print_ok();
            SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
            printf("[-]Press 'ALT + c' to record color under current cursor position\n");
            SetConsoleTextAttribute(hConsole, 15);
        }
        else{
            console_color_init();
            printf("[-]An error occured ...");
            print_error_code();
        }
      
        MSG msg = { 0 };
        while (GetMessage(&msg, NULL, 0, 0) != 0){
            //If WM_HOTKEY message is recieved
            if (msg.message == WM_HOTKEY){

                // Get the device context for the screen
                hDC = GetDC(NULL);
                if (hDC == NULL) {
                    console_color_init();
                    printf("[-]Unable to grab screen device context ...");
                    print_error();
                    return 3;
                }
                // Get the current cursor position
                b = GetCursorPos(&p);
                if (!b) {
                    console_color_init();
                    printf("[-]Unable to grab current cursor position ...");
                    print_error();
                    return 2;
                }
                // Retrieve the colour at cursor position
                color = GetPixel(hDC, p.x, p.y);
                if (color == CLR_INVALID) {
                    console_color_init();
                    printf("[-]Unable to retrieve pixel colour information (COLOR INVALID) ...");
                    print_error();
                    return 1;
                }

                ReleaseDC(GetDesktopWindow(), hDC);// Release the device context

                int cr = GetRValue(color);
                int cg = GetGValue(color);
                int cb = GetBValue(color);
                
                //print RGB value
                int bufferSize = snprintf(NULL, 0, "\nRGB: %i, %i, %i\n", cr, cg, cb);
                const char* buffer = malloc(bufferSize * sizeof(*buffer) + 1);
                if(buffer == NULL){     //check if malloc failed
                    console_color_init();
                    printf("[-]Failed to allocate buffer 1 ...");
                    print_error();
                    return -1;
                }
                snprintf(buffer, bufferSize + 1, "\nRGB: %i, %i, %i\n", cr, cg, cb);//write rbg value to buffer
                printf(buffer);

                //print HEX value
                int buffer2Size = snprintf(NULL, 0, "Hex: #%x\n", hexcolor(cr, cg, cb) + bufferSize);
                const char* buffer2 = malloc(buffer2Size * sizeof(*buffer2) + bufferSize + 1);
                if (buffer2 == NULL) {      //check if malloc failed
                    console_color_init();
                    printf("[-]Failed to allocate buffer 2 ...");
                    print_error();
                    return -1;
                }
                snprintf(buffer2, buffer2Size + bufferSize + 1, "Hex: #%x\n", hexcolor(cr, cg, cb));//write hex value to buffer
                printf(buffer2);

                buffer = ++buffer;//shift pointer right to remove \n
                
                strcat(buffer2, buffer);//concatenate string
                
                buffer = --buffer;//shift pointer back
                free(buffer);

                //copy output to clipboard
                const size_t len = strlen(buffer2) + 1;
                HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
                memcpy(GlobalLock(hMem), buffer2, len);
                GlobalUnlock(hMem);
                OpenClipboard(0);
                EmptyClipboard();
                SetClipboardData(CF_TEXT, hMem);
                CloseClipboard();

                free(buffer2);
            }         
        } 
    }   

    return 0;
}