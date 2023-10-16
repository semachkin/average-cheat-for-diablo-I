#pragma region headers

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"
#include "windows.h"
#include "processthreadsapi.h"
#include "memoryapi.h"
#include "tlhelp32.h"
#include "psapi.h"

#include "data.h"

#pragma endregion

#pragma region variables

HBRUSH hBrsh;
RECT   rect;

HWND hWin;
HWND healthButton;
HWND healthBox;

BOOL quit;
BOOL diabloProcessActive;

PAINTSTRUCT paint;
HDC         decal;

HANDLE         hProcessSnap;
PROCESSENTRY32 hProcessEntry;
DWORD          diabloProcessId;
HANDLE         diabloHandle;
size_t         diabloBaseAddress;

LPSTR healthBoxText;
SHORT message;

#pragma endregion

#define InvalidateScreen() InvalidateRect(hWin, NULL, FALSE);

process_error_t checkDiabloProcess();
error_status_t  setHealth();

LRESULT CALLBACK WinProc(HWND hWin, UINT msg, WPARAM wPar, LPARAM lPar) {

    switch (msg) {
        case WM_DESTROY:

            quit = TRUE;
            PostQuitMessage(QUIT_MESSAGE);
        break;
        case WM_COMMAND:
            
            switch (LOWORD(wPar)) {
                case BUTTON_1_ACTIVATED:
                    if (!diabloProcessActive) break;

                    DWORD healthCount;
                    healthCount = atoi(healthBoxText);

                    if (healthCount == ZERO) {
                        message = MESSAGE_WRONG_HEALTH;
                        InvalidateScreen();
                        break;
                    }
                    
                    setHealth(healthCount);
                break;
                case BOX_1_ACTIVATED:
                    INT textLength;

                    textLength = GetWindowTextLength(healthBox);
                    if (healthBoxText) free(healthBoxText);

                    healthBoxText = malloc(++textLength);
                    GetWindowTextA(healthBox, healthBoxText, textLength);
                break;
            }
        break;
        case WM_PAINT:
            decal = BeginPaint(hWin, &paint);

            GetClientRect(hWin, &rect);
            FillRect(decal, &rect, hBrsh);

            if (message) {

                LPCSTR messageText;

                switch (message) {
                    case MESSAGE_ERROR:            messageText = ERROR_MESSAGE;
                    break;
                    case MESSAGE_NEED_OPEN_DIABLO: messageText = PROCESS_WAIT_TEXT;
                    break;
                    case MESSAGE_WRONG_HEALTH:     messageText = WRONG_HEALTH;
                    break;
                }

                DrawText(decal, messageText, UNKNOWN_SIZE, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }

            EndPaint(hWin, &paint);
        break;
        case WM_CREATE:

            healthButton = CreateWindowEx(
                BUTTON_STYLE,
                BUTTON_CLASS,
                BUTTON_1_NAME,
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                BUTTON_1_POSITION,
                BUTTON_SIZE,
                hWin,
                (HMENU)BUTTON_1_ACTIVATED,
                (HINSTANCE)GetWindowLongPtr(hWin, GWLP_HINSTANCE),
                NULL
            );
            healthBox = CreateWindowEx(
                WS_EX_CLIENTEDGE,
                BOX_CLASS,
                VOID_STRING,
                WS_CHILD | WS_VISIBLE,
                BOX_1_POSITION, 
                BOX_SIZE, 
                hWin, 
                (HMENU)BOX_1_ACTIVATED,
                NULL_2X
            );
        default: return DefWindowProc (
            hWin, msg, wPar, lPar
        );
    }
    return ERROR_SUCCESS;
}

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hpInst, LPSTR cLine, INT cShow) {
    BOOL ciErr;

    WNDCLASSEX wClass;
    MSG        msg;

    hBrsh = CreateSolidBrush((COLORREF)0x000000AA);
    
    wClass.cbSize        = sizeof(WNDCLASSEX);
    wClass.style         = CS_DBLCLKS;
    wClass.lpfnWndProc   = WinProc;
    wClass.cbClsExtra    = EXTRA_BYTES;
    wClass.cbWndExtra    = EXTRA_BYTES;
    wClass.hInstance     = hInst;
    wClass.hIcon         = NULL;
    wClass.hCursor       = NULL;
    wClass.hbrBackground = hBrsh;
    wClass.lpszMenuName  = NULL;
    wClass.lpszClassName = WINDOW_CLASS;
    wClass.hIconSm       = NULL;

    if(!RegisterClassEx(&wClass)) {
        MessageBox(NULL, REGISTRATION_FAILED, ERROR_MESSAGE, MB_OK);
        return ERROR_CAN_NOT_COMPLETE;
    }

    quit = FALSE;

    hWin = CreateWindowEx(
        WS_EX_CLIENTEDGE, 
        WINDOW_CLASS, 
        WINDOW_NAME, 
        WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 
        CW_USEDEFAULT, 
        CW_USEDEFAULT, 
        WINDOW_SIZE, 
        NULL_2X,
        hInst, 
        NULL
    );

    ShowWindow(hWin, cShow);

    message = MESSAGE_NEED_OPEN_DIABLO;
    InvalidateScreen();

    hProcessEntry.dwSize = sizeof(PROCESSENTRY32);

    while (!quit) {
        while (PeekMessage(&msg, hWin, ZERO_2X, PM_REMOVE)) { 
            DispatchMessage(&msg);
            TranslateMessage(&msg);
        }
        ciErr = checkDiabloProcess();
        UpdateWindow(hWin);

        if (ciErr != PROCESS_ERROR_SUCCESS && ciErr != PROCESS_ERROR_CANNONT_FIND) 
            printf("error %d\n", ciErr);
    }
    
    return ERROR_SUCCESS;
}

