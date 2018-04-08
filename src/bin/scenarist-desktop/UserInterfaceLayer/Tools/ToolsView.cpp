#include "ToolsView.h"
#include "ToolsSettings.h"

#include <UserInterfaceLayer/ScenarioTextEdit/ScenarioTextEdit.h>

#include <3rd_party/Delegates/TreeViewItemDelegate/TreeViewItemDelegate.h>
#include <3rd_party/Styles/TreeViewProxyStyle/TreeViewProxyStyle.h>
#include <3rd_party/Widgets/FlatButton/FlatButton.h>
#include <3rd_party/Widgets/ScalableWrapper/ScalableWrapper.h>
#include <3rd_party/Widgets/WAF/StackedWidgetAnimation/StackedWidgetAnimation.h>

#include <QLabel>
#include <QSplitter>
#include <QStackedWidget>
#include <QTreeWidget>
#include <QVBoxLayout>

using UserInterface::ScenarioTextEdit;
using UserInterface::ToolsSettings;
using UserInterface::ToolsView;


ToolsView::ToolsView(QWidget* _parent) :
    QWidget(_parent),
    m_leftTopEmptyLabel(new QLabel(this)),
    m_rightTopEmptyLabel(new QLabel(this)),
    m_settings(new FlatButton(this)),
    m_restore(new FlatButton(this)),
    m_navigation(new QStackedWidget(this)),
    m_toolsTypes(new QTreeWidget(this)),
    m_toolsSettings(new ToolsSettings(this)),
    m_editor(new ScenarioTextEdit(this)),
    m_editorWrapper(new ScalableWrapper(m_editor, this))
{
    initView();
    initConnections();
    initStyleSheet();
}

void ToolsView::initView()
{
    m_leftTopEmptyLabel->setText(tr("Tools"));

    m_settings->setIcons(QIcon(":/Graphics/Iconset/settings.svg"));
    m_settings->setToolTip(tr("Tool settings"));
    m_restore->setIcons(QIcon(":/Graphics/Iconset/check.svg"));
    m_restore->setToolTip(tr("Restore script"));

    //
    // Настраиваем панель со списком инструментов
    //
    QTreeWidgetItem* versions = new QTreeWidgetItem(m_toolsTypes, { tr("Compare script versions") });
    versions->setData(0, Qt::DecorationRole, QPixmap(":/Graphics/Iconset/wrench.svg"));
    m_toolsTypes->addTopLevelItem(versions);
    QTreeWidgetItem* backups = new QTreeWidgetItem(m_toolsTypes, { tr("Restore script from backup") });
    backups->setData(0, Qt::DecorationRole, QPixmap(":/Graphics/Iconset/wrench.svg"));
    m_toolsTypes->addTopLevelItem(backups);

    m_toolsTypes->setObjectName("ToolsTypesTree");
    m_toolsTypes->expandAll();
    m_toolsTypes->setHeaderHidden(true);
    m_toolsTypes->setItemDelegate(new TreeViewItemDelegate(m_toolsTypes));
    m_toolsTypes->setStyle(new TreeViewProxyStyle(m_toolsTypes->style()));

    //
    // Настраиваем общую панель с группами отчётов
    //
    QHBoxLayout* toolsToolbarLayout = new QHBoxLayout;
    toolsToolbarLayout->setContentsMargins(QMargins());
    toolsToolbarLayout->setSpacing(0);
    toolsToolbarLayout->addWidget(m_leftTopEmptyLabel);
    toolsToolbarLayout->addWidget(m_settings);
    //
    QVBoxLayout* toolsLayout = new QVBoxLayout;
    toolsLayout->setContentsMargins(QMargins());
    toolsLayout->setSpacing(0);
    toolsLayout->addLayout(toolsToolbarLayout);
    toolsLayout->addWidget(m_toolsTypes);

    //
    // Настраиваем виджет навигации целиком
    //
    QWidget* toolsPanel = new QWidget(this);
    toolsPanel->setObjectName("toolsTypesPanel");
    toolsPanel->setLayout(toolsLayout);
    m_navigation->addWidget(toolsPanel);
    m_navigation->addWidget(m_toolsSettings);

    //
    // Настраиваем панель с результатами работы инструментов
    //
    QHBoxLayout* toolDataToolbarLayout = new QHBoxLayout;
    toolDataToolbarLayout->setContentsMargins(QMargins());
    toolDataToolbarLayout->setSpacing(0);
    toolDataToolbarLayout->addWidget(m_restore);
    toolDataToolbarLayout->addWidget(m_rightTopEmptyLabel);
    //
    QVBoxLayout* toolDataLayout = new QVBoxLayout;
    toolDataLayout->setContentsMargins(QMargins());
    toolDataLayout->setSpacing(0);
    toolDataLayout->addLayout(toolDataToolbarLayout);
    toolDataLayout->addWidget(m_editorWrapper, 1);

    //
    // Настраиваем виджет результатов работы инструментов целиком
    //
    QWidget* toolDataPanel = new QWidget(this);
    toolDataPanel->setObjectName("toolDataPanel");
    toolDataPanel->setLayout(toolDataLayout);


    //
    // Объединяем всё
    //
    QSplitter* splitter = new QSplitter(this);
    splitter->setObjectName("toolsSplitter");
    splitter->setHandleWidth(1);
    splitter->setOpaqueResize(false);
    splitter->setChildrenCollapsible(false);
    splitter->addWidget(m_navigation);
    splitter->addWidget(toolDataPanel);

    QHBoxLayout* layout = new QHBoxLayout;
    layout->setContentsMargins(QMargins());
    layout->setSpacing(0);
    layout->addWidget(splitter);
    setLayout(layout);
}

void ToolsView::initConnections()
{
    connect(m_toolsTypes, &QTreeWidget::currentItemChanged, [this] (QTreeWidgetItem* _item) {
        m_toolsSettings->setTitle(_item->text(0));
        m_toolsSettings->setCurrentType(m_toolsTypes->indexOfTopLevelItem(_item));
    });
    connect(m_settings, &FlatButton::clicked, [this] { WAF::StackedWidgetAnimation::slide(m_navigation, m_toolsSettings, WAF::FromRightToLeft); });
    connect(m_toolsSettings, &ToolsSettings::backPressed, [this] { WAF::StackedWidgetAnimation::slide(m_navigation, m_navigation->widget(0), WAF::FromLeftToRight); });
}

void ToolsView::initStyleSheet()
{
    m_leftTopEmptyLabel->setProperty("inTopPanel", true);
    m_leftTopEmptyLabel->setProperty("topPanelTopBordered", true);

    m_settings->setProperty("inTopPanel", true);

    m_rightTopEmptyLabel->setProperty("inTopPanel", true);
    m_rightTopEmptyLabel->setProperty("topPanelTopBordered", true);
    m_rightTopEmptyLabel->setProperty("topPanelRightBordered", true);

    m_restore->setProperty("inTopPanel", true);

    m_toolsTypes->setProperty("mainContainer", true);
    m_editorWrapper->setProperty("mainContainer", true);
}
