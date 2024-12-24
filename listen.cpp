#include "framework.h"
#include "myQQ.h"
unsigned __stdcall listeningPort(void* param) {
    listenData* data = (listenData*)param; // 获取主对话框的窗口句柄
    while (data->keep) {
        returnData* cData=new returnData; // 记得在主程那边用完delete
        SOCKET clientSocket;
        int size = sizeof(cData->clientAddr);
        clientSocket = accept(data->sock, (sockaddr*)&(cData->clientAddr), &size); // 等待接口消息
        // 输出连接的客户端地址
        cData->message = ReceiveData(clientSocket);
        if (cData->message.empty()) { closesocket(clientSocket); continue; }
        PostMessage(data->hWndParent, WM_LISTEN, 0, (LPARAM)cData);
        closesocket(clientSocket);
    }

    return 0;
}