#include <iostream>
#include <vector>
#include <map>
#include <windows.h>
#include <windowsx.h>
#include <winuser.h>
#include <stdio.h>

using namespace std;

#define HK_CTRL_F1 998
#define HK_SHIFT_F1 999
#define HK_F5 1000
#define HK_F6 1001
#define HK_F7 1002
#define HK_CTRL_U 1003
#define HK_SHIFT_F7 1004
#define HK_CTRL_I 1005
#define HK_CTRL_SHIFT_I 1006
#define HK_CTRL_P 1007
#define HK_ALT_I 1008
#define HK_ALT_C 1009

HWND window;
vector<HWND> hwnds;
vector<string> names {
            "Tyrans",
            "Raptus",
            "Calphayus",
            "Nihilus",
            "Malora",
            "Acina",
            "Arctis",
            "Decimus",
        };
int currentWnd = 0;

int getPositionOf(HWND hwnd, vector<HWND> array) {
    for (int i=0; i<array.size(); i++) {
        if (array[i] == hwnd) {
            return i;
        }
    }
    return -1;
}

bool startWith(char str[], char starter[])
{
    unsigned str_len = (unsigned) strlen(str);
    unsigned str_starter = (unsigned) strlen(starter);

    if (str_len < str_starter) {
        return false;
    }

    // cout << "not yet returned" << endl;
    // cout << "str: " << str << endl;
    // cout << "starter: " << starter << endl;

    for (int i=0; i < str_starter; i++) {
        // bool test = str[i] != starter[i];
        // cout << "i=" << i << " " << str[i] << " != " << starter[i] << " ? " << test << endl;
        if (str[i] != starter[i]) {
            return false;
        }
    }

    return true;
}

bool contains(char str[], char needle[])
{
    unsigned str_len = (unsigned) strlen(str);
    unsigned needle_len = (unsigned) strlen(needle);

    if (str_len < needle_len) {
        return false;
    }

    unsigned max_indice = str_len - needle_len;

    if (max_indice == 0) {
        return startWith(str, needle);
    }

    for (int i=0; i<max_indice; i++) {
        // cout << "str+" << i << " " << *(str+i) << endl;
        if (startWith(str+i, needle)) {
            return true;
        }
    }
    return false;
}

bool wndTitleContainsDofusNames(HWND hWnd) {
    char wndTitle[255];
    GetWindowText(hWnd, (LPSTR) wndTitle, 254);

    // convert wndTitle into string
    string title(wndTitle);

    bool res = false;
    for (auto name : names) {
        if (title.find(name) != std::string::npos) {
        // if (title.contains(name)) {
            res = true;
        }
    }
    return res;
}

// window Title - window class
// [Retro Classic 32Bits] Dofus Retro - ShockwaveFlash
// [Retro Remastered 64Bits] Dofus Retro v1.34.6 - Chrome_WingetWin_1
// [2.0 64bits] Aimed-Shot - Dofus 2.58.6.10 - ApolloRuntimeContentWindow
bool isDofusR(HWND hWnd) {
    char wndTitle[255];
    GetWindowText(hWnd, (LPSTR) wndTitle, 254);

    char wndClass[255];
    GetClassName(hWnd, (LPSTR) wndClass, 254);

    char dofusWndTitle[255];
    strcpy(dofusWndTitle, "Dofus Retro");

    char dofusWnd32Classname[255]; // 32 bits
    strcpy(dofusWnd32Classname, "ShockwaveFlash");
    
    char dofusWnd64classname[255] = "Chrome_WidgetWin_1";

    bool titleCheck = contains(wndTitle, dofusWndTitle);
    bool classCheck32 = strcmp(dofusWnd32Classname, wndClass) == 0;
    bool classCheck64 = strcmp(dofusWnd64classname, wndClass) == 0;

    if (
        (titleCheck and (classCheck32 or classCheck64)) // fenetre dofus native
        or wndTitleContainsDofusNames(hWnd)             // fenetre renomme par mes soins
    ) {
        return true;
    }

    return false;
}

