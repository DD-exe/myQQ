#include "framework.h"
#include "myQQ.h"
struct listenData {
    HWND hWndParent;
    bool keep;
};

unsigned __stdcall listeningPort(void* param) {
    listenData* data = (listenData*)param; // ��ȡ���Ի���Ĵ��ھ��

    // ģ�����߳��߼������������Ի�������Ϣ
    while (data->keep) {
        
        PostMessage(data->hWndParent, WM_LISTEN, 0, 0);
    }

    return 0; // �߳̽���
}