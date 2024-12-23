#include "framework.h"
#include "myQQ.h"
#define SERVERPORT 8080
#define SERVERADDR "127.0.0.1"

struct clientSession {
    int IP[4];
    int port;
    SOCKET sendSock;
    SOCKET getSock;
    sockaddr_in sendAddr;
    sockaddr_in getAddr;
}; // ��������struct

// ���ͻ������á������Ϣ�������
INT_PTR CALLBACK ClientSet(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam); // ��ʽ����
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
                data->port = GetDlgItemInt(hDlg, IDC_Port1, &targetBit, TRUE);
                int myPort = GetDlgItemInt(hDlg, IDC_Port2, &myportBit, TRUE);
                HWND Warning = GetDlgItem(hDlg, IDC_NeoSTATIC);
                if ((ipBit[0] & ipBit[1] & ipBit[2] & ipBit[3]) == FALSE) {
                    if (Warning != nullptr)SetWindowText(Warning, L"?������IP?");
                }
                else if (targetBit == FALSE) {
                    if (Warning != nullptr)SetWindowText(Warning, L"?������Ŀ��˿�?");
                }
                else if (myportBit == FALSE) {
                    if (Warning != nullptr)SetWindowText(Warning, L"?���������Ķ˿�?");
                }
                else {
                    data->sendSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                    data->getSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
                    if (data->sendSock == INVALID_SOCKET)return 1;
                    if (data->getSock == INVALID_SOCKET)return 1; // ��ʼ��socket
                    data->getAddr.sin_family = AF_INET;
                    inet_pton(AF_INET, SERVERADDR, &(data->getAddr.sin_addr));
                    data->getAddr.sin_port = htons(myPort); // ����getAddr
                    data->sendAddr.sin_family = AF_INET;
                    inet_pton(AF_INET, SERVERADDR, &(data->sendAddr.sin_addr));
                    data->sendAddr.sin_port = htons(SERVERPORT); // ����sendAddr

                    bind(data->getSock, (struct sockaddr*)&(data->getAddr), sizeof(data->getAddr));
                    listen(data->getSock, SOMAXCONN);
                    connect(data->sendSock, (struct sockaddr*)&(data->sendAddr), sizeof(data->sendAddr));

                    HWND neoDialog = CreateDialogParam(hInst, MAKEINTRESOURCE(IDD_CLIENT), hDlg, Client,
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

// ���ͻ��ˡ������Ϣ�������
INT_PTR CALLBACK Client(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    // ��ȡ�洢��IP�Ͷ˿�
    switch (message)
    {
        case WM_INITDIALOG:
        {
            clientSession* data = reinterpret_cast<clientSession*>(lParam);
            SetWindowLongPtr(hDlg, GWLP_USERDATA, lParam); // ��ʼ���Ự��Ϣ��������Ϊ���ڱ����洢
            HWND title = GetDlgItem(hDlg, IDC_CLIENTtitle);
            std::wstringstream ss;
            if (data != nullptr) {
                ss << L"���ں� "<<data->IP[0];
                for (int i = 1; i < 4; ++i)ss << L"." << data->IP[i];
                ss << L":" << data->port<<L" ͨ�š���";
            }
            else {
                ss << L"���ں� " << L"UNknown" << L" ͨ�š���";
            }
            std::wstring s = ss.str();
            const WCHAR* tit = s.c_str();
            if (title != nullptr) SetWindowText(title, tit);
            return (INT_PTR)TRUE;
        }
        case WM_COMMAND:
        {
            clientSession* data = reinterpret_cast<clientSession*>(GetWindowLongPtr(hDlg, GWLP_USERDATA));
            if (LOWORD(wParam) == IDCANCEL)
            {
                closesocket(data->getSock);
                closesocket(data->sendSock);
                delete data;
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            else if (LOWORD(wParam) == IDSENT) {
                int len = GetWindowTextLength(GetDlgItem(hDlg, IDC_TEXTING2)) + 1;
                std::wstring text; text.resize(len);
                GetDlgItemText(hDlg, IDC_TEXTING2, &text[0], len); // ��ȡ������Ϣ����
                std::wstringstream ss;
                ss<< data->IP[0];
                for (int i = 1; i < 4; ++i)ss << L"." << data->IP[i];
                ss << L":" << data->port << L"|" << text;
                std::wstring message = ss.str();
                if(SendData(data->sendSock, message))// ��Ϣ����
                {
                    HWND record = GetDlgItem(hDlg, IDC_RECORD);
                    std::wstringstream ss;
                    text.pop_back();
                    ss << L"send:" << text;
                    ss << L" to " << data->IP[0];
                    for (int i = 1; i < 4; ++i)ss << L"." << data->IP[i];
                    ss << L":" << data->port << L"\r\n";
                    std::wstring s = ss.str();
                    recordMaker(s, record);
                    return (INT_PTR)TRUE;
                }      
                // ��Ϣ��ʾ
            }
            else if (LOWORD(wParam) == IDC_STORE) {
                HWND record = GetDlgItem(hDlg, IDC_RECORD);
                int len = GetWindowTextLength(record);
                WCHAR* buffer = new WCHAR[len + 1];
                GetWindowText(record, buffer, len + 1);
                std::wofstream file("record.txt", std::ios_base::app);
                std::locale loc(std::locale::classic(), new std::codecvt_utf8<wchar_t>);
                file.imbue(loc); // ����������������
                if (file.is_open()) {
                    std::wstringstream ss;
                    ss << L"��" << data->IP[0];
                    for (int i = 1; i < 4; ++i)ss << L"." << data->IP[i];
                    ss << L":" << data->port << L"�ļ�¼";
                    std::time_t currentTime; std::time(&currentTime); // ��ʼ��ȡʱ��
                    struct tm now; localtime_s(&now, &currentTime);
                    ss << L"(" << (now.tm_year + 1900) << "-" << (now.tm_mon + 1) << "-" << now.tm_mday << " "
                        << now.tm_hour << ":" << now.tm_min << ":" << now.tm_sec << L")\r\n";
                    std::wstring s = ss.str();
                    const WCHAR* ti = s.c_str();
                    file << ti << buffer << L"\r\r\n";
                    file.close();
                }
                else {
                    MessageBox(NULL, L"��¼����ʧ��", L"����", MB_OK | MB_ICONERROR);
                }
                delete[]buffer;
                return (INT_PTR)TRUE;
            }
            break;
        }
        case WM_TIMER:
        {
            clientSession* data = reinterpret_cast<clientSession*>(GetWindowLongPtr(hDlg, GWLP_USERDATA));
            // TODO:��Ϣ����
            sockaddr_in clientAddr; SOCKET clientSocket;
            int size = sizeof(clientAddr);
            clientSocket = accept(data->getSock, (sockaddr*)&clientAddr, &size);
            if (clientSocket == INVALID_SOCKET)break;

            break;
        }
    }
    return (INT_PTR)FALSE;
}