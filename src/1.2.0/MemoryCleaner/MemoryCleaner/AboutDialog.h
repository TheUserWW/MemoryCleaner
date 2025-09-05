#pragma once

#include "AboutDialog.g.h"
// 移除以下重复包含的头文件，因为它们已经在 pch.h 中包含
// #include <winrt/Microsoft.UI.Windowing.h>
// #include <winrt/Microsoft.UI.Interop.h>

namespace winrt::MemoryCleaner::implementation
{
    struct AboutDialog : AboutDialogT<AboutDialog>
    {
        AboutDialog();
        void CloseButtonClick(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void Activate();
        winrt::Microsoft::UI::Windowing::AppWindow appWindow{ nullptr };
    };
}

namespace winrt::MemoryCleaner::factory_implementation
{
    struct AboutDialog : AboutDialogT<AboutDialog, implementation::AboutDialog>
    {
    };
}
