#include "framework.h"
#include "myQQ.h"

struct clientSession {
    int IP[4];
    int targetPort;
    int myPort;
    SOCKET sock;
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
            data->sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // 定义会话信息
            HWND Warning = GetDlgItem(hDlg, IDC_NeoSTATIC);
            if ((ipBit[0] & ipBit[1] & ipBit[2] & ipBit[3]) == FALSE) {
                if (Warning != nullptr)SetWindowText(Warning, L"?请输入IP?");
            }
            else if (targetBit == FALSE) {
                if (Warning != nullptr)SetWindowText(Warning, L"?请输入目标端口?");
            }
            else if (myportBit == FALSE) {
                if (Warning != nullptr)SetWindowText(Warning, L"?请输入您的端口?");
            }
            else {
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

// “客户端”框的消息处理程序。
INT_PTR CALLBACK Client(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    clientSession* data = reinterpret_cast<clientSession*>(GetWindowLongPtr(hDlg, GWLP_USERDATA)); 
    // 获取存储的IP和端口
    switch (message)
    {
    case WM_INITDIALOG:
    {
        clientSession* data = reinterpret_cast<clientSession*>(lParam);
        SetWindowLongPtr(hDlg, GWLP_USERDATA, lParam); // 初始化会话信息，将其作为窗口变量存储
        HWND title = GetDlgItem(hDlg, IDC_CLIENTtitle);
        std::wstringstream ss;
        if (data != nullptr) {
            ss << L"正在和 "<<data->IP[0];
            for (int i = 1; i < 4; ++i)ss << L"." << data->IP[i];
            ss << L":" << data->targetPort<<L" 通信……";
        }
        else {
            ss << L"正在和 " << L"UNknown" << L" 通信……";
        }
        std::wstring s = ss.str();
        const WCHAR* tit = s.c_str();
        if (title != nullptr) SetWindowText(title, tit);
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == IDCANCEL)
        {
            closesocket(data->sock);
            delete data;
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDSENT) {
            int len = GetWindowTextLength(GetDlgItem(hDlg, IDC_TEXTING2)) + 1;
            std::wstring text; text.resize(len);
            GetDlgItemText(hDlg, IDC_TEXTING2,&text[0], len); // 读取发送消息内容
            HWND sented = GetDlgItem(hDlg, IDC_RECORD);
            {
                std::wstringstream ss;
                text.pop_back();
                ss << text << L" to " << data->IP[0];
                for (int i = 1; i < 4; ++i)ss << L"." << data->IP[i];
                ss << L":" << data->targetPort << L"\r\n";
                std::wstring s = ss.str();
                const WCHAR* tit = s.c_str();
                // 将内容追加到文本框末尾
                if (sented != nullptr) {
                    int len = GetWindowTextLength(sented);
                    SendMessage(sented, EM_SETSEL, len, len);
                    SendMessage(sented, EM_REPLACESEL, TRUE, reinterpret_cast<LPARAM>(tit));
                }
            } // 阶段测试用代码
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_STORE) {
            HWND record = GetDlgItem(hDlg, IDC_RECORD);
            int len = GetWindowTextLength(record);
            WCHAR* buffer = new WCHAR[len + 1];
            GetWindowText(record, buffer, len + 1);
            std::wofstream file("record.txt", std::ios_base::app);
            std::locale loc(std::locale::classic(), new std::codecvt_utf8<wchar_t>);
            file.imbue(loc); // 看不懂，但是有用
            if (file.is_open()) {
                std::wstringstream ss;
                ss << L"与" << data->IP[0];
                for (int i = 1; i < 4; ++i)ss << L"." << data->IP[i];
                ss << L":" << data->targetPort << L"的记录";
                std::time_t currentTime; std::time(&currentTime); // 开始获取时间
                struct tm now; localtime_s(&now, &currentTime);
                ss << L"(" << (now.tm_year + 1900) << "-" << (now.tm_mon + 1) << "-" << now.tm_mday << " "
                    << now.tm_hour << ":" << now.tm_min << ":" << now.tm_sec<< L")\r\n";
                std::wstring s = ss.str();
                const WCHAR* ti = s.c_str();
                file << ti << buffer<<L"\r\n\r\n";
                file.close();
            }
            else {
                MessageBox(NULL, L"记录保存失败", L"错误", MB_OK | MB_ICONERROR);
            }
            delete[]buffer;
            return (INT_PTR)TRUE;
        }
        break;
    case WM_TIMER:

        break;
    }
    return (INT_PTR)FALSE;
}