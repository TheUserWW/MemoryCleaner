#include "pch.h"
#include "multilingual.h"
#include "MainWindow.xaml.h"
#include "settings.h"

namespace winrt::MemoryCleaner::Multilingual {


// 获取简化的日志文本
    LogTexts winrt::MemoryCleaner::Multilingual::GetLogTexts(Settings::Language language)
    {
        LogTexts texts;

        switch (language)
        {
        case Settings::Language::English:
            texts.cleanedMemory = L"Cleaned ";
            texts.megabytes = L" MB memory";
            break;
        case Settings::Language::ChineseSimplified:
            texts.cleanedMemory = L"清理了 ";
            texts.megabytes = L" MB 内存";
            break;
        case Settings::Language::ChineseTraditional:
            texts.cleanedMemory = L"清理了 ";
            texts.megabytes = L" MB 記憶體";
            break;
        case Settings::Language::Mongolian:
            texts.cleanedMemory = L"ᠴᠡᠪᠡᠷᠯᠡᠭᠰᠡᠨ ";
            texts.megabytes = L" MB ᠰᠠᠨᠠᠭᠤ ᠣᠶᠢ";
            break;
        case Settings::Language::Uyghur:
            texts.cleanedMemory = L"تازىلىدى ";
            texts.megabytes = L" MB ئەسلەك";
            break;
        case Settings::Language::French:
            texts.cleanedMemory = L"Nettoyé ";
            texts.megabytes = L" MB de mémoire";
            break;
        case Settings::Language::Russian:
            texts.cleanedMemory = L"Очищено ";
            texts.megabytes = L" MB памяти";
            break;
        case Settings::Language::Spanish:
            texts.cleanedMemory = L"Limpiado ";
            texts.megabytes = L" MB de memoria";
            break;
        case Settings::Language::German:
            texts.cleanedMemory = L"Bereinigt ";
            texts.megabytes = L" MB Speicher";
            break;
        case Settings::Language::Italian:
            texts.cleanedMemory = L"Pulito ";
            texts.megabytes = L" MB di memoria";
            break;
        case Settings::Language::Arabic:
            texts.cleanedMemory = L"تم تنظيف ";
            texts.megabytes = L" MB من الذاكرة";
            break;
        case Settings::Language::Korean:
            texts.cleanedMemory = L"정리됨 ";
            texts.megabytes = L" MB 메모리";
            break;
        case Settings::Language::Japanese:
            texts.cleanedMemory = L"クリーンアップ済み ";
            texts.megabytes = L" MB メモリ";
            break;
        default:
            texts.cleanedMemory = L"Cleaned ";
            texts.megabytes = L" MB memory";
            break;
        }

        return texts;
    }


    // 英文语言点击事件处理
    void OnLanguageEnglishClick(winrt::MemoryCleaner::implementation::MainWindow* mainWindow,
                               winrt::Windows::Foundation::IInspectable const& sender,
                               winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        SetEnglishLanguage(mainWindow);
    }


    // 简体中文语言点击事件处理
    void OnLanguageChineseSimplifiedClick(winrt::MemoryCleaner::implementation::MainWindow* mainWindow,
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        SetChineseSimplifiedLanguage(mainWindow);
    }

    // 繁体中文语言点击事件处理
    void OnLanguageChineseTraditionalClick(winrt::MemoryCleaner::implementation::MainWindow* mainWindow,
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        SetChineseTraditionalLanguage(mainWindow);
        // 添加这行代码来重新创建托盘菜单
        mainWindow->OnLanguageChanged(Settings::Language::ChineseTraditional);
    }

    // 蒙古语语言点击事件处理
    void OnLanguageMongolianClick(winrt::MemoryCleaner::implementation::MainWindow* mainWindow,
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        SetMongolianLanguage(mainWindow);
        // 添加这行代码来重新创建托盘菜单
        mainWindow->OnLanguageChanged(Settings::Language::Mongolian);
    }

    // 维吾尔语语言点击事件处理
    void OnLanguageUyghurClick(winrt::MemoryCleaner::implementation::MainWindow* mainWindow,
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        SetUyghurLanguage(mainWindow);
        // 添加这行代码来重新创建托盘菜单
        mainWindow->OnLanguageChanged(Settings::Language::Uyghur);
    }

    // 法语语言点击事件处理
    void OnLanguageFrenchClick(winrt::MemoryCleaner::implementation::MainWindow* mainWindow,
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        SetFrenchLanguage(mainWindow);
        // 添加这行代码来重新创建托盘菜单
        mainWindow->OnLanguageChanged(Settings::Language::French);
    }

    // 俄语语言点击事件处理
    void OnLanguageRussianClick(winrt::MemoryCleaner::implementation::MainWindow* mainWindow,
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        SetRussianLanguage(mainWindow);
        // 添加这行代码来重新创建托盘菜单
        mainWindow->OnLanguageChanged(Settings::Language::Russian);
    }

    // 西班牙语语言点击事件处理
    void OnLanguageSpanishClick(winrt::MemoryCleaner::implementation::MainWindow* mainWindow,
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        SetSpanishLanguage(mainWindow);
        // 添加这行代码来重新创建托盘菜单
        mainWindow->OnLanguageChanged(Settings::Language::Spanish);
    }

    // 德语语言点击事件处理
    void OnLanguageGermanClick(winrt::MemoryCleaner::implementation::MainWindow* mainWindow,
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        SetGermanLanguage(mainWindow);
        // 添加这行代码来重新创建托盘菜单
        mainWindow->OnLanguageChanged(Settings::Language::German);
    }

    // 意大利语语言点击事件处理
    void OnLanguageItalianClick(winrt::MemoryCleaner::implementation::MainWindow* mainWindow,
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        SetItalianLanguage(mainWindow);
        // 添加这行代码来重新创建托盘菜单
        mainWindow->OnLanguageChanged(Settings::Language::Italian);
    }

    // 阿拉伯语语言点击事件处理
    void OnLanguageArabicClick(winrt::MemoryCleaner::implementation::MainWindow* mainWindow,
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        SetArabicLanguage(mainWindow);
        // 添加这行代码来重新创建托盘菜单
        mainWindow->OnLanguageChanged(Settings::Language::Arabic);
    }

    // 韩语语言点击事件处理
    void OnLanguageKoreanClick(winrt::MemoryCleaner::implementation::MainWindow* mainWindow,
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        SetKoreanLanguage(mainWindow);
        // 添加这行代码来重新创建托盘菜单
        mainWindow->OnLanguageChanged(Settings::Language::Korean);
    }

    // 日语语言点击事件处理
    void OnLanguageJapaneseClick(winrt::MemoryCleaner::implementation::MainWindow* mainWindow,
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        SetJapaneseLanguage(mainWindow);
        // 添加这行代码来重新创建托盘菜单
        mainWindow->OnLanguageChanged(Settings::Language::Japanese);
    }