bool isDofus2(HWND hWnd) {
    char wndTitle[255];
    GetWindowText(hWnd, (LPSTR) wndTitle, 254);

    char wndClass[255];
    GetClassName(hWnd, (LPSTR) wndClass, 254);

    char dofusWndTitle[255];
    strcpy(dofusWndTitle, "Dofus 2.");

    char dofusWndClassname[255];
    strcpy(dofusWndClassname, "ApolloRuntimeContentWindow");
    
    bool titleCheck = contains(wndTitle, dofusWndTitle);
    bool classCheck = strcmp(dofusWndClassname, wndClass) == 0;
    
    if (
        (titleCheck and classCheck)         // fenetre dofus native
        or wndTitleContainsDofusNames(hWnd) // fenetre renomme par mes soins
    ) {
        return true;
    }

    return false;
}

bool isDofusWnd(HWND hWnd) {
    if (
        isDofusR(hWnd)
        or isDofus2(hWnd)
    ) {
        return true;
    }

    return false;
}

void printInfoOf(HWND hWnd) {
    char wndtitle[255];
    char classname[255];
    GetWindowText(hWnd, (LPSTR) wndtitle, 254);
    GetClassName(hWnd, (LPSTR) classname, 254);
    printf("%s | %s\n", wndtitle, classname);
}

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam) {
    if (
        IsWindowVisible(hWnd)
        and isDofusWnd(hWnd)
        and IsIconic(hWnd) == false
    ) {      
        hwnds.emplace_back(hWnd);
        // printInfoOf(hWnd);
    }

    return TRUE;
}

BOOL CALLBACK EnumChildProcInfos(HWND hWnd, LPARAM lParam) {
    printInfoOf(hWnd);
    DWORD pid;
    GetWindowThreadProcessId(hWnd, &pid);
    cout << "pid: " << pid << endl;

    return TRUE;
}

BOOL CALLBACK EnumChildSendMsgEsc(HWND hWnd, LPARAM lParam)
{
    DWORD pid;
    GetWindowThreadProcessId(hWnd, &pid);
    cout << "send ESC to " << pid << endl;
    SendMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
    SendMessage(hWnd, WM_KEYUP  , VK_ESCAPE, 0);

    return TRUE;
}       

void registerHotKey(pair<int,int> p, u_int fsModifiers) {
    // cout << "A: " << dec << p.first << " " << hex << p.second << endl;
    if (RegisterHotKey(
            NULL,
            p.first,
            fsModifiers,
            p.second)
        )
    {
        cout << "Hotkey "<< hex << p.second <<" registered with id "<< dec << p.first << ", using "<< hex << fsModifiers <<" flag" << endl;
    } else {
        cout << "Hotkey "<< hex << p.second <<" faild to register ErrorCode: " << GetLastError() << endl;
    }
}

void unregisterHotKey() {
    UnregisterHotKey(NULL, HK_CTRL_F1);
    UnregisterHotKey(NULL, HK_SHIFT_F1);
    UnregisterHotKey(NULL, HK_F5);
    UnregisterHotKey(NULL, HK_F6);

    UnregisterHotKey(NULL, HK_F7);
    UnregisterHotKey(NULL, HK_CTRL_U);
    UnregisterHotKey(NULL, HK_SHIFT_F7);
    UnregisterHotKey(NULL, HK_CTRL_I);
    
    UnregisterHotKey(NULL, HK_CTRL_SHIFT_I);
    UnregisterHotKey(NULL, HK_ALT_I);
    UnregisterHotKey(NULL, HK_ALT_C);
}

void registerHotKeys() {
    // Virtual key codes docs
    // https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
    
    map <int, int> hotkeys;
    hotkeys[HK_CTRL_F1] = VK_F1;
    hotkeys[HK_SHIFT_F1] = VK_F1;
    hotkeys[HK_F5] = VK_F5;
    hotkeys[HK_F6] = VK_F6;
    hotkeys[HK_F7] = VK_F7;
    hotkeys[HK_CTRL_U] = 0x4F;
    hotkeys[HK_SHIFT_F7] = VK_F7;
    hotkeys[HK_CTRL_I] = 0x49; // i
    hotkeys[HK_CTRL_SHIFT_I] = 0x49; // i
    hotkeys[HK_CTRL_P] = 0x50; // p
    hotkeys[HK_ALT_I] = 0x49; // alt + i
    hotkeys[HK_ALT_C] = 0x43; // alt + c

    // param
    // alt : MOD_ALT 0x0001
    // ctrl : MOD_CONTROL 0x0002 
    // shift : MOD_SHIFT 0x0004
    // win : MOD_WIN 0x0008
    // norepeat : MOD_NOREPEAT 0x4000
    map <int, int> hk_param;
    hk_param[HK_CTRL_F1] = 0x4000 + MOD_CONTROL;
    hk_param[HK_SHIFT_F1] = 0x4000 + MOD_SHIFT;
    hk_param[HK_F5] = 0x4000;
    hk_param[HK_F6] = 0x4000;
    hk_param[HK_F7] = 0x4000;
    hk_param[HK_CTRL_U] = 0x4000 + MOD_CONTROL;
    hk_param[HK_SHIFT_F7] = 0x4000 + MOD_SHIFT;
    hk_param[HK_CTRL_I] = 0x4000 + MOD_CONTROL;
    hk_param[HK_CTRL_SHIFT_I] = 0x4000 + MOD_CONTROL + MOD_SHIFT;
    hk_param[HK_CTRL_P] = 0x4000 + MOD_CONTROL;
    hk_param[HK_ALT_I] = 0x4000 + MOD_ALT;
    hk_param[HK_ALT_C] = 0x4000 + MOD_ALT;

    for (auto hotkey : hotkeys) {
        // cout << "B: " << dec << hotkey.first << " " << hex << hotkey.second << endl;
        registerHotKey(hotkey, hk_param[hotkey.first]);
    }
    cout << "Keybinds registered successfuly." << endl;
}

