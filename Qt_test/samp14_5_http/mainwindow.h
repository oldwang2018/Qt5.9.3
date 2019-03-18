#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QFile>
#include <QMainWindow>
#include<QtNetwork/QNetworkAccessManager>
#include<QtNetwork/QNetworkReply>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QNetworkAccessManager networkManager;
    QNetworkReply *reply;

    QFile *downloadFile;
private slots:
    void on_finished();
    void on_readyRead();
    void on_downloadProgress(qint64 bytesRead, qint64 totalBytes);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnDefaultPath_clicked();

    void on_btnDownload_clicked();

    void on_btnSetDir_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