    // 实现设置英文界面的方法
    void SetEnglishLanguage(winrt::MemoryCleaner::implementation::MainWindow* mainWindow)
    {
        if (!mainWindow) return;
        
        try {    

            // 获取并翻译菜单栏中的"帮助"菜单项
            auto menuBar = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(1).as<winrt::Microsoft::UI::Xaml::Controls::MenuBar>();
            if (menuBar && menuBar.Items().Size() > 0) {
                auto helpMenu = menuBar.Items().GetAt(0).as<winrt::Microsoft::UI::Xaml::Controls::MenuBarItem>();
                auto LanguageMenu = menuBar.Items().GetAt(1).as<winrt::Microsoft::UI::Xaml::Controls::MenuBarItem>();
                if (helpMenu) {
                    helpMenu.Title(L"Help");
                    LanguageMenu.Title(L"Language");
                }
            }

            auto memoryStateExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(3).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (memoryStateExpander) {
                memoryStateExpander.Header(winrt::box_value(L"Memory Status"));
            }
    
            // 更新清理选项区域文
            auto cleanOptionsExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(4).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (cleanOptionsExpander) {
                cleanOptionsExpander.Header(winrt::box_value(L"Clean Options"));
            }

            // 更新控制区域文本
            auto controlExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(5).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (controlExpander) {
                controlExpander.Header(winrt::box_value(L"Control"));
            }

            mainWindow->lbSystemWSValue().Text(L"0.00 GB (Shared 0.00 GB) / 0.00 GB");
            mainWindow->SetInterval().Text(L"Interval (sec)");
            mainWindow->cbWorkingSet().Content(winrt::box_value(L"Working Set"));
            mainWindow->cbSystemWorkingSet().Content(winrt::box_value(L"System Working Set"));
            mainWindow->cbStandbyList().Content(winrt::box_value(L"Standby List"));
            mainWindow->cbStandbyListLow().Content(winrt::box_value(L"Low Priority Standby List"));
            mainWindow->cbModifiedPageList().Content(winrt::box_value(L"Modified Page List"));
            mainWindow->cbCombinePageList().Content(winrt::box_value(L"Combine Page List"));

            mainWindow->cbAutoClean().Content(winrt::box_value(L"Auto"));
            mainWindow->lbPhysicalLabel().Text(L"Physical Memory:");
            mainWindow->lbVirtualLabel().Text(L"Virtual Memory:");
            mainWindow->lbSystemWSLabel().Text(L"System Working Set:");
    

    
            mainWindow->btnCleanNow().Content(winrt::box_value(L"Clean"));
            
            // 翻译帮助菜单相关内容
            if (mainWindow->actionAbout()) {
                mainWindow->actionAbout().Text(L"About");
            }
            

            // 更新托盘提示文本资源
            auto grid = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>();
            if (grid) {
                auto resources = grid.Resources();
                resources.Insert(winrt::box_value(L"TrayPhysicalMemoryText"), winrt::box_value(L"Physical Memory:"));
                resources.Insert(winrt::box_value(L"TrayVirtualMemoryText"), winrt::box_value(L"Virtual Memory:"));
                resources.Insert(winrt::box_value(L"TraySystemWorkingSetText"), winrt::box_value(L"System Working Set:"));
            }
    
        }
        catch (const winrt::hresult_error& ex) {
            // 处理异常
        }
    }


    // 实现设置简体中文界面的方法
    void SetChineseSimplifiedLanguage(winrt::MemoryCleaner::implementation::MainWindow* mainWindow)
    {
        if (!mainWindow) return;

        try {
            auto menuBar = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(1).as<winrt::Microsoft::UI::Xaml::Controls::MenuBar>();
            if (menuBar && menuBar.Items().Size() > 0) {
                auto helpMenu = menuBar.Items().GetAt(0).as<winrt::Microsoft::UI::Xaml::Controls::MenuBarItem>();
                auto LanguageMenu = menuBar.Items().GetAt(1).as<winrt::Microsoft::UI::Xaml::Controls::MenuBarItem>();
                if (helpMenu) {
                    helpMenu.Title(L"帮助");
                    LanguageMenu.Title(L"语言");
                }
            }

            auto memoryStateExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(3).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (memoryStateExpander) {
                memoryStateExpander.Header(winrt::box_value(L"内存状态"));
            }

            auto cleanOptionsExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(4).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (cleanOptionsExpander) {
                cleanOptionsExpander.Header(winrt::box_value(L"清理选项"));
            }

            auto controlExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(5).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (controlExpander) {
                controlExpander.Header(winrt::box_value(L"控制"));
            }

            mainWindow->SetInterval().Text(L"间隔(秒)");
            mainWindow->cbWorkingSet().Content(winrt::box_value(L"工作集"));
            mainWindow->cbSystemWorkingSet().Content(winrt::box_value(L"系统工作集"));
            mainWindow->cbStandbyList().Content(winrt::box_value(L"待机列表"));
            mainWindow->cbStandbyListLow().Content(winrt::box_value(L"低优先级待机列表"));
            mainWindow->cbModifiedPageList().Content(winrt::box_value(L"已修改页面列表"));
            mainWindow->cbCombinePageList().Content(winrt::box_value(L"合并页面列表"));

            mainWindow->cbAutoClean().Content(winrt::box_value(L"自动清理"));
            mainWindow->lbPhysicalLabel().Text(L"物理内存:");
            mainWindow->lbVirtualLabel().Text(L"虚拟内存:");
            mainWindow->lbSystemWSLabel().Text(L"系统工作集:");

            mainWindow->btnCleanNow().Content(winrt::box_value(L"立即清理"));

            if (mainWindow->actionAbout()) {
                mainWindow->actionAbout().Text(L"关于");
            }

            auto grid = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>();
            if (grid) {
                auto resources = grid.Resources();
                resources.Insert(winrt::box_value(L"TrayPhysicalMemoryText"), winrt::box_value(L"物理内存 :"));
                resources.Insert(winrt::box_value(L"TrayVirtualMemoryText"), winrt::box_value(L"虚拟内存 :"));
                resources.Insert(winrt::box_value(L"TraySystemWorkingSetText"), winrt::box_value(L"系统工作集:"));
            }
        }
        catch (const winrt::hresult_error& ex) {
            // 处理异常
        }
    }

    // 实现设置繁体中文界面的方法
    void SetChineseTraditionalLanguage(winrt::MemoryCleaner::implementation::MainWindow* mainWindow)
    {
        if (!mainWindow) return;

        try {
            auto menuBar = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(1).as<winrt::Microsoft::UI::Xaml::Controls::MenuBar>();
            if (menuBar && menuBar.Items().Size() > 0) {
                auto helpMenu = menuBar.Items().GetAt(0).as<winrt::Microsoft::UI::Xaml::Controls::MenuBarItem>();
                auto LanguageMenu = menuBar.Items().GetAt(1).as<winrt::Microsoft::UI::Xaml::Controls::MenuBarItem>();
                if (helpMenu) {
                    helpMenu.Title(L"幫助");
                    LanguageMenu.Title(L"語言");
                }
            }

            auto memoryStateExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(3).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (memoryStateExpander) {
                memoryStateExpander.Header(winrt::box_value(L"記憶體狀態"));
            }

            auto cleanOptionsExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(4).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (cleanOptionsExpander) {
                cleanOptionsExpander.Header(winrt::box_value(L"清理選項"));
            }

            auto controlExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(5).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (controlExpander) {
                controlExpander.Header(winrt::box_value(L"控制"));
            }

            mainWindow->SetInterval().Text(L"間隔(秒)");
            mainWindow->cbWorkingSet().Content(winrt::box_value(L"工作集"));
            mainWindow->cbSystemWorkingSet().Content(winrt::box_value(L"系統工作集"));
            mainWindow->cbStandbyList().Content(winrt::box_value(L"待機列表"));
            mainWindow->cbStandbyListLow().Content(winrt::box_value(L"低優先級待機列表"));
            mainWindow->cbModifiedPageList().Content(winrt::box_value(L"已修改頁面列表"));
            mainWindow->cbCombinePageList().Content(winrt::box_value(L"合併頁面列表"));

            mainWindow->cbAutoClean().Content(winrt::box_value(L"自動清理"));
            mainWindow->lbPhysicalLabel().Text(L"實體記憶體:");
            mainWindow->lbVirtualLabel().Text(L"虛擬記憶體:");
            mainWindow->lbSystemWSLabel().Text(L"系統工作集:");

            mainWindow->btnCleanNow().Content(winrt::box_value(L"立即清理"));

            if (mainWindow->actionAbout()) {
                mainWindow->actionAbout().Text(L"關於");
            }

            auto grid = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>();
            if (grid) {
                auto resources = grid.Resources();
                resources.Insert(winrt::box_value(L"TrayPhysicalMemoryText"), winrt::box_value(L"實體記憶體:"));
                resources.Insert(winrt::box_value(L"TrayVirtualMemoryText"), winrt::box_value(L"虛擬記憶體:"));
                resources.Insert(winrt::box_value(L"TraySystemWorkingSetText"), winrt::box_value(L"系統工作集:"));
            }
        }
        catch (const winrt::hresult_error& ex) {
            // 处理异常
        }
    }

