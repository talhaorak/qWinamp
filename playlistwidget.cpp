#include "playlistwidget.h"
#include "ui_playlistwidget.h"
#include <QCloseEvent>
#include <QResizeEvent>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QMediaMetaData>
#include <QThread>
#include <QMutex>
#include <QFuture>
#include <QtConcurrent>

CPlayListWidget::CPlayListWidget(QWidget *parent)
    : CBaseWidget(parent)
    , ui(new Ui::CPlayListWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    adjustTableSize();
}

CPlayListWidget::~CPlayListWidget()
{
    delete ui;
}

void CPlayListWidget::closeEvent(QCloseEvent *e)
{
    e->ignore();
    this->hide();
    emit closed();
}

void CPlayListWidget::resizeEvent(QResizeEvent *e)
{
    e->accept();
    this->adjustTableSize();
}

void CPlayListWidget::adjustTableSize()
{
    auto tableWidth = ui->tableWidgetPlaylist->horizontalHeader()->width();
    if(tableWidth<100)tableWidth = ui->tableWidgetPlaylist->width() - 10;
    ui->tableWidgetPlaylist->setColumnWidth(0, tableWidth * 0.1);
    ui->tableWidgetPlaylist->setColumnWidth(1,tableWidth * 0.8);
    ui->tableWidgetPlaylist->setColumnWidth(2,tableWidth * 0.1);
}

const SListEntry & CPlayListWidget::addFile(const QString & filename)
{
    auto entry = new SListEntry(filename);
    entry->title = filename;
    entry->order = playlist.count();
    playlist.append(*entry);
    updateUI();
    return *entry;
}

void CPlayListWidget::updateUI()
{
    ui->tableWidgetPlaylist->setRowCount(playlist.size());
    for (const auto& listEntry : playlist) {
        this->updateEntryUI(listEntry);
    }
}

void CPlayListWidget::updateEntryUI(const SListEntry &e)
{
    auto createOrGetItem = [&](int row, int column) -> QTableWidgetItem* {
        auto item = ui->tableWidgetPlaylist->item(row, column);
        if (!item) {
            item = new QTableWidgetItem;
            ui->tableWidgetPlaylist->setItem(row, column, item);
        }
        return item;
    };

    QTableWidgetItem* c1 = createOrGetItem(e.order, 0);
    QTableWidgetItem* c2 = createOrGetItem(e.order, 1);
    QTableWidgetItem* c3 = createOrGetItem(e.order, 2);

    c1->setText(QString::number(e.order));
    c2->setText(e.title);
    c3->setText(QString::number(e.duration));
}

void CPlayListWidget::on_pushButtonClose_clicked()
{
    close();
}


void CPlayListWidget::on_tableWidgetPlaylist_itemDoubleClicked(QTableWidgetItem *item)
{
    qDebug() << item->text();
}


void CPlayListWidget::on_pushButtonAdd_clicked()
{
    const auto basePath = QString("/Users/talhaorak/Music/mp3");
    auto filenames = QFileDialog::getOpenFileNames(this, "Add music", basePath);
    if(filenames.length() == 0)return;
    QList<const SListEntry*> newEntries;
    for (auto & filename : filenames) {
        auto entry = addFile(filename);
        newEntries.append(&entry);
    }

    for (auto& listEntry : newEntries) {
        QFuture<void> future = QtConcurrent::run([this, listEntry](){
            this->queryMediaMetadata(*listEntry);
        });
    }
}

void CPlayListWidget::queryMediaMetadata(SListEntry listEntry)
{
    qDebug() << "Query " << listEntry.filename;
    QMediaPlayer* mediaPlayer = new QMediaPlayer;

    // Use a mutex to prevent race conditions
    static QMutex mutex;

    // Connect to the mediaStatusChanged signal
    connect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, [this, mediaPlayer, listEntry](QMediaPlayer::MediaStatus status) {
        qDebug() << "media changed " << listEntry.filename << " : " << status;
        if (status == QMediaPlayer::LoadedMedia) {
            // Extract metadata
            QString title = mediaPlayer->metaData().stringValue(QMediaMetaData::Title);
            qint64 duration = mediaPlayer->metaData().value(QMediaMetaData::Duration).toLongLong();

            // Update entry
            SListEntry updatedEntry = listEntry;
            updatedEntry.title = title;
            updatedEntry.duration = duration;
            qDebug() << updatedEntry.toString();
            // Ensure UI updates are done in the main thread
            QMetaObject::invokeMethod(this, [this, updatedEntry]() {
                    updateEntryUI(updatedEntry);
                }, Qt::QueuedConnection);

            mediaPlayer->deleteLater();
        }
    });

    // Load the media file
    mediaPlayer->setSource(QUrl::fromLocalFile(listEntry.filename));
}


SListEntry::SListEntry(const QString &sFilename)
    :filename(sFilename)
{

}

QString SListEntry::toString() const
{
    return QString("order: %1, title:%2, duration:%3, filename:%4").arg(order).arg(title).arg(duration).arg(filename);
}

