// Demo.cpp : Defines the entry point for the application.
//

#pragma optimize("gsy", on)
#pragma comment(linker, "/FILEALIGN:0x200")

#include "framework.h"
#include "Demo.h"

#define MAX_LOADSTRING 100
  
// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING] = _T("Security demo");   // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
    }

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
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = nullptr;
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

   const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
   const int screenHeight = GetSystemMetrics(SM_CYSCREEN);

   HWND hWnd = CreateWindowW(
      szWindowClass,
      L"Security Awareness Training",
      WS_POPUP,
      0,
      0,
      screenWidth,
      screenHeight,
      nullptr,
      nullptr,
      hInstance,
      nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, screenWidth, screenHeight, SWP_SHOWWINDOW);
   ShowWindow(hWnd, SW_SHOWMAXIMIZED);
   UpdateWindow(hWnd);

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
    case WM_KEYDOWN:
        if (wParam == VK_F12 || wParam == VK_ESCAPE)
        {
            DestroyWindow(hWnd);
            return 0;
        }
        break;
    case WM_ERASEBKGND:
        return 1;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            RECT rc;
            GetClientRect(hWnd, &rc);

            HBRUSH backgroundBrush = CreateSolidBrush(RGB(180, 0, 0));
            FillRect(hdc, &rc, backgroundBrush);
            DeleteObject(backgroundBrush);

            SetBkMode(hdc, TRANSPARENT);

            HFONT titleFont = CreateFontW(60, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
            HFONT bodyFont = CreateFontW(32, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
            HFONT footerFont = CreateFontW(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");

            const int clientWidth = rc.right - rc.left;
            const int clientHeight = rc.bottom - rc.top;
            const int centerX = rc.left + clientWidth / 2;
            const int symbolCenterY = rc.top + MulDiv(clientHeight, 33, 100);
            const int triangleHalfWidth = MulDiv(clientHeight, 10, 100);
            const int triangleHeight = MulDiv(clientHeight, 18, 100);
            const int triangleTop = symbolCenterY - triangleHeight / 2;
            const int triangleBottom = symbolCenterY + triangleHeight / 2;

            POINT warningTriangle[3] = {
                { centerX, triangleTop },
                { centerX - triangleHalfWidth, triangleBottom },
                { centerX + triangleHalfWidth, triangleBottom }
            };
            HBRUSH warningBrush = CreateSolidBrush(RGB(255, 220, 0));
            HPEN warningPen = CreatePen(PS_SOLID, 6, RGB(255, 255, 255));
            HGDIOBJ oldBrush = SelectObject(hdc, warningBrush);
            HGDIOBJ oldPen = SelectObject(hdc, warningPen);
            Polygon(hdc, warningTriangle, 3);
            SelectObject(hdc, oldPen);
            SelectObject(hdc, oldBrush);
            DeleteObject(warningPen);
            DeleteObject(warningBrush);

            HPEN markPen = CreatePen(PS_SOLID, MulDiv(clientHeight, 2, 100), RGB(0, 0, 0));
            oldPen = SelectObject(hdc, markPen);
            const int markTop = triangleTop + MulDiv(clientHeight, 4, 100);
            const int markBottom = triangleTop + MulDiv(clientHeight, 11, 100);
            MoveToEx(hdc, centerX, markTop, nullptr);
            LineTo(hdc, centerX, markBottom);
            Ellipse(hdc, centerX - 6, triangleBottom - MulDiv(clientHeight, 4, 100), centerX + 6, triangleBottom - MulDiv(clientHeight, 4, 100) + 12);
            SelectObject(hdc, oldPen);
            DeleteObject(markPen);

            RECT titleRc = rc;
            titleRc.top = rc.top + MulDiv(clientHeight, 46, 100);
            titleRc.bottom = titleRc.top + MulDiv(clientHeight, 10, 100);
            SelectObject(hdc, titleFont);
            SetTextColor(hdc, RGB(255, 255, 0));
            DrawTextW(hdc, L"SECURITY AWARENESS TRAINING SIMULATION", -1, &titleRc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            RECT bodyRc = rc;
            bodyRc.top = rc.top + MulDiv(clientHeight, 58, 100);
            bodyRc.left += MulDiv(clientWidth, 8, 100);
            bodyRc.right -= MulDiv(clientWidth, 8, 100);
            bodyRc.bottom = bodyRc.top + MulDiv(clientHeight, 20, 100);
            SelectObject(hdc, bodyFont);
            SetTextColor(hdc, RGB(255, 255, 255));
            DrawTextW(hdc, L"This is a safe training demo.\r\nNo infection is present.", -1, &bodyRc, DT_CENTER | DT_WORDBREAK);

            RECT footerRc = rc;
            footerRc.top = rc.top + MulDiv(clientHeight, 88, 100);
            footerRc.bottom = footerRc.top + MulDiv(clientHeight, 6, 100);
            SelectObject(hdc, footerFont);
            SetTextColor(hdc, RGB(255, 255, 255));
            DrawTextW(hdc, L"Press F12 or Esc to exit the simulation.", -1, &footerRc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            DeleteObject(titleFont);
            DeleteObject(bodyFont);
            DeleteObject(footerFont);

            EndPaint(hWnd, &ps);
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