    // 实现设置蒙古语界面的方法
    void SetMongolianLanguage(winrt::MemoryCleaner::implementation::MainWindow* mainWindow)
    {
        if (!mainWindow) return;

        try {
            auto menuBar = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(1).as<winrt::Microsoft::UI::Xaml::Controls::MenuBar>();
            if (menuBar && menuBar.Items().Size() > 0) {
                auto helpMenu = menuBar.Items().GetAt(0).as<winrt::Microsoft::UI::Xaml::Controls::MenuBarItem>();
                auto LanguageMenu = menuBar.Items().GetAt(1).as<winrt::Microsoft::UI::Xaml::Controls::MenuBarItem>();
                if (helpMenu) {
                    helpMenu.Title(L"ᠲᠤᠰᠠᠯᠠᠮᠵ");
                    LanguageMenu.Title(L"ᠬᠡᠯᠡ");
                }
            }

            auto memoryStateExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(3).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (memoryStateExpander) {
                memoryStateExpander.Header(winrt::box_value(L"ᠰᠠᠨᠠᠭᠤ ᠣᠶᠢᠨ ᠪᠠᠶᠢᠳᠠᠯ"));
            }

            auto cleanOptionsExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(4).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (cleanOptionsExpander) {
                cleanOptionsExpander.Header(winrt::box_value(L"ᠴᠡᠪᠡᠷᠯᠡᠬᠦ ᠰᠣᠩᠭᠣᠯᠲᠠ"));
            }

            auto controlExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(5).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (controlExpander) {
                controlExpander.Header(winrt::box_value(L"ᠵᠠᠬᠢᠶᠠᠯᠭᠠ"));
            }

            mainWindow->SetInterval().Text(L"ᠵᠠᠭᠤᠷᠠᠮ (ᠰᠡᠺᠦᠨᠳ)");
            mainWindow->cbWorkingSet().Content(winrt::box_value(L"ᠠᠵᠢᠯᠤᠨ ᠪᠠᠭᠴᠠ"));
            mainWindow->cbSystemWorkingSet().Content(winrt::box_value(L"ᠰᠢᠰᠲ᠋ᠧᠮ ᠤ᠋ ᠠᠵᠢᠯᠤᠨ ᠪᠠᠭᠴᠠ"));
            mainWindow->cbStandbyList().Content(winrt::box_value(L"ᠪᠡᠯᠡᠨ ᠪᠠᠶᠢᠬᠤ ᠵᠠᠭᠠᠭᠠᠯᠲᠠ"));
            mainWindow->cbStandbyListLow().Content(winrt::box_value(L"ᠪᠠᠭᠠ ᠲᠤᠪᠴᠢ ᠪᠡᠯᠡᠨ ᠪᠠᠶᠢᠬᠤ ᠵᠠᠭᠠᠭᠠᠯᠲᠠ"));
            mainWindow->cbModifiedPageList().Content(winrt::box_value(L"ᠥᠬᠡᠷᠴᠢᠭᠰᠡᠨ ᠬᠦᠰᠦᠨᠦᠭᠲᠡᠢ ᠵᠠᠭᠠᠭᠠᠯᠲᠠ"));
            mainWindow->cbCombinePageList().Content(winrt::box_value(L"ᠨᠢᠭᠡᠳᠬᠡᠭᠰᠡᠨ ᠬᠦᠰᠦᠨᠦᠭᠲᠡᠢ ᠵᠠᠭᠠᠭᠠᠯᠲᠠ"));

            mainWindow->cbAutoClean().Content(winrt::box_value(L"ᠠᠦᠲ᠋ᠣᠮᠠᠲ᠋ᠢᠺ ᠴᠡᠪᠡᠷᠯᠡᠬᠦ"));
            mainWindow->lbPhysicalLabel().Text(L"ᠹᠢᠽᠢᠺ ᠰᠠᠨᠠᠭᠤ ᠣᠶᠢ:");
            mainWindow->lbVirtualLabel().Text(L"ᠸᠢᠷᠲᠤᠠᠯ ᠰᠠᠨᠠᠭᠤ ᠣᠶᠢ:");
            mainWindow->lbSystemWSLabel().Text(L"ᠰᠢᠰᠲ᠋ᠧᠮ ᠤ᠋ ᠠᠵᠢᠯᠤᠨ ᠪᠠᠭᠴᠠ:");

            mainWindow->btnCleanNow().Content(winrt::box_value(L"ᠴᠡᠪᠡᠷᠯᠡᠬᠦ"));

            if (mainWindow->actionAbout()) {
                mainWindow->actionAbout().Text(L"ᠲᠤᠬᠠᠢ");
            }

            auto grid = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>();
            if (grid) {
                auto resources = grid.Resources();
                resources.Insert(winrt::box_value(L"TrayPhysicalMemoryText"), winrt::box_value(L"ᠹᠢᠽᠢᠺ ᠰᠠᠨᠠᠭᠤ ᠣᠶᠢ:"));
                resources.Insert(winrt::box_value(L"TrayVirtualMemoryText"), winrt::box_value(L"ᠸᠢᠷᠲᠤᠠᠯ ᠰᠠᠨᠠᠭᠤ ᠣᠶᠢ:"));
                resources.Insert(winrt::box_value(L"TraySystemWorkingSetText"), winrt::box_value(L"ᠰᠢᠰᠲ᠋ᠧᠮ ᠤ᠋ ᠠᠵᠢᠯᠤᠨ ᠪᠠᠭᠴᠠ:"));
            }
        }
        catch (const winrt::hresult_error& ex) {
            // 处理异常
        }
    }

