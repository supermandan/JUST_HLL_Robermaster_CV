#include "serial.h"

namespace HCVC {
Serial::Serial()
{

}

bool Serial::init(QString portName)
{
    //串口信息类，用于读取可用串口所有的信息
    QSerialPortInfo serialPortInfo;
    foreach(serialPortInfo, QSerialPortInfo::availablePorts())
    {
        if(serialPortInfo.portName() == portName )
        {
            setPort(serialPortInfo);
            break;
        }
    }

    //输出所用串口的相关信息
    qDebug() << serialPortInfo.portName() << endl
             << serialPortInfo.description() << endl
             << serialPortInfo.serialNumber() << endl;

    if(open(QIODevice::ReadWrite))
    {
        qDebug() << "open(QIODevice::ReadWrite)";
        setBaudRate(QSerialPort::Baud115200);
        setParity(QSerialPort::NoParity);
        setDataBits(QSerialPort::Data8);
        setStopBits(QSerialPort::OneStop);
        setFlowControl(QSerialPort::NoFlowControl);

        clearError();
        clear();
        //设定触发事件，如果串口有数据，则触发读取函数
        connect(this, SIGNAL(readyRead()), this, SLOT(readBytes()));
        return true;
    }

    return false;
}

void Serial::writeBytes(short xDiff, short yDiff)
{
    //待写入数据缓冲区
    QByteArray buffer;
    //向缓冲区添加表示两个短整型数的四个字节
    buffer.append(reinterpret_cast<char*>(&xDiff), 2);
    buffer.append(reinterpret_cast<char*>(&yDiff), 2);

    //显示被写入的数据
    for(int i = 0; i < buffer.size(); i++)
    {
        qDebug() << static_cast<int>(buffer[i]) << ' ';
    }
    qDebug() << endl;

    write(buffer);
    waitForBytesWritten(5);
}

void Serial::readBytes()
{
    QByteArray buffer;
    buffer = readAll();
    qDebug() << buffer << endl;
}
}
