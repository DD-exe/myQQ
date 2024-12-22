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
            if (data->sock == INVALID_SOCKET)return 1;
            data->addr.sin_family = AF_INET;
            data->addr.sin_addr.s_addr = INADDR_ANY; // ������������ӿ�
            data->addr.sin_port = htons(SERVERPORT); // �����˿�
            bind(data->sock, (sockaddr*)&(data->addr), sizeof(data->addr)); // ��socket��addr
            listen(data->sock, SOMAXCONN); // �����Լ���ע�⿴
            SetWindowLongPtr(hDlg, GWLP_USERDATA, reinterpret_cast<LPARAM>(data)); // ����Ϊserver socket��ʼ��

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
            sockaddr_in clientAddr; SOCKET clientSocket; 
            int size = sizeof(clientAddr);
            clientSocket = accept(data->sock, (sockaddr*)&clientAddr, &size); // �ȴ��ӿ���Ϣ
            if (clientSocket == INVALID_SOCKET)break;
            {
                HWND record = GetDlgItem(hDlg, IDC_RECORD);
                std::string ip = inet_ntoa(clientAddr.sin_addr);
                unsigned short port = ntohs(clientAddr.sin_port);
                std::wstringstream ss;
                ss <<L"������Ϣ����" << S2W(ip) <<L":" << port<<L"\r\n";
                std::wstring s = ss.str();
                const WCHAR* w = s.c_str();
                if (record != nullptr) {
                    int len = GetWindowTextLength(record);
                    SendMessage(record, EM_SETSEL, len, len);
                    SendMessage(record, EM_REPLACESEL, TRUE, reinterpret_cast<LPARAM>(w));
                }
            } 
            // ������ӵĿͻ��˵�ַ
            std::wstring message = ReceiveData(clientSocket);
            if (message.empty()) { closesocket(clientSocket); break; } // �ַ�����ʽ�洢�ӿ���Ϣ
            size_t gunPos = message.find(L'|');
            size_t portPos = message.find(L':');
            if (gunPos == std::wstring::npos) { closesocket(clientSocket); break; }//Invalid message format

            SOCKET targetSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (targetSocket == INVALID_SOCKET)break;
            std::wstring targetIP = message.substr(0, portPos);
            std::wstring targetPORT = message.substr(portPos + 1, gunPos);
            std::wstring targetTEXT = message.substr(gunPos + 1);
            sockaddr_in targetAddr;
            targetAddr.sin_family = AF_INET;
            std::string targetIPs = W2S(targetIP);
            std::string targetPORTs = W2S(targetPORT);
            int targetPort = std::stoi(targetPORTs);
            inet_pton(AF_INET, targetIPs.c_str(), &targetAddr.sin_addr);
            targetAddr.sin_port = htons(targetPort); // ����ת����socket��addr
            connect(targetSocket, (sockaddr*)&targetAddr, sizeof(targetAddr));
            SendData(targetSocket, message);
            {
                HWND record = GetDlgItem(hDlg, IDC_RECORD);
                std::wstringstream ss;
                ss << L"ת����Ϣȥ��" << targetIP << L":" << targetPORT << L"\r\n";
                std::wstring s = ss.str();
                const WCHAR* w = s.c_str();
                if (record != nullptr) {
                    int len = GetWindowTextLength(record);
                    SendMessage(record, EM_SETSEL, len, len);
                    SendMessage(record, EM_REPLACESEL, TRUE, reinterpret_cast<LPARAM>(w));
                }
            }
            closesocket(clientSocket);
            closesocket(targetSocket);
            break;
        }
    }
    return (INT_PTR)FALSE;
}