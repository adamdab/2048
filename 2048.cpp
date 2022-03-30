// 2048.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "2048.h"
#include <time.h>
#include <fstream>

#define MAX_LOADSTRING 100
#define BOX_SIZE 60
#define GAP 10
#define COLOUR_OF_2 RGB(238, 228, 198)
#define COLOUR_OF_4 RGB(239, 225, 218)
#define COLOUR_OF_8 RGB(243, 179, 124)
#define COLOUR_OF_16 RGB(246, 153, 100)
#define COLOUR_OF_32 RGB(246, 125, 98)
#define COLOUR_OF_64 RGB(247, 93, 60)
#define COLOUR_OF_128 RGB(237, 206, 116)
#define COLOUR_OF_256 RGB(239, 204, 98)
#define COLOUR_OF_512 RGB(243, 201, 85)
#define COLOUR_OF_1024 RGB(238, 200, 72)
#define COLOUR_OF_2048 RGB(239, 192, 47)

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


ATOM                PlayBoxRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK    PlayBoxWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
ATOM                MyRegisterClass2(HINSTANCE hInstance);
LRESULT CALLBACK    WndProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
// global window handles
HWND hWndMain1, hWndMain2;
HWND hBoxWMain1[4][4];
HWND hBoxWMain2[4][4];
HWND hScoreMain1, hScoreMain2;
// global screen stats
int xScreenMiddle = GetSystemMetrics(SM_CXSCREEN)/2;
int yScreenMiddle = GetSystemMetrics(SM_CYSCREEN)/2;
int GOAL = 2048;
int tab[4][4];
int score = 0;
// functions
//inspired by code from https://code-projects.org/2048-game-in-c-with-source-code/
void addblock(int a[4][4]);
int checkover(int a[4][4]);
void MoveUp();
void MoveDown();
void MoveLeft();
void MoveRight();
//fully own functions 
bool isFull();
bool Win();
void SaveGame();
void LoadGame();
void SetMenue();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
    for (int i = 0; i < 4; i++) // inicjalizacja tablicy
    {
        for (int j = 0; j < 4; j++) tab[i][j] = 0;
    }
    addblock(tab);
    LoadGame();
    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY2048, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    PlayBoxRegisterClass(hInstance);
    MyRegisterClass2(hInstance);
    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY2048));

    MSG msg;

    SetMenue();
    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    SaveGame();
    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(CreateSolidBrush(RGB(250, 247, 238)));
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY2048);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   RECT rc;
   rc.left = 0;
   rc.top = 0;
   rc.right = 4 * BOX_SIZE + 5 * GAP;
   rc.bottom = 5* BOX_SIZE + 8 * GAP;
   /*
   rc.bottom = 4 * BOX_SIZE + 5 * GAP;
   */
   AdjustWindowRectEx(&rc, WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN, false, 0);

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME|WS_CLIPCHILDREN,
      CW_USEDEFAULT, 0, rc.left-rc.right, rc.bottom-rc.left, nullptr, nullptr, hInstance, nullptr);
  

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   hWndMain1 = hWnd;

   hWndMain2= CreateWindowW(L"MainWindow2", szTitle, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME | WS_CLIPCHILDREN,
       CW_USEDEFAULT, 0, rc.left - rc.right, rc.bottom - rc.left, hWnd, nullptr, hInstance, nullptr);
   if (!hWndMain2)
   {
       return FALSE;
   }

   ShowWindow(hWndMain2, nCmdShow);
   UpdateWindow(hWndMain2);
   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT: //8
                GOAL = 8;
                CheckMenuItem(GetMenu(hWnd), IDM_ABOUT, MF_CHECKED);
                CheckMenuItem(GetMenu(hWnd), ID_GOAL_16, MF_UNCHECKED);
                CheckMenuItem(GetMenu(hWnd), ID_GOAL_64, MF_UNCHECKED);
                CheckMenuItem(GetMenu(hWnd), ID_GOAL_2048, MF_UNCHECKED);
                break;
            case ID_GOAL_16:
                GOAL = 16;
                CheckMenuItem(GetMenu(hWnd), IDM_ABOUT, MF_UNCHECKED);
                CheckMenuItem(GetMenu(hWnd), ID_GOAL_16, MF_CHECKED);
                CheckMenuItem(GetMenu(hWnd), ID_GOAL_64, MF_UNCHECKED);
                CheckMenuItem(GetMenu(hWnd), ID_GOAL_2048, MF_UNCHECKED);
                break;
            case ID_GOAL_64:
                GOAL = 64;
                CheckMenuItem(GetMenu(hWnd), IDM_ABOUT, MF_UNCHECKED);
                CheckMenuItem(GetMenu(hWnd), ID_GOAL_16, MF_UNCHECKED);
                CheckMenuItem(GetMenu(hWnd), ID_GOAL_64, MF_CHECKED);
                CheckMenuItem(GetMenu(hWnd), ID_GOAL_2048, MF_UNCHECKED);
                break;
            case ID_GOAL_2048:
                GOAL = 2048;
                CheckMenuItem(GetMenu(hWnd), IDM_ABOUT, MF_UNCHECKED);
                CheckMenuItem(GetMenu(hWnd), ID_GOAL_16, MF_UNCHECKED);
                CheckMenuItem(GetMenu(hWnd), ID_GOAL_64, MF_UNCHECKED);
                CheckMenuItem(GetMenu(hWnd), ID_GOAL_2048, MF_CHECKED);
                break;
            case IDM_EXIT: //NEW GAME
               
                for (int i = 0; i < 4; i++) // inicjalizacja tablicy
                {
                    for (int j = 0; j < 4; j++) tab[i][j] = 0;
                }
                addblock(tab);
                score = 0;
                for (int i = 0; i < 4; i++)
                {
                    for (int j = 0; j < 4; j++)
                    {
                        InvalidateRect(hBoxWMain1[i][j], NULL, NULL);
                        InvalidateRect(hBoxWMain2[i][j], NULL, NULL);
                    }
                }
                InvalidateRect(hScoreMain1, NULL, NULL);
                InvalidateRect(hScoreMain2, NULL, NULL);
                //MessageBox(hWnd, L"New game", L"NEW GAME", MB_OK);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_GETMINMAXINFO:
    {
        MINMAXINFO* minMaxInfo = (MINMAXINFO*)lParam;
        RECT rc;
        rc.left = 0;
        rc.top = 0;
        rc.right = 4 * BOX_SIZE + 5 * GAP;
        rc.bottom = 5 * BOX_SIZE + 8 * GAP;
        AdjustWindowRectEx(&rc, WS_OVERLAPPEDWINDOW, false, 0);
        minMaxInfo->ptMaxTrackSize.x = minMaxInfo->ptMinTrackSize.x = rc.right - rc.left;
       //minMaxInfo->ptMaxTrackSize.x = rc.right - rc.left;
        minMaxInfo->ptMaxTrackSize.y = minMaxInfo->ptMinTrackSize.y = rc.bottom - rc.top;
       // minMaxInfo->ptMaxTrackSize.y = rc.bottom - rc.top;
    }
    break;
    case WM_CREATE:
    {
        RECT rc;
        hScoreMain1 = CreateWindowExA(0, "PlayBox", NULL, WS_CHILD, GAP, GAP, 4 * BOX_SIZE + 3 * GAP, BOX_SIZE, hWnd, (HMENU)100, hInst, NULL);
        GetClientRect(hScoreMain1, &rc);
        HRGN hRgn = CreateRoundRectRgn(rc.left, rc.top, rc.right, rc.bottom, 20, 20);
        SetWindowRgn(hScoreMain1, hRgn, TRUE);
        ShowWindow(hScoreMain1, SW_SHOW);
        UpdateWindow(hScoreMain1);
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                hBoxWMain1[i][j]= CreateWindowExA(0, "PlayBox", NULL, WS_CHILD, i * BOX_SIZE + (i + 1) * GAP, (j+1) * BOX_SIZE + (j + 2) * GAP,
                    BOX_SIZE, BOX_SIZE, hWnd, (HMENU)100, hInst, NULL);
                GetClientRect(hBoxWMain1[i][j], &rc);
               hRgn=CreateRoundRectRgn(rc.left, rc.top, rc.right, rc.bottom, 20,20);
                SetWindowRgn(hBoxWMain1[i][j], hRgn, TRUE);
                ShowWindow(hBoxWMain1[i][j], SW_SHOW);
                UpdateWindow(hBoxWMain1[i][j]);
            }
        }
    }
    break;
    case WM_KEYDOWN:
    {
        if (checkover(tab) != 1 || Win())
        {
            break;
        }
        
        switch (wParam)
        {
        case 0x57: //W
        {
            //MessageBox(hWnd, L"Hello",L"HELLO",MB_OK);
            //upmove(tab);
            
            MoveUp();
            if (!isFull())
            {
                addblock(tab);
            }
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    InvalidateRect(hBoxWMain1[i][j], NULL, NULL);
                    InvalidateRect(hBoxWMain2[i][j], NULL, NULL);
                }
            }
            InvalidateRect(hScoreMain1, NULL, NULL);
            InvalidateRect(hScoreMain2, NULL, NULL);
        }
        break;
        case 0x53: //S
        {
            MoveDown();
            if (!isFull())
            {
                addblock(tab);
            }
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    InvalidateRect(hBoxWMain1[i][j], NULL, NULL);
                    InvalidateRect(hBoxWMain2[i][j], NULL, NULL);
                }
            }
            InvalidateRect(hScoreMain1, NULL, NULL);
            InvalidateRect(hScoreMain2, NULL, NULL);
        }
        break;
        case 0x41: //A
        {
            
            MoveLeft();
            if (!isFull())
            {
                addblock(tab);
            }
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    InvalidateRect(hBoxWMain1[i][j], NULL, NULL);
                    InvalidateRect(hBoxWMain2[i][j], NULL, NULL);
                }
            }
            InvalidateRect(hScoreMain1, NULL, NULL);
            InvalidateRect(hScoreMain2, NULL, NULL);
            //UpdateAllWindows();
        }
        break;
        case 0x44: //D
        {
            
            MoveRight();
            if (!isFull())
            {
                addblock(tab);
            }
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    InvalidateRect(hBoxWMain1[i][j], NULL, NULL);
                    InvalidateRect(hBoxWMain2[i][j], NULL, NULL);
                }
            }
            InvalidateRect(hScoreMain1, NULL, NULL);
            InvalidateRect(hScoreMain2, NULL, NULL);
        }
        break;

        default:
            break;
        }
    }
    break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_MOVE:
    {
        
        SendMessageW(hWndMain2, WM_MOVE, NULL, NULL);
        if (hWnd == GetActiveWindow())
        {
            RECT rc;
            GetWindowRect(hWnd, &rc);
            int changeX = xScreenMiddle - rc.right;
            int changeY = yScreenMiddle - rc.bottom;
            int width = rc.right - rc.left;
            int height = rc.bottom - rc.top;
            int xScreenMiddle = GetSystemMetrics(SM_CXSCREEN) / 2;
            int yScreenMiddle = GetSystemMetrics(SM_CYSCREEN) / 2;
            MoveWindow(hWndMain2, xScreenMiddle + changeX, yScreenMiddle + changeY, width, height, TRUE);
        }
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

ATOM PlayBoxRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    //wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    //wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    //wcex.hbrBackground = CreateSolidBrush(RGB(250, 247, 238));//(HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName = NULL;
    //wcex.lpszClassName = szWindowClass;
    //wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));


    wcex.hbrBackground = CreateSolidBrush(RGB(204, 192, 174));
    wcex.lpszClassName = L"PlayBox";
    wcex.hIcon = NULL;
    wcex.hIconSm = NULL;
    wcex.lpfnWndProc = PlayBoxWndProc;

    return RegisterClassExW(&wcex);

}

