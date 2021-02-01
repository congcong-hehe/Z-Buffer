#include "Window.h"

Window* my_window = NULL;

void windowInit(int w, int h)
{
	my_window = new Window;
	WNDCLASS wc = { CS_BYTEALIGNCLIENT, (WNDPROC)msgCallback, 0, 0, 0,
		NULL, NULL, NULL, NULL, _T("RenderWindow") };
	BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), w, -h, 1, 32, BI_RGB,
		w * h * 4, 0, 0, 0, 0 } };
	RECT rect = { 0, 0, w, h };
	int wx, wy, sx, sy;
	LPVOID ptr;
	HDC hDC;

	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	RegisterClass(&wc);

	my_window->hwnd_ = CreateWindow(_T("RenderWindow"), _T("RenderLab"),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);
	assert(my_window->hwnd_ != NULL);

	my_window->window_close_ = 0;
	hDC = GetDC(my_window->hwnd_);
	my_window->hdc_ = CreateCompatibleDC(hDC);
	ReleaseDC(my_window->hwnd_, hDC);

	my_window->hbm_ = CreateDIBSection(my_window->hdc_, &bi, DIB_RGB_COLORS, &ptr, 0, 0);
	assert(my_window->hbm_ != NULL);

	my_window->hbm_old_ = (HBITMAP)SelectObject(my_window->hdc_, my_window->hbm_);
	my_window->framebuffer_ = (unsigned char*)ptr;
	my_window->width_ = w;
	my_window->height_ = h;

	AdjustWindowRect(&rect, GetWindowLong(my_window->hwnd_, GWL_STYLE), 0);
	wx = rect.right - rect.left;
	wy = rect.bottom - rect.top;
	sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
	sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
	if (sy < 0) sy = 0;
	SetWindowPos(my_window->hwnd_, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
	SetForegroundWindow(my_window->hwnd_);

	ShowWindow(my_window->hwnd_, SW_NORMAL);
	msgDispatch();

	memset(my_window->framebuffer_, 0, w * h * 4);
}

void windowClose()
{
	if (my_window->hdc_) {
		if (my_window->hbm_old_) {
			SelectObject(my_window->hdc_, my_window->hbm_old_);
			my_window->hbm_old_ = NULL;
		}
		DeleteDC(my_window->hdc_);
		my_window->hdc_ = NULL;
	}
	if (my_window->hbm_) {
		DeleteObject(my_window->hbm_);
		my_window->hbm_ = NULL;
	}
	if (my_window->hwnd_) {
		CloseWindow(my_window->hwnd_);
		my_window->hwnd_ = NULL;
	}
	free(my_window);
}

void msgDispatch()
{
	MSG msg;
	while (1) {
		if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) break;
		if (!GetMessage(&msg, NULL, 0, 0)) break;
		DispatchMessage(&msg);
	}
}

void windowDraw(unsigned char* framebuffer)
{
	for (int i = 0; i < my_window->height_; ++i)
	{
		for (int j = 0; j < my_window->width_; ++j)
		{
			for (int k = 0; k < 4; ++k)
			{
				my_window->framebuffer_[(i * my_window->width_ + j) * 4 + k] = framebuffer[(i * my_window->width_ + j) * 4 + k];
			}
		}
	}

	HDC hDC = GetDC(my_window->hwnd_);
	BitBlt(hDC, 0, 0, my_window->width_, my_window->height_, my_window->hdc_, 0, 0, SRCCOPY);
	ReleaseDC(my_window->hwnd_, hDC);
	msgDispatch();
}

LRESULT CALLBACK msgCallback(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg) 
	{
		case WM_CLOSE: my_window->window_close_ = true; break;
		default: return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return 0;
}