#include "framework.h"
#include "myQQ.h"
int serverPort = 8080;
// ������ˡ������Ϣ�������
INT_PTR CALLBACK Server(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam); // ��ʽ����
    switch (message)
    {
    case WM_INITDIALOG:
    {
        std::wstringstream ss;
        ss << L"���ڼ��� " << serverPort;
        std::wstring s = ss.str();
        const WCHAR* tit = s.c_str();
        HWND title = GetDlgItem(hDlg, IDC_CLIENTtitle);
        if (title != nullptr) SetWindowText(title, tit);
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    case WM_TIMER:

        break;
    }
    return (INT_PTR)FALSE;
}