LRESULT CALLBACK PlayBoxWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        RECT rc;
        HDC hdc = BeginPaint(hWnd, &ps);
        SetBkMode(hdc, TRANSPARENT);
        int ii=-1, jj=-1, which=-1;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (hBoxWMain1[i][j] == hWnd)
                {
                    ii = i;
                    jj = j;
                    which = 1;
                }
                if (hBoxWMain2[i][j] == hWnd)
                {
                    ii = i;
                    jj = j;
                    which = 2;
                }
            }
        }
        if (ii>=0 && jj>=0)
        {
            HBRUSH hBrush;
            GetClientRect(hWnd, &rc);
            switch (tab[ii][jj])
            {
            case 2:
                hBrush = CreateSolidBrush(COLOUR_OF_2);
                break;
            case 4:
                hBrush = CreateSolidBrush(COLOUR_OF_4);
                break;
            case 8:
                hBrush = CreateSolidBrush(COLOUR_OF_8);
                break;
            case 16:
                hBrush = CreateSolidBrush(COLOUR_OF_16);
                break;
            case 32:
                hBrush = CreateSolidBrush(COLOUR_OF_32);
                break;
            case 64:
                hBrush = CreateSolidBrush(COLOUR_OF_64);
                break;
            case 128:
                hBrush = CreateSolidBrush(COLOUR_OF_128);
                break;
            case 256:
                hBrush = CreateSolidBrush(COLOUR_OF_256);
                break;
            case 512:
                hBrush = CreateSolidBrush(COLOUR_OF_512);
                break;
            case 1024:
                hBrush = CreateSolidBrush(COLOUR_OF_1024);
                break;
            case 2048:
                hBrush = CreateSolidBrush(COLOUR_OF_2048);
                break;
            default:
                hBrush = CreateSolidBrush(RGB(204, 192, 174));
                break;
            }
            FillRect(hdc, &rc, hBrush);
            DeleteObject(hBrush);
            if (tab[ii][jj] != 0)
            {
                wchar_t s[256];
                swprintf_s(s, 256, L"%d", tab[ii][jj]);
                DrawText(hdc, s, (int)_tcslen(s), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }
        }
        else
        {
            HBRUSH hBrush=CreateSolidBrush(RGB(204, 192, 174));
            GetClientRect(hWnd, &rc);
            FillRect(hdc, &rc, hBrush);
            DeleteObject(hBrush);
            wchar_t s[256];
            if (!Win() && checkover(tab)==1)
            {
                swprintf_s(s, 256, L"%d", score);
                //DrawText(hdc, s, (int)_tcslen(s), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE |DT_CALCRECT);
                DrawText(hdc, s, (int)_tcslen(s), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
            }
            else
            {
                if (!Win())
                {
                    swprintf_s(s, 256, L"You lost");
                    DrawText(hdc, s, (int)_tcslen(s), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                }
                else
                {
                    swprintf_s(s, 256, L"You won");
                    DrawText(hdc, s, (int)_tcslen(s), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                }
            }
        }
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        //PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

ATOM MyRegisterClass2(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc2;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(250, 247, 238)));
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY2048);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"MainWindow2";
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));
    return RegisterClassExW(&wcex);
}

