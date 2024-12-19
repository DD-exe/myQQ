#pragma once

#include "resource.h"
#include <winsock2.h>
#include <string>
#include <sstream>
INT_PTR CALLBACK ClientSet(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK Client(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK ServerSet(HWND, UINT, WPARAM, LPARAM);