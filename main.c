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
HWND updateButton;

HWND healthBox;;
HWND strengthBox;
HWND dexterityBox;
HWND vitalityBox;

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
LPSTR strengthBoxText;
LPSTR dexterityBoxText;
LPSTR vitalityBoxText;

SHORT message;
SHORT activatedButton;

#pragma endregion

#define InvalidateScreen() InvalidateRect(hWin, NULL, FALSE)
#define WRITE_MEMORY(offset, ptr, size) WriteProcessMemory(diabloHandle, (LPVOID)(diabloBaseAddress + offset), ptr, size, EXTRA_BYTES)

process_error_t checkDiabloProcess();

void obfuscateHealthCount(DWORD*);

void rewriteHealth();
void rewriteStrength();
void rewriteDexterity();
void rewriteVitality();

void drawTextA(LPSTR text, UINT16 x, UINT16 y);

LPSTR checkBoxText(HWND *box);

LRESULT CALLBACK WinProc(HWND hWin, UINT msg, WPARAM wPar, LPARAM lPar) {

    switch (msg) {
        case WM_DESTROY:

            quit = TRUE;
            PostQuitMessage(QUIT_MESSAGE);
        break;
        case WM_COMMAND:
            
            switch (LOWORD(wPar)) {
                case BUTTON_ACTIVATED:
                    if (!diabloBaseAddress) break;

                    rewriteHealth();
                    rewriteStrength();
                    rewriteDexterity();
                    rewriteVitality();
                break;
                case BOX_1_ACTIVATED: 
                    if (healthBoxText) free(healthBoxText);
                    healthBoxText = checkBoxText(&healthBox);
                break;
                case BOX_2_ACTIVATED: 
                    if (strengthBoxText) free(strengthBoxText);
                    strengthBoxText = checkBoxText(&strengthBox);
                break;
                case BOX_3_ACTIVATED:
                    if (dexterityBoxText) free(dexterityBoxText);
                    dexterityBoxText = checkBoxText(&dexterityBox);
                break;
                case BOX_4_ACTIVATED:
                    if (vitalityBoxText) free(vitalityBoxText);
                    vitalityBoxText = checkBoxText(&vitalityBox);
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
                    case MESSAGE_WRONG_STRENGTH:   messageText = WRONG_STRENGTH;
                    break;
                    case MESSAGE_WRONG_DEXTERITY:  messageText = WRONG_DEXTERITY;
                    break;
                    case MESSAGE_WRONG_VITALITY:   messageText = WRONG_VITALITY;
                    break;
                }

                DrawText(decal, messageText, UNKNOWN_SIZE, &rect, DT_BOTTOM | DT_SINGLELINE);
            }

            drawTextA(BOX_1_NAME, BOX_1_X_POSITION + BOX_WIDTH + BOX_X_OFFSET, BOX_1_Y_POSITION);
            drawTextA(BOX_2_NAME, BOX_2_X_POSITION + BOX_WIDTH + BOX_X_OFFSET, BOX_2_Y_POSITION);
            drawTextA(BOX_3_NAME, BOX_3_X_POSITION + BOX_WIDTH + BOX_X_OFFSET, BOX_3_Y_POSITION);
            drawTextA(BOX_4_NAME, BOX_4_X_POSITION + BOX_WIDTH + BOX_X_OFFSET, BOX_4_Y_POSITION);

            EndPaint(hWin, &paint);
        break;
        case WM_CREATE:

            HINSTANCE windowPtr = (HINSTANCE)GetWindowLongPtr(hWin, GWLP_HINSTANCE);

            #pragma region init windows

            updateButton = CreateWindowEx(
                BUTTON_STYLE, BUTTON_CLASS, BUTTON_NAME, BUTTON_EX_STYLE, BUTTON_POSITION, BUTTON_SIZE, hWin, (HMENU)BUTTON_ACTIVATED, windowPtr, NULL
            );

            healthBox = CreateWindowEx(
                WS_EX_CLIENTEDGE, BOX_CLASS, VOID_STRING, BOX_EX_STYLE, BOX_1_POSITION, BOX_SIZE, hWin, (HMENU)BOX_1_ACTIVATED, NULL_2X
            );
            strengthBox = CreateWindowEx(
                WS_EX_CLIENTEDGE, BOX_CLASS, VOID_STRING, BOX_EX_STYLE, BOX_2_POSITION, BOX_SIZE, hWin, (HMENU)BOX_2_ACTIVATED, NULL_2X
            );
            dexterityBox = CreateWindowEx(
                WS_EX_CLIENTEDGE, BOX_CLASS, VOID_STRING, BOX_EX_STYLE, BOX_3_POSITION, BOX_SIZE, hWin, (HMENU)BOX_3_ACTIVATED, NULL_2X
            );
            vitalityBox = CreateWindowEx(
                WS_EX_CLIENTEDGE, BOX_CLASS, VOID_STRING, BOX_EX_STYLE, BOX_4_POSITION, BOX_SIZE, hWin, (HMENU)BOX_4_ACTIVATED, NULL_2X
            );

            #pragma endregion
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
    DWORD modulesSize = DIABLO_MODULES_COUNT * sizeof(HMODULE);

    HMODULE *modules = malloc(modulesSize);

    DWORD realSize;
    BOOL enumResult;
    
    enumResult = K32EnumProcessModules(diabloHandle, modules, modulesSize, &realSize);
    if (!enumResult) return QUIT_MESSAGE;

    uint32_t i;

    for (i = 0; i < DIABLO_MODULES_COUNT; i++) {
        HMODULE module = modules[i];

        char moduleName[MAX_PATH];

        BOOL nameGetResult;

        nameGetResult = GetModuleFileNameEx(diabloHandle, module, moduleName, MAX_PATH);
        if (!nameGetResult) return QUIT_MESSAGE;

        if (strstr(moduleName, DIABLO_PROCESS_NAME)) {
            free(modules);
            return (uint_fast64_t)module;
        }
    }

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

void drawTextA(LPSTR text, UINT16 x, UINT16 y) {
    for (LPCSTR c = text; *c; c++) {
        TextOutA(decal, x, y, c, 1);

        LPSIZE charSize = (LPSIZE)malloc(sizeof(SIZE));
        GetTextExtentPoint32A(decal, c, 1, charSize);

        x += charSize->cx;
        free(charSize);
    }
}

void rewriteHealth() {
    DWORD healthCount;

    healthCount = atoi(healthBoxText);

    if (!healthCount) {
        message = MESSAGE_WRONG_HEALTH;
        InvalidateScreen();
        return;
    }

    obfuscateHealthCount(&healthCount);

    WRITE_MEMORY(DIABLO_HEALTH_ADDRESS_OFFSET,     &healthCount, sizeof(DWORD));
    WRITE_MEMORY(DIABLO_MAX_HEALTH_ADDRESS_OFFSET, &healthCount, sizeof(DWORD));

    message = QUIT_MESSAGE;
    InvalidateScreen();
}
void rewriteStrength() {
    DWORD strengthCount;

    strengthCount = atoi(strengthBoxText);
    if (!strengthCount) {
        message = MESSAGE_WRONG_STRENGTH;
        InvalidateScreen();
        return;
    }

    WRITE_MEMORY(DIABLO_STRENGTH_ADDRESS_OFFSET,     &strengthCount, sizeof(DWORD));
    WRITE_MEMORY(DIABLO_STRENGTH_CMP_ADDRESS_OFFSET, &strengthCount, sizeof(DWORD));

    message = QUIT_MESSAGE;
    InvalidateScreen();
}
void rewriteDexterity() {
    DWORD dexterityCount;

    dexterityCount = atoi(dexterityBoxText);
    if (!dexterityCount) {
        message = MESSAGE_WRONG_DEXTERITY;
        InvalidateScreen();
        return;
    }

    WRITE_MEMORY(DIABLO_DEXTERITY_ADDRESS_OFFSET,     &dexterityCount, sizeof(DWORD));
    WRITE_MEMORY(DIABLO_DEXTERITY_CMP_ADDRESS_OFFSET, &dexterityCount, sizeof(DWORD));

    message = QUIT_MESSAGE;
    InvalidateScreen();
}
void rewriteVitality() {
    DWORD vitalityCount;

    vitalityCount = atoi(vitalityBoxText);
    if (!vitalityCount) {
        message = MESSAGE_WRONG_VITALITY;
        InvalidateScreen();
        return;
    }

    WRITE_MEMORY(DIABLO_VITALITY_ADDRESS_OFFSET,     &vitalityCount, sizeof(DWORD));
    WRITE_MEMORY(DIABLO_VITALITY_CMP_ADDRESS_OFFSET, &vitalityCount, sizeof(DWORD));

    message = QUIT_MESSAGE;
    InvalidateScreen();
}

LPSTR checkBoxText(HWND *box) {
    INT textLength;
    LPSTR boxText;

    textLength = GetWindowTextLength(*box);

    boxText = (LPSTR)malloc(++textLength);

    GetWindowTextA(*box, boxText, textLength);

    return boxText;
}

void obfuscateHealthCount(DWORD *countPtr) {
    double count = (double)*countPtr;
    *countPtr = (DWORD)(63.75 * count + 54.45);
}