    // 实现设置维吾尔语界面的方法
    void SetUyghurLanguage(winrt::MemoryCleaner::implementation::MainWindow* mainWindow)
    {
        if (!mainWindow) return;

        try {
            auto menuBar = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(1).as<winrt::Microsoft::UI::Xaml::Controls::MenuBar>();
            if (menuBar && menuBar.Items().Size() > 0) {
                auto helpMenu = menuBar.Items().GetAt(0).as<winrt::Microsoft::UI::Xaml::Controls::MenuBarItem>();
                auto LanguageMenu = menuBar.Items().GetAt(1).as<winrt::Microsoft::UI::Xaml::Controls::MenuBarItem>();
                if (helpMenu) {
                    helpMenu.Title(L"ياردەم");
                    LanguageMenu.Title(L"تىل");
                }
            }

            auto memoryStateExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(3).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (memoryStateExpander) {
                memoryStateExpander.Header(winrt::box_value(L"ئەسلەك ھالىتى"));
            }

            auto cleanOptionsExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(4).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (cleanOptionsExpander) {
                cleanOptionsExpander.Header(winrt::box_value(L"تازىلاش تاللانمىلىرى"));
            }

            auto controlExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(5).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (controlExpander) {
                controlExpander.Header(winrt::box_value(L"كونترول"));
            }

            mainWindow->SetInterval().Text(L"ئارىلىق (سېكۇنت)");
            mainWindow->cbWorkingSet().Content(winrt::box_value(L"خىزمەت توپلىمى"));
            mainWindow->cbSystemWorkingSet().Content(winrt::box_value(L"سىستېما خىزمەت توپلىمى"));
            mainWindow->cbStandbyList().Content(winrt::box_value(L"زاپاسلاش تىزىملىكى"));
            mainWindow->cbStandbyListLow().Content(winrt::box_value(L"تۆۋەن ئەۋزەللىك زاپاسلاش تىزىملىكى"));
            mainWindow->cbModifiedPageList().Content(winrt::box_value(L"ئۆزگەرتىلگەن بەت تىزىملىكى"));
            mainWindow->cbCombinePageList().Content(winrt::box_value(L"بىرلەشتۈرۈلگەن بەت تىزىмلىكى"));

            mainWindow->cbAutoClean().Content(winrt::box_value(L"ئاپتوماتىك"));
            mainWindow->lbPhysicalLabel().Text(L"فىزىكىلىق ئەسلەك:");
            mainWindow->lbVirtualLabel().Text(L"مەۋھۇم ئەسلەك:");
            mainWindow->lbSystemWSLabel().Text(L"سىستېما خىزمەت توپلىمى:");

            mainWindow->btnCleanNow().Content(winrt::box_value(L"تازىلاش"));

            if (mainWindow->actionAbout()) {
                mainWindow->actionAbout().Text(L"ھەققىدە");
            }

            auto grid = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>();
            if (grid) {
                auto resources = grid.Resources();
                resources.Insert(winrt::box_value(L"TrayPhysicalMemoryText"), winrt::box_value(L"فىزىكىلىق ئەسلەك:"));
                resources.Insert(winrt::box_value(L"TrayVirtualMemoryText"), winrt::box_value(L"مەۋھۇم ئەسلەك:"));
                resources.Insert(winrt::box_value(L"TraySystemWorkingSetText"), winrt::box_value(L"سىستېما خىزمەت توپلىمى:"));
            }
        }
        catch (const winrt::hresult_error& ex) {
            // 处理异常
        }
    }

    // 实现设置法语界面的方法
    void SetFrenchLanguage(winrt::MemoryCleaner::implementation::MainWindow* mainWindow)
    {
        if (!mainWindow) return;

        try {
            auto menuBar = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(1).as<winrt::Microsoft::UI::Xaml::Controls::MenuBar>();
            if (menuBar && menuBar.Items().Size() > 0) {
                auto helpMenu = menuBar.Items().GetAt(0).as<winrt::Microsoft::UI::Xaml::Controls::MenuBarItem>();
                auto LanguageMenu = menuBar.Items().GetAt(1).as<winrt::Microsoft::UI::Xaml::Controls::MenuBarItem>();
                if (helpMenu) {
                    helpMenu.Title(L"Aide");
                    LanguageMenu.Title(L"Langue");
                }
            }

            auto memoryStateExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(3).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (memoryStateExpander) {
                memoryStateExpander.Header(winrt::box_value(L"État de la mémoire"));
            }

            auto cleanOptionsExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(4).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (cleanOptionsExpander) {
                cleanOptionsExpander.Header(winrt::box_value(L"Options de nettoyage"));
            }

            auto controlExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(5).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (controlExpander) {
                controlExpander.Header(winrt::box_value(L"Contrôle"));
            }

            mainWindow->SetInterval().Text(L"Intervalle(sec)");
            mainWindow->cbWorkingSet().Content(winrt::box_value(L"Plage de travail"));
            mainWindow->cbSystemWorkingSet().Content(winrt::box_value(L"Plage de travail système"));
            mainWindow->cbStandbyList().Content(winrt::box_value(L"Liste de veille"));
            mainWindow->cbStandbyListLow().Content(winrt::box_value(L"Liste de veille basse priorité"));
            mainWindow->cbModifiedPageList().Content(winrt::box_value(L"Liste des pages modifiées"));
            mainWindow->cbCombinePageList().Content(winrt::box_value(L"Liste des pages combinées"));

            mainWindow->cbAutoClean().Content(winrt::box_value(L"Auto"));
            mainWindow->lbPhysicalLabel().Text(L"Mémoire physique:");
            mainWindow->lbVirtualLabel().Text(L"Mémoire virtuelle:");
            mainWindow->lbSystemWSLabel().Text(L"Plage de travail système:");

            mainWindow->btnCleanNow().Content(winrt::box_value(L"Nettoyer"));

            if (mainWindow->actionAbout()) {
                mainWindow->actionAbout().Text(L"À propos");
            }


            auto grid = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>();
            if (grid) {
                auto resources = grid.Resources();
                resources.Insert(winrt::box_value(L"TrayPhysicalMemoryText"), winrt::box_value(L"Mémoire physique:"));
                resources.Insert(winrt::box_value(L"TrayVirtualMemoryText"), winrt::box_value(L"Mémoire virtuelle:"));
                resources.Insert(winrt::box_value(L"TraySystemWorkingSetText"), winrt::box_value(L"Plage de travail système:"));
            }
        }
        catch (const winrt::hresult_error& ex) {
            // 处理异常
        }
    }

    // 实现设置俄语界面的方法
    void SetRussianLanguage(winrt::MemoryCleaner::implementation::MainWindow* mainWindow)
    {
        if (!mainWindow) return;

        try {
            auto menuBar = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(1).as<winrt::Microsoft::UI::Xaml::Controls::MenuBar>();
            if (menuBar && menuBar.Items().Size() > 0) {
                auto helpMenu = menuBar.Items().GetAt(0).as<winrt::Microsoft::UI::Xaml::Controls::MenuBarItem>();
                auto LanguageMenu = menuBar.Items().GetAt(1).as<winrt::Microsoft::UI::Xaml::Controls::MenuBarItem>();
                if (helpMenu) {
                    helpMenu.Title(L"Помощь");
                    LanguageMenu.Title(L"Язык");
                }
            }

            auto memoryStateExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(3).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (memoryStateExpander) {
                memoryStateExpander.Header(winrt::box_value(L"Состояние памяти"));
            }

            auto cleanOptionsExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(4).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (cleanOptionsExpander) {
                cleanOptionsExpander.Header(winrt::box_value(L"Опции очистки"));
            }

            auto controlExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(5).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (controlExpander) {
                controlExpander.Header(winrt::box_value(L"Управление"));
            }

            mainWindow->SetInterval().Text(L"Интервал (сек)");
            mainWindow->cbWorkingSet().Content(winrt::box_value(L"Рабочий набор"));
            mainWindow->cbSystemWorkingSet().Content(winrt::box_value(L"Системный рабочий набор"));
            mainWindow->cbStandbyList().Content(winrt::box_value(L"Список ожидания"));
            mainWindow->cbStandbyListLow().Content(winrt::box_value(L"Список ожидания низкого приоритета"));
            mainWindow->cbModifiedPageList().Content(winrt::box_value(L"Список измененных страниц"));
            mainWindow->cbCombinePageList().Content(winrt::box_value(L"Список объединенных страниц"));

            mainWindow->cbAutoClean().Content(winrt::box_value(L"Авто"));
            mainWindow->lbPhysicalLabel().Text(L"Физическая память:");
            mainWindow->lbVirtualLabel().Text(L"Виртуальная память:");
            mainWindow->lbSystemWSLabel().Text(L"Системный рабочий набор:");

            mainWindow->btnCleanNow().Content(winrt::box_value(L"Очистить"));

            if (mainWindow->actionAbout()) {
                mainWindow->actionAbout().Text(L"О программе");
            }

            auto grid = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>();
            if (grid) {
                auto resources = grid.Resources();
                resources.Insert(winrt::box_value(L"TrayPhysicalMemoryText"), winrt::box_value(L"Физическая память:"));
                resources.Insert(winrt::box_value(L"TrayVirtualMemoryText"), winrt::box_value(L"Виртуальная память:"));
                resources.Insert(winrt::box_value(L"TraySystemWorkingSetText"), winrt::box_value(L"Системный рабочий набор:"));
            }
        }
        catch (const winrt::hresult_error& ex) {
            // 处理异常
        }
    }

