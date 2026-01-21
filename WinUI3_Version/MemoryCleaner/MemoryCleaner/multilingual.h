#pragma once

#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include "settings.h"  // 添加这一行以引用Settings类

// 前向声明 MainWindow 类
namespace winrt::MemoryCleaner::implementation {
    struct MainWindow;
}

// 传统托盘菜单文本结构体
struct TrayMenuTexts {
    std::wstring showMainWindow;      // 显示主窗口
    std::wstring cleanOptions;        // 清理选项
    std::wstring cleanNow;            // 立即清理内存
    std::wstring exit;                // 退出
    std::wstring workingSet;          // 工作集
    std::wstring systemWorkingSet;    // 系统工作集
    std::wstring standbyList;         // 备用列表
    std::wstring standbyListLow;      // 低优先级备用列表
    std::wstring modifiedPageList;    // 修改页面列表
    std::wstring combinePageList;     // 组合页面列表
};

// 语言相关函数声明
// 简化的日志文本结构体
struct LogTexts {
    std::wstring cleanedMemory;       // 清理了
    std::wstring megabytes;           // MB
};

namespace winrt::MemoryCleaner::Multilingual {
    // 设置英文界面
    void SetEnglishLanguage(winrt::MemoryCleaner::implementation::MainWindow* mainWindow);
    
    // 设置简体中文界面
    void SetChineseSimplifiedLanguage(winrt::MemoryCleaner::implementation::MainWindow* mainWindow);
    
    // 设置繁体中文界面
    void SetChineseTraditionalLanguage(winrt::MemoryCleaner::implementation::MainWindow* mainWindow);
    
    // 设置蒙古语界面
    void SetMongolianLanguage(winrt::MemoryCleaner::implementation::MainWindow* mainWindow);
    
    // 设置维吾尔语界面
    void SetUyghurLanguage(winrt::MemoryCleaner::implementation::MainWindow* mainWindow);
    
    // 设置法语界面
    void SetFrenchLanguage(winrt::MemoryCleaner::implementation::MainWindow* mainWindow);
    
    // 设置俄语界面
    void SetRussianLanguage(winrt::MemoryCleaner::implementation::MainWindow* mainWindow);
    
    // 设置西班牙语界面
    void SetSpanishLanguage(winrt::MemoryCleaner::implementation::MainWindow* mainWindow);
    
    // 设置德语界面
    void SetGermanLanguage(winrt::MemoryCleaner::implementation::MainWindow* mainWindow);
    
    // 设置意大利语界面
    void SetItalianLanguage(winrt::MemoryCleaner::implementation::MainWindow* mainWindow);
    
    // 设置阿拉伯语界面
    void SetArabicLanguage(winrt::MemoryCleaner::implementation::MainWindow* mainWindow);
    
    // 设置韩语界面
    void SetKoreanLanguage(winrt::MemoryCleaner::implementation::MainWindow* mainWindow);
    
    // 设置日语界面
    void SetJapaneseLanguage(winrt::MemoryCleaner::implementation::MainWindow* mainWindow);
    
    // 英文语言点击事件处理
    void OnLanguageEnglishClick(winrt::MemoryCleaner::implementation::MainWindow* mainWindow,
                               winrt::Windows::Foundation::IInspectable const& sender,
                               winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    
    // 简体中文语言点击事件处理
    void OnLanguageChineseSimplifiedClick(winrt::MemoryCleaner::implementation::MainWindow* mainWindow,
                               winrt::Windows::Foundation::IInspectable const& sender,
                               winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    
    // 繁体中文语言点击事件处理
    void OnLanguageChineseTraditionalClick(winrt::MemoryCleaner::implementation::MainWindow* mainWindow,
                               winrt::Windows::Foundation::IInspectable const& sender,
                               winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    
    // 蒙古语语言点击事件处理
    void OnLanguageMongolianClick(winrt::MemoryCleaner::implementation::MainWindow* mainWindow,
                               winrt::Windows::Foundation::IInspectable const& sender,
                               winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    
    // 维吾尔语语言点击事件处理
    void OnLanguageUyghurClick(winrt::MemoryCleaner::implementation::MainWindow* mainWindow,
                               winrt::Windows::Foundation::IInspectable const& sender,
                               winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    
    // 法语语言点击事件处理
    void OnLanguageFrenchClick(winrt::MemoryCleaner::implementation::MainWindow* mainWindow,
                               winrt::Windows::Foundation::IInspectable const& sender,
                               winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    
    // 俄语语言点击事件处理
    void OnLanguageRussianClick(winrt::MemoryCleaner::implementation::MainWindow* mainWindow,
                               winrt::Windows::Foundation::IInspectable const& sender,
                               winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    
    // 西班牙语语言点击事件处理
    void OnLanguageSpanishClick(winrt::MemoryCleaner::implementation::MainWindow* mainWindow,
                               winrt::Windows::Foundation::IInspectable const& sender,
                               winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    
    // 德语语言点击事件处理
    void OnLanguageGermanClick(winrt::MemoryCleaner::implementation::MainWindow* mainWindow,
                               winrt::Windows::Foundation::IInspectable const& sender,
                               winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    
    // 意大利语语言点击事件处理
    void OnLanguageItalianClick(winrt::MemoryCleaner::implementation::MainWindow* mainWindow,
                               winrt::Windows::Foundation::IInspectable const& sender,
                               winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    
    // 阿拉伯语语言点击事件处理
    void OnLanguageArabicClick(winrt::MemoryCleaner::implementation::MainWindow* mainWindow,
                               winrt::Windows::Foundation::IInspectable const& sender,
                               winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    
    // 韩语语言点击事件处理
    void OnLanguageKoreanClick(winrt::MemoryCleaner::implementation::MainWindow* mainWindow,
                               winrt::Windows::Foundation::IInspectable const& sender,
                               winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    
    // 日语语言点击事件处理
    void OnLanguageJapaneseClick(winrt::MemoryCleaner::implementation::MainWindow* mainWindow,
                               winrt::Windows::Foundation::IInspectable const& sender,
                               winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    
    // 获取传统托盘菜单文本
    TrayMenuTexts GetTrayMenuTexts(Settings::Language language);
    
    // 获取简化的日志文本
    LogTexts GetLogTexts(Settings::Language language);
}