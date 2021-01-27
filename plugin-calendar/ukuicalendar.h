﻿/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * Copyright: 2012-2013 Razor team
 * Authors:
 *   Kuzma Shapran <kuzma.shapran@gmail.com>
 *
 * Copyright: 2019 Tianjin KYLIN Information Technology Co., Ltd. *
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */


#include <QTimeZone>

#include <QDialog>
#include <QLabel>
#include <QtWebKitWidgets/QWebView>
#include <QGSettings>
#include <QMenu>
#include "../panel/iukuipanelplugin.h"
#include "ukuiwebviewdialog.h"
class QTimer;
class CalendarActiveLabel;
class UkuiCalendarWebView;

class IndicatorCalendar : public QWidget, public IUKUIPanelPlugin
{
    Q_OBJECT
public:
    IndicatorCalendar(const IUKUIPanelPluginStartupInfo &startupInfo);
    ~IndicatorCalendar();

    virtual QWidget *widget() { return mMainWidget; }
    virtual QString themeId() const { return QLatin1String("Calendar"); }
//    virtual IUKUIPanelPlugin::Flags flags() const { return PreferRightAlignment | HaveConfigDialog ; }
    bool isSeparate() const { return true; }
    void activated(ActivationReason reason);

    void realign()override;
    void initializeCalendar();
    /**
     * @brief setbackground 设置背景相关，mate-panel以及最初的ukui-panel使用了此样式，后续此样式无效
     */
    void setbackground();
    void setTimeShowStyle();
    /**
     * @brief modifyCalendarWidget 修改日历显示位置
     */
    void modifyCalendarWidget();
//signals:
Q_SIGNALS:
    void deactivated();

private Q_SLOTS:
    void timeout();
    void deletePopup();
    void updateTimeText();
    void hidewebview();

private:
    QWidget *mMainWidget;
    UkuiWebviewDialog   *mWebViewDiag;
    bool mbActived;
    bool mbHasCreatedWebView;
    int font_size;
    CalendarActiveLabel *mContent;

    QTimer *mTimer;
    int mUpdateInterval;

    int16_t mViewWidht;
    int16_t mViewHeight;

    QStringList mTimeZones;
    QMap<QString, QString> mTimeZoneCustomNames;
    QString mDefaultTimeZone;
    QString mActiveTimeZone;
    QString mFormat;

    bool mAutoRotate;
    QLabel *mPopupContent;

    QDateTime mShownTime;

    void restartTimer();

    bool mbIsNeedUpdate;
    QGSettings *gsettings;
    QString hourSystemMode;

    QString hourSystem_24_horzontal;
    QString hourSystem_24_vartical;
    QString hourSystem_12_horzontal;
    QString hourSystem_12_vartical;
    QString current_date;
    IUKUIPanelPlugin * mPlugin;

};


class CalendarActiveLabel : public QLabel
{
Q_OBJECT

public:
    explicit CalendarActiveLabel(IUKUIPanelPlugin *plugin,QWidget * = NULL);

    IUKUIPanelPlugin * mPlugin;
Q_SIGNALS:
    void wheelScrolled(int);
    void leftMouseButtonClicked();
    void middleMouseButtonClicked();

protected:
    void wheelEvent(QWheelEvent *);
    void mouseReleaseEvent(QMouseEvent* event);
    /**
     * @brief contextMenuEvent 右键菜单设置项
     * @param event
     */
    virtual void contextMenuEvent(QContextMenuEvent *event);

private Q_SLOTS:
    /**
     * @brief setControlTime 右键菜单选项，在控制面板设置时间
     */
    void setControlTime();
    /**
     * @brief setUpPanel 设置任务栏选项
     */
    void setUpPanel();


};

class UKUICalendarPluginLibrary: public QObject, public IUKUIPanelPluginLibrary
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ukui.org/Panel/PluginInterface/3.0")
    Q_INTERFACES(IUKUIPanelPluginLibrary)
public:
    IUKUIPanelPlugin *instance(const IUKUIPanelPluginStartupInfo &startupInfo) const
    {
        return new IndicatorCalendar(startupInfo);
    }
};
