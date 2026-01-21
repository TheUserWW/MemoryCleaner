#include "pch.h"
#include "settings.h"
#include <shlobj.h>
#include <fstream>
#include <sstream>
#include <libloaderapi.h> // 添加此头文件以使用GetModuleFileNameW

#pragma comment(lib, "shell32.lib")

Settings::Settings() {
    InitializeIniFilePath();
    CreateDefaultIniFileIfNeeded();
    LoadSettings();
}

Settings::~Settings() {
    SaveSettings();
}

void Settings::InitializeIniFilePath() {
    // 获取用户的AppData目录
    wchar_t appDataPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, 0, appDataPath))) {
        // 创建应用程序在AppData中的目录路径
        std::wstring appDir = std::wstring(appDataPath) + L"\\MemoryCleaner";
        
        // 确保目录存在
        CreateDirectoryW(appDir.c_str(), NULL);
        
        // 设置INI文件路径
        m_iniFilePath = appDir + L"\\settings.ini";
    }
    else {
        // 如果无法获取AppData路径，使用当前目录作为备选
        m_iniFilePath = L"settings.ini";
    }
}

void Settings::CreateDefaultIniFileIfNeeded() {
    // 检查文件是否存在
    WIN32_FIND_DATAW findData;
    HANDLE hFind = FindFirstFileW(m_iniFilePath.c_str(), &findData);
    
    if (hFind == INVALID_HANDLE_VALUE) {
        // 文件不存在，创建默认INI文件
        std::wofstream iniFile(m_iniFilePath);
        if (iniFile.is_open()) {
            iniFile << L"[Language]\n";
            iniFile << L"Current=English\n\n";
            iniFile << L"[CleanupOptions]\n";
            iniFile << L"WorkingSet=1\n";
            iniFile << L"SystemWorkingSet=0\n";
            iniFile << L"StandbyList=1\n";
            iniFile << L"StandbyListLowPriority=0\n";
            iniFile << L"ModifiedPageList=0\n";
            iniFile << L"CombinePageList=0\n";
            iniFile << L"AutoClean=0\n";
            iniFile << L"CleanInterval=60\n";
            iniFile.close();
        }
    }
    else {
        FindClose(hFind);
    }
}

void Settings::LoadSettings() {
    // 读取语言设置
    wchar_t languageStr[64];
    GetPrivateProfileStringW(L"Language", L"Current", L"English", languageStr, 64, m_iniFilePath.c_str());
    m_currentLanguage = StringToLanguage(languageStr);
    
    // 读取清理选项
    m_cleanupOptions.workingSet = GetPrivateProfileIntW(L"CleanupOptions", L"WorkingSet", 1, m_iniFilePath.c_str()) != 0;
    m_cleanupOptions.systemWorkingSet = GetPrivateProfileIntW(L"CleanupOptions", L"SystemWorkingSet", 0, m_iniFilePath.c_str()) != 0;
    m_cleanupOptions.standbyList = GetPrivateProfileIntW(L"CleanupOptions", L"StandbyList", 1, m_iniFilePath.c_str()) != 0;
    m_cleanupOptions.standbyListLowPriority = GetPrivateProfileIntW(L"CleanupOptions", L"StandbyListLowPriority", 0, m_iniFilePath.c_str()) != 0;
    m_cleanupOptions.modifiedPageList = GetPrivateProfileIntW(L"CleanupOptions", L"ModifiedPageList", 0, m_iniFilePath.c_str()) != 0;
    m_cleanupOptions.combinePageList = GetPrivateProfileIntW(L"CleanupOptions", L"CombinePageList", 0, m_iniFilePath.c_str()) != 0;
    m_cleanupOptions.autoClean = GetPrivateProfileIntW(L"CleanupOptions", L"AutoClean", 0, m_iniFilePath.c_str()) != 0;
    m_cleanupOptions.cleanInterval = GetPrivateProfileIntW(L"CleanupOptions", L"CleanInterval", 60, m_iniFilePath.c_str());
}