LRESULT CALLBACK WndProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    //case WM_COMMAND:
    //{
    //    int wmId = LOWORD(wParam);
    //    // Parse the menu selections:
    //    switch (wmId)
    //    {
    //    case IDM_ABOUT:
    //        DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
    //        break;
    //    case IDM_EXIT:
    //        DestroyWindow(hWnd);
    //        break;
    //    default:
    //        return DefWindowProc(hWnd, message, wParam, lParam);
    //    }
    //}
    //break;
    case WM_KEYDOWN:
    {
        if (checkover(tab) != 1 || Win())
        {
            break;
        }
       
        switch (wParam)
        {
        case 0x57: //W
        {
            //MessageBox(hWnd, L"Hello",L"HELLO",MB_OK);
            //upmove(tab);

            MoveUp();
            if (!isFull())
            {
                addblock(tab);
            }
            
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    InvalidateRect(hBoxWMain1[i][j], NULL, NULL);
                    InvalidateRect(hBoxWMain2[i][j], NULL, NULL);
                }
            }
            InvalidateRect(hScoreMain1, NULL, NULL);
            InvalidateRect(hScoreMain2, NULL, NULL);
        }
        break;
        case 0x53: //S
        {

            MoveDown();
            if (!isFull())
            {
                addblock(tab);
            }
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    InvalidateRect(hBoxWMain1[i][j], NULL, NULL);
                    InvalidateRect(hBoxWMain2[i][j], NULL, NULL);
                }
            }
            InvalidateRect(hScoreMain1, NULL, NULL);
            InvalidateRect(hScoreMain2, NULL, NULL);
        }
        break;
        case 0x41: //A
        {

            MoveLeft();
            if (!isFull())
            {
                addblock(tab);
            }
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    InvalidateRect(hBoxWMain1[i][j], NULL, NULL);
                    InvalidateRect(hBoxWMain2[i][j], NULL, NULL);
                }
            }
            InvalidateRect(hScoreMain1, NULL, NULL);
            InvalidateRect(hScoreMain2, NULL, NULL);
            //UpdateAllWindows();
        }
        break;
        case 0x44: //D
        {

            MoveRight();
            if (!isFull())
            {
                addblock(tab);
            }
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    InvalidateRect(hBoxWMain1[i][j], NULL, NULL);
                    InvalidateRect(hBoxWMain2[i][j], NULL, NULL);
                }
            }
            InvalidateRect(hScoreMain1, NULL, NULL);
            InvalidateRect(hScoreMain2, NULL, NULL);
        }
        break;

        default:
            break;
        }
    }
    break;
    case WM_GETMINMAXINFO:
    {
        MINMAXINFO* minMaxInfo = (MINMAXINFO*)lParam;
        RECT rc;
        rc.left = 0;
        rc.top = 0;
        rc.right = 4 * BOX_SIZE + 5 * GAP;
        rc.bottom = 5 * BOX_SIZE + 7 * GAP;
        AdjustWindowRectEx(&rc, WS_OVERLAPPEDWINDOW, false, 0);
        minMaxInfo->ptMaxTrackSize.x = minMaxInfo->ptMinTrackSize.x = rc.right - rc.left;
        //minMaxInfo->ptMaxTrackSize.x = rc.right - rc.left;
        minMaxInfo->ptMaxTrackSize.y = minMaxInfo->ptMinTrackSize.y = rc.bottom - rc.top;
        // minMaxInfo->ptMaxTrackSize.y = rc.bottom - rc.top;
    }
    break;
    case WM_CREATE:
    {
        RECT rc;
        hScoreMain2 = CreateWindowExA(0, "PlayBox", NULL, WS_CHILD, GAP, GAP, 4 * BOX_SIZE + 3 * GAP, BOX_SIZE, hWnd, (HMENU)100, hInst, NULL);
        GetClientRect(hScoreMain2, &rc);
        HRGN hRgn = CreateRoundRectRgn(rc.left, rc.top, rc.right, rc.bottom, 20, 20);
        SetWindowRgn(hScoreMain2, hRgn, TRUE);
        ShowWindow(hScoreMain2, SW_SHOW);
        UpdateWindow(hScoreMain2);
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                hBoxWMain2[i][j] = CreateWindowExA(0, "PlayBox", NULL, WS_CHILD, i * BOX_SIZE + (i + 1) * GAP, (j+1) * BOX_SIZE + (j + 2) * GAP,
                    BOX_SIZE, BOX_SIZE, hWnd, (HMENU)100, hInst, NULL);
                GetClientRect(hBoxWMain2[i][j], &rc);
                hRgn = CreateRoundRectRgn(rc.left, rc.top, rc.right, rc.bottom, 20, 20);
                SetWindowRgn(hBoxWMain2[i][j], hRgn, TRUE);
                ShowWindow(hBoxWMain2[i][j], SW_SHOW);
                UpdateWindow(hBoxWMain2[i][j]);
            }
        }
    }
    break;

    case WM_PAINT:
    {
        
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps); 
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hWnd, &ps);
       

    }
    break;
    case WM_MOVE:
    {
        
          RECT rMain1, rMain2;
        GetClientRect(hWndMain1, &rMain1);
        GetClientRect(hWndMain2, &rMain2);
        POINT p1left, p1right, p2left, p2right;
        p1left = { rMain1.left,rMain1.top };
        p1right = { rMain1.right,rMain1.bottom };
        p2left = { rMain2.left,rMain2.top };
        p2right = { rMain2.right,rMain2.bottom };
        ClientToScreen(hWndMain1, &p1left);
        ClientToScreen(hWndMain1, &p1right);
        ClientToScreen(hWndMain2, &p2left);
        ClientToScreen(hWndMain2, &p2right);
        bool intersect =((p1left.x<=p2left.x && p1right.x>=p2left.x)||(p1left.x <= p2right.x && p1right.x >= p2right.x))
            &&((p1left.y<=p2left.y && p1right.y>=p2left.y)||(p1left.y<=p2right.y&&p1right.y>=p2right.y));
        if (intersect)
        {  
            SetWindowLong(hWnd, GWL_EXSTYLE,
                GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
            SetLayeredWindowAttributes(hWnd, 0, (255 * 50) / 100, LWA_ALPHA);
            
        }
        else
        {
            SetWindowText(hWndMain2, L"2048");
            SetWindowLong(hWnd, GWL_EXSTYLE,
                GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
            SetLayeredWindowAttributes(hWnd, 0, 255, LWA_ALPHA);
        }
        if (hWnd == GetActiveWindow())
        {
            RECT rc;
            GetWindowRect(hWnd, &rc);
            int changeX = xScreenMiddle - rc.right;
            int changeY = yScreenMiddle - rc.bottom;
            int width = rc.right - rc.left;
            int height = rc.bottom - rc.top;
            int xScreenMiddle = GetSystemMetrics(SM_CXSCREEN) / 2;
            int yScreenMiddle = GetSystemMetrics(SM_CYSCREEN) / 2;
            MoveWindow(hWndMain1, xScreenMiddle + changeX, yScreenMiddle + changeY, width, height, TRUE); 
        };
        //MessageBox(hWnd, L"move", L"move", MB_OK);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

//from:https://code-projects.org/2048-game-in-c-with-source-code/
void addblock(int a[4][4])
{
    int li, ri;
    srand(time(NULL));
    while (1)
    {
        li = rand() % 4;
        ri = rand() % 4;
        if (a[li][ri] == 0)
        {
            //a[li][ri] = pow(2, li % 2 + 1);
            a[li][ri] = 2;
            break;
        }
    }

}
//from:https://code-projects.org/2048-game-in-c-with-source-code/
int checkover(int a[4][4])
{
    int fl = 0, gl = 0, i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            if (a[i][j] == 0)
            {
                fl = 1;
                break;
            }

    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            if (a[i + 1][j] == a[i][j] || a[i][j + 1] == a[i][j])
            {
                gl = 1;
                break;
            }

    if (fl || gl) return 1;
    else return 0;
}
bool isFull()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (tab[i][j] == 0)
            {
                
                return false;
            }
        }
    }
    return true;
  
}


