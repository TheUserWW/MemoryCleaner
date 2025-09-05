#include "pch.h"
#include "AboutDialog.h"
#include "AboutDialog.g.cpp"
#include <microsoft.ui.xaml.window.h>
// 移除以下重复包含的头文件，因为它们已经在 pch.h 中包含
// #include <winrt/Microsoft.UI.Interop.h>
// #include <winrt/Microsoft.UI.Windowing.h>

// 添加Windows API头文件
#include <windowsx.h>
#include <winuser.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Windowing;

namespace winrt::MemoryCleaner::implementation
{
    AboutDialog::AboutDialog()
    {
        InitializeComponent();
        versionTextBlock().Text(L"v 1.2.0");

        // 设置自定义标题栏
        auto titleBar = AboutDialogTitleBar();
        this->SetTitleBar(titleBar);
        this->ExtendsContentIntoTitleBar(true);

        // 使用IWindowNative接口和AppWindow设置窗口大小
        auto windowNative{ this->try_as<::IWindowNative>() };
        if (windowNative)
        {
            HWND hWnd{ nullptr };
            windowNative->get_WindowHandle(&hWnd);

            if (hWnd)
            {
                // 获取并存储AppWindow
                appWindow = AppWindow::GetFromWindowId(
                    Microsoft::UI::GetWindowIdFromWindow(hWnd));

                if (appWindow)
                {
                    appWindow.Resize({ 640, 480 });

                    // 使用Windows API禁止调整窗口大小
                    // 获取当前窗口样式
                    LONG_PTR style = GetWindowLongPtr(hWnd, GWL_STYLE);
                    // 移除WS_THICKFRAME样式（允许调整大小的边框）
                    style &= ~WS_THICKFRAME;
                    // 移除WS_MAXIMIZEBOX样式（禁用最大化按钮）
                    style &= ~WS_MAXIMIZEBOX;
                    // 应用新的窗口样式
                    SetWindowLongPtr(hWnd, GWL_STYLE, style);

                    // 刷新窗口
                    SetWindowPos(hWnd, nullptr, 0, 0, 0, 0,
                        SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
                }
            }
        }
    }

    void AboutDialog::CloseButtonClick(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        Close();
    }
}
