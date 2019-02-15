#include "Window.hpp"
#include "Engine/Internal/WindowsCommon.hpp"

//===============================================================================================
static Window *g_theWindow = nullptr; // Instance Pointer; 

//===============================================================================================
LRESULT CALLBACK GameWndProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{	
	// NEW:  Give the custom handlers a chance to run first; 
	Window *window = Window::GetInstance(); 
	if (nullptr != window) {
		for (int i = 0; i < (int)g_theWindow->listeners.size(); ++i) {			// CHANGED THIS TO LISTENERS INSTEAD OF HANDLERS
			window->listeners[i]( msg, wparam, lparam ); 
		}
	}

	// this is so the the app still renders and doesnt freeze when it doesnt have focus
	switch(msg)
	{
	case WM_NCACTIVATE:
		return 0;
	}

	//WM_CLOSE
	//DebuggerPrintf("This is the msg thing: %08x \n", msg);

	// do default windows behavior (return before this if you don't want default windows behavior for certain messages)
	return ::DefWindowProc( hwnd, msg, wparam, lparam );
}

//===============================================================================================
Window::Window(const char * name, float aspectRatio)
{
	// Make sure we don't already have one
	if(g_theWindow != nullptr)
	{
		delete[] g_theWindow;
	}

	// Creating the aspect ratio instead of passing it
	float clientAspect = aspectRatio;

	// storing the aspect in case we want it later
	m_aspect = aspectRatio;

	// 	Define a window style/class
	WNDCLASSEX windowClassDescription;
	memset( &windowClassDescription, 0, sizeof( windowClassDescription ) );
	windowClassDescription.cbSize = sizeof( windowClassDescription );
	windowClassDescription.style = CS_OWNDC; // Redraw on move, request own Display Context
	windowClassDescription.lpfnWndProc = static_cast< WNDPROC >( GameWndProc ); // Register our Windows message-handling function
	windowClassDescription.hInstance = GetModuleHandle( NULL );
	windowClassDescription.hIcon = NULL;
	windowClassDescription.hCursor = NULL;
	windowClassDescription.lpszClassName = TEXT( "Simple Window Class" ); // this is the name of the class
	RegisterClassEx( &windowClassDescription );

	// Add support for fullscreen mode (requires different window style flags than windowed mode)
	const DWORD windowStyleFlags = WS_CAPTION | WS_BORDER | WS_THICKFRAME | WS_SYSMENU | WS_OVERLAPPED;
	const DWORD windowStyleExFlags = WS_EX_APPWINDOW;

	// Get desktop rect, dimensions, aspect
	RECT desktopRect;
	HWND desktopWindowHandle = GetDesktopWindow();
	GetClientRect( desktopWindowHandle, &desktopRect );
	float desktopWidth = (float)( desktopRect.right - desktopRect.left );
	float desktopHeight = (float)( desktopRect.bottom - desktopRect.top );
	float desktopAspect = desktopWidth / desktopHeight;

	// Calculate maximum client size (as some % of desktop size)
	constexpr float maxClientFractionOfDesktop = 0.90f;
	float clientWidth = desktopWidth * maxClientFractionOfDesktop;
	float clientHeight = desktopHeight * maxClientFractionOfDesktop;
	if( clientAspect > desktopAspect )
	{
		// Client window has a wider aspect than desktop; shrink client height to match its width
		clientHeight = clientWidth / clientAspect;
	}
	else
	{
		// Client window has a taller aspect than desktop; shrink client width to match its height
		clientWidth = clientHeight * clientAspect;
	}

	// STORING THE WIDTH AND HEIGHT HERE 
	m_height = clientHeight;
	m_width = clientWidth;

	// Calculate client rect bounds by centering the client area
	float clientMarginX = 0.5f * (desktopWidth - clientWidth);
	float clientMarginY = 0.5f * (desktopHeight - clientHeight);
	RECT clientRect;
	clientRect.left = (int) clientMarginX;
	clientRect.right = clientRect.left + (int) clientWidth;
	clientRect.top = (int) clientMarginY;
	clientRect.bottom = clientRect.top + (int) clientHeight;

	// Calculate the outer dimensions of the physical window, including frame et. al.
	RECT windowRect = clientRect;
	AdjustWindowRectEx( &windowRect, windowStyleFlags, FALSE, windowStyleExFlags );

	WCHAR windowTitle[ 1024 ];
	MultiByteToWideChar( GetACP(), 0, name, -1, windowTitle, sizeof( windowTitle ) / sizeof( windowTitle[ 0 ] ) );
	m_hwnd = CreateWindowEx(
		windowStyleExFlags,
		windowClassDescription.lpszClassName,
		windowTitle,
		windowStyleFlags,
		windowRect.left,
		windowRect.top,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		GetModuleHandle( NULL ),
		NULL );

	ShowWindow( (HWND)m_hwnd, SW_SHOW );
	SetForegroundWindow( (HWND)m_hwnd );
	SetFocus( (HWND)m_hwnd );

	//g_displayDeviceContext = GetDC( (HWND)m_hwnd );

	m_cursor = (void*) LoadCursor( NULL, IDC_ARROW );
	SetCursor( (HCURSOR ) m_cursor );

	PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
	memset( &pixelFormatDescriptor, 0, sizeof( pixelFormatDescriptor ) );
	pixelFormatDescriptor.nSize = sizeof( pixelFormatDescriptor );
	pixelFormatDescriptor.nVersion = 1;
	pixelFormatDescriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
	pixelFormatDescriptor.cColorBits = 24;
	pixelFormatDescriptor.cDepthBits = 24;
	pixelFormatDescriptor.cAccumBits = 0;
	pixelFormatDescriptor.cStencilBits = 8;;

	// assign the static variable
	g_theWindow = this;
}

//-----------------------------------------------------------------------------------------------
Window::~Window()
{
	//delete g_theWindow;
	//g_theWindow = nullptr;
}

//-----------------------------------------------------------------------------------------------
void Window::RegisterHandler(windows_message_handler_cb cb)
{
	listeners.push_back(cb);
}

//-----------------------------------------------------------------------------------------------
void Window::UnregisterHandler(windows_message_handler_cb cb)
{
}

//-----------------------------------------------------------------------------------------------
Window * Window::GetInstance()
{
	return g_theWindow;
}

//-----------------------------------------------------------------------------------------------
void Window::SetTitle(char const * new_title)
{
	SetWindowTextA((HWND)m_hwnd, new_title);
}
