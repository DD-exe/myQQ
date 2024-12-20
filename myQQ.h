#pragma once

#include "resource.h"
#include <winsock2.h>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
#include <codecvt>
INT_PTR CALLBACK ClientSet(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK Client(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK Server(HWND, UINT, WPARAM, LPARAM);