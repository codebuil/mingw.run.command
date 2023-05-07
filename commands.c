#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define TIMER_ID 1
#define TIMER_INTERVAL 10000
#define MAX_BUFFER_SIZE 1024

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    static TCHAR szAppName[] = TEXT("Temporizador");
    HWND hwnd;
    MSG msg;
    WNDCLASS wndclass;

    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;

    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("Este programa requer Windows NT!"), szAppName, MB_ICONERROR);
        return 0;
    }

    hwnd = CreateWindow(szAppName, TEXT("Temporizador"),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    SetTimer(hwnd, TIMER_ID, TIMER_INTERVAL, NULL);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static TCHAR szBuffer[80];
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;
    time_t rawtime;
    struct tm *timeinfo;
    static HWND hStartEdit,hEndEdit,hButton;
    static char startStr[1024], endStr[600000];
    char buffer[MAX_BUFFER_SIZE];
    

    switch (message)
    {
        case WM_CREATE:
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        wsprintf(szBuffer, TEXT("%02d/%02d/%04d %02d:%02d"), timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min);
        SetWindowText(hwnd, szBuffer);
        hStartEdit =CreateWindow(TEXT("EDIT"), NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
        10, 10, 100, 30,  hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);
        hEndEdit = CreateWindow(TEXT("EDIT"), NULL,
        WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL,
        10, 40, 550, 310,  hwnd, (HMENU)2, GetModuleHandle(NULL), NULL);
        hButton = CreateWindow("BUTTON", "run", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            120, 10, 100, 30,  hwnd, (HMENU)3, GetModuleHandle(NULL), NULL);
        
        break;
        case WM_COMMAND:
        switch (LOWORD(wParam))
        {
            case 3:
                GetWindowText(hStartEdit, startStr, 1000);
                strcpy(endStr,"");
                FILE *fp = popen(startStr, "r");
                while (fgets(buffer, MAX_BUFFER_SIZE, fp) != NULL) {
                    strcat(endStr,buffer); // Imprime o output para a tela
                    strcat(endStr,"\r\n");
                }
                pclose(fp);
                SetWindowText(hEndEdit,endStr);
            break;
        }
    break;   
    case WM_TIMER:
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        wsprintf(szBuffer, TEXT("%02d/%02d/%04d %02d:%02d"), timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min);
        SetWindowText(hwnd, szBuffer);
        break;

    case WM_PAINT:
            
            hdc = BeginPaint(hwnd, &ps);
            
            // Define a cor de fundo da janela para azul
            HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 255));
            FillRect(hdc, &ps.rcPaint, hBrush);
            DeleteObject(hBrush);
            
            // Define a cor das linhas para branco
            HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
            SelectObject(hdc, hPen);
            
            // Desenha as linhas com base nas coordenadas fornecidas
            
            
            // Libera os recursos utilizados
            DeleteObject(hPen);
            EndPaint(hwnd, &ps);
            break;
    

    case WM_DESTROY:
        KillTimer(hwnd, TIMER_ID);
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}