int getPreviousPos() {
    int res = currentWnd - 1;
    
    if (res < 0) {
        res += hwnds.size();
    }

    currentWnd = res;
    return res;
}

int getNextPos() {
    int res = currentWnd + 1;
    
    if (res >= hwnds.size()) {
        res -= hwnds.size();
    }

    currentWnd = res;
    return res;
}

LPARAM BuildLParam(u_short low, u_short high)
{
    return (LPARAM)(((u_int)high << 16) | (u_int)low);
}

void bringToForeground(HWND hwnd) {
    if (IsIconic(hwnd)) {
        ShowWindowAsync(hwnd, SW_RESTORE);
    } else {
        ShowWindowAsync(hwnd, SW_SHOW);
    }
        
    SetForegroundWindow(hwnd);
    SetActiveWindow(hwnd);
    SetFocus(hwnd);
    BringWindowToTop(hwnd);

    POINT p;
    GetCursorPos(&p);
    SendMessage(hwnd, WM_MOUSEMOVE, 0, BuildLParam(p.x, p.y));
}

void Keyboard(WPARAM param) {
    switch (param)
    {
    case HK_SHIFT_F1:
    {
        // Press F1 in all tabs
        for(auto hwnd : hwnds){
            SendMessage(hwnd, WM_KEYDOWN, VK_F1, 0);
            SendMessage(hwnd, WM_KEYUP  , VK_F1, 0);
        }

        cout << "F1 pressed in all dofus window." << endl;
        break;
    }
    case HK_F5:
    {
        // Afficher la fenetre precedente dofus
        getPreviousPos();

        // Handle the case where the dofus was minimized
        bringToForeground(hwnds[currentWnd]);

        cout << "Window " << dec << currentWnd << hex << " (" << hwnds[currentWnd] << ")" <<  " put in foreground" << endl;
        // cout << "show previous dofus" << endl;
        break;
    }

    case HK_F6:
    {
        // afficher la fenetre suivante dofus
        getNextPos();

        // Handle the case where the dofus was minimized
        bringToForeground(hwnds[currentWnd]);

        cout << "Window " << dec << currentWnd << hex << " (" << hwnds[currentWnd] << ")" <<  " put in foreground" << endl;
        // cout << "show next dofus" << endl;
        break;
    }

    case HK_F7:
    {
        for(auto hwnd : hwnds){
            // SetForegroundWindow(hwnd); // ok
            // SetActiveWindow(hwnd); // nope
            // SetFocus(hwnd); // nope
            // BringWindowToTop(hwnd); // nope
            SendMessage(hwnd, WM_KEYDOWN, VK_ESCAPE, 0);
            SendMessage(hwnd, WM_KEYUP  , VK_ESCAPE, 0);
        }
        // SetForegroundWindow(hwnds[currentWnd]);

        cout << "ESCAPE pressed in all dofus window." << endl;
        break;
    }

    case HK_SHIFT_F7:
    {
        // Maximize all tabs
        for(auto hwnd : hwnds) {
            ShowWindow(hwnd, SW_MAXIMIZE);
        }

        cout << "All windows maximized" << endl;
        break;
    }

    case HK_CTRL_U:
    {
        /* Ordre initial :
         * Tyrans  Raptus  Calphayus Nihilus
         * Malora  Acina   Arctis    Decimus
         *
         * Ordre transformer abra :
         * Malora  Raptus  Acina     Calphayus
         * Decimus Nihilus Arctis    Tyrans
         */
        vector<HWND> tmp(hwnds);
        
        hwnds[0] = tmp[4];
        hwnds[1] = tmp[1];
        hwnds[2] = tmp[5];
        hwnds[3] = tmp[2];
        hwnds[4] = tmp[7];
        hwnds[5] = tmp[3];
        hwnds[6] = tmp[6];
        hwnds[7] = tmp[0];

        cout << "CTRL-U PRESSED" << endl;
        break;
    }

    case HK_CTRL_SHIFT_I:
    {
        vector<string> names {
            "Tyrans",
            "Raptus",
            "Calphayus",
            "Nihilus",
            "Malora",
            "Acina",
            "Arctis",
            "Decimus",
        };

        // const_cast<char *>(str.c_str())

        for (int i=0; i<hwnds.size(); i++) {
            SetWindowText(hwnds[i], const_cast<char *>(names[i].c_str()));
        }

        cout << "CTRL-SHIFT-I PRESSED" << endl;
        break;
    }

    case HK_CTRL_I:
    {
        // Refresh hwnds
        hwnds.clear();
        EnumWindows(EnumWindowsProc, 0);
        cout << "Liste des fenetres mis à jours." << endl;
        for (int i=0; i<hwnds.size(); i++) {
            char wndtitle[255];
            char classname[255];
            GetWindowText(hwnds[i], (LPSTR) wndtitle, 254);
            GetClassName(hwnds[i], (LPSTR) classname, 254);
            printf("%s - %s\n", wndtitle, classname);
        }

        cout << "CTRL-I PRESSED" << endl;
        break;
    }
        
    case HK_CTRL_P:
    {
        if (IsWindowVisible(window) == 1) {
            ShowWindow(window, 0);
            unregisterHotKey();
            cout << "Console cachée" << endl;
        } else {
            ShowWindow(window, 1);
            registerHotKeys();
            SetForegroundWindow(window);
            cout << "Console affichée" << endl;
        }
        break;
    }

    case HK_ALT_I:
    {
        for(auto hwnd : hwnds){
            SendMessage(hwnd, WM_KEYDOWN, 0x49, 0);
            SendMessage(hwnd, WM_KEYUP  , 0x49, 0);
        }

        cout << "I pressed in all dofus window." << endl;
        break;
    }

    case HK_ALT_C:
    {
        for(auto hwnd : hwnds){
            SendMessage(hwnd, WM_KEYDOWN, 0x43, 0);
            SendMessage(hwnd, WM_KEYUP  , 0x43, 0);
        }

        cout << "C pressed in all dofus window." << endl;
        break;
    }

    case HK_CTRL_F1:
    {
        DWORD pos = GetMessagePos();

        POINT screenPoint;
        screenPoint.x = GET_X_LPARAM(pos);
        screenPoint.y = GET_Y_LPARAM(pos);

        HWND hWnd = WindowFromPoint(screenPoint);

        POINT wndPoint;
        wndPoint.x = screenPoint.x;
        wndPoint.y = screenPoint.y;
        ScreenToClient(hWnd, &wndPoint);
        
        POINT p;
        p.x = wndPoint.x;
        p.y = wndPoint.y;
        ClientToScreen(hWnd, &p);

        // cout << "WindowFromPoint: " << hWnd << " in position " << getPositionOf(hWnd, hwnds) << endl;
        // cout << dec << "Screen coords : (" << screenPoint.x << ";" << screenPoint.y << ")" << endl;
        // cout << dec << "Client coords : (" << wndPoint.x << ";" << wndPoint.y << ")" << endl;
        // cout << dec << "Screen (from client) coords : (" << p.x << ";" << p.y << ")" << endl;

        INPUT inputs[3];
        ZeroMemory(inputs, sizeof(inputs));

        inputs[0].type = INPUT_MOUSE;
        inputs[0].mi.dwFlags = MOUSEEVENTF_MOVE;

        inputs[1].type = INPUT_MOUSE;
        inputs[1].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;

        inputs[2].type = INPUT_MOUSE;
        inputs[2].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP;

        for(int i=currentWnd; i<currentWnd + hwnds.size(); i++) {
            HWND hwnd;
            if (i >= hwnds.size()) {
                hwnd = hwnds[i - hwnds.size()];
            } else {
                hwnd = hwnds[i];
            }
             
            cout << "Clicking Window " << dec << getPositionOf(hwnd, hwnds) << hex << " (" << hwnds[i] << ")" <<  " @ (" << dec << wndPoint.x << ";" << wndPoint.y << ") - ";
            printInfoOf(hwnd);

            if (isDofusR(hwnd)) {
                bringToForeground(hwnd);
                while (GetForegroundWindow() != hwnd) {
                    Sleep(15);
                }
                Sleep(10);
                if (i%2 == 0) {
                    inputs[0].mi.dx = 5; 
                } else {
                    inputs[0].mi.dx = -5; 
                }

                inputs[0].mi.dy = 0;
                inputs[1].mi.dx = screenPoint.x; 
                inputs[1].mi.dy = screenPoint.y;
                inputs[2].mi.dx = screenPoint.x; 
                inputs[2].mi.dy = screenPoint.y;
                SendInput(3, inputs, sizeof(INPUT));
                Sleep(100);
            } else {
                SendMessage(hwnd, WM_LBUTTONDOWN, 0x0001, BuildLParam(wndPoint.x, wndPoint.y));
                SendMessage(hwnd, WM_LBUTTONUP  , 0     , BuildLParam(wndPoint.x, wndPoint.y));
            }
        }
        bringToForeground(hwnds[currentWnd]);
        break;
    }

    default:
        cout << "default: " << param << endl; 
        break;
    }
}