    // 实现设置西班牙语界面的方法
    void SetSpanishLanguage(winrt::MemoryCleaner::implementation::MainWindow* mainWindow)
    {
        if (!mainWindow) return;

        try {
            auto menuBar = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(1).as<winrt::Microsoft::UI::Xaml::Controls::MenuBar>();
            if (menuBar && menuBar.Items().Size() > 0) {
                auto helpMenu = menuBar.Items().GetAt(0).as<winrt::Microsoft::UI::Xaml::Controls::MenuBarItem>();
                auto LanguageMenu = menuBar.Items().GetAt(1).as<winrt::Microsoft::UI::Xaml::Controls::MenuBarItem>();
                if (helpMenu) {
                    helpMenu.Title(L"Ayuda");
                    LanguageMenu.Title(L"Idioma");
                }
            }

            auto memoryStateExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(3).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (memoryStateExpander) {
                memoryStateExpander.Header(winrt::box_value(L"Estado de la memoria"));
            }

            auto cleanOptionsExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(4).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (cleanOptionsExpander) {
                cleanOptionsExpander.Header(winrt::box_value(L"Opciones de limpieza"));
            }

            auto controlExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(5).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (controlExpander) {
                controlExpander.Header(winrt::box_value(L"Control"));
            }

            mainWindow->SetInterval().Text(L"Intervalo (seg)");
            mainWindow->cbWorkingSet().Content(winrt::box_value(L"Conjunto de trabajo"));
            mainWindow->cbSystemWorkingSet().Content(winrt::box_value(L"Conjunto de trabajo del sistema"));
            mainWindow->cbStandbyList().Content(winrt::box_value(L"Lista de espera"));
            mainWindow->cbStandbyListLow().Content(winrt::box_value(L"Lista de espera de baja prioridad"));
            mainWindow->cbModifiedPageList().Content(winrt::box_value(L"Lista de páginas modificadas"));
            mainWindow->cbCombinePageList().Content(winrt::box_value(L"Lista de páginas combinadas"));

            mainWindow->cbAutoClean().Content(winrt::box_value(L"Auto"));
            mainWindow->lbPhysicalLabel().Text(L"Memoria física:");
            mainWindow->lbVirtualLabel().Text(L"Memoria virtual:");
            mainWindow->lbSystemWSLabel().Text(L"Conjunto de trabajo del sistema:");

            mainWindow->btnCleanNow().Content(winrt::box_value(L"Limpiar"));

            if (mainWindow->actionAbout()) {
                mainWindow->actionAbout().Text(L"Acerca de");
            }


            auto grid = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>();
            if (grid) {
                auto resources = grid.Resources();
                resources.Insert(winrt::box_value(L"TrayPhysicalMemoryText"), winrt::box_value(L"Memoria física:"));
                resources.Insert(winrt::box_value(L"TrayVirtualMemoryText"), winrt::box_value(L"Memoria virtual:"));
                resources.Insert(winrt::box_value(L"TraySystemWorkingSetText"), winrt::box_value(L"Conjunto de trabajo del sistema:"));
            }
        }
        catch (const winrt::hresult_error& ex) {
            // 处理异常
        }
    }

    // 实现设置德语界面的方法
    void SetGermanLanguage(winrt::MemoryCleaner::implementation::MainWindow* mainWindow)
    {
        if (!mainWindow) return;

        try {
            auto menuBar = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(1).as<winrt::Microsoft::UI::Xaml::Controls::MenuBar>();
            if (menuBar && menuBar.Items().Size() > 0) {
                auto helpMenu = menuBar.Items().GetAt(0).as<winrt::Microsoft::UI::Xaml::Controls::MenuBarItem>();
                auto LanguageMenu = menuBar.Items().GetAt(1).as<winrt::Microsoft::UI::Xaml::Controls::MenuBarItem>();
                if (helpMenu) {
                    helpMenu.Title(L"Hilfe");
                    LanguageMenu.Title(L"Sprache");
                }
            }

            auto memoryStateExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(3).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (memoryStateExpander) {
                memoryStateExpander.Header(winrt::box_value(L"Speicherstatus"));
            }

            auto cleanOptionsExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(4).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (cleanOptionsExpander) {
                cleanOptionsExpander.Header(winrt::box_value(L"Bereinigungseinstellungen"));
            }

            auto controlExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(5).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (controlExpander) {
                controlExpander.Header(winrt::box_value(L"Steuerung"));
            }

            mainWindow->SetInterval().Text(L"Intervall (Sek)");
            mainWindow->cbWorkingSet().Content(winrt::box_value(L"Arbeitsspeicher"));
            mainWindow->cbSystemWorkingSet().Content(winrt::box_value(L"System-Arbeitsspeicher"));
            mainWindow->cbStandbyList().Content(winrt::box_value(L"Standby-Liste"));
            mainWindow->cbStandbyListLow().Content(winrt::box_value(L"Standby-Liste niedriger Priorität"));
            mainWindow->cbModifiedPageList().Content(winrt::box_value(L"Modifizierte Seitenliste"));
            mainWindow->cbCombinePageList().Content(winrt::box_value(L"Kombinierte Seitenliste"));

            mainWindow->cbAutoClean().Content(winrt::box_value(L"Auto"));
            mainWindow->lbPhysicalLabel().Text(L"Physischer Speicher:");
            mainWindow->lbVirtualLabel().Text(L"Virtueller Speicher:");
            mainWindow->lbSystemWSLabel().Text(L"System-Arbeitsspeicher:");

            mainWindow->btnCleanNow().Content(winrt::box_value(L"leeren"));

            if (mainWindow->actionAbout()) {
                mainWindow->actionAbout().Text(L"Über");
            }

            auto grid = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>();
            if (grid) {
                auto resources = grid.Resources();
                resources.Insert(winrt::box_value(L"TrayPhysicalMemoryText"), winrt::box_value(L"Physischer Speicher:"));
                resources.Insert(winrt::box_value(L"TrayVirtualMemoryText"), winrt::box_value(L"Virtueller Speicher:"));
                resources.Insert(winrt::box_value(L"TraySystemWorkingSetText"), winrt::box_value(L"System-Arbeitsspeicher:"));
            }
        }
        catch (const winrt::hresult_error& ex) {
            // 处理异常
        }
    }

