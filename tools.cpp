#include "framework.h"
#include "myQQ.h"
std::string W2S(const std::wstring& wideString) { //wstring to string
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wideString);
}

std::wstring S2W(const std::string& utf8String) { //string to wstring
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(utf8String);
}

bool SendData(SOCKET socket, const std::wstring& data) {
    std::string utf8Data = W2S(data);
    uint32_t dataLength = static_cast<uint32_t>(utf8Data.size());
    if (send(socket, reinterpret_cast<const char*>(&dataLength), sizeof(dataLength), 0) == SOCKET_ERROR)return false;
    if (send(socket, utf8Data.c_str(), utf8Data.size(), 0) == SOCKET_ERROR)return false;

    return true;
}

std::wstring ReceiveData(SOCKET socket) {    
    uint32_t dataLength = 0;
    int result = recv(socket, reinterpret_cast<char*>(&dataLength), sizeof(dataLength), 0); // 接收数据长度
    if (result <= 0)return L"";
    std::vector<char> buffer(dataLength);
    result = recv(socket, buffer.data(), dataLength, 0);
    if (result <= 0)return L"";

    return S2W(std::string(buffer.data(), result));
}

std::string ip2S(struct in_addr addr) {
    char ipc[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr, ipc, INET_ADDRSTRLEN);
    std::string ip(ipc);

    return ip;
}

void recordMaker(std::wstring s, HWND record) {
    const WCHAR* tit = s.c_str();
    // 将内容追加到文本框末尾
    if (record != nullptr) {
        int len = GetWindowTextLength(record);
        SendMessage(record, EM_SETSEL, len, len);
        SendMessage(record, EM_REPLACESEL, TRUE, reinterpret_cast<LPARAM>(tit));
    }
}