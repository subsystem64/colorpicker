#include <Windows.h>
#include <stdio.h>


//function to convert RGB to HEX value
int hexcolor(int r, int g, int b)
{
    return (r << 16) | (g << 8) | b;
}
//initializes console
void console_color_init() 
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
}
//print ok
void print_ok() 
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 10);
    printf("OK\n");
}
//print error code
void print_error() 
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 207);
    printf("Error Code: %d\n", GetLastError());
    SetConsoleTextAttribute(hConsole, 15);
}


int main() {

    //Variable declarations
    POINT p;
    COLORREF color;
    HDC hDC;
    BOOL b;

    while (1) {
        
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 3);
        printf("+-+-+-+-+-+-+-+-+-\n");
        printf("   COLOR PICKER\n");
        printf("+-+-+-+-+-+-+-+-+-\n");

        //Register hotkey
        if (RegisterHotKey(NULL, 1, MOD_ALT | MOD_NOREPEAT, 0x43))
        {
            console_color_init();
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            printf("Copyright (c) 2021 subsystem64\n");
            printf("\n[*]Registering hotkey using MOD_NOREPEAT flag ... ");
            print_ok();
            SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
            printf("[-]Press 'ALT + c' to record color under current cursor position\n");
            SetConsoleTextAttribute(hConsole, 15);
            
        }
        else
        {   
            console_color_init();
            printf("[-]An error occured ... ");
            print_error();
        }

      
        MSG msg = { 0 };
        while (GetMessage(&msg, NULL, 0, 0) != 0)
        {
            //If WM_HOTKEY message is recieved
            if (msg.message == WM_HOTKEY)
            {
                // Get the device context for the screen
                hDC = GetDC(NULL);
                if (hDC == NULL)
                    return 3;

                // Get the current cursor position
                b = GetCursorPos(&p);
                if (!b)
                    return 2;

                // Retrieve the color at cursor position
                color = GetPixel(hDC, p.x, p.y);
                if (color == CLR_INVALID)
                    return 1;

                // Release the device context
                ReleaseDC(GetDesktopWindow(), hDC);

                int cr = GetRValue(color);
                int cg = GetGValue(color);
                int cb = GetBValue(color);

                //print RGB value
                printf("\nRGB: %i, %i, %i\n", cr, cg, cb);
                //print HEX value
                printf("Hex: #%x\n", hexcolor(cr, cg, cb));
            }

            
        }


    
    }
    

    return 0;

}