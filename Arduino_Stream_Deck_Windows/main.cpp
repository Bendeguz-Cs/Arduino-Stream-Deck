#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QDebug>

int main(int argc, char *argv[]) {
    ///////////////////// Qml steps start
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.loadFromModule("AppStreamDeckURI", "Main");
    ///////////////////// Qml steps end

    ///////////////////// QSerialPortInfo start
    const auto ports = QSerialPortInfo::availablePorts();

    for (const QSerialPortInfo &port : ports) {
        qDebug() << port.portName()
                 << port.description()
                 << port.manufacturer();
    }
    ///////////////////// QSerialPortInfo end

    ///////////////////// QSerialPort start
    QSerialPort serial;

    serial.setPortName(ports.first().portName());

    serial.setBaudRate(QSerialPort::Baud9600);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    //Serial buffer, only print when full message arrived
    QByteArray buffer;

    //handle the data stream
    QObject::connect(&serial, &QSerialPort::readyRead, [&serial, &buffer]()
                     {
                         buffer += serial.readAll(); //store in buffer

                         while (buffer.contains('\n')) { //only print when the buffer has line end
                             QByteArray line = buffer.left(buffer.indexOf('\n'));
                             buffer.remove(0, buffer.indexOf('\n') + 1);

                             qDebug() << line.trimmed(); //remove any unnecessary characters and print
                         }
                     });

    //open port
    if (serial.open(QIODevice::ReadWrite))
        qDebug() << "Port opened!";
    else
        qDebug() << "Failed:" << serial.errorString();
    ///////////////////// QSerialPort end

    //execute the app and return the exit code when done
    return app.exec();
}