// inspired by code from:https://code-projects.org/2048-game-in-c-with-source-code/
void MoveUp()
{
    int i, j, li, ri;
    for (i = 0; i < 4; i++)
    {
        li = i, ri = 0;
        for (j = 1; j < 4; j++)
        {
            if (tab[i][j] != 0)
            {
                if (tab[i][j - 1] == 0 || tab[i][j - 1] == tab[i][j])
                {
                    if (tab[li][ri] == tab[i][j])
                    {
                        tab[li][ri] *= 2;
                        score += tab[i][j];
                        tab[i][j] = 0;
                    }
                    else
                    {
                        if (tab[li][ri] == 0)
                        {
                            tab[li][ri] = tab[i][j];
                            tab[i][j] = 0;
                        }
                        else
                        {
                            tab[li][++ri] = tab[i][j];
                            tab[i][j] = 0;
                        }
                    }
                }
                else ri++;
            }
        }
    }
}
// inspired by code from:https://code-projects.org/2048-game-in-c-with-source-code/
void MoveDown()
{
    int i, j, li, ri;
    for (i = 0; i < 4; i++)
    {
        li = i, ri = 3;
        for (j = 2; j >= 0; j--)
        {
            if (tab[i][j] != 0)
            {
                if (tab[i][j + 1] == 0 || tab[i][j + 1] == tab[i][j])
                {
                    if (tab[li][ri] == tab[i][j])
                    {
                        tab[li][ri] *= 2;
                        score += tab[i][j];
                        tab[i][j] = 0;
                    }
                    else
                    {
                        if (tab[li][ri] == 0)
                        {
                            tab[li][ri] = tab[i][j];
                            tab[i][j] = 0;
                        }
                        else
                        {
                            tab[li][--ri] = tab[i][j];
                            tab[i][j] = 0;
                        }
                    }
                }
                else ri--;
            }
        }
    }
}
// inspired by code from:https://code-projects.org/2048-game-in-c-with-source-code/
void MoveLeft()
{
    int i, j, li, ri;
    for (j = 0; j < 4; j++)
    {
        li = 0, ri = j;
        for (i = 1; i < 4; i++)
        {
            if (tab[i][j] != 0)
            {
                if (tab[i - 1][j] == 0 || tab[i - 1][j] == tab[i][j])
                {
                    if (tab[li][ri] == tab[i][j])
                    {
                        tab[li][ri] *= 2;
                        score += tab[i][j];
                        tab[i][j] = 0;
                    }
                    else
                    {
                        if (tab[li][ri] == 0)
                        {
                            tab[li][ri] = tab[i][j];
                            tab[i][j] = 0;
                        }
                        else
                        {
                            tab[++li][ri] = tab[i][j];
                            tab[i][j] = 0;
                        }
                    }
                }
                else li++;
            }
        }
    }
}
// inspired by code from:https://code-projects.org/2048-game-in-c-with-source-code/
void MoveRight()
{
    int i, j, li, ri;
    for (j = 0; j < 4; j++)
    {
        li = 3, ri = j;
        for (i = 2; i >= 0; i--)
        {
            if (tab[i][j] != 0)
            {
                if (tab[i + 1][j] == 0 || tab[i + 1][j] == tab[i][j])
                {
                    if (tab[li][ri] == tab[i][j])
                    {
                        tab[li][ri] *= 2;
                        score += tab[i][j];
                        tab[i][j] = 0;
                    }
                    else
                    {
                        if (tab[li][ri] == 0)
                        {
                            tab[li][ri] = tab[i][j];
                            tab[i][j] = 0;
                        }
                        else
                        {
                            tab[--li][ri] = tab[i][j];
                            tab[i][j] = 0;
                        }
                    }
                }
                else li--;
            }
        }
    }
}


