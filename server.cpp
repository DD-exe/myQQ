#include "framework.h"
#include "myQQ.h"

// “服务端设置”框的消息处理程序。
INT_PTR CALLBACK ServerSet(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
            HWND neoDialog = CreateDialog(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hDlg, ServerSet);
            ShowWindow(neoDialog, SW_SHOW);
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}