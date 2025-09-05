#pragma once

#include <string>
#include <Windows.h>

// 设置管理类 - 负责从INI文件读取和保存用户偏好设置
class Settings {
public:
    // 语言类型枚举
    enum class Language {
        English,              // 英语
        ChineseSimplified,   // 简体中文
        ChineseTraditional,  // 繁体中文
        Mongolian,            // 蒙古语
        Uyghur,              // 维吾尔语
        French,               // 法语
        Russian,              // 俄语
        Spanish,              // 西班牙语
        German,               // 德语
        Italian,              // 意大利语
        Arabic,               // 阿拉伯语
        Korean,               // 韩语
        Japanese              // 日语
    };

    // 清理选项结构体
    struct CleanupOptions {
        bool workingSet = true;              // 清理当前进程工作集
        bool systemWorkingSet = false;        // 清理系统工作集
        bool standbyList = true;              // 清理待机列表
        bool standbyListLowPriority = false;  // 清理低优先级待机列表
        bool modifiedPageList = false;        // 清理修改页面列表
        bool combinePageList = false;         // 合并页面列表
        bool autoClean = false;               // 自动清理
        int cleanInterval = 60;              // 清理间隔（秒）
    };

    // 构造函数和析构函数
    Settings();
    ~Settings();

    // 禁用拷贝构造和赋值操作
    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;

    // 加载和保存设置
    void LoadSettings();
    void SaveSettings();

    // 获取和设置语言
    Language GetLanguage() const;
    void SetLanguage(Language language);

    // 获取和设置清理选项
    const CleanupOptions& GetCleanupOptions() const;
    void SetCleanupOptions(const CleanupOptions& options);

    // 获取INI文件路径
    const std::wstring& GetIniFilePath() const;

private:
    // 成员变量
    Language m_currentLanguage = Language::English;  // 当前语言
    CleanupOptions m_cleanupOptions;                  // 清理选项
    std::wstring m_iniFilePath;                       // INI文件路径

    // 私有方法
    void InitializeIniFilePath();                     // 初始化INI文件路径
    void CreateDefaultIniFileIfNeeded();              // 如果需要，创建默认INI文件
    std::wstring LanguageToString(Language language) const;  // 将语言枚举转换为字符串
    Language StringToLanguage(const std::wstring& str) const; // 将字符串转换为语言枚举
};