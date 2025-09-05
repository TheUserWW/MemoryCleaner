#pragma once

// 添加智能指针支持
#include <memory>

// ==================== 头文件包含区域 ====================
// 标准库头文件
#include <commctrl.h> // Windows通用控件API，用于SetWindowSubclass等函数

// WinRT核心头文件
#include <winrt/Windows.Foundation.h> // WinRT基础类型和接口

// WinRT命名空间头文件（按字母顺序排列）
#include <winrt/Microsoft.UI.Xaml.h> // Microsoft UI XAML核心功能
#include <winrt/Microsoft.UI.Xaml.Controls.h> // XAML控件相关功能
#include <winrt/Windows.System.Diagnostics.h> // 系统诊断功能，用于内存监控
#include "settings.h" // 添加Settings类头文件

// 项目生成的头文件
#include "MainWindow.g.h" // 自动生成的主窗口类定义

// GDI对象智能指针定义
struct GDIObjectDeleter {
    void operator()(HGDIOBJ obj) const {
        if (obj) DeleteObject(obj);
    }
};

using UniqueHBITMAP = std::unique_ptr<std::remove_pointer<HBITMAP>::type, GDIObjectDeleter>;
using UniqueHICON = std::unique_ptr<std::remove_pointer<HICON>::type, GDIObjectDeleter>;
using UniqueHFONT = std::unique_ptr<std::remove_pointer<HFONT>::type, GDIObjectDeleter>;
using UniqueHDC = std::unique_ptr<std::remove_pointer<HDC>::type, decltype(&DeleteDC)>;

// 添加Windows句柄智能指针定义
struct HandleDeleter {
    void operator()(HANDLE handle) const {
        if (handle && handle != INVALID_HANDLE_VALUE) {
            CloseHandle(handle);
        }
    }
};
using UniqueHandle = std::unique_ptr<std::remove_pointer<HANDLE>::type, HandleDeleter>;

// 窗口子类化RAII包装器
class WindowSubclass {
public:
    WindowSubclass(HWND hWnd, SUBCLASSPROC proc, UINT_PTR id, DWORD_PTR data)
        : hWnd_(hWnd), proc_(proc), id_(id) {
        SetWindowSubclass(hWnd_, proc_, id_, data);
    }

    ~WindowSubclass() {
        RemoveWindowSubclass(hWnd_, proc_, id_);
    }

    // 禁止拷贝
    WindowSubclass(const WindowSubclass&) = delete;
    WindowSubclass& operator=(const WindowSubclass&) = delete;

private:
    HWND hWnd_;
    SUBCLASSPROC proc_;
    UINT_PTR id_;
};

// ==================== 命名空间定义区域 ====================
namespace winrt::MemoryCleaner::implementation
{
    // 清理选项枚举 - 定义内存清理的不同选项
    // 使用位掩码设计，支持多选组合
    enum class CleanOption
    {
        WorkingSet = 1,              // 清理当前进程工作集
        SystemWorkingSet = 2,        // 清理系统工作集
        StandbyList = 4,             // 清理待机列表
        StandbyListLowPriority = 8,  // 清理低优先级待机列表
        ModifiedPageList = 16,        // 清理修改页面列表
        CombinePageList = 32         // 合并页面列表
    };

    // 主窗口实现类 - 继承自自动生成的MainWindowT模板类
    struct MainWindow : MainWindowT<MainWindow>
    {
        // ========== 公共接口 ==========
        MainWindow();  // 构造函数
        ~MainWindow(); // 析构函数 - 负责资源清理


        void ShowAboutDialog(); // 显示关于对话框

        // 语言变更处理方法
        void OnLanguageChanged(Settings::Language newLanguage);

