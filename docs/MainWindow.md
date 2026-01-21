# MainWindow Documentation

## Overview

The `MainWindow` class is the main user interface class for the MemoryCleaner application. It provides a comprehensive GUI for memory management operations, system tray integration, and real-time memory monitoring.

## Class Structure

### Public Members

#### Constructor and Destructor

**`MainWindow(QWidget *parent = nullptr)`**
- **Description**: Main constructor that initializes the application window
- **Parameters**: 
  - `parent`: Parent widget (optional)
- **Initialization Tasks**:
  - Sets up UI components
  - Creates system tray icon
  - Initializes timers for auto-clean and monitoring
  - Loads application settings
  - Sets up language support

**`~MainWindow()`**
- **Description**: Destructor that cleans up resources
- **Tasks**:
  - Saves current settings
  - Deletes UI components

#### Public Methods

**`cleanSelectedMemory()`**
- **Description**: Executes memory cleaning operations based on user-selected options
- **Implementation**:
  - Checks which memory cleaning options are enabled
  - Calls corresponding `MemoryCleaner` functions
  - Handles error reporting for failed operations

### Protected Methods

**`closeEvent(QCloseEvent *event)`**
- **Description**: Handles window close events
- **Behavior**:
  - Saves settings before closing
  - Minimizes to system tray instead of closing
  - Prevents actual application termination

**`changeEvent(QEvent *event)`**
- **Description**: Handles window state change events
- **Behavior**:
  - Automatically hides window when minimized
  - Maintains system tray functionality

### Private Slots (Event Handlers)

#### Memory Cleaning Operations

**`on_btnCleanNow_clicked()`**
- **Description**: Triggered when "Clean Now" button is clicked
- **Functionality**:
  - Captures memory usage before cleaning
  - Executes selected cleaning operations
  - Calculates and displays freed memory
  - Shows notification if enabled

**`onTrayCleanWorkingSet()`**
- **Description**: Tray menu action for cleaning working set
- **Behavior**: Toggles working set cleaning option

**`onTrayCleanSystemWS()`**
- **Description**: Tray menu action for cleaning system working set
- **Behavior**: Toggles system working set cleaning option

**`onTrayCleanStandbyList()`**
- **Description**: Tray menu action for cleaning standby list
- **Behavior**: Toggles standby list cleaning option

**`onTrayCleanStandbyListLow()`**
- **Description**: Tray menu action for cleaning low-priority standby list
- **Behavior**: Toggles low-priority standby list cleaning option

**`onTrayCleanModifiedList()`**
- **Description**: Tray menu action for cleaning modified page list
- **Behavior**: Toggles modified page list cleaning option

**`onTrayCleanCombinedList()`**
- **Description**: Tray menu action for cleaning combined page list
- **Behavior**: Toggles combined page list cleaning option

#### Auto-Clean Management

**`on_cbAutoClean_stateChanged(int arg1)`**
- **Description**: Manages auto-clean timer based on checkbox state
- **Parameters**:
  - `arg1`: Checkbox state (Qt::Checked or Qt::Unchecked)
- **Behavior**:
  - Starts timer when checked
  - Stops timer when unchecked

**`on_sbInterval_valueChanged(int arg1)`**
- **Description**: Updates auto-clean interval
- **Parameters**:
  - `arg1`: New interval value in seconds
- **Behavior**: Updates timer interval if auto-clean is active

#### System Tray Operations

**`trayIconActivated(QSystemTrayIcon::ActivationReason reason)`**
- **Description**: Handles system tray icon interactions
- **Parameters**:
  - `reason`: Type of activation (click, double-click, etc.)
- **Behavior**:
  - Double-click toggles window visibility
  - Other interactions show context menu

**`showAboutFromTray()`**
- **Description**: Shows About dialog from tray menu
- **Behavior**: Displays application information dialog

**`openAuthorWebsite()`**
- **Description**: Opens author's website
- **Behavior**: Launches default browser with author URL

#### Language Support

**Language Change Handlers**
- `onChineseTriggered()` - Switch to Simplified Chinese
- `onTraditionalChineseTriggered()` - Switch to Traditional Chinese
- `onMongolianTriggered()` - Switch to Mongolian
- `onUyghurTriggered()` - Switch to Uyghur
- `onEnglishTriggered()` - Switch to English
- `onFrenchTriggered()` - Switch to French
- `onRussianTriggered()` - Switch to Russian
- `onSpanishTriggered()` - Switch to Spanish
- `onGermanTriggered()` - Switch to German
- `onItalianTriggered()` - Switch to Italian
- `onArabicTriggered()` - Switch to Arabic
- `onKoreanTriggered()` - Switch to Korean
- `onJapaneseTriggered()` - Switch to Japanese

**`onLanguageChanged(const QString& locale)`**
- **Description**: Centralized language change handler
- **Parameters**:
  - `locale`: Language locale code (e.g., "zh_CN", "en_US")
- **Behavior**: Updates UI translations and settings

#### Application Settings

**`onAutoStartTriggered()`**
- **Description**: Toggles auto-start on system boot
- **Behavior**:
  - Updates registry settings
  - Synchronizes with system startup configuration

**`onEnableMessageTriggered()`**
- **Description**: Toggles notification messages
- **Behavior**: Controls whether to show cleanup notifications

### Private Methods

#### UI Management

**`createTrayIcon()`**
- **Description**: Creates and configures system tray icon
- **Components**:
  - Tray icon with memory usage visualization
  - Context menu with memory cleaning options
  - Language selection menu
  - Application control options

