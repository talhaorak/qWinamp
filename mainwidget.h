#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "basewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class CMainWidget;
}
QT_END_NAMESPACE

class QMediaPlayer;

class CMainWidget : public CBaseWidget
{
    Q_OBJECT

public:
    CMainWidget(QWidget *parent = nullptr);
    ~CMainWidget();

private slots:
    void on_pushButtonCloseApp_clicked();
    void on_actionOpen_File_triggered();

    void onMediaPosChanged(qint64);
    void onMediaMetaDataChanged();
    void openMediaFile(const QString&);
    void resetUI();
    void on_horizontalSliderPosition_sliderReleased();

    void on_actionShow_Playlist_toggled(bool arg1);

public slots:
    void onPlaylistClosed();

private:

    QMediaPlayer *player;
    Ui::CMainWidget *ui;
};
#endif // MAINWIDGET_H

