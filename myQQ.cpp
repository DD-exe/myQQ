﻿#include "framework.h"
#include "myQQ.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")

// 全局变量
#define MAX_LOADSTRING 100
HINSTANCE hInst; // 当前实例
WCHAR szTitle[MAX_LOADSTRING]; // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING]; // 主窗口类名

struct clientSession {
    int IP[4];
    int targetPort;
    int myPort;
}; // 传参所需struct

// 函数前向声明
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK ClientSet(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK Client(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK Server(HWND, UINT, WPARAM, LPARAM);

// winMain
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR lpCmdLine,_In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine); // 形式主义

    // 初始化winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        MessageBox(nullptr, L"Failed to initialize Winsock", L"Error", MB_OK);
        return 1;
    }
    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MYQQ, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))return FALSE; // 初始化执行应用程序

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYQQ));
    MSG msg;

    // 主消息循环
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//注册窗口类。
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYQQ));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MYQQ);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(255, 255, 255));

    return RegisterClassExW(&wcex);
}

//保存实例句柄并创建主窗口
//在此函数中，我们在全局变量中保存实例句柄并创建和显示主程序窗口。
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
   if (!hWnd)return FALSE;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//处理主窗口的消息。
//WM_COMMAND  - 处理应用程序菜单
//WM_PAINT    - 绘制主窗口
//WM_DESTROY  - 发送退出消息并返回
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case ID_AsClient:
            {
                HWND neoDialog = CreateDialog(hInst, MAKEINTRESOURCE(IDD_CLIENTSET), hWnd, ClientSet);
                ShowWindow(neoDialog, SW_SHOW);
                break;
            } // 变量作用域问题
            case ID_AsServer:
            {
                HWND neoDialog = CreateDialog(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, Server);
                ShowWindow(neoDialog, SW_SHOW);
                break;
            }
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            Ellipse(hdc, 0, 0, 800,130); // 绘制一个椭圆
            TextOut(hdc, 250, 60, L"YourFriend is listening on port L0V3 :)", 39);
            HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, L"image/touhou.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);           
            if (hBitmap != NULL)
            {
                HDC hdcMem = CreateCompatibleDC(hdc);
                HBITMAP hBitmapOld = (HBITMAP)SelectObject(hdcMem, hBitmap);
                BITMAP bmpInfo; GetObject(hBitmap, sizeof(BITMAP), &bmpInfo);
                BitBlt(hdc, 600, 130, bmpInfo.bmWidth, bmpInfo.bmHeight, hdcMem, 0, 0, SRCCOPY);
                SelectObject(hdcMem, hBitmapOld);
                DeleteDC(hdcMem);
                DeleteObject(hBitmap);
            }
            Ellipse(hdc, 650, 200, 750, 230);
            Ellipse(hdc, 550, 140, 700, 180); // 绘制气泡
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam); // 形式主义
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;

    }

    return (INT_PTR)FALSE;
}

// “客户端设置”框的消息处理程序。
INT_PTR CALLBACK ClientSet(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam); // 形式主义
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDOK) {
            clientSession* data = new clientSession;
            BOOL ipBit[4] = { FALSE, FALSE, FALSE, FALSE };
            BOOL myportBit = FALSE; BOOL targetBit = FALSE;
            data->IP[0] = GetDlgItemInt(hDlg, IDC_IP, &ipBit[0], TRUE);
            data->IP[1] = GetDlgItemInt(hDlg, IDC_IP2, &ipBit[1], TRUE);
            data->IP[2] = GetDlgItemInt(hDlg, IDC_IP3, &ipBit[2], TRUE);
            data->IP[3] = GetDlgItemInt(hDlg, IDC_IP4, &ipBit[3], TRUE);
            data->targetPort= GetDlgItemInt(hDlg, IDC_Port1, &targetBit, TRUE);
            data->myPort = GetDlgItemInt(hDlg, IDC_Port2, &myportBit, TRUE);
            if (ipBit[0] && ipBit[1] && ipBit[2] && ipBit[3]==FALSE) {

            }
            else if (targetBit == FALSE) {

            }
            else if (myportBit == FALSE) {

            }
            else {
                HWND neoDialog = CreateDialogParam(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hDlg, Client,
                    reinterpret_cast<LPARAM>(data));
                //HWND neoDialog = CreateDialog(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hDlg, Client);
                ShowWindow(neoDialog, SW_SHOW);
                EndDialog(hDlg, LOWORD(wParam));
            }
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// “客户端”框的消息处理程序。
INT_PTR CALLBACK Client(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    clientSession* data = reinterpret_cast<clientSession*>(GetWindowLongPtr(hDlg, GWLP_USERDATA));
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            delete data;
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;

    }
    return (INT_PTR)FALSE;
}

// “服务端设置”框的消息处理程序。
INT_PTR CALLBACK Server(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam); // 形式主义
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDOK) {
            HWND neoDialog = CreateDialog(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hDlg, Server);
            ShowWindow(neoDialog, SW_SHOW);
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}