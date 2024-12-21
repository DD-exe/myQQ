#include "framework.h"
#include "myQQ.h"
int serverPort = 8080;
// “服务端”框的消息处理程序。
INT_PTR CALLBACK Server(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam); // 形式主义
    switch (message)
    {
    case WM_INITDIALOG:
    {
        std::wstringstream ss;
        ss << L"正在监听 " << serverPort;
        std::wstring s = ss.str();
        const WCHAR* tit = s.c_str();
        HWND title = GetDlgItem(hDlg, IDC_CLIENTtitle);
        if (title != nullptr) SetWindowText(title, tit);
        SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sock == INVALID_SOCKET)return 1;
        SetWindowLongPtr(hDlg, GWLP_USERDATA, reinterpret_cast<LPARAM>(&sock));

        return (INT_PTR)TRUE;
    }
    case WM_COMMAND:
    {
        SOCKET sock = *(reinterpret_cast<SOCKET*>(lParam));
        if (LOWORD(wParam) == IDCANCEL)
        {
            closesocket(sock);
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    case WM_TIMER:
    {
        SOCKET sock = *(reinterpret_cast<SOCKET*>(lParam));

        break;
    }
    }
    return (INT_PTR)FALSE;
}