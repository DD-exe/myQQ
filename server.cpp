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
            SOCKET* sock = new SOCKET;
            *sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (*sock == NULL) {
                int error = WSAGetLastError();
                return error;
            }
            int enable = 1;
            if (setsockopt(*sock, SOL_SOCKET, SO_REUSEADDR, 
                reinterpret_cast<char*>(&enable),
                sizeof(enable)) == SOCKET_ERROR)return 1;
            sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = INADDR_ANY; // ������������ӿ�
            addr.sin_port = htons(8080); // ����8080�˿�
            SetWindowLongPtr(hDlg, GWLP_USERDATA, reinterpret_cast<LPARAM>(sock));

            return (INT_PTR)TRUE;
        }
        case WM_COMMAND:
        {
            SOCKET* sock = reinterpret_cast<SOCKET*>(GetWindowLongPtr(hDlg, GWLP_USERDATA));
            if (LOWORD(wParam) == IDCANCEL)
            {
                closesocket(*sock);
                delete sock;
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            break;
        }
        case WM_TIMER:
        {
            SOCKET* sock = reinterpret_cast<SOCKET*>(GetWindowLongPtr(hDlg, GWLP_USERDATA));

            break;
        }
    }
    return (INT_PTR)FALSE;
}