void Settings::SaveSettings() {
    // 保存语言设置
    WritePrivateProfileStringW(L"Language", L"Current", LanguageToString(m_currentLanguage).c_str(), m_iniFilePath.c_str());
    
    // 保存清理选项
    WritePrivateProfileStringW(L"CleanupOptions", L"WorkingSet", m_cleanupOptions.workingSet ? L"1" : L"0", m_iniFilePath.c_str());
    WritePrivateProfileStringW(L"CleanupOptions", L"SystemWorkingSet", m_cleanupOptions.systemWorkingSet ? L"1" : L"0", m_iniFilePath.c_str());
    WritePrivateProfileStringW(L"CleanupOptions", L"StandbyList", m_cleanupOptions.standbyList ? L"1" : L"0", m_iniFilePath.c_str());
    WritePrivateProfileStringW(L"CleanupOptions", L"StandbyListLowPriority", m_cleanupOptions.standbyListLowPriority ? L"1" : L"0", m_iniFilePath.c_str());
    WritePrivateProfileStringW(L"CleanupOptions", L"ModifiedPageList", m_cleanupOptions.modifiedPageList ? L"1" : L"0", m_iniFilePath.c_str());
    WritePrivateProfileStringW(L"CleanupOptions", L"CombinePageList", m_cleanupOptions.combinePageList ? L"1" : L"0", m_iniFilePath.c_str());
    WritePrivateProfileStringW(L"CleanupOptions", L"AutoClean", m_cleanupOptions.autoClean ? L"1" : L"0", m_iniFilePath.c_str());
    
    wchar_t intervalStr[16];
    _itow_s(m_cleanupOptions.cleanInterval, intervalStr, 10);
    WritePrivateProfileStringW(L"CleanupOptions", L"CleanInterval", intervalStr, m_iniFilePath.c_str());
}

Settings::Language Settings::GetLanguage() const {
    return m_currentLanguage;
}

void Settings::SetLanguage(Language language) {
    m_currentLanguage = language;
    SaveSettings();
}

const Settings::CleanupOptions& Settings::GetCleanupOptions() const {
    return m_cleanupOptions;
}

void Settings::SetCleanupOptions(const CleanupOptions& options) {
    m_cleanupOptions = options;
    SaveSettings();
}

const std::wstring& Settings::GetIniFilePath() const {
    return m_iniFilePath;
}

std::wstring Settings::LanguageToString(Language language) const {
    switch (language) {
        case Language::English: return L"English";
        case Language::ChineseSimplified: return L"ChineseSimplified";
        case Language::ChineseTraditional: return L"ChineseTraditional";
        case Language::Mongolian: return L"Mongolian";
        case Language::Uyghur: return L"Uyghur";
        case Language::French: return L"French";
        case Language::Russian: return L"Russian";
        case Language::Spanish: return L"Spanish";
        case Language::German: return L"German";
        case Language::Italian: return L"Italian";
        case Language::Arabic: return L"Arabic";
        case Language::Korean: return L"Korean";
        case Language::Japanese: return L"Japanese";
        default: return L"English";
    }
}

Settings::Language Settings::StringToLanguage(const std::wstring& str) const {
    if (str == L"English") return Language::English;
    if (str == L"ChineseSimplified") return Language::ChineseSimplified;
    if (str == L"ChineseTraditional") return Language::ChineseTraditional;
    if (str == L"Mongolian") return Language::Mongolian;
    if (str == L"Uyghur") return Language::Uyghur;
    if (str == L"French") return Language::French;
    if (str == L"Russian") return Language::Russian;
    if (str == L"Spanish") return Language::Spanish;
    if (str == L"German") return Language::German;
    if (str == L"Italian") return Language::Italian;
    if (str == L"Arabic") return Language::Arabic;
    if (str == L"Korean") return Language::Korean;
    if (str == L"Japanese") return Language::Japanese;
    return Language::English; // 默认为英语
}