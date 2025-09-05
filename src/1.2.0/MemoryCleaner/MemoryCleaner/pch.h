#pragma once
#include <windows.h>
#include <unknwn.h>
#include <restrictederrorinfo.h>
#include <hstring.h>

// Undefine GetCurrentTime macro to prevent
// conflict with Storyboard::GetCurrentTime
#undef GetCurrentTime

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.ApplicationModel.Activation.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.h>
// 添加以下头文件包含
#include <winrt/Microsoft.UI.Windowing.h>
#include <winrt/Microsoft.UI.Interop.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <winrt/Microsoft.UI.Xaml.Data.h>
#include <winrt/Microsoft.UI.Xaml.Interop.h>
#include <winrt/Microsoft.UI.Xaml.Markup.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Xaml.Navigation.h>
#include <winrt/Microsoft.UI.Xaml.Shapes.h>
#include <winrt/Microsoft.UI.Dispatching.h>
// 添加Windows.UI.Core命名空间以支持CoreDispatcherPriority
#include <winrt/Windows.UI.Core.h>
#include <wil/cppwinrt_helpers.h>

#include <winrt/Microsoft.Web.WebView2.Core.h>
#include <winrt/Windows.System.h>

// 添加PDH（Performance Data Helper）相关头文件
#include <pdh.h>
#include <pdhmsg.h>

#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "pdh.lib")  // 添加PDH库链接

// 添加菜单样式常量定义
#ifndef MNS_OWNERDRAW
#define MNS_OWNERDRAW 0x10000000L
#endif
