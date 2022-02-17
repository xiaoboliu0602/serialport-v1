#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QMessageBox>
#include <QtNetwork/QNetworkConfigurationManager>
#include <QTimer>

class QSoundEffect;

void sleep(unsigned int msec);

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_openButton_clicked();

    void on_closeButton_clicked();

    void on_onButton_clicked();

    void on_offButton_clicked();

    void on_testButton_clicked();

    void netTest();

private:
    Ui::Widget *ui;
    QSerialPort *serialPort;
    QNetworkConfigurationManager *mgr;
};
#endif // WIDGET_H
