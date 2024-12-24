#include "framework.h"
#include "myQQ.h"
struct listenData {
    HWND hWndParent;
    bool keep;
};

unsigned __stdcall listeningPort(void* param) {
    listenData* data = (listenData*)param; // 获取主对话框的窗口句柄

    // 模拟子线程逻辑，定期向主对话框发送消息
    while (data->keep) {
        
        PostMessage(data->hWndParent, WM_LISTEN, 0, 0);
    }

    return 0; // 线程结束
}