    // 实现设置意大利语界面的方法
    void SetItalianLanguage(winrt::MemoryCleaner::implementation::MainWindow* mainWindow)
    {
        if (!mainWindow) return;

        try {
            auto menuBar = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(1).as<winrt::Microsoft::UI::Xaml::Controls::MenuBar>();
            if (menuBar && menuBar.Items().Size() > 0) {
                auto helpMenu = menuBar.Items().GetAt(0).as<winrt::Microsoft::UI::Xaml::Controls::MenuBarItem>();
                auto LanguageMenu = menuBar.Items().GetAt(1).as<winrt::Microsoft::UI::Xaml::Controls::MenuBarItem>();
                if (helpMenu) {
                    helpMenu.Title(L"Aiuto");
                    LanguageMenu.Title(L"Lingua");
                }
            }

            auto memoryStateExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(3).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (memoryStateExpander) {
                memoryStateExpander.Header(winrt::box_value(L"Stato della memoria"));
            }

            auto cleanOptionsExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(4).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (cleanOptionsExpander) {
                cleanOptionsExpander.Header(winrt::box_value(L"Opzioni di pulizia"));
            }

            auto controlExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(5).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (controlExpander) {
                controlExpander.Header(winrt::box_value(L"Controllo"));
            }

            mainWindow->SetInterval().Text(L"Intervallo (sec)");
            mainWindow->cbWorkingSet().Content(winrt::box_value(L"Working Set"));
            mainWindow->cbSystemWorkingSet().Content(winrt::box_value(L"Working Set di sistema"));
            mainWindow->cbStandbyList().Content(winrt::box_value(L"Lista standby"));
            mainWindow->cbStandbyListLow().Content(winrt::box_value(L"Lista standby a bassa priorità"));
            mainWindow->cbModifiedPageList().Content(winrt::box_value(L"Lista pagine modificate"));
            mainWindow->cbCombinePageList().Content(winrt::box_value(L"Lista pagine combinate"));

            mainWindow->cbAutoClean().Content(winrt::box_value(L"Auto"));
            mainWindow->lbPhysicalLabel().Text(L"Memoria fisica:");
            mainWindow->lbVirtualLabel().Text(L"Memoria virtuale:");
            mainWindow->lbSystemWSLabel().Text(L"Working Set di sistema:");

            mainWindow->btnCleanNow().Content(winrt::box_value(L"pulire"));

            if (mainWindow->actionAbout()) {
                mainWindow->actionAbout().Text(L"Informazioni");
            }

            auto grid = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>();
            if (grid) {
                auto resources = grid.Resources();
                resources.Insert(winrt::box_value(L"TrayPhysicalMemoryText"), winrt::box_value(L"Memoria fisica:"));
                resources.Insert(winrt::box_value(L"TrayVirtualMemoryText"), winrt::box_value(L"Memoria virtuale:"));
                resources.Insert(winrt::box_value(L"TraySystemWorkingSetText"), winrt::box_value(L"Working Set di sistema:"));
            }
        }
        catch (const winrt::hresult_error& ex) {
            // 处理异常
        }
    }

    // 实现设置阿拉伯语界面的方法
    void SetArabicLanguage(winrt::MemoryCleaner::implementation::MainWindow* mainWindow)
    {
        if (!mainWindow) return;

        try {
            auto menuBar = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(1).as<winrt::Microsoft::UI::Xaml::Controls::MenuBar>();
            if (menuBar && menuBar.Items().Size() > 0) {
                auto helpMenu = menuBar.Items().GetAt(0).as<winrt::Microsoft::UI::Xaml::Controls::MenuBarItem>();
                auto LanguageMenu = menuBar.Items().GetAt(1).as<winrt::Microsoft::UI::Xaml::Controls::MenuBarItem>();
                if (helpMenu) {
                    helpMenu.Title(L"مساعدة");
                    LanguageMenu.Title(L"اللغة");
                }
            }

            auto memoryStateExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(3).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (memoryStateExpander) {
                memoryStateExpander.Header(winrt::box_value(L"حالة الذاكرة"));
            }

            auto cleanOptionsExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(4).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (cleanOptionsExpander) {
                cleanOptionsExpander.Header(winrt::box_value(L"خيارات التنظيف"));
            }

            auto controlExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(5).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (controlExpander) {
                controlExpander.Header(winrt::box_value(L"التحكم"));
            }

            mainWindow->SetInterval().Text(L"الفاصل (ثانية)");
            mainWindow->cbWorkingSet().Content(winrt::box_value(L"مجموعة العمل"));
            mainWindow->cbSystemWorkingSet().Content(winrt::box_value(L"مجموعة عمل النظام"));
            mainWindow->cbStandbyList().Content(winrt::box_value(L"قائمة الانتظار"));
            mainWindow->cbStandbyListLow().Content(winrt::box_value(L"قائمة الانتظار ذات الأولوية المنخفضة"));
            mainWindow->cbModifiedPageList().Content(winrt::box_value(L"قائمة الصفحات المعدلة"));
            mainWindow->cbCombinePageList().Content(winrt::box_value(L"قائمة الصفحات المجمعة"));

            mainWindow->cbAutoClean().Content(winrt::box_value(L"تلقائي"));
            mainWindow->lbPhysicalLabel().Text(L"الذاكرة الفعلية:");
            mainWindow->lbVirtualLabel().Text(L"الذاكرة الظاهرية:");
            mainWindow->lbSystemWSLabel().Text(L"مجموعة عمل النظام:");

            mainWindow->btnCleanNow().Content(winrt::box_value(L"تنظيف"));

            if (mainWindow->actionAbout()) {
                mainWindow->actionAbout().Text(L"حول");
            }

            auto grid = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>();
            if (grid) {
                auto resources = grid.Resources();
                resources.Insert(winrt::box_value(L"TrayPhysicalMemoryText"), winrt::box_value(L"الذاكرة الفعلية:"));
                resources.Insert(winrt::box_value(L"TrayVirtualMemoryText"), winrt::box_value(L"الذاكرة الظاهرية:"));
                resources.Insert(winrt::box_value(L"TraySystemWorkingSetText"), winrt::box_value(L"مجموعة عمل النظام:"));
            }
        }
        catch (const winrt::hresult_error& ex) {
            // 处理异常
        }
    }

    // 实现设置韩语界面的方法
    void SetKoreanLanguage(winrt::MemoryCleaner::implementation::MainWindow* mainWindow)
    {
        if (!mainWindow) return;

        try {
            auto menuBar = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(1).as<winrt::Microsoft::UI::Xaml::Controls::MenuBar>();
            if (menuBar && menuBar.Items().Size() > 0) {
                auto helpMenu = menuBar.Items().GetAt(0).as<winrt::Microsoft::UI::Xaml::Controls::MenuBarItem>();
                auto LanguageMenu = menuBar.Items().GetAt(1).as<winrt::Microsoft::UI::Xaml::Controls::MenuBarItem>();
                if (helpMenu) {
                    helpMenu.Title(L"도움말");
                    LanguageMenu.Title(L"언어");
                }
            }

            auto memoryStateExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(3).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (memoryStateExpander) {
                memoryStateExpander.Header(winrt::box_value(L"메모리 상태"));
            }

            auto cleanOptionsExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(4).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (cleanOptionsExpander) {
                cleanOptionsExpander.Header(winrt::box_value(L"정리 옵션"));
            }

            auto controlExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(5).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (controlExpander) {
                controlExpander.Header(winrt::box_value(L"제어"));
            }

            mainWindow->SetInterval().Text(L"간격 (초)");
            mainWindow->cbWorkingSet().Content(winrt::box_value(L"작업 세트"));
            mainWindow->cbSystemWorkingSet().Content(winrt::box_value(L"시스템 작업 세트"));
            mainWindow->cbStandbyList().Content(winrt::box_value(L"대기 목록"));
            mainWindow->cbStandbyListLow().Content(winrt::box_value(L"낮은 우선순위 대기 목록"));
            mainWindow->cbModifiedPageList().Content(winrt::box_value(L"수정된 페이지 목록"));
            mainWindow->cbCombinePageList().Content(winrt::box_value(L"결합된 페이지 목록"));

            mainWindow->cbAutoClean().Content(winrt::box_value(L"자동 정리"));
            mainWindow->lbPhysicalLabel().Text(L"물리적 메모리:");
            mainWindow->lbVirtualLabel().Text(L"가상 메모리:");
            mainWindow->lbSystemWSLabel().Text(L"시스템 작업 세트:");

            mainWindow->btnCleanNow().Content(winrt::box_value(L"정리"));

            if (mainWindow->actionAbout()) {
                mainWindow->actionAbout().Text(L"정보");
            }

            auto grid = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>();
            if (grid) {
                auto resources = grid.Resources();
                resources.Insert(winrt::box_value(L"TrayPhysicalMemoryText"), winrt::box_value(L"물리적 메모리:"));
                resources.Insert(winrt::box_value(L"TrayVirtualMemoryText"), winrt::box_value(L"가상 메모리:"));
                resources.Insert(winrt::box_value(L"TraySystemWorkingSetText"), winrt::box_value(L"시스템 작업 세트:"));
            }
        }
        catch (const winrt::hresult_error& ex) {
            // 处理异常
        }
    }

