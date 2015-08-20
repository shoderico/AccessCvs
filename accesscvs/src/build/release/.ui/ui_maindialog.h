/********************************************************************************
** Form generated from reading UI file 'maindialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINDIALOG_H
#define UI_MAINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_MainDialog
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QTreeView *treeView;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_3;
    QLabel *elapsedTimeLabel;
    QLabel *progressCountLabel;
    QLabel *processTypeNameLabel;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_2;
    QProgressBar *progressBar;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_2;
    QCheckBox *showSelectedOnlyCheckBox;
    QCheckBox *showAllCheckBox;
    QCheckBox *showTableCheckBox;
    QCheckBox *showQueryCheckBox;
    QCheckBox *showFormCheckBox;
    QCheckBox *showReportCheckBox;
    QCheckBox *showMacroCheckBox;
    QCheckBox *showModuleCheckBox;
    QCheckBox *showReferenceCheckBox;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QPushButton *selectAutoButton;
    QCheckBox *selectAllCheckBox;
    QCheckBox *selectTableCheckBox;
    QCheckBox *selectQueryCheckBox;
    QCheckBox *selectFormCheckBox;
    QCheckBox *selectReportCheckBox;
    QCheckBox *selectMacroCheckBox;
    QCheckBox *selectModuleCheckBox;
    QCheckBox *selectReferenceCheckBox;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *clearCacheButton;
    QPushButton *refreshItemsButton;
    QPushButton *executeExportButton;
    QPushButton *executeImportButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *MainDialog)
    {
        if (MainDialog->objectName().isEmpty())
            MainDialog->setObjectName(QStringLiteral("MainDialog"));
        MainDialog->resize(982, 563);
        gridLayout = new QGridLayout(MainDialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        treeView = new QTreeView(MainDialog);
        treeView->setObjectName(QStringLiteral("treeView"));

        verticalLayout->addWidget(treeView);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_3 = new QLabel(MainDialog);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_5->addWidget(label_3);

        elapsedTimeLabel = new QLabel(MainDialog);
        elapsedTimeLabel->setObjectName(QStringLiteral("elapsedTimeLabel"));

        horizontalLayout_5->addWidget(elapsedTimeLabel);

        progressCountLabel = new QLabel(MainDialog);
        progressCountLabel->setObjectName(QStringLiteral("progressCountLabel"));

        horizontalLayout_5->addWidget(progressCountLabel);

        processTypeNameLabel = new QLabel(MainDialog);
        processTypeNameLabel->setObjectName(QStringLiteral("processTypeNameLabel"));

        horizontalLayout_5->addWidget(processTypeNameLabel);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));

        verticalLayout->addLayout(horizontalLayout_2);

        progressBar = new QProgressBar(MainDialog);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(24);

        verticalLayout->addWidget(progressBar);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_2 = new QLabel(MainDialog);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_4->addWidget(label_2);

        showSelectedOnlyCheckBox = new QCheckBox(MainDialog);
        showSelectedOnlyCheckBox->setObjectName(QStringLiteral("showSelectedOnlyCheckBox"));

        horizontalLayout_4->addWidget(showSelectedOnlyCheckBox);

        showAllCheckBox = new QCheckBox(MainDialog);
        showAllCheckBox->setObjectName(QStringLiteral("showAllCheckBox"));

        horizontalLayout_4->addWidget(showAllCheckBox);

        showTableCheckBox = new QCheckBox(MainDialog);
        showTableCheckBox->setObjectName(QStringLiteral("showTableCheckBox"));

        horizontalLayout_4->addWidget(showTableCheckBox);

        showQueryCheckBox = new QCheckBox(MainDialog);
        showQueryCheckBox->setObjectName(QStringLiteral("showQueryCheckBox"));

        horizontalLayout_4->addWidget(showQueryCheckBox);

        showFormCheckBox = new QCheckBox(MainDialog);
        showFormCheckBox->setObjectName(QStringLiteral("showFormCheckBox"));

        horizontalLayout_4->addWidget(showFormCheckBox);

        showReportCheckBox = new QCheckBox(MainDialog);
        showReportCheckBox->setObjectName(QStringLiteral("showReportCheckBox"));

        horizontalLayout_4->addWidget(showReportCheckBox);

        showMacroCheckBox = new QCheckBox(MainDialog);
        showMacroCheckBox->setObjectName(QStringLiteral("showMacroCheckBox"));

        horizontalLayout_4->addWidget(showMacroCheckBox);

        showModuleCheckBox = new QCheckBox(MainDialog);
        showModuleCheckBox->setObjectName(QStringLiteral("showModuleCheckBox"));

        horizontalLayout_4->addWidget(showModuleCheckBox);

        showReferenceCheckBox = new QCheckBox(MainDialog);
        showReferenceCheckBox->setObjectName(QStringLiteral("showReferenceCheckBox"));

        horizontalLayout_4->addWidget(showReferenceCheckBox);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label = new QLabel(MainDialog);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_3->addWidget(label);

        selectAutoButton = new QPushButton(MainDialog);
        selectAutoButton->setObjectName(QStringLiteral("selectAutoButton"));

        horizontalLayout_3->addWidget(selectAutoButton);

        selectAllCheckBox = new QCheckBox(MainDialog);
        selectAllCheckBox->setObjectName(QStringLiteral("selectAllCheckBox"));

        horizontalLayout_3->addWidget(selectAllCheckBox);

        selectTableCheckBox = new QCheckBox(MainDialog);
        selectTableCheckBox->setObjectName(QStringLiteral("selectTableCheckBox"));

        horizontalLayout_3->addWidget(selectTableCheckBox);

        selectQueryCheckBox = new QCheckBox(MainDialog);
        selectQueryCheckBox->setObjectName(QStringLiteral("selectQueryCheckBox"));

        horizontalLayout_3->addWidget(selectQueryCheckBox);

        selectFormCheckBox = new QCheckBox(MainDialog);
        selectFormCheckBox->setObjectName(QStringLiteral("selectFormCheckBox"));

        horizontalLayout_3->addWidget(selectFormCheckBox);

        selectReportCheckBox = new QCheckBox(MainDialog);
        selectReportCheckBox->setObjectName(QStringLiteral("selectReportCheckBox"));

        horizontalLayout_3->addWidget(selectReportCheckBox);

        selectMacroCheckBox = new QCheckBox(MainDialog);
        selectMacroCheckBox->setObjectName(QStringLiteral("selectMacroCheckBox"));

        horizontalLayout_3->addWidget(selectMacroCheckBox);

        selectModuleCheckBox = new QCheckBox(MainDialog);
        selectModuleCheckBox->setObjectName(QStringLiteral("selectModuleCheckBox"));

        horizontalLayout_3->addWidget(selectModuleCheckBox);

        selectReferenceCheckBox = new QCheckBox(MainDialog);
        selectReferenceCheckBox->setObjectName(QStringLiteral("selectReferenceCheckBox"));

        horizontalLayout_3->addWidget(selectReferenceCheckBox);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        clearCacheButton = new QPushButton(MainDialog);
        clearCacheButton->setObjectName(QStringLiteral("clearCacheButton"));

        horizontalLayout->addWidget(clearCacheButton);

        refreshItemsButton = new QPushButton(MainDialog);
        refreshItemsButton->setObjectName(QStringLiteral("refreshItemsButton"));

        horizontalLayout->addWidget(refreshItemsButton);

        executeExportButton = new QPushButton(MainDialog);
        executeExportButton->setObjectName(QStringLiteral("executeExportButton"));

        horizontalLayout->addWidget(executeExportButton);

        executeImportButton = new QPushButton(MainDialog);
        executeImportButton->setObjectName(QStringLiteral("executeImportButton"));

        horizontalLayout->addWidget(executeImportButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        okButton = new QPushButton(MainDialog);
        okButton->setObjectName(QStringLiteral("okButton"));

        horizontalLayout->addWidget(okButton);

        cancelButton = new QPushButton(MainDialog);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));

        horizontalLayout->addWidget(cancelButton);


        verticalLayout->addLayout(horizontalLayout);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(MainDialog);

        QMetaObject::connectSlotsByName(MainDialog);
    } // setupUi

    void retranslateUi(QDialog *MainDialog)
    {
        MainDialog->setWindowTitle(QApplication::translate("MainDialog", "Dialog", 0));
        label_3->setText(QApplication::translate("MainDialog", "Elapsed Time : ", 0));
        elapsedTimeLabel->setText(QApplication::translate("MainDialog", "00:00:00.000", 0));
        progressCountLabel->setText(QApplication::translate("MainDialog", "( 0000 / 0000 )", 0));
        processTypeNameLabel->setText(QString());
        progressBar->setFormat(QApplication::translate("MainDialog", "%p%", 0));
        label_2->setText(QApplication::translate("MainDialog", "Filter :", 0));
        showSelectedOnlyCheckBox->setText(QApplication::translate("MainDialog", "Selected Only", 0));
        showAllCheckBox->setText(QApplication::translate("MainDialog", "All", 0));
        showTableCheckBox->setText(QApplication::translate("MainDialog", "Tables", 0));
        showQueryCheckBox->setText(QApplication::translate("MainDialog", "Queries", 0));
        showFormCheckBox->setText(QApplication::translate("MainDialog", "Forms", 0));
        showReportCheckBox->setText(QApplication::translate("MainDialog", "Reports", 0));
        showMacroCheckBox->setText(QApplication::translate("MainDialog", "Macros", 0));
        showModuleCheckBox->setText(QApplication::translate("MainDialog", "Modules", 0));
        showReferenceCheckBox->setText(QApplication::translate("MainDialog", "Reference", 0));
        label->setText(QApplication::translate("MainDialog", "Selection :", 0));
        selectAutoButton->setText(QApplication::translate("MainDialog", "Auto", 0));
        selectAllCheckBox->setText(QApplication::translate("MainDialog", "All", 0));
        selectTableCheckBox->setText(QApplication::translate("MainDialog", "Tables", 0));
        selectQueryCheckBox->setText(QApplication::translate("MainDialog", "Queries", 0));
        selectFormCheckBox->setText(QApplication::translate("MainDialog", "Forms", 0));
        selectReportCheckBox->setText(QApplication::translate("MainDialog", "Reports", 0));
        selectMacroCheckBox->setText(QApplication::translate("MainDialog", "Macros", 0));
        selectModuleCheckBox->setText(QApplication::translate("MainDialog", "Modules", 0));
        selectReferenceCheckBox->setText(QApplication::translate("MainDialog", "Reference", 0));
        clearCacheButton->setText(QApplication::translate("MainDialog", "clear cache", 0));
        refreshItemsButton->setText(QApplication::translate("MainDialog", "Refresh", 0));
        executeExportButton->setText(QApplication::translate("MainDialog", "Export", 0));
        executeImportButton->setText(QApplication::translate("MainDialog", "Import", 0));
        okButton->setText(QApplication::translate("MainDialog", "OK", 0));
        cancelButton->setText(QApplication::translate("MainDialog", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class MainDialog: public Ui_MainDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINDIALOG_H
