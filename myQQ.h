#pragma once

#include "resource.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
#include <codecvt>
#include <vector>
INT_PTR CALLBACK ClientSet(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK Client(HWND, UINT, WPARAM, LPARAM); // Client
INT_PTR CALLBACK Server(HWND, UINT, WPARAM, LPARAM); // Server
std::string W2S(const std::wstring& wideString);
std::wstring S2W(const std::string& utf8String);
bool SendData(SOCKET socket, const std::wstring& data);
std::wstring ReceiveData(SOCKET socket); // tools