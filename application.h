#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

class CMainWidget;
class CPlayListWidget;

class CApplication : public QApplication
{
public:
    CApplication(int &argc, char **argv);
    int Run();

public slots:
    void showPlaylist(bool);


private:
    CMainWidget *mainWidget;
    CPlayListWidget *playListWidget;
};

#endif // APPLICATION_H