**`showWindow()`**
- **Description**: Shows and activates main window
- **Behavior**: Brings window to foreground

**`hideWindow()`**
- **Description**: Hides main window
- **Behavior**: Minimizes to system tray

**`exitApp()`**
- **Description**: Exits application completely
- **Behavior**: Closes all windows and terminates process

#### Settings Management

**`loadSettings()`**
- **Description**: Loads application settings from INI file
- **Settings Loaded**:
  - Memory cleaning options
  - Auto-clean configuration
  - Language preferences
  - Auto-start settings
  - Notification preferences

**`saveSettings()`**
- **Description**: Saves current settings to INI file
- **Settings Saved**:
  - All user preferences
  - Current application state
  - Error handling for write permissions

**`saveCurrentOptions()`**
- **Description**: Saves current tray menu options
- **Behavior**: Synchronizes tray menu with main UI settings

#### Memory Information Display

**`displayMemoryInfo()`**
- **Description**: Updates memory usage display
- **Information Displayed**:
  - Physical memory usage with percentage
  - Virtual memory usage with percentage
  - System working set information
  - Color-coded progress bars

**`updateMemoryInfo()`**
- **Description**: Real-time memory monitoring
- **Frequency**: Called every second by monitor timer
- **Updates**:
  - System tray icon
  - Tooltip with detailed memory information
  - Main window display

**`getSystemWorkingSetInfo(quint64& totalWS, quint64& sharedWS, quint64& privateWS)`**
- **Description**: Retrieves system working set statistics
- **Parameters** (output):
  - `totalWS`: Total working set size
  - `sharedWS`: Shared working set size (estimated)
  - `privateWS`: Private working set size
- **Return**: `true` if successful, `false` otherwise

#### UI Utilities

**`createTextIcon(int usagePercent)`**
- **Description**: Creates dynamic tray icon showing memory usage
- **Parameters**:
  - `usagePercent`: Current memory usage percentage
- **Visualization**:
  - Circular progress indicator
  - Percentage text overlay
  - Color coding based on usage level

**`setProgressBarStyle(QProgressBar* bar, int value, QLabel*& label)`**
- **Description**: Customizes progress bar appearance
- **Parameters**:
  - `bar`: Progress bar widget to style
  - `value`: Current progress value
  - `label`: Reference to percentage label
- **Features**:
  - Percentage overlay labels
  - Dynamic color schemes

**`getContrastColor(const QColor& background)`**
- **Description**: Calculates contrasting text color
- **Parameters**:
  - `background`: Background color to contrast with
- **Return**: Black or white depending on background luminance

**`syncTrayMenuWithSettings()`**
- **Description**: Synchronizes tray menu with current settings
- **Behavior**: Updates checkboxes in tray context menu

**`changeLanguage(const QString& locale)`**
- **Description**: Changes application language
- **Parameters**:
  - `locale`: Target language locale
- **Behavior**:
  - Loads translation files
  - Updates all UI text
  - Saves language preference

**`retranslateUi()`**
- **Description**: Updates all UI text after language change
- **Behavior**: Applies translations to all interface elements

#### System Integration

**`setAutoStart(bool enable)`**
- **Description**: Configures auto-start on system boot
- **Parameters**:
  - `enable`: `true` to enable, `false` to disable
- **Implementation**: Modifies Windows registry settings

**`isAutoStartEnabled()`**
- **Description**: Checks if auto-start is enabled
- **Return**: `true` if auto-start is configured, `false` otherwise
- **Implementation**: Queries Windows registry

### Static Members

**`tooltipTemplates[]`**
- **Description**: Array of tooltip templates for different languages
- **Languages Supported**:
  - English, Chinese (Simplified/Traditional), French, Russian
  - Spanish, German, Italian, Arabic, Korean, Japanese
  - Uyghur, Mongolian
- **Format**: String templates with placeholders for memory statistics

## Key Features

### Multi-language Support
- Supports 13 different languages
- Dynamic language switching
- Culturally appropriate number formatting

### System Tray Integration
- Always-available memory monitoring
- Quick access to cleaning operations
- Visual memory usage indicator
- Context menu with full functionality

### Real-time Monitoring
- 1-second update intervals
- Comprehensive memory statistics
- Visual progress indicators
- Dynamic tooltip information

### Automated Operations
- Configurable auto-clean intervals
- Selective memory area cleaning
- Background operation capability

### Settings Persistence
- INI-based configuration storage
- Automatic save/load on startup/shutdown
- Registry integration for auto-start

## Dependencies

- **Qt Framework**: UI components, timers, system integration
- **Windows API**: Memory management, process enumeration, registry access
- **MemoryCleaner**: Core memory cleaning functionality
- **QTranslator**: Internationalization support

## Error Handling

- Comprehensive error checking for memory operations
- Permission validation for settings storage
- Graceful fallbacks for unsupported operations
- User-friendly error messages

## Usage Patterns

### Manual Cleaning
1. Select desired memory areas to clean
2. Click "Clean Now" button
3. View results in status bar and log

### Auto-clean Configuration
1. Enable auto-clean checkbox
2. Set desired interval (seconds)
3. Application runs cleaning automatically

### System Tray Usage
1. Right-click tray icon for menu
2. Select cleaning options
3. Double-click to show/hide main window

### Language Switching
1. Use Language menu
2. Select desired language
3. Interface updates immediately

## Performance Considerations

- Memory monitoring has minimal performance impact
- Cleaning operations are optimized for efficiency
- UI updates are throttled to prevent excessive CPU usage
- System tray operations are lightweight and non-blocking