int main () {
    // Set console encoding to UTF-8
    SetConsoleOutputCP(65001);

    // Création de la console et stockage de son HandleWindow
    AllocConsole();
    window = FindWindowA("ConsoleWindowClass", NULL);

    // Print start up message
    cout << "Hello there ! \n © Elouan GOUINGUENET" << endl;
    
    // Scan all visible window and initialize hwnds
    EnumWindows(EnumWindowsProc, 0);

    // Print detected window
    for (int i=0; i<hwnds.size(); i++) {
        char wndtitle[255];
        char classname[255];
        GetWindowText(hwnds[i], (LPSTR) wndtitle, 254);
        GetClassName(hwnds[i], (LPSTR) classname, 254);
        printf("%s - %s\n", wndtitle, classname);
    }

    cout << "-----" << endl;

    // for(auto hwnd : hwnds){
    //     printInfoOf(hwnd);
    //     EnumChildWindows(hwnd, EnumChildProcInfos, 0);
    // }
    
    // Register the hotkey
    registerHotKeys();

    char needleR [100] = "Dofus Retro";
    char needleR_false [100] = "Dofus Ratro";
    char needle2 [100] = "Dofus 2.";
    char test_1 [100] = "Tyrans - Dofus Retro 1.34.6";
    char test_2 [100] = "Raptus - Dofus Retro 1.34.6";
    char test_3 [100] = "Dofus Retro";
    char test_4 [100] = "Cheap-Shot - Dofus 2.58.7.13";

    // for (int i=0; i<strlen(needleR); i++) {
    //     cout << "str+" << i << " = " << *(needleR+i) << endl;
    //     char* cpy = needleR + i;
    //     cout << "cpy: " << cpy << endl;
    // }

    // cout << test_1 << " contains " << needleR << " " << contains(test_1, needleR) << endl;
    // cout << test_1 << " contains " << needleR_false << " " << contains(test_1, needleR_false) << endl;
    // cout << test_2 << " contains " << needleR << " " << contains(test_2, needleR) << endl;
    // cout << test_2 << " contains " << needle2 << " " << contains(test_2, needle2) << endl;
    // cout << test_2 << " contains " << needleR_false << " " << contains(test_2, needleR_false) << endl;
    // cout << test_3 << " contains " << needleR << " " << contains(test_3, needleR) << endl;
    // cout << test_3 << " contains " << needleR_false << " " << contains(test_3, needleR_false) << endl;
    // cout << test_4 << " contains " << needle2 << " " << contains(test_4, needle2) << endl;
    // cout << test_4 << " contains " << needleR << " " << contains(test_4, needleR) << endl;


    // Start listen to MSG
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        if (msg.message == WM_HOTKEY ) //and isDofusWnd(foregroundWnd)
        {
            // cout << "Message received:\n" << "  " << msg.lParam << "\n  " << msg.wParam << endl;
            Keyboard(msg.wParam);
        }
    }

    return 0;
}