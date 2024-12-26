#include "framework.h"
#include "myQQ.h"
struct serverSession {
    sockaddr_in addr;
    SOCKET sock;
    listenData cpData;
    HANDLE cp;
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
            // ����Ϊserver socket��ʼ��

            data->cpData.hWndParent = hDlg;
            data->cpData.keep = 1;
            data->cpData.sock = data->sock;
            data->cp = (HANDLE)(_beginthreadex(NULL, 0, listeningPort, &(data->cpData), 0, NULL)); // ����cp������

            SetWindowLongPtr(hDlg, GWLP_USERDATA, reinterpret_cast<LPARAM>(data));

            return (INT_PTR)TRUE;
        }
        case WM_COMMAND:
        {
            serverSession* data = reinterpret_cast<serverSession*>(GetWindowLongPtr(hDlg, GWLP_USERDATA));
            if (LOWORD(wParam) == IDCANCEL)
            {
                closesocket(data->sock);
                data->cpData.keep = 0;
                WaitForSingleObject(data->cp, INFINITE);
                delete data;
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            break;
        }
        case WM_LISTEN:
        {
            serverSession* data = reinterpret_cast<serverSession*>(GetWindowLongPtr(hDlg, GWLP_USERDATA));
            returnData* cData = reinterpret_cast<returnData*>(lParam);
            sockaddr_in clientAddr = cData->clientAddr;
            std::wstring message = cData->message;
            delete cData; // �����ӽ��̴�����Ϣ����
            HWND record = GetDlgItem(hDlg, IDC_RECORD);
            std::string ip = ip2S(clientAddr.sin_addr);
            unsigned short port = ntohs(clientAddr.sin_port);
            std::wstringstream ss1;
            ss1 <<L"������Ϣ����" << S2W(ip) <<L":" << port <<L"\r\n";
            std::wstring s1 = ss1.str();
            recordMaker(s1, record); // ������ӵĿͻ��˵�ַ
            size_t gunPos = message.find(L'|');
            size_t portPos = message.find(L':');
            if (gunPos == std::wstring::npos || portPos == std::wstring::npos) { break; }//Invalid message format

            SOCKET targetSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (targetSocket == INVALID_SOCKET)break;
            std::wstring targetIP = message.substr(0, portPos);
            std::wstring targetPORT = message.substr(portPos + 1, gunPos-(portPos + 1));
            std::wstring targetTEXT = message.substr(gunPos + 1);
            sockaddr_in targetAddr;
            targetAddr.sin_family = AF_INET;
            std::string targetIPs = W2S(targetIP);
            std::string targetPORTs = W2S(targetPORT);
            int targetPort = std::stoi(targetPORTs);
            inet_pton(AF_INET, targetIPs.c_str(), &targetAddr.sin_addr);
            targetAddr.sin_port = htons(targetPort); // ����ת����socket��addr
            std::wstringstream ss3;
            ss3 << S2W(ip) << L":" << port << L"|" << targetTEXT;
            std::wstring message2 = ss3.str();
            if(connect(targetSocket, (sockaddr*)&targetAddr, sizeof(targetAddr))== SOCKET_ERROR)break;
            SendData(targetSocket, message2);
            std::wstringstream ss2;
            ss2 << L"ת����Ϣȥ��" << targetIP << L":" << targetPORT << L"\r\n";
            std::wstring s2 = ss2.str();
            recordMaker(s2, record); // ���ת������Ϣȥ��
            closesocket(targetSocket);
            break;
        }
    }
    return (INT_PTR)FALSE;
}