        // 事件处理方法
        void CleanButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void OnTrayShowWindow(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void OnTrayCleanMemory(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void OnTrayExit(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);

        // 语言切换方法
        void OnLanguageEnglishClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void SetEnglishLanguage();

        // 添加其他语言的事件处理方法声明
        void OnLanguageChineseSimplifiedClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void OnLanguageChineseTraditionalClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void OnLanguageMongolianClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void OnLanguageUyghurClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void OnLanguageFrenchClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void OnLanguageRussianClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void OnLanguageSpanishClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void OnLanguageGermanClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void OnLanguageItalianClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void OnLanguageArabicClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void OnLanguageKoreanClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void OnLanguageJapaneseClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    private:

        HMENU m_hTrayMenu{ nullptr };  // 传统托盘菜单句柄

        // 传统托盘菜单方法
        void CreateTrayMenu();          // 创建传统托盘菜单
        void ShowTrayMenu();            // 显示传统托盘菜单
        void DestroyTrayMenu();         // 销毁传统托盘菜单
        void HandleTrayMenuCommand(WPARAM wParam);  // 处理传统托盘菜单命令


        // ========== 私有成员变量 ==========
        // 定时器相关
        winrt::Microsoft::UI::Xaml::DispatcherTimer memoryTimer{ nullptr }; // 内存监控定时器
        bool isWindowClosed = false;
        // 窗口尺寸限制
        int minWindowWidth{ 0 };   // 最小窗口宽度
        int minWindowHeight{ 0 };  // 最小窗口高度

        // 系统托盘相关成员
        NOTIFYICONDATA nid{};              // 托盘图标数据结构
        bool isInTray{ false };            // 标记窗口是否已最小化到托盘

        // 应用窗口管理
        winrt::Microsoft::UI::Windowing::AppWindow appWindow{ nullptr }; // 现代应用窗口对象
        bool isDestroying{ false };        // 标记窗口是否正在销毁过程中

        // 窗口子类化
        std::unique_ptr<WindowSubclass> windowSubclass; // 窗口子类化RAII包装器

        // 托盘菜单置顶状态
        bool wasWindowTopMost{ false };    // 保存窗口原始置顶状态

        // 托盘图标智能指针
        UniqueHICON trayIcon{ nullptr };

        // 设置管理器
        std::unique_ptr<Settings> m_settings;

        // ========== 私有方法声明 ==========
        // 窗口过程回调函数 - 静态方法用于窗口消息处理
        static LRESULT CALLBACK SubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

        // 系统托盘功能方法
        void CreateTrayIcon();             // 创建系统托盘图标
        void RemoveTrayIcon();             // 移除系统托盘图标
        void ShowTrayContextMenu();        // 显示托盘上下文菜单
        void OnTrayCommand(WPARAM wParam); // 处理托盘命令消息
        void MinimizeToTray();             // 最小化窗口到托盘
        void RestoreFromTray();            // 从托盘恢复窗口

        // 托盘图标更新方法
        void UpdateTrayIcon(double memoryPercentage, double virtualUsage, double privateWS_GB, double sharedWS_GB, double totalWS_GB);

        // 内存监控功能方法
        void InitializeMemoryMonitoring(); // 初始化内存监控
        void UpdateMemoryUsage(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e); // 更新内存使用情况
        double GetPhysicalMemoryUsagePercentage(); // 获取物理内存使用百分比
        double GetVirtualMemoryUsagePercentage();  // 获取虚拟内存使用百分比
        bool getSystemWorkingSetInfo(uint64_t& totalWS, uint64_t& sharedWS, uint64_t& privateWS); // 获取系统工作集信息

        // 内存清理功能
        void PerformCleanup();             // 执行内存清理操作

        // 选项同步方法
        void SyncCleanupOptions();         // 同步清理选项状态

        // 设置相关方法
        void ApplySettings();              // 应用设置
        void SaveCurrentCleanupOptions();  // 保存当前清理选项
        
        // 系统主题检测和托盘图标主题切换方法
        bool IsSystemUsingDarkTheme();     // 检测系统是否使用深色主题
        void UpdateTrayIconTheme();       // 更新托盘图标主题
        void HandleSystemThemeChange();    // 处理系统主题变化
        
        // 自定义菜单绘制方法
        void OnMeasureMenuItem(WPARAM wParam, LPARAM lParam);  // 处理WM_MEASUREITEM消息
        void OnDrawMenuItem(WPARAM wParam, LPARAM lParam);    // 处理WM_DRAWITEM消息

    };
}


// ==================== 工厂实现命名空间 ====================
namespace winrt::MemoryCleaner::factory_implementation
{
    // 主窗口工厂实现类 - 用于XAML运行时创建窗口实例
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
        // 事件处理方法（与implementation命名空间中的方法对应）
        void CleanButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void OnTrayShowWindow(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void OnTrayCleanMemory(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void OnTrayExit(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);

        winrt::Microsoft::UI::Xaml::DispatcherTimer memoryTimer{ nullptr };
        int minWindowWidth{ 0 };
        int minWindowHeight{ 0 };
        NOTIFYICONDATA nid{};
        HMENU hTrayMenu{ nullptr };
        bool isInTray{ false };
        winrt::Microsoft::UI::Windowing::AppWindow appWindow{ nullptr };
        bool isDestroying{ false };
        UINT_PTR subclassId{ 0 };

        // 窗口过程回调函数
        static LRESULT CALLBACK SubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

        // 系统托盘功能方法
        void CreateTrayIcon();
        void RemoveTrayIcon();
        void ShowTrayContextMenu();
        void OnTrayCommand(WPARAM wParam);
        void MinimizeToTray();
        void RestoreFromTray();

    private:


        // 内存监控功能方法
        void InitializeMemoryMonitoring();
        void UpdateMemoryUsage(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e);
        double GetPhysicalMemoryUsagePercentage();
        double GetVirtualMemoryUsagePercentage();
        bool getSystemWorkingSetInfo(uint64_t& totalWS, uint64_t& sharedWS, uint64_t& privateWS);

        // 内存清理功能
        void PerformCleanup();

        // 选项同步方法
        void SyncCleanupOptions();

        // 添加设置管理器
        std::unique_ptr<Settings> m_settings;

        // 添加设置相关方法
        void ApplySettings();                    // 应用设置
        void SaveCurrentCleanupOptions();        // 保存当前清理选项
        void OnLanguageChanged(Settings::Language newLanguage); // 语言变更处理
    };
}