    // 实现设置日语界面的方法
    void SetJapaneseLanguage(winrt::MemoryCleaner::implementation::MainWindow* mainWindow)
    {
        if (!mainWindow) return;

        try {
            auto menuBar = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(1).as<winrt::Microsoft::UI::Xaml::Controls::MenuBar>();
            if (menuBar && menuBar.Items().Size() > 0) {
                auto helpMenu = menuBar.Items().GetAt(0).as<winrt::Microsoft::UI::Xaml::Controls::MenuBarItem>();
                auto LanguageMenu = menuBar.Items().GetAt(1).as<winrt::Microsoft::UI::Xaml::Controls::MenuBarItem>();
                if (helpMenu) {
                    helpMenu.Title(L"ヘルプ");
                    LanguageMenu.Title(L"言語");
                }
            }

            auto memoryStateExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(3).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (memoryStateExpander) {
                memoryStateExpander.Header(winrt::box_value(L"メモリ状態"));
            }

            auto cleanOptionsExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(4).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (cleanOptionsExpander) {
                cleanOptionsExpander.Header(winrt::box_value(L"クリーンオプション"));
            }

            auto controlExpander = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>().Children().GetAt(5).as<winrt::Microsoft::UI::Xaml::Controls::Expander>();
            if (controlExpander) {
                controlExpander.Header(winrt::box_value(L"制御"));
            }

            mainWindow->SetInterval().Text(L"間隔 (秒)");
            mainWindow->cbWorkingSet().Content(winrt::box_value(L"ワーキングセット"));
            mainWindow->cbSystemWorkingSet().Content(winrt::box_value(L"システムワーキングセット"));
            mainWindow->cbStandbyList().Content(winrt::box_value(L"待機リスト"));
            mainWindow->cbStandbyListLow().Content(winrt::box_value(L"低優先度待機リスト"));
            mainWindow->cbModifiedPageList().Content(winrt::box_value(L"変更ページリスト"));
            mainWindow->cbCombinePageList().Content(winrt::box_value(L"結合ページリスト"));

            mainWindow->cbAutoClean().Content(winrt::box_value(L"自動"));
            mainWindow->lbPhysicalLabel().Text(L"物理メモリ:");
            mainWindow->lbVirtualLabel().Text(L"仮想メモリ:");
            mainWindow->lbSystemWSLabel().Text(L"システムワーキングセット:");

            mainWindow->btnCleanNow().Content(winrt::box_value(L"クリーン"));

            if (mainWindow->actionAbout()) {
                mainWindow->actionAbout().Text(L"情報");
            }

            auto grid = mainWindow->Content().as<winrt::Microsoft::UI::Xaml::Controls::Grid>();
            if (grid) {
                auto resources = grid.Resources();
                resources.Insert(winrt::box_value(L"TrayPhysicalMemoryText"), winrt::box_value(L"物理メモリ:"));
                resources.Insert(winrt::box_value(L"TrayVirtualMemoryText"), winrt::box_value(L"仮想メモリ:"));
                resources.Insert(winrt::box_value(L"TraySystemWorkingSetText"), winrt::box_value(L"システムワーキングセット:"));
            }
        }
        catch (const winrt::hresult_error& ex) {
            // 処理例外
        }
    }

