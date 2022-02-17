#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("Easy4G");

    // 串口列表对象、串口对象、网络管理对象、定时器对象

    QStringList serialNamePort;

    /* 创建一个串口对象 */
    serialPort = new QSerialPort(this);

    /* 搜索所有可用串口 */
    foreach (const QSerialPortInfo &inf0, QSerialPortInfo::availablePorts()) {
        serialNamePort<<inf0.portName();
    }
    ui->serialBox->addItems(serialNamePort);
    // 如果串口号不唯一，则选择第二个串口号，若超出 2个串口号，则需要手动选择
    if (ui->serialBox->count() != 1)
        ui->serialBox->setCurrentIndex(1);

    /* 定时任务实时监测: 4G 网络连接状态 */
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(netTest()));
    timer->start(1000);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_openButton_clicked()
{
    /* 串口设置 */
    serialPort->setPortName(ui->serialBox->currentText());
    serialPort->setBaudRate(ui->baudrateBox->currentText().toInt());
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setParity(QSerialPort::NoParity);

    /* 串口打开成功：串口号栏和波特率栏失能；串口打开失败：弹出提示框； */
    if (true == serialPort->open(QIODevice::ReadWrite))
    {
        ui->serialBox->setEnabled(false);
        ui->baudrateBox->setEnabled(false);
        ui->openButton->setEnabled(false);
    }
    else
    {
        QMessageBox::critical(this,"提示","串口打开失败");
    }
}

void Widget::on_closeButton_clicked()
{
    /* 关闭串口后，恢复显示串口号栏和波特率选择栏 */
    serialPort->close();
    ui->serialBox->setEnabled(true);
    ui->baudrateBox->setEnabled(true);
    ui->openButton->setEnabled(true);
}

/* 打开串口 -> 开启网络连接 -> 关闭串口 -> 网络状态显示 */
void Widget::on_onButton_clicked()
{
    /* 串口打开成功：串口号栏和波特率栏失能；串口打开失败：弹出提示框； */
    if (true == serialPort->isOpen())
    {
        ui->serialBox->setEnabled(false);
        ui->baudrateBox->setEnabled(false);
        ui->openButton->setEnabled(false);
    }
    else
    {
        /* 串口设置 */
        serialPort->setPortName(ui->serialBox->currentText());
        serialPort->setBaudRate(ui->baudrateBox->currentText().toInt());
        serialPort->setDataBits(QSerialPort::Data8);
        serialPort->setStopBits(QSerialPort::OneStop);
        serialPort->setParity(QSerialPort::NoParity);

        /* 串口打开成功：串口号栏和波特率栏失能；串口打开失败：弹出提示框； */
        if (true == serialPort->open(QIODevice::ReadWrite))
        {
            ui->serialBox->setEnabled(false);
            ui->baudrateBox->setEnabled(false);
            ui->openButton->setEnabled(false);
        }
        else
        {
            QMessageBox::critical(this,"提示","串口打开失败");
        }
    }

    /* 发送开启网络命令 */
    serialPort->write("AT+GTRNDIS=1,1\r\n");
    qDebug("AT+GTRNDIS=1,1\r\n");

    /* 等待命令发送结束 */
    serialPort->waitForBytesWritten();

    /* 关闭串口后，恢复显示串口号栏和波特率选择栏 */
    serialPort->close();
    ui->serialBox->setEnabled(true);
    ui->baudrateBox->setEnabled(true);
    ui->openButton->setEnabled(true);
}

/* 打开串口 -> 关闭网络连接 -> 关闭串口 -> 网络状态显示 */
void Widget::on_offButton_clicked()
{
    /* 串口打开成功：串口号栏和波特率栏失能；串口打开失败：弹出提示框； */
    if (true == serialPort->isOpen())
    {
        ui->serialBox->setEnabled(false);
        ui->baudrateBox->setEnabled(false);
        ui->openButton->setEnabled(false);
    }
    else
    {
        /* 串口设置 */
        serialPort->setPortName(ui->serialBox->currentText());
        serialPort->setBaudRate(ui->baudrateBox->currentText().toInt());
        serialPort->setDataBits(QSerialPort::Data8);
        serialPort->setStopBits(QSerialPort::OneStop);
        serialPort->setParity(QSerialPort::NoParity);

        /* 串口打开成功：串口号栏和波特率栏失能；串口打开失败：弹出提示框； */
        if (true == serialPort->open(QIODevice::ReadWrite))
        {
            ui->serialBox->setEnabled(false);
            ui->baudrateBox->setEnabled(false);
            ui->openButton->setEnabled(false);
        }
        else
        {
            QMessageBox::critical(this,"提示","串口打开失败");
        }
    }

    /* 发送关闭网络命令 */
    serialPort->write("AT+GTRNDIS=0,1\r\n");
    qDebug("AT+GTRNDIS=0,1\r\n");
    ui->onButton->setEnabled(true);

    /* 等待命令发送结束 */
    serialPort->waitForBytesWritten();

    /* 关闭串口后，恢复显示串口号栏和波特率选择栏 */
    serialPort->close();
    ui->serialBox->setEnabled(true);
    ui->baudrateBox->setEnabled(true);
    ui->openButton->setEnabled(true);
}

void Widget::on_testButton_clicked()
{
    /* 判断是否连接网络，如果已经联网 */
    if(mgr->isOnline())
    {
        ui->onLabel->setStyleSheet("color:green");
        ui->onLabel->setText("ON");
    }
    else
    {
        ui->onLabel->setStyleSheet("color:red");
        ui->onLabel->setText("OFF");
    }
}

/* 4G 网络连接状态 */
void Widget::netTest()
{
    /* 创建一个网络管理对象 */
    mgr = new QNetworkConfigurationManager(this);

    /* 判断是否连接网络，如果已经联网 */
    if(mgr->isOnline())
    {
        ui->onLabel->setStyleSheet("color:green");
        ui->onLabel->setText("ON");
    }
    else
    {
        ui->onLabel->setStyleSheet("color:red");
        ui->onLabel->setText("OFF");
    }
}

