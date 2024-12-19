#include "framework.h"
#include "myQQ.h"

struct clientSession {
    int IP[4];
    int targetPort;
    int myPort;
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
            data->targetPort = GetDlgItemInt(hDlg, IDC_Port1, &targetBit, TRUE);
            data->myPort = GetDlgItemInt(hDlg, IDC_Port2, &myportBit, TRUE);
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
    clientSession* data = reinterpret_cast<clientSession*>(GetWindowLongPtr(hDlg, GWLP_USERDATA));
    switch (message)
    {
    case WM_INITDIALOG:
    {
        HWND title = GetDlgItem(hDlg, IDC_CLIENTtitle);
        std::wstringstream ss;
        if (data != nullptr) {
            ss << L"���ں� "<<data->IP[0];
            for (int i = 1; i < 4; ++i)ss << L"." << data->IP[i];
            ss << L":" << data->targetPort<<L" ͨ��";
        }
        else {
            ss << L"���ں� " << L"UNknown" << L" ͨ��";
        }
        std::wstring s = ss.str();
        const WCHAR* tit = s.c_str();
        if (title != nullptr) SetWindowText(title, tit);
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == IDCANCEL)
        {
            delete data;
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDSENT) {

        }
        break;
    }
    return (INT_PTR)FALSE;
}