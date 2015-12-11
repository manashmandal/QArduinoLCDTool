#include "qarduinodialog.h"
#include "ui_qarduinodialog.h"

QArduinoDialog::QArduinoDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QArduinoDialog), commTabText("Communication Manager"), lcdTabText("LCD Controller"), defaultBaudRate(9600)
{
    ui->setupUi(this);
    ui->tabWidget->tabBar()->setTabText(0, this->commTabText);
    ui->tabWidget->tabBar()->setTabText(1, this->lcdTabText);

    //Initializing baudRateList
    baudRateList << "1200" << "2400" << "4800" << "9600" << "19200" << "38400" << "57600" << "115200";
    ui->baudRateComboBox->addItems(baudRateList);
    ui->baudRateComboBox->setCurrentIndex(3);

    //Gets current ports
    portInfoList = QSerialPortInfo::availablePorts();
    //Adds an iterator to the port
    QListIterator <QSerialPortInfo> portInfoListIterator(portInfoList);
    while(portInfoListIterator.hasNext()){
        ui->comportComboBox->addItem(portInfoListIterator.next().portName());
    }

    //Initialize arduino
    arduino = new QSerialPort(this);

    connect(arduino, SIGNAL(readyRead()), this, SLOT(readArduino()));

    ui->disconnectButton->setEnabled(false);

}

QArduinoDialog::~QArduinoDialog()
{
    delete ui;
}

void QArduinoDialog::on_baudRateComboBox_currentIndexChanged(const QString &arg1)
{
    currentBaudRate = (qint32) arg1.toInt();
    qDebug() << currentBaudRate;
}

void QArduinoDialog::on_comportComboBox_currentIndexChanged(const QString &arg1)
{
    currentPort = arg1;
    qDebug() << currentPort;
}

void QArduinoDialog::on_portStatusButton_clicked()
{
    QSerialPortInfo info(currentPort);

    QString portInfo = "Manufacturer: " + info.manufacturer() + "\nBoard Model : " + info.description();

    QMessageBox::information(this, "PORT Information",portInfo);
}

void QArduinoDialog::on_connectButton_clicked()
{
    arduino->setPortName(currentPort);
    if (arduino->open(QIODevice::ReadWrite)){
        arduino->setBaudRate(currentBaudRate);
            arduino->setDataBits(QSerialPort::Data8);
            arduino->setParity(QSerialPort::NoParity);
            arduino->setStopBits(QSerialPort::OneStop);
            // Skipping hw/sw control
            arduino->setFlowControl(QSerialPort::NoFlowControl);
            QMessageBox::information(this, "Connection successful", "Successfully connected to arduino");
    } else {
        QMessageBox::warning(this, "Connection Failed", "Retry");
    }

    ui->connectButton->setEnabled(false);
    ui->disconnectButton->setEnabled(true);

}

void QArduinoDialog::on_disconnectButton_clicked()
{
    arduino->clear();
    arduino->close();
    if (!arduino->isOpen()) QMessageBox::information(this, "Port is closed", "Connected device is closed successfully");
    ui->connectButton->setEnabled(true);
    ui->disconnectButton->setEnabled(false);
}

void QArduinoDialog::on_sendTextButton_clicked()
{
    //If no arduino is detected it shows an warning
    if (!arduino->isOpen()) QMessageBox::information(this, "No Arduino Detected", "Please connect an Arduino and click connect from Connection Manager");

    else {
        QByteArray text = ui->messageTextEdit->toPlainText().toLatin1();
        arduino->write(text);
    }
}

void QArduinoDialog::readArduino(){
    if (arduino->canReadLine()) data = arduino->readLine();
    data = data.remove(QRegExp("[\\n\\t\\r]"));
    if (data.contains("X")){
        qDebug() << data << "\n";
        ui->dimensionLabel->setText(data);
    }
}

void QArduinoDialog::on_getLCDDimensionButton_clicked()
{
    if (arduino->isOpen()){
        arduino->write("g");
    }
    else
        QMessageBox::information(this, "No Arduino Detected", "Connect an Arduino and Click connect button on Connection Manager");
}

void QArduinoDialog::on_clearLCDButton_clicked()
{
    if (arduino->isOpen()) arduino->write("#");
    else QMessageBox::information(this, "No Arduino Detected", "Connect an Arduino and Click connect button on Connection Manager");
}

void QArduinoDialog::on_pushButton_clicked()
{
    ui->messageTextEdit->clear();
}
