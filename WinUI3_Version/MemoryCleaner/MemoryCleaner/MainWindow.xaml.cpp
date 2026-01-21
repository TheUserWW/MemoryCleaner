#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif
#include <microsoft.ui.xaml.window.h>
#include <winrt/Microsoft.UI.Interop.h>
#include <winrt/Windows.Foundation.h>
#include <Psapi.h>
#include <TlHelp32.h>
#include <winrt/Windows.System.Diagnostics.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <windows.h>
#include "memorycleaner.h"
#include <winrt/Microsoft.UI.Windowing.h>
#include <winrt/Windows.Graphics.h>
#include <shellapi.h> // 添加Shell API头文件
#include <commctrl.h> // 添加CommCtrl头文件
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "Comctl32.lib") // 添加Comctl32库
#pragma comment(lib, "gdi32.lib") // 添加GDI库
#include "multilingual.h" // 添加对多语言头文件的引用
#include "settings.h" // 添加对设置头文件的引用
#include <memory> // 添加对memory头文件的引用，用于std::unique_ptr

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Windowing;
using namespace Windows::Graphics;

namespace winrt::MemoryCleaner::implementation
{

    MainWindow::MainWindow()
    {
        InitializeComponent();
        
        // 设置最小窗口尺寸
        minWindowWidth = 590;
        minWindowHeight = 920;
        maxWindowWidth = 590;  // 设置最大宽度与最小宽度相同，使宽度不可变
        maxWindowHeight = 940; // 设置最大高度与最小高度相同，使高度不可变
        // 初始化设置管理器
        m_settings = std::make_unique<Settings>();
       
        
        // 应用设置
        ApplySettings();
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
                    appWindow.Resize({ 590, 920 });
                }