    // 获取传统托盘菜单文本
    TrayMenuTexts GetTrayMenuTexts(Settings::Language language)
    {
        TrayMenuTexts texts;
        
        switch (language) {
            case Settings::Language::English:
                texts.showMainWindow = L"Show Main Window";
                texts.cleanOptions = L"Clean Options";
                texts.cleanNow = L"Clean Memory Now";
                texts.exit = L"Exit";
                texts.workingSet = L"Working Set";
                texts.systemWorkingSet = L"System Working Set";
                texts.standbyList = L"Standby List";
                texts.standbyListLow = L"Low Priority Standby List";
                texts.modifiedPageList = L"Modified Page List";
                texts.combinePageList = L"Combined Page List";
                break;
                
            case Settings::Language::ChineseSimplified:
                texts.showMainWindow = L"显示主窗口";
                texts.cleanOptions = L"清理选项";
                texts.cleanNow = L"立即清理内存";
                texts.exit = L"退出";
                texts.workingSet = L"工作集";
                texts.systemWorkingSet = L"系统工作集";
                texts.standbyList = L"备用列表";
                texts.standbyListLow = L"低优先级备用列表";
                texts.modifiedPageList = L"修改页面列表";
                texts.combinePageList = L"组合页面列表";
                break;
                
            case Settings::Language::ChineseTraditional:
                texts.showMainWindow = L"顯示主視窗";
                texts.cleanOptions = L"清理選項";
                texts.cleanNow = L"立即清理記憶體";
                texts.exit = L"退出";
                texts.workingSet = L"工作集";
                texts.systemWorkingSet = L"系統工作集";
                texts.standbyList = L"備用清單";
                texts.standbyListLow = L"低優先級備用清單";
                texts.modifiedPageList = L"修改頁面清單";
                texts.combinePageList = L"組合頁面清單";
                break;
                
            // 为其他语言添加类似的文本...
            case Settings::Language::Mongolian:
                // 修改为回鹘蒙文形式
                texts.showMainWindow = L"ᠤᠬᠠᠭᠠᠨ ᠴᠣᠩᠬᠣ ᠢᠯᠡᠷᠡᠬᠦ";
                texts.cleanOptions = L"ᠠᠷᠢᠯᠭᠠᠬᠤ ᠰᠣᠩᠭᠣᠯᠲᠠ";
                texts.cleanNow = L"ᠳᠦᠳᠡ ᠰᠠᠨᠠᠭᠤ ᠰᠢᠯᠵᠢᠭᠦᠯᠡᠬᠦ";
                texts.exit = L"ᠭᠠᠷᠬᠤ";
                texts.workingSet = L"ᠠᠵᠢᠯᠤᠨ ᠬᠦᠷᠢᠶᠡᠯᠡᠩ";
                texts.systemWorkingSet = L"ᠰᠢᠰᠲ᠋ᠧᠮ ᠤ᠋ ᠠᠵᠢᠯᠤᠨ ᠬᠦᠷᠢᠶᠡᠯᠡᠩ";
                texts.standbyList = L"ᠬᠦᠯᠢᠶᠡᠯᠡᠯᠲᠡᠨ ᠦ ᠵᠠᠭᠰᠠᠬᠤ";
                texts.standbyListLow = L"ᠪᠠᠭᠠ ᠳᠠᠷᠠᠭᠠᠲᠠᠢ ᠬᠦᠯᠢᠶᠡᠯᠡᠯᠲᠡᠨ ᠦ ᠵᠠᠭᠰᠠᠬᠤ";
                texts.modifiedPageList = L"ᠦᠵᠡᠭᠦᠯᠡᠰᠡᠨ ᠬᠦᠰᠦᠨᠦᠭ ᠲᠡᠮᠳᠡᠭ ᠵᠠᠭᠰᠠᠬᠤ";
                texts.combinePageList = L"ᠨᠡᠭᠡᠳᠬᠡᠭᠰᠡᠨ ᠬᠦᠰᠦᠨᠦᠭ ᠲᠡᠮᠳᠡᠭ ᠵᠠᠭᠰᠠᠬᠤ";
                break;
                
            // 为其他语言继续添加...
            case Settings::Language::Uyghur:
                texts.showMainWindow = L"ئاساسلىق كۆزنەكنى كۆرسىتىش";
                texts.cleanOptions = L"تازىلاش تاللانمىلىرى";
                texts.cleanNow = L"ھازىرلا ئەسلەكنى تازىلاش";
                texts.exit = L"چېكىنىش";
                texts.workingSet = L"خىزمەت يىغىندىسى";
                texts.systemWorkingSet = L"سىستېما خىزمەت يىغىندىسى";
                texts.standbyList = L"كۈتۈش تىزىملىكى";
                texts.standbyListLow = L"تۆۋەن ئەۋزەللىكتىكى كۈتۈش تىزىملىكى";
                texts.modifiedPageList = L"ئۆزگەرتىلگەن بەت تىزىملىكى";
                texts.combinePageList = L"بىرلەشتۈرۈلگەن بەت تىزىملىكى";
                break;
                
            case Settings::Language::French:
                texts.showMainWindow = L"Afficher la fenêtre principale";
                texts.cleanOptions = L"Options de nettoyage";
                texts.cleanNow = L"Nettoyer la mémoire maintenant";
                texts.exit = L"Quitter";
                texts.workingSet = L"Ensemble de travail";
                texts.systemWorkingSet = L"Ensemble de travail système";
                texts.standbyList = L"Liste d'attente";
                texts.standbyListLow = L"Liste d'attente basse priorité";
                texts.modifiedPageList = L"Liste des pages modifiées";
                texts.combinePageList = L"Liste des pages combinées";
                break;
                
            case Settings::Language::Russian:
                texts.showMainWindow = L"Показать главное окно";
                texts.cleanOptions = L"Опции очистки";
                texts.cleanNow = L"Очистить память сейчас";
                texts.exit = L"Выход";
                texts.workingSet = L"Рабочий набор";
                texts.systemWorkingSet = L"Системный рабочий набор";
                texts.standbyList = L"Список ожидания";
                texts.standbyListLow = L"Список ожидания низкой приоритетности";
                texts.modifiedPageList = L"Список измененных страниц";
                texts.combinePageList = L"Список объединенных страниц";
                break;
                
            case Settings::Language::Spanish:
                texts.showMainWindow = L"Mostrar ventana principal";
                texts.cleanOptions = L"Opciones de limpieza";
                texts.cleanNow = L"Limpiar memoria ahora";
                texts.exit = L"Salir";
                texts.workingSet = L"Conjunto de trabajo";
                texts.systemWorkingSet = L"Conjunto de trabajo del sistema";
                texts.standbyList = L"Lista de espera";
                texts.standbyListLow = L"Lista de espera de baja prioridad";
                texts.modifiedPageList = L"Lista de páginas modificadas";
                texts.combinePageList = L"Lista de páginas combinadas";
                break;
                
            case Settings::Language::German:
                texts.showMainWindow = L"Hauptfenster anzeigen";
                texts.cleanOptions = L"Bereinigungseinstellungen";
                texts.cleanNow = L"Speicher jetzt bereinigen";
                texts.exit = L"Beenden";
                texts.workingSet = L"Arbeitsspeicher";
                texts.systemWorkingSet = L"System-Arbeitsspeicher";
                texts.standbyList = L"Standby-Liste";
                texts.standbyListLow = L"Standby-Liste niedriger Priorität";
                texts.modifiedPageList = L"Modifizierte Seitenliste";
                texts.combinePageList = L"Kombinierte Seitenliste";
                break;
                
            case Settings::Language::Italian:
                texts.showMainWindow = L"Mostra finestra principale";
                texts.cleanOptions = L"Opzioni di pulizia";
                texts.cleanNow = L"Pulisci memoria ora";
                texts.exit = L"Esci";
                texts.workingSet = L"Working Set";
                texts.systemWorkingSet = L"Working Set di sistema";
                texts.standbyList = L"Lista standby";
                texts.standbyListLow = L"Lista standby a bassa priorità";
                texts.modifiedPageList = L"Lista pagine modificate";
                texts.combinePageList = L"Lista pagine combinate";
                break;
                
            case Settings::Language::Arabic:
                texts.showMainWindow = L"إظهار النافذة الرئيسية";
                texts.cleanOptions = L"خيارات التنظيف";
                texts.cleanNow = L"تنظيف الذاكرة الآن";
                texts.exit = L"خروج";
                texts.workingSet = L"مجموعة العمل";
                texts.systemWorkingSet = L"مجموعة عمل النظام";
                texts.standbyList = L"قائمة الانتظار";
                texts.standbyListLow = L"قائمة الانتظار ذات الأولوية المنخفضة";
                texts.modifiedPageList = L"قائمة الصفحات المعدلة";
                texts.combinePageList = L"قائمة الصفحات المجمعة";
                break;
                
            case Settings::Language::Korean:
                texts.showMainWindow = L"주 창 표시";
                texts.cleanOptions = L"정리 옵션";
                texts.cleanNow = L"지금 메모리 정리";
                texts.exit = L"종료";
                texts.workingSet = L"작업 세트";
                texts.systemWorkingSet = L"시스템 작업 세트";
                texts.standbyList = L"대기 목록";
                texts.standbyListLow = L"낮은 우선순위 대기 목록";
                texts.modifiedPageList = L"수정된 페이지 목록";
                texts.combinePageList = L"결합된 페이지 목록";
                break;
                
            case Settings::Language::Japanese:
                texts.showMainWindow = L"メインウィンドウを表示";
                texts.cleanOptions = L"クリーンオプション";
                texts.cleanNow = L"今すぐメモリをクリーン";
                texts.exit = L"終了";
                texts.workingSet = L"ワーキングセット";
                texts.systemWorkingSet = L"システムワーキングセット";
                texts.standbyList = L"待機リスト";
                texts.standbyListLow = L"低優先度待機リスト";
                texts.modifiedPageList = L"変更ページリスト";
                texts.combinePageList = L"結合ページリスト";
                break;
                
            default:
                // 默认使用英语
                texts.showMainWindow = L"Show Main Window";
                texts.cleanOptions = L"Clean Options";
                texts.cleanNow = L"Clean Memory Now";
                texts.exit = L"Exit";
                texts.workingSet = L"Working Set";
                texts.systemWorkingSet = L"System Working Set";
                texts.standbyList = L"Standby List";
                texts.standbyListLow = L"Low Priority Standby List";
                texts.modifiedPageList = L"Modified Page List";
                texts.combinePageList = L"Combined Page List";
                break;
        }
        
        return texts;
    }
}