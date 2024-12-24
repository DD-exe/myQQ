#include "framework.h"
#include "myQQ.h"
unsigned __stdcall listeningPort(void* param) {
    listenData* data = (listenData*)param; // ��ȡ���Ի���Ĵ��ھ��
    while (data->keep) {
        returnData* cData=new returnData; // �ǵ��������Ǳ�����delete
        SOCKET clientSocket;
        int size = sizeof(cData->clientAddr);
        clientSocket = accept(data->sock, (sockaddr*)&(cData->clientAddr), &size); // �ȴ��ӿ���Ϣ
        // ������ӵĿͻ��˵�ַ
        cData->message = ReceiveData(clientSocket);
        if (cData->message.empty()) { closesocket(clientSocket); continue; }
        PostMessage(data->hWndParent, WM_LISTEN, 0, (LPARAM)cData);
        closesocket(clientSocket);
    }

    return 0;
}