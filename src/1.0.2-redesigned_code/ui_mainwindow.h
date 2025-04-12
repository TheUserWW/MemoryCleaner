/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionAbout;
    QAction *actionAutoStart;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBoxMemory;
    QGridLayout *gridLayoutMemory;
    QLabel *lbPhysicalValue;
    QLabel *labelPhysical;
    QLabel *labelSystemWS;
    QLabel *lbVirtualValue;
    QLabel *lbSystemWSValue;
    QLabel *labelVirtual;
    QProgressBar *pbPhysical;
    QProgressBar *pbVirtual;
    QProgressBar *pbSystemWS;
    QGroupBox *groupBoxOptions;
    QGridLayout *gridLayoutOptions;
    QCheckBox *cbWorkingSet;
    QCheckBox *cbSystemWorkingSet;
    QCheckBox *cbStandbyList;
    QCheckBox *cbStandbyListLow;
    QCheckBox *cbModifiedPageList;
    QCheckBox *cbCombinePageList;
    QGroupBox *groupBoxControl;
    QHBoxLayout *horizontalLayoutControl;
    QPushButton *btnCleanNow;
    QCheckBox *cbAutoClean;
    QLabel *labelInterval;
    QSpinBox *sbInterval;
    QTextBrowser *textBrowser;
    QSpacerItem *verticalSpacer;
    QLabel *lbLastUpdate;
    QMenuBar *menubar;
    QMenu *menuHelp;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(400, 500);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName("actionAbout");
        actionAutoStart = new QAction(MainWindow);
        actionAutoStart->setObjectName("actionAutoStart");
        actionAutoStart->setCheckable(true);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        groupBoxMemory = new QGroupBox(centralwidget);
        groupBoxMemory->setObjectName("groupBoxMemory");
        gridLayoutMemory = new QGridLayout(groupBoxMemory);
        gridLayoutMemory->setObjectName("gridLayoutMemory");
        lbPhysicalValue = new QLabel(groupBoxMemory);
        lbPhysicalValue->setObjectName("lbPhysicalValue");
        lbPhysicalValue->setAlignment(Qt::AlignmentFlag::AlignRight);

        gridLayoutMemory->addWidget(lbPhysicalValue, 0, 1, 1, 1);

        labelPhysical = new QLabel(groupBoxMemory);
        labelPhysical->setObjectName("labelPhysical");

        gridLayoutMemory->addWidget(labelPhysical, 0, 0, 1, 1);

        labelSystemWS = new QLabel(groupBoxMemory);
        labelSystemWS->setObjectName("labelSystemWS");

        gridLayoutMemory->addWidget(labelSystemWS, 5, 0, 1, 1);

        lbVirtualValue = new QLabel(groupBoxMemory);
        lbVirtualValue->setObjectName("lbVirtualValue");
        lbVirtualValue->setAlignment(Qt::AlignmentFlag::AlignRight);

        gridLayoutMemory->addWidget(lbVirtualValue, 3, 1, 1, 1);

        lbSystemWSValue = new QLabel(groupBoxMemory);
        lbSystemWSValue->setObjectName("lbSystemWSValue");
        lbSystemWSValue->setAlignment(Qt::AlignmentFlag::AlignRight);

        gridLayoutMemory->addWidget(lbSystemWSValue, 5, 1, 1, 1);

        labelVirtual = new QLabel(groupBoxMemory);
        labelVirtual->setObjectName("labelVirtual");

        gridLayoutMemory->addWidget(labelVirtual, 3, 0, 1, 1);

        pbPhysical = new QProgressBar(groupBoxMemory);
        pbPhysical->setObjectName("pbPhysical");
        pbPhysical->setValue(0);
        pbPhysical->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayoutMemory->addWidget(pbPhysical, 1, 0, 1, 2);

        pbVirtual = new QProgressBar(groupBoxMemory);
        pbVirtual->setObjectName("pbVirtual");
        pbVirtual->setValue(0);
        pbVirtual->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayoutMemory->addWidget(pbVirtual, 4, 0, 1, 2);

        pbSystemWS = new QProgressBar(groupBoxMemory);
        pbSystemWS->setObjectName("pbSystemWS");
        pbSystemWS->setValue(0);
        pbSystemWS->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayoutMemory->addWidget(pbSystemWS, 6, 0, 1, 2);


        verticalLayout->addWidget(groupBoxMemory);

        groupBoxOptions = new QGroupBox(centralwidget);
        groupBoxOptions->setObjectName("groupBoxOptions");
        gridLayoutOptions = new QGridLayout(groupBoxOptions);
        gridLayoutOptions->setObjectName("gridLayoutOptions");
        cbWorkingSet = new QCheckBox(groupBoxOptions);
        cbWorkingSet->setObjectName("cbWorkingSet");
        cbWorkingSet->setChecked(true);

        gridLayoutOptions->addWidget(cbWorkingSet, 0, 0, 1, 1);

        cbSystemWorkingSet = new QCheckBox(groupBoxOptions);
        cbSystemWorkingSet->setObjectName("cbSystemWorkingSet");

        gridLayoutOptions->addWidget(cbSystemWorkingSet, 0, 1, 1, 1);

        cbStandbyList = new QCheckBox(groupBoxOptions);
        cbStandbyList->setObjectName("cbStandbyList");
        cbStandbyList->setChecked(true);

        gridLayoutOptions->addWidget(cbStandbyList, 1, 0, 1, 1);

        cbStandbyListLow = new QCheckBox(groupBoxOptions);
        cbStandbyListLow->setObjectName("cbStandbyListLow");

        gridLayoutOptions->addWidget(cbStandbyListLow, 1, 1, 1, 1);

        cbModifiedPageList = new QCheckBox(groupBoxOptions);
        cbModifiedPageList->setObjectName("cbModifiedPageList");

        gridLayoutOptions->addWidget(cbModifiedPageList, 2, 0, 1, 1);

        cbCombinePageList = new QCheckBox(groupBoxOptions);
        cbCombinePageList->setObjectName("cbCombinePageList");

        gridLayoutOptions->addWidget(cbCombinePageList, 2, 1, 1, 1);


        verticalLayout->addWidget(groupBoxOptions);

        groupBoxControl = new QGroupBox(centralwidget);
        groupBoxControl->setObjectName("groupBoxControl");
        horizontalLayoutControl = new QHBoxLayout(groupBoxControl);
        horizontalLayoutControl->setObjectName("horizontalLayoutControl");
        btnCleanNow = new QPushButton(groupBoxControl);
        btnCleanNow->setObjectName("btnCleanNow");

        horizontalLayoutControl->addWidget(btnCleanNow);

        cbAutoClean = new QCheckBox(groupBoxControl);
        cbAutoClean->setObjectName("cbAutoClean");

        horizontalLayoutControl->addWidget(cbAutoClean);

        labelInterval = new QLabel(groupBoxControl);
        labelInterval->setObjectName("labelInterval");

        horizontalLayoutControl->addWidget(labelInterval);

        sbInterval = new QSpinBox(groupBoxControl);
        sbInterval->setObjectName("sbInterval");
        sbInterval->setMinimum(5);
        sbInterval->setMaximum(3600);
        sbInterval->setValue(60);

        horizontalLayoutControl->addWidget(sbInterval);


        verticalLayout->addWidget(groupBoxControl);

        textBrowser = new QTextBrowser(centralwidget);
        textBrowser->setObjectName("textBrowser");

        verticalLayout->addWidget(textBrowser);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        lbLastUpdate = new QLabel(centralwidget);
        lbLastUpdate->setObjectName("lbLastUpdate");
        lbLastUpdate->setAlignment(Qt::AlignmentFlag::AlignRight);

        verticalLayout->addWidget(lbLastUpdate);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 400, 17));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName("menuHelp");

        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);


        menubar->addAction(menuHelp->menuAction());
        menuHelp->addAction(actionAbout);


        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\345\206\205\345\255\230\346\270\205\347\220\206\345\267\245\345\205\267", nullptr));
        actionAbout->setText(QCoreApplication::translate("MainWindow", "\345\205\263\344\272\216", nullptr));
        actionAutoStart->setText(QCoreApplication::translate("MainWindow", "\345\274\200\346\234\272\350\207\252\345\220\257", nullptr));
        groupBoxMemory->setTitle(QCoreApplication::translate("MainWindow", "\345\206\205\345\255\230\347\212\266\346\200\201", nullptr));
        lbPhysicalValue->setText(QCoreApplication::translate("MainWindow", "0.00 GB / 0.00 GB", nullptr));
        labelPhysical->setText(QCoreApplication::translate("MainWindow", "\347\211\251\347\220\206\345\206\205\345\255\230:", nullptr));
        labelSystemWS->setText(QCoreApplication::translate("MainWindow", "\347\263\273\347\273\237\345\267\245\344\275\234\351\233\206:", nullptr));
        lbVirtualValue->setText(QCoreApplication::translate("MainWindow", "0.00 GB / 0.00 GB", nullptr));
        lbSystemWSValue->setText(QCoreApplication::translate("MainWindow", "0.00 GB (\345\205\261\344\272\253 0.00 GB) / 0.00 GB", nullptr));
        labelVirtual->setText(QCoreApplication::translate("MainWindow", "\350\231\232\346\213\237\345\206\205\345\255\230:", nullptr));
        pbPhysical->setFormat(QCoreApplication::translate("MainWindow", "%p%", nullptr));
        pbVirtual->setFormat(QCoreApplication::translate("MainWindow", "%p%", nullptr));
        pbSystemWS->setFormat(QCoreApplication::translate("MainWindow", "%p%", nullptr));
        groupBoxOptions->setTitle(QCoreApplication::translate("MainWindow", "\346\270\205\347\220\206\351\200\211\351\241\271", nullptr));
        cbWorkingSet->setText(QCoreApplication::translate("MainWindow", "Working Set", nullptr));
        cbSystemWorkingSet->setText(QCoreApplication::translate("MainWindow", "System Working Set", nullptr));
        cbStandbyList->setText(QCoreApplication::translate("MainWindow", "Standby List", nullptr));
        cbStandbyListLow->setText(QCoreApplication::translate("MainWindow", "Standby List (Low Priority)", nullptr));
        cbModifiedPageList->setText(QCoreApplication::translate("MainWindow", "Modified Page List", nullptr));
        cbCombinePageList->setText(QCoreApplication::translate("MainWindow", "Combine Page List", nullptr));
        groupBoxControl->setTitle(QCoreApplication::translate("MainWindow", "\346\216\247\345\210\266", nullptr));
        btnCleanNow->setText(QCoreApplication::translate("MainWindow", "\347\253\213\345\215\263\346\270\205\347\220\206", nullptr));
        cbAutoClean->setText(QCoreApplication::translate("MainWindow", "\350\207\252\345\212\250\346\270\205\347\220\206", nullptr));
        labelInterval->setText(QCoreApplication::translate("MainWindow", "\351\227\264\351\232\224(\347\247\222):", nullptr));
        lbLastUpdate->setText(QCoreApplication::translate("MainWindow", "\346\234\200\345\220\216\346\233\264\346\226\260: -", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "\345\270\256\345\212\251", nullptr));

    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
