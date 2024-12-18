#include "framework.h"
#include "myQQ.h"

struct clientSession {
    int IP[4];
    int targetPort;
    int myPort;
}; // 传参所需struct

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
            data->targetPort = GetDlgItemInt(hDlg, IDC_Port1, &targetBit, TRUE);
            data->myPort = GetDlgItemInt(hDlg, IDC_Port2, &myportBit, TRUE);
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hDlg, &ps);
            if ((ipBit[0] & ipBit[1] & ipBit[2] & ipBit[3]) == FALSE) {
                TextOut(hdc, 800, 600, L"YourFriend is listening on port L0V3 :)", 39);
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
            EndPaint(hDlg, &ps);
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