size_t getDiabloBaseAddress() {
    if (!diabloHandle) {
        SetLastError(PROCESS_ERROR_INVALID_HANDLE_VALUE);
        return QUIT_MESSAGE;
    }
    DWORD modulesSize = 1024 * sizeof(HMODULE);

    HMODULE *modules = malloc(modulesSize);

    DWORD realSize;
    BOOL enumResult;
    
    enumResult = K32EnumProcessModules(diabloHandle, modules, modulesSize, &realSize);
    if (!enumResult) return QUIT_MESSAGE;

    uint32_t modulesCount, i;
    modulesCount = realSize / sizeof(HMODULE);

    char *moduleName = malloc(MAX_PATH);

    for (i = 0; i < modulesCount; i++) {
        HMODULE module = modules[i];

        BOOL nameGetResult;

        nameGetResult = GetModuleFileNameEx(diabloHandle, module, moduleName, MAX_PATH);
        if (!nameGetResult) return QUIT_MESSAGE;

        if (strstr(moduleName, DIABLO_PROCESS_NAME) == NULL)
            return (uint_fast64_t)module;
    }

    free(moduleName);
    free(modules);

    return PROCESS_ERROR_CANNONT_FIND;
}

process_error_t checkDiabloProcess() {
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, ZERO);
    if (hProcessSnap == INVALID_HANDLE_VALUE) return PROCESS_ERROR_INVALID_HANDLE_VALUE;

    if (!Process32First(hProcessSnap, &hProcessEntry)) {
        CloseHandle(hProcessSnap);
        return PROCESS_ERROR_EXIT_UNSUCESSFUL;
    }

    do {
        if (!strcmp(DIABLO_PROCESS_NAME, hProcessEntry.szExeFile)) {
            diabloProcessActive = TRUE;
            if (diabloProcessId == ZERO) {
                diabloProcessId = hProcessEntry.th32ProcessID;
                message = QUIT_MESSAGE;
                InvalidateScreen();

                diabloHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, diabloProcessId);
                diabloBaseAddress = getDiabloBaseAddress();

                if (diabloBaseAddress == QUIT_MESSAGE) return GetLastError();
            }
            if (diabloProcessActive) return PROCESS_ERROR_SUCCESS;
        }
    } while (Process32Next(hProcessSnap, &hProcessEntry));

    diabloProcessActive = FALSE;
    if (diabloProcessId != ZERO) { 
        diabloProcessId = ZERO;
        message = MESSAGE_NEED_OPEN_DIABLO;
        InvalidateScreen();
    }

    return PROCESS_ERROR_CANNONT_FIND;
}

void obfuscateHealthCount(DWORD*);

error_status_t setHealth(DWORD count) {
    if (!diabloBaseAddress) return PROCESS_ERROR_EXIT_UNSUCESSFUL;

    obfuscateHealthCount(&count);

    WriteProcessMemory(
        diabloHandle, 
        (LPVOID)(diabloBaseAddress + DIABLO_HEALTH_ADDRESS_OFFSET), 
        (LPCVOID)&count,
        sizeof(DWORD),
        EXTRA_BYTES
    );

    message = QUIT_MESSAGE;
    InvalidateScreen();
}

void obfuscateHealthCount(DWORD *countPtr) {
    double count = (double)*(countPtr);
    *(countPtr) = (DWORD)(63.75 * count + 54.45);
}
