#include "windowwidgetmanager.h"

#include <QWinWidget>

#include "pch.hpp"

WindowWidgetManager::WindowWidgetManager(int applicationHwnd, QObject *parent)
    : QObject(parent)
{
    m_winWidget = new QWinWidget( (HWND)applicationHwnd );
    m_winWidget->showCentered();
}

WindowWidgetManager::~WindowWidgetManager()
{
    delete m_winWidget;
}

QWidget *WindowWidgetManager::widget() const
{
    return m_winWidget;
}

