#include "pch.h"
#include "./Core/Render.h"
#include "./Core/Shader.h"
#include "./Core/Mesh.h"
#include "./Core/Camera.h"


//Window
HINSTANCE mainHInstance;
HWND mainHwnd;



LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    mainHInstance = hInstance;

    //Registe Window
    LPCSTR CLASS_NAME = "BKEngine";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    // CreateWindow
    HWND hwnd = CreateWindowEx(0, CLASS_NAME, "BKEngine", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 800, NULL, NULL, hInstance, NULL);
    mainHwnd = hwnd;

    // ShowWindow
    ShowWindow(hwnd, SW_NORMAL);


    //Start
    Start(hwnd);


    MSG msg = {};
    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        //Loop
        Update();

        Sleep(16.67f);
    }
    
    return 0;

}












