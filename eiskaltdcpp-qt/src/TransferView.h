/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 3 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef TRANSFERVIEW_H
#define TRANSFERVIEW_H

#include <QWidget>
#include <QAction>
#include <QMenu>
#include <QCloseEvent>
#include <QResizeEvent>
#include <QHideEvent>
#include <QHeaderView>

#include "ui_UITransferView.h"

#include "dcpp/stdinc.h"
#include "dcpp/DCPlusPlus.h"
#include "dcpp/Singleton.h"
#include "dcpp/ConnectionManager.h"
#include "dcpp/DownloadManager.h"
#include "dcpp/LogManager.h"
#include "dcpp/QueueManager.h"
#include "dcpp/UploadManager.h"


class TransferViewModel;

class TransferView : public QWidget,
                     private Ui::UITransferView,
                     public dcpp::Singleton<TransferView>,
                     public dcpp::ConnectionManagerListener,
                     public dcpp::DownloadManagerListener,
                     public dcpp::QueueManagerListener,
                     public dcpp::UploadManagerListener
{
    Q_OBJECT

friend class dcpp::Singleton<TransferView>;

typedef QMap<QString, QVariant> VarMap;

class Menu{

public:
    enum Action {
        Browse=0,
        SearchAlternates,
        MatchQueue,
        SendPM,
        AddToFav,
        GrantExtraSlot,
        Copy,
        RemoveFromQueue,
        Force,
        Close,
        None
    };

    Menu();
    virtual ~Menu();

    Action exec();
    int copyColumn() const {return selectedColumn; }

private:
    QMenu *menu;
    QMenu *copy_column;
    QMap <QAction*, Action> actions;
    int selectedColumn;
};

public:
    QSize sizeHint() const;

Q_SIGNALS:
    /** DownloadManger signals */
    void coreDMRequesting(VarMap);
    void coreDMStarting(VarMap);
    void coreDMTick(VarMap);
    void coreDMComplete(VarMap);
    void coreDMFailed(VarMap);

    /** ConnectionManager  signals */
    void coreCMAdded(VarMap);
    void coreCMConnected(VarMap);
    void coreCMRemoved(VarMap);
    void coreCMFailed(VarMap);
    void coreCMStatusChanged(VarMap);

    /** QueueManager signals */
    void coreQMFinished(VarMap);
    void coreQMRemoved(VarMap);

    /** UploadManager signals */
    void coreUMStarting(VarMap);
    void coreUMTick(VarMap);
    void coreUMComplete(VarMap);
    void coreUMFailed(VarMap);

    void coreUpdateParents();
    void coreUpdateTransferPosition(VarMap, qint64);

    void coreDownloadComplete(QString);

    void loadDMD(QString);

protected:
    virtual void resizeEvent(QResizeEvent *);
    virtual void closeEvent(QCloseEvent *);
    virtual void hideEvent(QHideEvent *);

    void getFileList(const QString &, const QString &);
    void matchQueue(const QString &, const QString &);
    void addFavorite(const QString&);
    void grantSlot(const QString&, const QString&);
    void removeFromQueue(const QString&);
    void forceAttempt(const QString&);
    void closeConection(const QString &, bool);
    void searchAlternates(const QString &tth);
    // DownloadManager
    virtual void on(dcpp::DownloadManagerListener::Requesting, dcpp::Download* dl) throw();
    virtual void on(dcpp::DownloadManagerListener::Starting, dcpp::Download* dl) throw();
    virtual void on(dcpp::DownloadManagerListener::Tick, const dcpp::DownloadList& dls) throw();
    virtual void on(dcpp::DownloadManagerListener::Complete, dcpp::Download* dl) throw();
    virtual void on(dcpp::DownloadManagerListener::Failed, dcpp::Download* dl, const std::string& reason) throw();
    // ConnectionManager
    virtual void on(dcpp::ConnectionManagerListener::Added, dcpp::ConnectionQueueItem* cqi) throw();
    virtual void on(dcpp::ConnectionManagerListener::Connected, dcpp::ConnectionQueueItem* cqi) throw();
    virtual void on(dcpp::ConnectionManagerListener::Removed, dcpp::ConnectionQueueItem* cqi) throw();
    virtual void on(dcpp::ConnectionManagerListener::Failed, dcpp::ConnectionQueueItem* cqi, const std::string&) throw();
    virtual void on(dcpp::ConnectionManagerListener::StatusChanged, dcpp::ConnectionQueueItem* cqi) throw();
    // QueueManager
    virtual void on(dcpp::QueueManagerListener::Finished, dcpp::QueueItem*, const std::string&, int64_t size) throw();
    virtual void on(dcpp::QueueManagerListener::Removed, dcpp::QueueItem*) throw();
    // UploadManager
    virtual void on(dcpp::UploadManagerListener::Starting, dcpp::Upload* ul) throw();
    virtual void on(dcpp::UploadManagerListener::Tick, const dcpp::UploadList& uls) throw();
    virtual void on(dcpp::UploadManagerListener::Complete, dcpp::Upload* ul) throw();
    virtual void on(dcpp::UploadManagerListener::Failed, dcpp::Upload* ul, const std::string& reason) throw();

private Q_SLOTS:
    void slotContextMenu(const QPoint&);
    void slotHeaderMenu(const QPoint&);
    void downloadComplete(QString);
    \
private:
    TransferView(QWidget* = NULL);
    virtual ~TransferView();

    void load();
    void save();

    inline QString      vstr(const QVariant &var) { return var.toString(); }
    inline int          vint(const QVariant &var) { return var.toInt(); }
    inline double       vdbl(const QVariant &var) { return var.toDouble(); }
    inline qlonglong    vlng(const QVariant &var) { return var.toLongLong(); }

    void getParams(VarMap&, const dcpp::ConnectionQueueItem*);
    void getParams(VarMap&, const dcpp::Transfer*);

    void init();

    TransferViewModel *model;
};

#endif // TRANSFERVIEW_H
