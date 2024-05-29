#ifndef PLAYLISTWIDGET_H
#define PLAYLISTWIDGET_H

#include <QWidget>
#include "basewidget.h"
#include <QList>

class QTableWidgetItem;

namespace Ui {
class CPlayListWidget;
}

struct SListEntry{
    bool isCurrent{false};
    QString filename;
    QString title;
    int order{-1};
    qint64 duration{-1};
    SListEntry(const QString & filename = QString());
    QString toString()const;
};

class CPlayListWidget : public CBaseWidget
{
    Q_OBJECT

public:
    explicit CPlayListWidget(QWidget *parent = nullptr);
    ~CPlayListWidget();

signals:
    void closed();

private slots:
    void on_pushButtonClose_clicked();

    void on_tableWidgetPlaylist_itemDoubleClicked(QTableWidgetItem *item);

    void on_pushButtonAdd_clicked();
    void updateEntryUI(const SListEntry & e);
    void queryMediaMetadata(SListEntry listEntry);
private:
    void closeEvent(QCloseEvent *e)override;
    void resizeEvent(QResizeEvent *e)override;
    void adjustTableSize();
    const SListEntry &addFile(const QString &);
    void updateUI();

    QList<SListEntry> playlist;
    Ui::CPlayListWidget *ui;
};

#endif // PLAYLISTWIDGET_H
