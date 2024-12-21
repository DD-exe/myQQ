#include "framework.h"
#include "myQQ.h"
int serverPort = 8080;
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
            ss << L"���ڼ��� " << serverPort;
            std::wstring s = ss.str();
            const WCHAR* tit = s.c_str();
            HWND title = GetDlgItem(hDlg, IDC_CLIENTtitle);
            if (title != nullptr) SetWindowText(title, tit);
            serverSession* data = new serverSession;
            data->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (data->sock == NULL) {
                int error = WSAGetLastError();
                return error;
            }
            int enable = 1;
            if (setsockopt(data->sock, SOL_SOCKET, SO_REUSEADDR,
                reinterpret_cast<char*>(&enable),
                sizeof(enable)) == SOCKET_ERROR)return 1;
            data->addr.sin_family = AF_INET;
            data->addr.sin_addr.s_addr = INADDR_ANY; // ������������ӿ�
            data->addr.sin_port = htons(8080); // ����8080�˿�
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