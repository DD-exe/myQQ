#include "framework.h"
#include "myQQ.h"
#define SERVERPORT 8080
struct serverSession {
    sockaddr_in addr;
    SOCKET sock;
};
// ������ˡ������Ϣ�������
INT_PTR CALLBACK Server(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam); // ��ʽ����
    switch (message)
    {
        case WM_INITDIALOG:
        {
            std::wstringstream ss;
            ss << L"���ڼ��� " << SERVERPORT;
            std::wstring s = ss.str();
            const WCHAR* tit = s.c_str();
            HWND title = GetDlgItem(hDlg, IDC_CLIENTtitle);
            if (title != nullptr) SetWindowText(title, tit); // ����Ϊ�������
            serverSession* data = new serverSession;
            data->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (data->sock == NULL)return 1;
            int enable = 1;
            data->addr.sin_family = AF_INET;
            data->addr.sin_addr.s_addr = INADDR_ANY; // ������������ӿ�
            data->addr.sin_port = htons(SERVERPORT); // �����˿�
            bind(data->sock, (sockaddr*)&(data->addr), sizeof(data->addr)); // ��socket��addr
            listen(data->sock, SOMAXCONN);
            SetWindowLongPtr(hDlg, GWLP_USERDATA, reinterpret_cast<LPARAM>(data));
            return (INT_PTR)TRUE;
        }
        case WM_COMMAND:
        {
            serverSession* data = reinterpret_cast<serverSession*>(GetWindowLongPtr(hDlg, GWLP_USERDATA));
            if (LOWORD(wParam) == IDCANCEL)
            {
                closesocket(data->sock);
                delete data;
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            break;
        }
        case WM_TIMER:
        {
            serverSession* data = reinterpret_cast<serverSession*>(GetWindowLongPtr(hDlg, GWLP_USERDATA));


            break;
        }
    }
    return (INT_PTR)FALSE;
}