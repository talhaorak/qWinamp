#include "application.h"
#include "mainwidget.h"
#include "playlistwidget.h"

CApplication::CApplication(int &argc, char **argv)
    :QApplication(argc, argv)
{
    mainWidget = new CMainWidget();
    playListWidget = new CPlayListWidget();

    connect(playListWidget, &CPlayListWidget::closed, mainWidget, &CMainWidget::onPlaylistClosed);
}

int CApplication::Run()
{
    mainWidget->show();
    return exec();
}

void CApplication::showPlaylist(bool yes)
{
    playListWidget->setVisible(yes);
    if(yes){
        playListWidget->move(mainWidget->geometry().bottomLeft());
        playListWidget->resize(mainWidget->width(), 600);
    }
}