                // 使用SetWindowSubclass替代窗口过程替换
                UINT_PTR subclassId = reinterpret_cast<UINT_PTR>(this);
                SetWindowSubclass(hWnd, &MainWindow::SubclassProc, subclassId, reinterpret_cast<DWORD_PTR>(this));
            }
            CreateTrayIcon(); 
            CreateTrayMenu();
    

        }


        InitializeMemoryMonitoring();
        
        // 初始化托盘图标主题
        UpdateTrayIconTheme();

        // 修复第75行附近的事件绑定
        // 绑定清理按钮点击事件
        btnCleanNow().Click([this](IInspectable const& sender, RoutedEventArgs const& e) {
            PerformCleanup();
            });

        // 绑定关于按钮点击事件
        actionAbout().Click([this](IInspectable const& sender, RoutedEventArgs const& e) {
            ShowAboutDialog();
            });

        // 绑定AuthorWebsite菜单项点击事件（新增）
        AuthorWebsite().Click([this](IInspectable const& sender, RoutedEventArgs const& e) {
            // 打开GitHub网页
            winrt::Windows::System::Launcher::LaunchUriAsync(winrt::Windows::Foundation::Uri(L"https://github.com/TheUserWW"));
            });
            
        // 绑定English按钮点击事件
        actionEnglish().Click([this](IInspectable const& sender, RoutedEventArgs const& e) {
            OnLanguageEnglishClick(sender, e);
            });

        // 绑定简体中文按钮点击事件
        actionChinese().Click([this](IInspectable const& sender, RoutedEventArgs const& e) {
            OnLanguageChineseSimplifiedClick(sender, e);
            });

        // 绑定繁体中文按钮点击事件
        actionTraditionalChinese().Click([this](IInspectable const& sender, RoutedEventArgs const& e) {
            OnLanguageChineseTraditionalClick(sender, e);
            });

        // 绑定蒙古语按钮点击事件
        actionMongolian().Click([this](IInspectable const& sender, RoutedEventArgs const& e) {
            OnLanguageMongolianClick(sender, e);
            });

        // 绑定维吾尔语按钮点击事件
        actionUyghur().Click([this](IInspectable const& sender, RoutedEventArgs const& e) {
            OnLanguageUyghurClick(sender, e);
            });

        // 绑定法语按钮点击事件
        actionFrench().Click([this](IInspectable const& sender, RoutedEventArgs const& e) {
            OnLanguageFrenchClick(sender, e);
            });

        // 绑定俄语按钮点击事件
        actionRussian().Click([this](IInspectable const& sender, RoutedEventArgs const& e) {
            OnLanguageRussianClick(sender, e);
            });

        // 绑定西班牙语按钮点击事件
        actionSpanish().Click([this](IInspectable const& sender, RoutedEventArgs const& e) {
            OnLanguageSpanishClick(sender, e);
            });

        // 绑定德语按钮点击事件
        actionGerman().Click([this](IInspectable const& sender, RoutedEventArgs const& e) {
            OnLanguageGermanClick(sender, e);
            });

        // 绑定意大利语按钮点击事件
        actionItalian().Click([this](IInspectable const& sender, RoutedEventArgs const& e) {
            OnLanguageItalianClick(sender, e);
            });

        // 绑定阿拉伯语按钮点击事件
        actionArabic().Click([this](IInspectable const& sender, RoutedEventArgs const& e) {
            OnLanguageArabicClick(sender, e);
            });

        // 绑定韩语按钮点击事件
        actionKorean().Click([this](IInspectable const& sender, RoutedEventArgs const& e) {
            OnLanguageKoreanClick(sender, e);
            });

        // 绑定日语按钮点击事件
        actionJapanese().Click([this](IInspectable const& sender, RoutedEventArgs const& e) {
            OnLanguageJapaneseClick(sender, e);
            });

        cbWorkingSet().Click([this](IInspectable const& sender, RoutedEventArgs const& e) {
            SaveCurrentCleanupOptions();
            });

        cbSystemWorkingSet().Click([this](IInspectable const& sender, RoutedEventArgs const& e) {
            SaveCurrentCleanupOptions();
            });

        cbStandbyList().Click([this](IInspectable const& sender, RoutedEventArgs const& e) {
            SaveCurrentCleanupOptions();
            });

        cbStandbyListLow().Click([this](IInspectable const& sender, RoutedEventArgs const& e) {
            SaveCurrentCleanupOptions();
            });

        cbModifiedPageList().Click([this](IInspectable const& sender, RoutedEventArgs const& e) {
            SaveCurrentCleanupOptions();
            });

        cbCombinePageList().Click([this](IInspectable const& sender, RoutedEventArgs const& e) {
            SaveCurrentCleanupOptions();
            });

        cbAutoClean().Click([this](IInspectable const& sender, RoutedEventArgs const& e) {
            SaveCurrentCleanupOptions();
            });


        // 获取 XAML 中定义的标题栏元素
        auto titleBar = AppTitleBar();

        // 设置该元素为自定义标题栏，等效于 ExtendsContentIntoTitleBar = true
        this->SetTitleBar(titleBar);

        // 显式设置扩展内容到标题栏
        this->ExtendsContentIntoTitleBar(true);
    }

    // MainWindow.xaml.cpp
    MainWindow::~MainWindow()
    {
        isDestroying = true;

        // 保存当前清理选项
        SaveCurrentCleanupOptions();

        // 先移除窗口子类化
        auto windowNative{ this->try_as<::IWindowNative>() };
        if (windowNative)
        {
            HWND hWnd{ nullptr };
            windowNative->get_WindowHandle(&hWnd);
            isDestroying = true;
            DestroyTrayMenu();
            if (hWnd)
            {
                UINT_PTR subclassId = reinterpret_cast<UINT_PTR>(this);
                RemoveWindowSubclass(hWnd, &MainWindow::SubclassProc, subclassId);
            }
        }

        // 然后移除托盘图标
        RemoveTrayIcon();

        // 停止定时器
        if (memoryTimer)
        {
            memoryTimer.Stop();
        }
    }


    // 实现应用设置方法
    void MainWindow::ApplySettings()
    {
        if (!m_settings) return;
        
        // 应用语言设置
        OnLanguageChanged(m_settings->GetLanguage());
        
        // 应用清理选项设置
        const auto& options = m_settings->GetCleanupOptions();
        cbWorkingSet().IsChecked(options.workingSet);
        cbSystemWorkingSet().IsChecked(options.systemWorkingSet);
        cbStandbyList().IsChecked(options.standbyList);
        cbStandbyListLow().IsChecked(options.standbyListLowPriority);
        cbModifiedPageList().IsChecked(options.modifiedPageList);
        cbCombinePageList().IsChecked(options.combinePageList);
        cbAutoClean().IsChecked(options.autoClean);
        
        // 设置清理间隔
        wchar_t intervalStr[16];
        _itow_s(options.cleanInterval, intervalStr, 10);
        SetInterval().Text(intervalStr);
        
        // 同步到托盘菜单
        SyncCleanupOptions();
    }
    
    // 实现保存当前清理选项方法
    void MainWindow::SaveCurrentCleanupOptions()
    {
        if (!m_settings) return;
        
        Settings::CleanupOptions options;
        options.workingSet = cbWorkingSet().IsChecked().Value();
        options.systemWorkingSet = cbSystemWorkingSet().IsChecked().Value();
        options.standbyList = cbStandbyList().IsChecked().Value();
        options.standbyListLowPriority = cbStandbyListLow().IsChecked().Value();
        options.modifiedPageList = cbModifiedPageList().IsChecked().Value();
        options.combinePageList = cbCombinePageList().IsChecked().Value();
        options.autoClean = cbAutoClean().IsChecked().Value();
        
        // 获取清理间隔
        try {
            options.cleanInterval = std::stoi(SetInterval().Text().c_str());
        }
        catch (...) {
            options.cleanInterval = 60; // 默认值
        }
        
        m_settings->SetCleanupOptions(options);
    }
    
    // 实现语言变更处理方法
    void MainWindow::OnLanguageChanged(Settings::Language newLanguage)
    {
        switch (newLanguage) {
            case Settings::Language::English:
                winrt::MemoryCleaner::Multilingual::SetEnglishLanguage(this);
                break;
            case Settings::Language::ChineseSimplified:
                winrt::MemoryCleaner::Multilingual::SetChineseSimplifiedLanguage(this);
                break;
            case Settings::Language::ChineseTraditional:
                winrt::MemoryCleaner::Multilingual::SetChineseTraditionalLanguage(this);
                break;
            case Settings::Language::Mongolian:
                winrt::MemoryCleaner::Multilingual::SetMongolianLanguage(this);
                break;
            case Settings::Language::Uyghur:
                winrt::MemoryCleaner::Multilingual::SetUyghurLanguage(this);
                break;
            case Settings::Language::French:
                winrt::MemoryCleaner::Multilingual::SetFrenchLanguage(this);
                break;
            case Settings::Language::Russian:
                winrt::MemoryCleaner::Multilingual::SetRussianLanguage(this);
                break;
            case Settings::Language::Spanish:
                winrt::MemoryCleaner::Multilingual::SetSpanishLanguage(this);
                break;
            case Settings::Language::German:
                winrt::MemoryCleaner::Multilingual::SetGermanLanguage(this);
                break;
            case Settings::Language::Italian:
                winrt::MemoryCleaner::Multilingual::SetItalianLanguage(this);
                break;
            case Settings::Language::Arabic:
                winrt::MemoryCleaner::Multilingual::SetArabicLanguage(this);
                break;
            case Settings::Language::Korean:
                winrt::MemoryCleaner::Multilingual::SetKoreanLanguage(this);
                break;
            case Settings::Language::Japanese:
                winrt::MemoryCleaner::Multilingual::SetJapaneseLanguage(this);
                break;
        }
        
        // 重新创建托盘菜单以应用新的语言设置
        CreateTrayMenu();
    }

    void MainWindow::InitializeMemoryMonitoring()
    {
        memoryTimer = Microsoft::UI::Xaml::DispatcherTimer();
        memoryTimer.Interval(std::chrono::milliseconds(1000)); // 每秒更新一次
        memoryTimer.Tick({ this, &MainWindow::UpdateMemoryUsage });
        memoryTimer.Start();
    }

    void MainWindow::UpdateTrayIcon(double memoryPercentage, double virtualUsage,
        double privateWS_GB, double sharedWS_GB, double totalWS_GB)
    {
        auto windowNative{ this->try_as<::IWindowNative>() };
        if (!windowNative) return;

        HWND hWnd{ nullptr };
        windowNative->get_WindowHandle(&hWnd);
        if (!hWnd) return;

        // 确保nid已初始化
        if (nid.hWnd != hWnd) {
            nid = { sizeof(NOTIFYICONDATA) }; // 重新初始化
            nid.hWnd = hWnd;
            nid.uID = 1;
            nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
            nid.uCallbackMessage = WM_APP + 1;
        }

        // 选择颜色基于内存使用率
        COLORREF bgColor = RGB(0x0, 0x80, 0x40); // 绿色 (0-59%)
        if (memoryPercentage >= 90.0) bgColor = RGB(0xD3, 0x2F, 0x2F); // 红色 (90-100%)
        else if (memoryPercentage >= 60.0) bgColor = RGB(0xF5, 0x7C, 0x00); // 橙色 (60-89%)

        HDC hdc = GetDC(NULL);
        if (!hdc) return;

        // 使用智能指针管理HDC
        UniqueHDC guardHdc(hdc, &DeleteDC);

        // 使用智能指针管理内存DC
        UniqueHDC memDC(CreateCompatibleDC(hdc), &DeleteDC);
        UniqueHDC maskDC(CreateCompatibleDC(hdc), &DeleteDC);

        if (!memDC || !maskDC) return;

        // 创建位图 - 将尺寸从32x32增加到48x48
        UniqueHBITMAP memBitmap(CreateCompatibleBitmap(hdc, 48, 48));
        UniqueHBITMAP maskBitmap(CreateCompatibleBitmap(hdc, 48, 48));

        if (!memBitmap || !maskBitmap) return;

        // 选择对象到DC中（需要保存旧对象以便恢复）
        HBITMAP oldMemBitmap = static_cast<HBITMAP>(SelectObject(memDC.get(), memBitmap.get()));
        HBITMAP oldMaskBitmap = static_cast<HBITMAP>(SelectObject(maskDC.get(), maskBitmap.get()));

        // 绘制背景 - 矩形大小从32x32增加到48x48
        HBRUSH bgBrush = CreateSolidBrush(bgColor);
        RECT rect = { 0, 0, 48, 48 };
        FillRect(memDC.get(), &rect, bgBrush);
        DeleteObject(bgBrush);

        // 绘制文本
        SetBkMode(memDC.get(), TRANSPARENT);
        SetTextColor(memDC.get(), RGB(255, 255, 255));

        // 使用智能指针管理字体 - 增大字体大小以适应更大的图标
        UniqueHFONT hFont(CreateFont(45, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, L"Arial"));

        HFONT oldFont = static_cast<HFONT>(SelectObject(memDC.get(), hFont.get()));

        wchar_t text[10];
        swprintf_s(text, L"%.0f", memoryPercentage);

        SIZE textSize;
        GetTextExtentPoint32W(memDC.get(), text, wcslen(text), &textSize);
        int x = (48 - textSize.cx) / 2; // 修改为48以匹配新的图标大小
        int y = 0;

        TextOutW(memDC.get(), x, y, text, wcslen(text));

        // 创建掩码（纯黑）- 矩形大小从32x32增加到48x48
        HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
        FillRect(maskDC.get(), &rect, blackBrush);
        DeleteObject(blackBrush);

        // 创建图标
        ICONINFO iconInfo = {};
        iconInfo.fIcon = TRUE;
        iconInfo.hbmMask = maskBitmap.get();
        iconInfo.hbmColor = memBitmap.get();

        // 使用智能指针管理图标
        UniqueHICON hIcon(CreateIconIndirect(&iconInfo));
        if (!hIcon) return;

        // 恢复DC中的旧对象
        SelectObject(memDC.get(), oldMemBitmap);
        SelectObject(memDC.get(), oldFont);
        SelectObject(maskDC.get(), oldMaskBitmap);

        // 更新托盘图标
        if (nid.hIcon) DestroyIcon(nid.hIcon);
        nid.hIcon = hIcon.release(); // 转移所有权给nid

        // 更新提示文本
        wchar_t tip[256];
        
        // 从XAML资源中获取文本 - 修复资源访问方式
        auto grid = this->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>();
        auto resources = grid.Resources();
        auto physicalMemoryText = resources.Lookup(winrt::box_value(L"TrayPhysicalMemoryText")).as<winrt::hstring>();
        auto virtualMemoryText = resources.Lookup(winrt::box_value(L"TrayVirtualMemoryText")).as<winrt::hstring>();
        auto systemWorkingSetText = resources.Lookup(winrt::box_value(L"TraySystemWorkingSetText")).as<winrt::hstring>();
        
        swprintf_s(tip, L"%s %.1f%%\n%s %.1f%%\n%s %.2f GB / %.2f GB",
            physicalMemoryText.c_str(), memoryPercentage, 
            virtualMemoryText.c_str(), virtualUsage, 
            systemWorkingSetText.c_str(), privateWS_GB, totalWS_GB);

        wcscpy_s(nid.szTip, tip);

        Shell_NotifyIcon(NIM_MODIFY, &nid);
    }


    void MainWindow::UpdateMemoryUsage(IInspectable const&, IInspectable const&)
    {
        // 获取内存使用信息
        double usagePercentage = GetPhysicalMemoryUsagePercentage();
        double virtualUsage = GetVirtualMemoryUsagePercentage();

        uint64_t totalWS = 0, sharedWS = 0, privateWS = 0;
        getSystemWorkingSetInfo(totalWS, sharedWS, privateWS);

        // 获取虚拟内存信息
        MEMORYSTATUSEX memStatus;
        memStatus.dwLength = sizeof(memStatus);
        uint64_t totalVirtual = 0, usedVirtual = 0;
        uint64_t totalPhysical = 0, usedPhysical = 0;

        if (GlobalMemoryStatusEx(&memStatus))
        {
            totalVirtual = memStatus.ullTotalPageFile;
            uint64_t availableVirtual = memStatus.ullAvailPageFile;
            usedVirtual = totalVirtual - availableVirtual;

            // 获取物理内存信息
            totalPhysical = memStatus.ullTotalPhys;
            uint64_t availablePhysical = memStatus.ullAvailPhys;
            usedPhysical = totalPhysical - availablePhysical;
        }

        // 计算系统工作集使用率
        double systemWSUsage = (totalWS > 0) ? (static_cast<double>(privateWS) / static_cast<double>(totalWS)) * 100.0 : 0.0;

        // 在 UI 线程上更新控件
        this->DispatcherQueue().TryEnqueue([this, usagePercentage, virtualUsage, privateWS, sharedWS, totalWS, systemWSUsage, usedVirtual, totalVirtual, usedPhysical, totalPhysical]() {
            // 更新物理内存文本显示 - 修改为GB格式
            double totalPhysical_GB = static_cast<double>(totalPhysical) / (1024.0 * 1024.0 * 1024.0);
            double usedPhysical_GB = static_cast<double>(usedPhysical) / (1024.0 * 1024.0 * 1024.0);

            wchar_t physicalBuffer[50];
            swprintf_s(physicalBuffer, L"%.2f GB / %.2f GB", usedPhysical_GB, totalPhysical_GB);
            lbPhysicalValue().Text(physicalBuffer);

            // 更新物理内存进度条
            pbPhysical().Value(usagePercentage);

            // 更新虚拟内存文本显示
            double totalVirtual_GB = static_cast<double>(totalVirtual) / (1024.0 * 1024.0 * 1024.0);
            double usedVirtual_GB = static_cast<double>(usedVirtual) / (1024.0 * 1024.0 * 1024.0);

            wchar_t virtualBuffer[50];
            swprintf_s(virtualBuffer, L"%.2f GB / %.2f GB", usedVirtual_GB, totalVirtual_GB);
            lbVirtualValue().Text(virtualBuffer);

            // 更新虚拟内存进度条
            pbVirtual().Value(virtualUsage);

            // 更新系统工作集文本显示
            double totalWS_GB = static_cast<double>(totalWS) / (1024.0 * 1024.0 * 1024.0);
            double sharedWS_GB = static_cast<double>(sharedWS) / (1024.0 * 1024.0 * 1024.0);
            double privateWS_GB = static_cast<double>(privateWS) / (1024.0 * 1024.0 * 1024.0);

            wchar_t systemWSBuffer[150];
            swprintf_s(systemWSBuffer, L"%.2f GB / %.2f GB",
                privateWS_GB, totalWS_GB);
            lbSystemWSValue().Text(systemWSBuffer);

            // 更新系统工作集进度条
            pbSystemWS().Value(systemWSUsage);

            // 更新托盘图标
            UpdateTrayIcon(usagePercentage, virtualUsage, privateWS_GB, sharedWS_GB, totalWS_GB);
            });
    }

    double MainWindow::GetVirtualMemoryUsagePercentage()
    {
        MEMORYSTATUSEX memStatus;
        memStatus.dwLength = sizeof(memStatus);

        if (GlobalMemoryStatusEx(&memStatus))
        {
            double total = static_cast<double>(memStatus.ullTotalPageFile);
            double available = static_cast<double>(memStatus.ullAvailPageFile);
            double used = total - available;

            if (total > 0)
            {
                return (used / total) * 100.0;
            }
        }

        return 0.0;
    }

    double MainWindow::GetPhysicalMemoryUsagePercentage()
    {
        MEMORYSTATUSEX memStatus;
        memStatus.dwLength = sizeof(memStatus);

        if (GlobalMemoryStatusEx(&memStatus))
        {
            double total = static_cast<double>(memStatus.ullTotalPhys);
            double available = static_cast<double>(memStatus.ullAvailPhys);
            double used = total - available;

            return (used / total) * 100.0;
        }

        return 0.0;
    }

    bool MainWindow::getSystemWorkingSetInfo(uint64_t& totalWS,
        uint64_t& sharedWS,
        uint64_t& privateWS) {
        totalWS = sharedWS = privateWS = 0;
        
        // 使用性能计数器获取更准确的系统内存信息
        PDH_HQUERY cpuQuery;
        PDH_HCOUNTER totalCounter;
        PDH_HCOUNTER sharedCounter;
        
        if (PdhOpenQuery(NULL, 0, &cpuQuery) == ERROR_SUCCESS) {
            // 手动实现PDH查询句柄的RAII包装器
            struct PdhQueryDeleter {
                void operator()(PDH_HQUERY query) const {
                    if (query) PdhCloseQuery(query);
                }
            };
            std::unique_ptr<std::remove_pointer<PDH_HQUERY>::type, PdhQueryDeleter> uniqueQuery(cpuQuery);
            
            // 添加计数器
            if (PdhAddEnglishCounter(uniqueQuery.get(), L"\\Memory\\Committed Bytes", 0, &totalCounter) == ERROR_SUCCESS &&
                PdhAddEnglishCounter(uniqueQuery.get(), L"\\Memory\\Shared Bytes", 0, &sharedCounter) == ERROR_SUCCESS) {
                
                // 收集数据
                PdhCollectQueryData(uniqueQuery.get());
                
                PDH_FMT_COUNTERVALUE counterVal;
                
                // 获取总内存
                if (PdhGetFormattedCounterValue(totalCounter, PDH_FMT_LARGE, NULL, &counterVal) == ERROR_SUCCESS) {
                    totalWS = counterVal.largeValue;
                }
                
                // 获取共享内存
                if (PdhGetFormattedCounterValue(sharedCounter, PDH_FMT_LARGE, NULL, &counterVal) == ERROR_SUCCESS) {
                    sharedWS = counterVal.largeValue;
                }
            }
            
            PdhCloseQuery(cpuQuery);
        }
        
        // 如果性能计数器失败，回退到原来的方法
        if (totalWS == 0) {
            HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            if (snap == INVALID_HANDLE_VALUE) return false;
            
            PROCESSENTRY32 pe{};
            pe.dwSize = sizeof(pe);
            if (!Process32First(snap, &pe)) {
                CloseHandle(snap);
                return false;
            }
            
            do {
                if (pe.th32ProcessID == 0) continue;
                HANDLE h = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe.th32ProcessID);
                if (!h) continue;
                
                // 使用 RAII 确保句柄被关闭
                struct HandleGuard {
                    HANDLE handle;
                    HandleGuard(HANDLE h) : handle(h) {}
                    ~HandleGuard() { if (handle) CloseHandle(handle); }
                };
                
                HandleGuard guard(h);
                
                PROCESS_MEMORY_COUNTERS_EX pmc{};
                if (GetProcessMemoryInfo(h,
                    reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc),
                    sizeof(pmc))) {
                    totalWS += static_cast<uint64_t>(pmc.WorkingSetSize);
                }
            } while (Process32Next(snap, &pe));
            
            CloseHandle(snap);
            
            // 使用更精确的估算，基于系统内存状态
            MEMORYSTATUSEX memStatus = { sizeof(memStatus) };
            if (GlobalMemoryStatusEx(&memStatus)) {
                // 根据系统内存压力调整共享内存比例
                double memoryPressure = static_cast<double>(memStatus.dwMemoryLoad) / 100.0;
                // 内存压力越大，共享内存比例越高（范围 20%-40%）
                double sharedRatio = 0.2 + (0.2 * memoryPressure);
                sharedWS = static_cast<uint64_t>(totalWS * sharedRatio);
            } else {
                // 默认保守估算
                sharedWS = static_cast<uint64_t>(totalWS * 0.3);
            }
        }
        
        privateWS = totalWS - sharedWS;
        return true;
    }

    void MainWindow::CleanButton_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        PerformCleanup();
    }

    void MainWindow::PerformCleanup()
{
    // 获取清理前的内存使用情况
    MEMORYSTATUSEX memStatusBefore = { sizeof(memStatusBefore) };
    GlobalMemoryStatusEx(&memStatusBefore);
    uint64_t availableMemoryBefore = memStatusBefore.ullAvailPhys;
    
    // 获取用户选择的清理选项
    int cleanOptions = 0;

    // 检查各个清理选项的UI控件状态并设置相应的位标志
    if (cbWorkingSet().IsChecked().GetBoolean())
        cleanOptions |= static_cast<int>(CleanOption::WorkingSet);
    if (cbSystemWorkingSet().IsChecked().GetBoolean())
        cleanOptions |= static_cast<int>(CleanOption::SystemWorkingSet);
    if (cbStandbyList().IsChecked().GetBoolean())
        cleanOptions |= static_cast<int>(CleanOption::StandbyList);
    if (cbStandbyListLow().IsChecked().GetBoolean())
        cleanOptions |= static_cast<int>(CleanOption::StandbyListLowPriority);
    if (cbModifiedPageList().IsChecked().GetBoolean())
        cleanOptions |= static_cast<int>(CleanOption::ModifiedPageList);
    if (cbCombinePageList().IsChecked().GetBoolean())
        cleanOptions |= static_cast<int>(CleanOption::CombinePageList);

    // 执行清理操作
    for (int option = 1; option <= 32; option <<= 1)
    {
        if (cleanOptions & option)
        {
            switch (static_cast<CleanOption>(option))
            {
            case CleanOption::WorkingSet:
                ::MemoryCleaner::cleanWorkingSet();
                break;

            case CleanOption::SystemWorkingSet:
                ::MemoryCleaner::cleanSystemWorkingSet();
                break;

            case CleanOption::StandbyList:
                ::MemoryCleaner::cleanStandbyList(false);
                break;

            case CleanOption::StandbyListLowPriority:
                ::MemoryCleaner::cleanStandbyList(true);
                break;

            case CleanOption::ModifiedPageList:
                ::MemoryCleaner::cleanModifiedPageList();
                break;

            case CleanOption::CombinePageList:
                ::MemoryCleaner::cleanCombinePageList();
                break;

            default:
                break;
            }
        }
    }
    
    // 获取清理后的内存使用情况
    MEMORYSTATUSEX memStatusAfter = { sizeof(memStatusAfter) };
    GlobalMemoryStatusEx(&memStatusAfter);
    uint64_t availableMemoryAfter = memStatusAfter.ullAvailPhys;
    
    // 计算清理的内存量（以MB为单位）
    uint64_t totalCleanedMB = (availableMemoryAfter - availableMemoryBefore) / (1024 * 1024);
    
    // 确保显示的清理量不为负数
    if (availableMemoryAfter <= availableMemoryBefore) {
        totalCleanedMB = 0;
    }

    // 获取当前系统时间
    SYSTEMTIME st;
    GetLocalTime(&st);
    wchar_t timeStr[20];
    swprintf_s(timeStr, L"%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);

    // 获取当前语言设置
    Settings::Language currentLanguage = m_settings->GetLanguage();
    LogTexts logTexts = winrt::MemoryCleaner::Multilingual::GetLogTexts(currentLanguage);
    
    // 格式化结果显示 - 使用多语言文本
    wchar_t resultText[100];
    swprintf_s(resultText, L"[%s] %s %llu %s", 
               timeStr, 
               logTexts.cleanedMemory.c_str(), 
               totalCleanedMB, 
               logTexts.megabytes.c_str());

    textBrowser().Text(resultText);
}

    void MainWindow::ShowAboutDialog()
{
    auto aboutDialog = winrt::MemoryCleaner::AboutDialog();
    aboutDialog.Activate();
}


    // 系统托盘相关方法实现
    void MainWindow::CreateTrayIcon()
{
    try
    {
        auto windowNative{ this->try_as<::IWindowNative>() };
        if (!windowNative) {
            OutputDebugStringW(L"CreateTrayIcon: Failed to get IWindowNative\n");
            return;
        }

        HWND hWnd{ nullptr };
        check_hresult(windowNative->get_WindowHandle(&hWnd));
        if (!hWnd) {
            OutputDebugStringW(L"CreateTrayIcon: Failed to get window handle\n");
            return;
        }

        // 确保先移除可能存在的旧图标
        RemoveTrayIcon();

        // 初始化托盘图标
        this->nid = { sizeof(NOTIFYICONDATA) };
        this->nid.hWnd = hWnd;
        this->nid.uID = 1;
        this->nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_SHOWTIP;
        this->nid.uCallbackMessage = WM_APP + 1;

        // 使用更安全的图标加载方式，根据系统主题选择图标
        bool isDarkTheme = this->IsSystemUsingDarkTheme();
        int iconResourceId = isDarkTheme ? 2 : 1; // 假设1是浅色主题图标，2是深色主题图标
        
        this->nid.hIcon = static_cast<HICON>(LoadImage(
            GetModuleHandle(NULL),
            MAKEINTRESOURCE(iconResourceId),
            IMAGE_ICON,
            GetSystemMetrics(SM_CXSMICON),
            GetSystemMetrics(SM_CYSMICON),
            LR_DEFAULTCOLOR));

        if (!this->nid.hIcon)
        {
            OutputDebugStringW(L"CreateTrayIcon: Failed to load icon from resource, using default\n");
            this->nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        }

        // 设置托盘提示文本
        wcscpy_s(this->nid.szTip, L"MemoryCleaner");

        // 使用 NIM_ADD 添加托盘图标
        if (!Shell_NotifyIcon(NIM_ADD, &this->nid))
        {
            DWORD error = GetLastError();
            wchar_t errorMsg[256];
            swprintf_s(errorMsg, L"CreateTrayIcon: Shell_NotifyIcon failed with error %d\n", error);
            OutputDebugStringW(errorMsg);
            
            // 如果失败，尝试使用 NIM_SETVERSION
            this->nid.uVersion = NOTIFYICON_VERSION_4;
            Shell_NotifyIcon(NIM_SETVERSION, &this->nid);

            // 再次尝试添加
            if (!Shell_NotifyIcon(NIM_ADD, &this->nid)) {
                error = GetLastError();
                swprintf_s(errorMsg, L"CreateTrayIcon: Second attempt failed with error %d\n", error);
                OutputDebugStringW(errorMsg);
            }
        }
        else {
            OutputDebugStringW(L"CreateTrayIcon: Successfully created tray icon\n");
        }
    }
    catch (winrt::hresult_error const& ex)
    {
        OutputDebugStringW(L"CreateTrayIcon failed: ");
        OutputDebugStringW(ex.message().c_str());
        OutputDebugStringW(L"\n");
    }
    catch (...)
    {
        OutputDebugStringW(L"CreateTrayIcon failed with unknown error\n");
    }
}

    void MainWindow::ShowTrayContextMenu()
    {
        // 使用传统托盘菜单替代WinUI3托盘菜单
        ShowTrayMenu();
    }

    void MainWindow::OnTrayCommand(WPARAM wParam)
    {
        if (isDestroying) return;

        switch (LOWORD(wParam))
        {
        case 1001: // 显示主窗口
            RestoreFromTray();
            break;
        case 1002: // 立即清理内存
            PerformCleanup();
            break;
        case 1003: // 退出
            RemoveTrayIcon();
            PostQuitMessage(0);
            break;
        }
    }

    void MainWindow::MinimizeToTray()
    {
        // 使用更可靠的方式隐藏窗口
        auto windowNative{ this->try_as<::IWindowNative>() };
        if (windowNative)
        {
            HWND hWnd{ nullptr };
            windowNative->get_WindowHandle(&hWnd);
            if (hWnd)
            {
                // 确保托盘图标使用正确的窗口句柄
                nid.hWnd = hWnd;
                Shell_NotifyIcon(NIM_MODIFY, &nid);

                // 隐藏窗口
                ShowWindow(hWnd, SW_HIDE);

                OutputDebugStringW(L"[Tray] Window minimized to tray\n");
            }
            else
            {
                OutputDebugStringW(L"[Tray] Failed to get window handle\n");
            }
        }
        else
        {
            OutputDebugStringW(L"[Tray] Failed to get IWindowNative\n");
        }
        isInTray = true;
    }
    

    void MainWindow::RestoreFromTray()
    {
        auto windowNative{ this->try_as<::IWindowNative>() };
        if (windowNative)
        {
            HWND hWnd{ nullptr };
            windowNative->get_WindowHandle(&hWnd);
            if (hWnd)
            {
                ShowWindow(hWnd, SW_SHOW);
                SetForegroundWindow(hWnd);

                // 确保托盘图标使用正确的窗口句柄
                nid.hWnd = hWnd;
                Shell_NotifyIcon(NIM_MODIFY, &nid);
            }
        }
        isInTray = false;
        isWindowClosed = false; // 重置窗口关闭标志

        // 同步清理选项状态
        SyncCleanupOptions();
    }

    void MainWindow::SyncCleanupOptions()
    {
        // 保存当前清理选项
        SaveCurrentCleanupOptions();

        // 更新传统托盘菜单以反映选项变化
        CreateTrayMenu();
    }

    void MainWindow::OnTrayShowWindow(IInspectable const& sender, RoutedEventArgs const& e)
    {
        RestoreFromTray();
    }

    void MainWindow::OnTrayCleanMemory(IInspectable const& sender, RoutedEventArgs const& e)
    {
        PerformCleanup();
    }

    void MainWindow::OnTrayExit(IInspectable const& sender, RoutedEventArgs const& e)
    {
        RemoveTrayIcon();
        PostQuitMessage(0);
    }

    LRESULT CALLBACK MainWindow::SubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
    {
        auto pThis = reinterpret_cast<MainWindow*>(dwRefData);

        if (uMsg == WM_GETMINMAXINFO) // 设置窗口最小和最大尺寸
        {
            auto pMinMaxInfo = reinterpret_cast<MINMAXINFO*>(lParam);
            pMinMaxInfo->ptMinTrackSize.x = pThis->minWindowWidth;
            pMinMaxInfo->ptMinTrackSize.y = pThis->minWindowHeight;
            pMinMaxInfo->ptMaxTrackSize.x = pThis->maxWindowWidth;  // 设置最大宽度与最小宽度相同
            pMinMaxInfo->ptMaxTrackSize.y = pThis->maxWindowHeight; // 设置最大高度与最小高度相同
            return 0;
        }
        else if (uMsg == WM_SYSCOMMAND) // 处理系统命令
    {
        // 检查是否是最大化命令
        if (wParam == SC_MAXIMIZE)
        {
            // 阻止窗口最大化，从而禁用全屏功能
            return 0;
        }
    }
    else if (uMsg == (WM_APP + 1)) // 托盘图标消息
    {
        OutputDebugStringW(L"Received tray icon message\n");
        if (lParam == WM_RBUTTONUP) // 右键点击
        {
            pThis->ShowTrayMenu();  // 使用传统托盘菜单
            return 0;
        }
        else if (lParam == WM_LBUTTONDBLCLK) // 左键双击
        {
            pThis->RestoreFromTray();
            return 0;
        }
    }
        else if (uMsg == WM_COMMAND) // 处理菜单命令
        {
            pThis->HandleTrayMenuCommand(wParam);
            return 0;
        }
        else if (uMsg == WM_SETTINGCHANGE) // 处理系统设置变化消息
        {
            // 检查是否是主题变化
            if (lParam && wcscmp(reinterpret_cast<LPCWSTR>(lParam), L"ImmersiveColorSet") == 0)
            {
                pThis->HandleSystemThemeChange();
            }
            return DefSubclassProc(hWnd, uMsg, wParam, lParam);
        }
        else if (uMsg == WM_CLOSE) // 处理窗口关闭消息
        {
            if (!pThis->isInTray)
            {
                pThis->MinimizeToTray();
                pThis->isWindowClosed = true;

                // 重新设置托盘图标的窗口句柄，确保后续消息能正确处理
                auto windowNative = pThis->try_as<::IWindowNative>();
                if (windowNative)
                {
                    HWND hWnd{ nullptr };
                    windowNative->get_WindowHandle(&hWnd);
                    if (hWnd)
                    {
                        pThis->nid.hWnd = hWnd;
                        Shell_NotifyIcon(NIM_MODIFY, &pThis->nid);
                    }
                }
            }
            return 0;
        }
        else if (uMsg == WM_DESTROY)
        {
            // 确保移除子类化
            RemoveWindowSubclass(hWnd, &MainWindow::SubclassProc, uIdSubclass);
        }

    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
    }


    void MainWindow::RemoveTrayIcon()
    {
        try
        {
            if (nid.hWnd && nid.uID)
            {
                // 先尝试修改，再删除
                nid.uFlags = 0;
                Shell_NotifyIcon(NIM_MODIFY, &nid);

                Shell_NotifyIcon(NIM_DELETE, &nid);

                if (nid.hIcon)
                {
                    DestroyIcon(nid.hIcon);
                    nid.hIcon = nullptr;
                }

                // 重置结构体但不完全清零
                nid.cbSize = sizeof(NOTIFYICONDATA);
                nid.uFlags = 0;
                nid.szTip[0] = L'\0';
            }
        }
        catch (...)
        {
            // 忽略清理过程中的异常
        }
    }

    // 检测系统是否使用深色主题
    bool MainWindow::IsSystemUsingDarkTheme()
    {
        // 使用 Windows API 检测系统主题
        HKEY hKey;
        if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
        {
            DWORD value = 0;
            DWORD size = sizeof(DWORD);
            if (RegQueryValueEx(hKey, L"AppsUseLightTheme", nullptr, nullptr, reinterpret_cast<BYTE*>(&value), &size) == ERROR_SUCCESS)
            {
                RegCloseKey(hKey);
                // 如果值为0，表示使用深色主题；如果值为1，表示使用浅色主题
                return value == 0;
            }
            RegCloseKey(hKey);
        }

        // 默认返回浅色主题
        return false;
    }

    // 更新托盘图标主题
    void MainWindow::UpdateTrayIconTheme()
    {
        try
        {
            // 检测当前系统主题
            bool isDarkTheme = IsSystemUsingDarkTheme();
            
            // 根据主题选择不同的图标资源
            int iconResourceId = isDarkTheme ? 2 : 1; // 假设1是浅色主题图标，2是深色主题图标
            
            // 加载对应主题的图标
            HICON hNewIcon = static_cast<HICON>(LoadImage(
                GetModuleHandle(NULL),
                MAKEINTRESOURCE(iconResourceId),
                IMAGE_ICON,
                GetSystemMetrics(SM_CXSMICON),
                GetSystemMetrics(SM_CYSMICON),
                LR_DEFAULTCOLOR));
                
            if (hNewIcon)
            {
                // 更新托盘图标
                if (nid.hWnd && nid.uID)
                {
                    // 释放旧图标
                    if (nid.hIcon)
                    {
                        DestroyIcon(nid.hIcon);
                    }
                    
                    // 设置新图标
                    nid.hIcon = hNewIcon;
                    nid.uFlags = NIF_ICON;
                    
                    // 更新托盘图标
                    Shell_NotifyIcon(NIM_MODIFY, &nid);
                }
            }
        }
        catch (winrt::hresult_error const& ex)
        {
            OutputDebugStringW(L"UpdateTrayIconTheme failed: ");
            OutputDebugStringW(ex.message().c_str());
            OutputDebugStringW(L"\n");
        }
        catch (...)
        {
            OutputDebugStringW(L"UpdateTrayIconTheme failed with unknown error\n");
        }
    }

    // 处理系统主题变化
    void MainWindow::HandleSystemThemeChange()
    {
        try
        {
            // 当系统主题变化时，更新托盘图标主题
            UpdateTrayIconTheme();
            OutputDebugStringW(L"System theme changed, tray icon updated\n");
        }
        catch (winrt::hresult_error const& ex)
        {
            OutputDebugStringW(L"HandleSystemThemeChange failed: ");
            OutputDebugStringW(ex.message().c_str());
            OutputDebugStringW(L"\n");
        }
        catch (...)
        {
            OutputDebugStringW(L"HandleSystemThemeChange failed with unknown error\n");
        }
    }



    // 实现English按钮点击事件处理
    void MainWindow::OnLanguageEnglishClick(IInspectable const& sender, RoutedEventArgs const& e)
    {
        if (m_settings) {
            m_settings->SetLanguage(Settings::Language::English);
        }
        winrt::MemoryCleaner::Multilingual::OnLanguageEnglishClick(this, sender, e);
        CreateTrayMenu(); // 添加这行
    }

    // 实现简体中文按钮点击事件处理
    void MainWindow::OnLanguageChineseSimplifiedClick(IInspectable const& sender, RoutedEventArgs const& e)
    {
        if (m_settings) {
            m_settings->SetLanguage(Settings::Language::ChineseSimplified);
        }
        winrt::MemoryCleaner::Multilingual::OnLanguageChineseSimplifiedClick(this, sender, e);
        CreateTrayMenu(); // 添加这行
    }

    // 实现繁体中文按钮点击事件处理
    void MainWindow::OnLanguageChineseTraditionalClick(IInspectable const& sender, RoutedEventArgs const& e)
    {
        if (m_settings) {
            m_settings->SetLanguage(Settings::Language::ChineseTraditional);
        }
        winrt::MemoryCleaner::Multilingual::OnLanguageChineseTraditionalClick(this, sender, e);
        CreateTrayMenu(); // 添加这行
    }

    // 实现蒙古语按钮点击事件处理
    void MainWindow::OnLanguageMongolianClick(IInspectable const& sender, RoutedEventArgs const& e)
    {
        if (m_settings) {
            m_settings->SetLanguage(Settings::Language::Mongolian);
        }
        winrt::MemoryCleaner::Multilingual::OnLanguageMongolianClick(this, sender, e);
        CreateTrayMenu(); // 添加这行
    }

    // 实现维吾尔语按钮点击事件处理
    void MainWindow::OnLanguageUyghurClick(IInspectable const& sender, RoutedEventArgs const& e)
    {
        if (m_settings) {
            m_settings->SetLanguage(Settings::Language::Uyghur);
        }
        winrt::MemoryCleaner::Multilingual::OnLanguageUyghurClick(this, sender, e);
        CreateTrayMenu(); // 添加这行
    }

    // 实现法语按钮点击事件处理
    void MainWindow::OnLanguageFrenchClick(IInspectable const& sender, RoutedEventArgs const& e)
    {
        if (m_settings) {
            m_settings->SetLanguage(Settings::Language::French);
        }
        winrt::MemoryCleaner::Multilingual::OnLanguageFrenchClick(this, sender, e);
        CreateTrayMenu(); // 添加这行
    }

    // 实现俄语按钮点击事件处理
    void MainWindow::OnLanguageRussianClick(IInspectable const& sender, RoutedEventArgs const& e)
    {
        if (m_settings) {
            m_settings->SetLanguage(Settings::Language::Russian);
        }
        winrt::MemoryCleaner::Multilingual::OnLanguageRussianClick(this, sender, e);
        CreateTrayMenu(); // 添加这行
    }

    // 实现西班牙语按钮点击事件处理
    void MainWindow::OnLanguageSpanishClick(IInspectable const& sender, RoutedEventArgs const& e)
    {
        if (m_settings) {
            m_settings->SetLanguage(Settings::Language::Spanish);
        }
        winrt::MemoryCleaner::Multilingual::OnLanguageSpanishClick(this, sender, e);
        CreateTrayMenu(); // 添加这行
    }

    // 实现德语按钮点击事件处理
    void MainWindow::OnLanguageGermanClick(IInspectable const& sender, RoutedEventArgs const& e)
    {
        if (m_settings) {
            m_settings->SetLanguage(Settings::Language::German);
        }
        winrt::MemoryCleaner::Multilingual::OnLanguageGermanClick(this, sender, e);
        CreateTrayMenu(); // 添加这行
    }

    // 实现意大利语按钮点击事件处理
    void MainWindow::OnLanguageItalianClick(IInspectable const& sender, RoutedEventArgs const& e)
    {
        if (m_settings) {
            m_settings->SetLanguage(Settings::Language::Italian);
        }
        winrt::MemoryCleaner::Multilingual::OnLanguageItalianClick(this, sender, e);
        CreateTrayMenu(); // 添加这行
    }

    // 实现阿拉伯语按钮点击事件处理
    void MainWindow::OnLanguageArabicClick(IInspectable const& sender, RoutedEventArgs const& e)
    {
        if (m_settings) {
            m_settings->SetLanguage(Settings::Language::Arabic);
        }
        winrt::MemoryCleaner::Multilingual::OnLanguageArabicClick(this, sender, e);
        CreateTrayMenu(); // 添加这行
    }

    // 实现韩语按钮点击事件处理
    void MainWindow::OnLanguageKoreanClick(IInspectable const& sender, RoutedEventArgs const& e)
    {
        if (m_settings) {
            m_settings->SetLanguage(Settings::Language::Korean);
        }
        winrt::MemoryCleaner::Multilingual::OnLanguageKoreanClick(this, sender, e);
        CreateTrayMenu(); // 添加这行
    }

    // 实现日语按钮点击事件处理
    void MainWindow::OnLanguageJapaneseClick(IInspectable const& sender, RoutedEventArgs const& e)
    {
        if (m_settings) {
            m_settings->SetLanguage(Settings::Language::Japanese);
        }
        winrt::MemoryCleaner::Multilingual::OnLanguageJapaneseClick(this, sender, e);
        CreateTrayMenu(); // 添加这行
    }

    void MainWindow::CreateTrayMenu()
    {
        // 销毁可能存在的旧菜单
        DestroyTrayMenu();

        // 创建托盘菜单
        m_hTrayMenu = CreatePopupMenu();
        if (!m_hTrayMenu) return;

        // 获取当前语言设置和对应的菜单文本
        Settings::Language currentLanguage = m_settings ? m_settings->GetLanguage() : Settings::Language::ChineseSimplified;
        TrayMenuTexts texts = winrt::MemoryCleaner::Multilingual::GetTrayMenuTexts(currentLanguage);

        // 添加菜单项
        AppendMenu(m_hTrayMenu, MF_STRING, 1001, texts.showMainWindow.c_str());
        AppendMenu(m_hTrayMenu, MF_SEPARATOR, 0, NULL);

        // 添加清理选项子菜单
        HMENU hCleanupMenu = CreatePopupMenu();
        if (hCleanupMenu)
        {
            // 获取当前清理选项状态
            bool workingSet = cbWorkingSet().IsChecked().Value();
            bool systemWorkingSet = cbSystemWorkingSet().IsChecked().Value();
            bool standbyList = cbStandbyList().IsChecked().Value();
            bool standbyListLow = cbStandbyListLow().IsChecked().Value();
            bool modifiedPageList = cbModifiedPageList().IsChecked().Value();
            bool combinePageList = cbCombinePageList().IsChecked().Value();

            // 添加清理选项菜单项
            AppendMenu(hCleanupMenu, MF_STRING | (workingSet ? MF_CHECKED : MF_UNCHECKED), 2001, texts.workingSet.c_str());
            AppendMenu(hCleanupMenu, MF_STRING | (systemWorkingSet ? MF_CHECKED : MF_UNCHECKED), 2002, texts.systemWorkingSet.c_str());
            AppendMenu(hCleanupMenu, MF_STRING | (standbyList ? MF_CHECKED : MF_UNCHECKED), 2003, texts.standbyList.c_str());
            AppendMenu(hCleanupMenu, MF_STRING | (standbyListLow ? MF_CHECKED : MF_UNCHECKED), 2004, texts.standbyListLow.c_str());
            AppendMenu(hCleanupMenu, MF_STRING | (modifiedPageList ? MF_CHECKED : MF_UNCHECKED), 2005, texts.modifiedPageList.c_str());
            AppendMenu(hCleanupMenu, MF_STRING | (combinePageList ? MF_CHECKED : MF_UNCHECKED), 2006, texts.combinePageList.c_str());

            // 将子菜单添加到主菜单
            AppendMenu(m_hTrayMenu, MF_POPUP, reinterpret_cast<UINT_PTR>(hCleanupMenu), texts.cleanOptions.c_str());
        }

        AppendMenu(m_hTrayMenu, MF_STRING, 1002, texts.cleanNow.c_str());
        AppendMenu(m_hTrayMenu, MF_SEPARATOR, 0, NULL);
        AppendMenu(m_hTrayMenu, MF_STRING, 1003, texts.exit.c_str());
    }
    // 显示传统托盘菜单
void MainWindow::ShowTrayMenu()
{
    if (!m_hTrayMenu || !IsWindow(nid.hWnd)) return;

    // 获取当前鼠标位置
    POINT pt;
    GetCursorPos(&pt);

    // 设置前景窗口，确保菜单能正确接收命令
    SetForegroundWindow(nid.hWnd);

    // 显示托盘菜单并获取用户选择的菜单项ID
    LRESULT result = TrackPopupMenu(m_hTrayMenu,
        TPM_RIGHTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD,
        pt.x, pt.y, 0, nid.hWnd, NULL);

    // 如果用户选择了菜单项，处理相应的命令
    if (result != 0)
    {
        HandleTrayMenuCommand(result);
    }
}

    // 处理WM_DRAWITEM消息，用于自定义绘制菜单项

    void MainWindow::DestroyTrayMenu()
    {
        if (m_hTrayMenu)
        {
            // 销毁子菜单
            int itemCount = GetMenuItemCount(m_hTrayMenu);
            for (int i = 0; i < itemCount; i++)
            {
                HMENU hSubMenu = GetSubMenu(m_hTrayMenu, i);
                if (hSubMenu)
                {
                    DestroyMenu(hSubMenu);
                }
            }

            // 销毁主菜单
            DestroyMenu(m_hTrayMenu);
            m_hTrayMenu = nullptr;
        }
    }

    // 处理传统托盘菜单命令
    void MainWindow::HandleTrayMenuCommand(WPARAM wParam)
    {
        switch (LOWORD(wParam))
        {
        case 1001: // 显示主窗口
            RestoreFromTray();
            break;
        case 1002: // 立即清理内存
            PerformCleanup();
            break;
        case 1003: // 退出
            RemoveTrayIcon();
            PostQuitMessage(0);
            break;

            // 清理选项菜单项
        case 2001: // 工作集
            cbWorkingSet().IsChecked(!cbWorkingSet().IsChecked().Value());
            SaveCurrentCleanupOptions();
            break;
        case 2002: // 系统工作集
            cbSystemWorkingSet().IsChecked(!cbSystemWorkingSet().IsChecked().Value());
            SaveCurrentCleanupOptions();
            break;
        case 2003: // 备用列表
            cbStandbyList().IsChecked(!cbStandbyList().IsChecked().Value());
            SaveCurrentCleanupOptions();
            break;
        case 2004: // 低优先级备用列表
            cbStandbyListLow().IsChecked(!cbStandbyListLow().IsChecked().Value());
            SaveCurrentCleanupOptions();
            break;
        case 2005: // 修改页面列表
            cbModifiedPageList().IsChecked(!cbModifiedPageList().IsChecked().Value());
            SaveCurrentCleanupOptions();
            break;
        case 2006: // 组合页面列表
            cbCombinePageList().IsChecked(!cbCombinePageList().IsChecked().Value());
            SaveCurrentCleanupOptions();
            break;
        }

        // 更新托盘菜单以反映选项变化
        CreateTrayMenu();
    }

 
// 添加辅助方法来查找菜单项
winrt::Microsoft::UI::Xaml::Controls::MenuFlyoutItem FindMenuItemByName(
    winrt::Microsoft::UI::Xaml::Controls::MenuFlyout const& flyout, 
    std::wstring const& name)
{
    for (auto const& item : flyout.Items()) {
        if (auto menuItem = item.try_as<winrt::Microsoft::UI::Xaml::Controls::MenuFlyoutItem>()) {
            if (menuItem.Name() == name) {
                return menuItem;
            }
        }
    }
    return nullptr;
}




}






