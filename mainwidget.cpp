#include "mainwidget.h"
#include "./ui_mainwidget.h"
#include <QTimer>
#include <QMouseEvent>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QMediaMetaData>
#include <QTime>
#include "application.h"

CMainWidget::CMainWidget(QWidget *parent)
    : CBaseWidget(parent)
    , ui(new Ui::CMainWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    player = new QMediaPlayer;
    auto audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);
    connect(player, &QMediaPlayer::positionChanged, this, &CMainWidget::onMediaPosChanged);
    connect(player, &QMediaPlayer::metaDataChanged, this, &CMainWidget::onMediaMetaDataChanged);
    //player->setSource(QUrl::fromLocalFile("/Users/me/Music/coolsong.mp3"));
    audioOutput->setVolume(75);
    // player->play();
}

CMainWidget::~CMainWidget()
{
    delete ui;
}


void CMainWidget::on_pushButtonCloseApp_clicked()
{
    qApp->exit(0);
}


void CMainWidget::on_actionOpen_File_triggered()
{
    auto filename = QFileDialog::getOpenFileName(this, "Open Media...");
    if(filename.isEmpty())return;
    openMediaFile(filename);
}

void CMainWidget::onMediaPosChanged(qint64 pos)
{
    if(!ui->horizontalSliderPosition->isSliderDown()){
        auto sliderPos = ui->horizontalSliderPosition->maximum() * pos / player->duration();
        ui->horizontalSliderPosition->setSliderPosition(sliderPos);
    }

    auto posStr = QTime(0,0).addMSecs(pos).toString("hh:mm:ss");
    ui->labelInfo->setText(posStr);
}

void CMainWidget::onMediaMetaDataChanged()
{
    ui->horizontalSliderPosition->setSliderPosition(0);
    auto trackInfo = player->metaData().stringValue(QMediaMetaData::Title);
    ui->labelTrackName->setText(trackInfo);

    auto bitRate =player->metaData().value(QMediaMetaData::AudioBitRate).toInt() / 1000;
    ui->labelKbps->setText(QString("%1").arg(bitRate));
}

void CMainWidget::openMediaFile(const QString & filename)
{
    player->setSource(QUrl::fromLocalFile(filename));
    player->play();

    ui->horizontalSliderPosition->setEnabled(true);
}

void CMainWidget::resetUI()
{
    ui->horizontalSliderPosition->setSliderPosition(0);
    ui->labelTrackName->setText("---");
    ui->labelKbps->setText("0");
    ui->labelKhz->setText("0");
    ui->horizontalSliderPosition->setEnabled(false);
}


void CMainWidget::on_horizontalSliderPosition_sliderReleased()
{
    auto pos = player->duration() * ui->horizontalSliderPosition->sliderPosition() / ui->horizontalSliderPosition->maximum();
    player->setPosition(pos);
}


void CMainWidget::on_actionShow_Playlist_toggled(bool arg1)
{
    ((CApplication*)qApp)->showPlaylist(arg1);
}

void CMainWidget::onPlaylistClosed()
{
    ui->checkBoxPL->setCheckState(Qt::Unchecked);
}
