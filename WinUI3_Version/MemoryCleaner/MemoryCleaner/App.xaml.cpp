#include "pch.h"
#include "App.xaml.h"
#include "MainWindow.xaml.h"
#include <winrt/Microsoft.UI.Interop.h> // 添加这行
#include <microsoft.ui.xaml.window.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::MemoryCleaner::implementation
{
    // 全局互斥锁句柄
    static HANDLE hMutex = NULL;
    static const wchar_t* MutexName = L"MemoryCleaner_SingleInstance_Mutex";

    /// <summary>
    /// 激活已存在的应用程序窗口
    /// </summary>
    void ActivateExistingWindow()
    {
        // 查找已存在的窗口
        HWND hWnd = FindWindowW(L"MemoryCleanerMainWindow", L"MemoryCleaner");
        if (hWnd)
        {
            // 如果窗口被最小化，则恢复它
            if (IsIconic(hWnd))
            {
                ShowWindow(hWnd, SW_RESTORE);
            }
            // 将窗口置于前台
            SetForegroundWindow(hWnd);
            // 激活窗口
            SetActiveWindow(hWnd);
        }
    }

    /// <summary>
    /// Initializes the singleton application object.  This is the first line of authored code
    /// executed, and as such is the logical equivalent of main() or WinMain().
    /// </summary>
    App::App()
    {
        // 创建互斥锁，确保只有一个实例运行
        hMutex = CreateMutexW(NULL, TRUE, MutexName);
        if (hMutex == NULL || GetLastError() == ERROR_ALREADY_EXISTS)
        {
            // 互斥锁已存在，说明已有实例在运行
            if (hMutex != NULL)
            {
                CloseHandle(hMutex);
                hMutex = NULL;
            }
            
            // 激活已存在的窗口
            ActivateExistingWindow();
            
            // 退出当前实例
            ExitProcess(0);
            return;
        }

        // Xaml objects should not call InitializeComponent during construction.
        // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BREAK_ON_UNHANDLED_EXCEPTION
        UnhandledException([](IInspectable const&, UnhandledExceptionEventArgs const& e)
        {
            if (IsDebuggerPresent())
            {
                auto errorMessage = e.Message();
                __debugbreak();
            }
        });
#endif
    }

    /// <summary>
    /// Invoked when the application is launched.
    /// </summary>
    /// <param name="e">Details about the launch request and process.</param>
    void App::OnLaunched([[maybe_unused]] LaunchActivatedEventArgs const& e)
    {
        window = make<MainWindow>();
        window.Activate();
        
        // 设置窗口类名和标题，以便其他实例可以找到它
        if (window)
        {
            auto windowNative = window.try_as<::IWindowNative>(); // 修改这行
            if (windowNative)
            {
                HWND hWnd = nullptr;
                windowNative->get_WindowHandle(&hWnd);
                if (hWnd)
                {
                    // 设置窗口类名
                    SetClassLongW(hWnd, GCLP_HCURSOR, (LONG)LoadCursorW(NULL, IDC_ARROW));
                    // 设置窗口标题
                    SetWindowTextW(hWnd, L"MemoryCleaner");
                }
            }
        }
    }
}