bool Win()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (tab[i][j] == GOAL) return true;
        }
    }
    return false;
}

void SaveGame()
{
    std::ofstream in;
    in.open("GameSave.txt");
    in << GOAL;
    in << std::endl;
    in << score;
    in << std::endl;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            in << tab[i][j];
            in << std::endl;
        }
    }
    in.close();
}

void LoadGame()
{
    std::ifstream in;
    in.open("GameSave.txt");
    in >> GOAL;
    in >> score;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            in >> tab[i][j];
        }
    }
    in.close();

}

void SetMenue()
{
    switch (GOAL)
    {
    case 8:
    {
        CheckMenuItem(GetMenu(hWndMain1), IDM_ABOUT, MF_CHECKED);
        CheckMenuItem(GetMenu(hWndMain1), ID_GOAL_16, MF_UNCHECKED);
        CheckMenuItem(GetMenu(hWndMain1), ID_GOAL_64, MF_UNCHECKED);
        CheckMenuItem(GetMenu(hWndMain1), ID_GOAL_2048, MF_UNCHECKED);
    }
    break;
    case 16:
    {
        CheckMenuItem(GetMenu(hWndMain1), IDM_ABOUT, MF_UNCHECKED);
        CheckMenuItem(GetMenu(hWndMain1), ID_GOAL_16, MF_CHECKED);
        CheckMenuItem(GetMenu(hWndMain1), ID_GOAL_64, MF_UNCHECKED);
        CheckMenuItem(GetMenu(hWndMain1), ID_GOAL_2048, MF_UNCHECKED);
    }
    break;
    case 64:
    {
        CheckMenuItem(GetMenu(hWndMain1), IDM_ABOUT, MF_UNCHECKED);
        CheckMenuItem(GetMenu(hWndMain1), ID_GOAL_16, MF_UNCHECKED);
        CheckMenuItem(GetMenu(hWndMain1), ID_GOAL_64, MF_CHECKED);
        CheckMenuItem(GetMenu(hWndMain1), ID_GOAL_2048, MF_UNCHECKED);
    }
    break;
    case 2048:
    {
        CheckMenuItem(GetMenu(hWndMain1), IDM_ABOUT, MF_UNCHECKED);
        CheckMenuItem(GetMenu(hWndMain1), ID_GOAL_16, MF_UNCHECKED);
        CheckMenuItem(GetMenu(hWndMain1), ID_GOAL_64, MF_UNCHECKED);
        CheckMenuItem(GetMenu(hWndMain1), ID_GOAL_2048, MF_CHECKED);
    }
    break;
    default:
        break;
    }
}