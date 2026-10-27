#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QTimer>
#include <QTime>
#include <QFont>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QQuickWindow>
#include <QStyle>
#include <QDebug>
#include <QLockFile>
#include <QDir>
#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

#include "actionhandler.h"
#include "configManager.h"

int getVolume() {
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

    IMMDeviceEnumerator *enumerator = nullptr;
    IMMDevice *device = nullptr;
    IAudioEndpointVolume *volume = nullptr;

    float level = 0.0f;
    int result = 0;

    if (SUCCEEDED(CoCreateInstance(
            __uuidof(MMDeviceEnumerator), nullptr,
            CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
            (void**)&enumerator)))
    {
        if (SUCCEEDED(enumerator->GetDefaultAudioEndpoint(
                eRender, eMultimedia, &device)))
        {
            if (SUCCEEDED(device->Activate(
                    __uuidof(IAudioEndpointVolume),
                    CLSCTX_ALL, nullptr, (void**)&volume)))
            {
                if (SUCCEEDED(volume->GetMasterVolumeLevelScalar(&level)))
                    result = qRound(level * 100.0f);

                volume->Release();
            }

            device->Release();
        }

        enumerator->Release();
    }

    CoUninitialize();

    return result;
}

void setVolume(int percent) {
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

    IMMDeviceEnumerator *enumerator = nullptr;
    IMMDevice *device = nullptr;
    IAudioEndpointVolume *volume = nullptr;

    if (SUCCEEDED(CoCreateInstance(
            __uuidof(MMDeviceEnumerator), nullptr,
            CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
            (void**)&enumerator)))
    {
        if (SUCCEEDED(enumerator->GetDefaultAudioEndpoint(
                eRender, eMultimedia, &device)))
        {
            if (SUCCEEDED(device->Activate(
                    __uuidof(IAudioEndpointVolume),
                    CLSCTX_ALL, nullptr, (void**)&volume)))
            {
                volume->SetMasterVolumeLevelScalar(
                    qBound(0.0f, percent / 100.0f, 1.0f),
                    nullptr);

                volume->Release();
            }

            device->Release();
        }

        enumerator->Release();
    }

    CoUninitialize();
}

bool getMute() {
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

    IMMDeviceEnumerator *enumerator = nullptr;
    IMMDevice *device = nullptr;
    IAudioEndpointVolume *volume = nullptr;

    bool muted = false;

    if (SUCCEEDED(CoCreateInstance(
            __uuidof(MMDeviceEnumerator), nullptr,
            CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
            (void**)&enumerator)))
    {
        if (SUCCEEDED(enumerator->GetDefaultAudioEndpoint(
                eRender, eMultimedia, &device)))
        {
            if (SUCCEEDED(device->Activate(
                    __uuidof(IAudioEndpointVolume),
                    CLSCTX_ALL, nullptr, (void**)&volume)))
            {
                BOOL state = FALSE;

                if (SUCCEEDED(volume->GetMute(&state)))
                    muted = state;

                volume->Release();
            }

            device->Release();
        }

        enumerator->Release();
    }

    CoUninitialize();

    return muted;
}

void setMute(bool mute) {
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

    IMMDeviceEnumerator *enumerator = nullptr;
    IMMDevice *device = nullptr;
    IAudioEndpointVolume *volume = nullptr;

    if (SUCCEEDED(CoCreateInstance(
            __uuidof(MMDeviceEnumerator), nullptr,
            CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
            (void**)&enumerator)))
    {
        if (SUCCEEDED(enumerator->GetDefaultAudioEndpoint(
                eRender, eMultimedia, &device)))
        {
            if (SUCCEEDED(device->Activate(
                    __uuidof(IAudioEndpointVolume),
                    CLSCTX_ALL, nullptr, (void**)&volume)))
            {
                volume->SetMute(mute, nullptr);
                volume->Release();
            }

            device->Release();
        }

        enumerator->Release();
    }

    CoUninitialize();
}

int getMicVolume() {
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

    IMMDeviceEnumerator *enumerator = nullptr;
    IMMDevice *device = nullptr;
    IAudioEndpointVolume *volume = nullptr;

    float level = 0.0f;
    int result = 0;

    if (SUCCEEDED(CoCreateInstance(
            __uuidof(MMDeviceEnumerator), nullptr,
            CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
            (void**)&enumerator)))
    {
        if (SUCCEEDED(enumerator->GetDefaultAudioEndpoint(
                eCapture, eMultimedia, &device)))
        {
            if (SUCCEEDED(device->Activate(
                    __uuidof(IAudioEndpointVolume),
                    CLSCTX_ALL, nullptr, (void**)&volume)))
            {
                if (SUCCEEDED(volume->GetMasterVolumeLevelScalar(&level)))
                    result = qRound(level * 100.0f);

                volume->Release();
            }

            device->Release();
        }

        enumerator->Release();
    }

    CoUninitialize();
    return result;
}

void setMicVolume(int percent) {
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

    IMMDeviceEnumerator *enumerator = nullptr;
    IMMDevice *device = nullptr;
    IAudioEndpointVolume *volume = nullptr;

    if (SUCCEEDED(CoCreateInstance(
            __uuidof(MMDeviceEnumerator), nullptr,
            CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
            (void**)&enumerator)))
    {
        if (SUCCEEDED(enumerator->GetDefaultAudioEndpoint(
                eCapture, eMultimedia, &device)))
        {
            if (SUCCEEDED(device->Activate(
                    __uuidof(IAudioEndpointVolume),
                    CLSCTX_ALL, nullptr, (void**)&volume)))
            {
                volume->SetMasterVolumeLevelScalar(
                    qBound(0.0f, percent / 100.0f, 1.0f),
                    nullptr);

                volume->Release();
            }

            device->Release();
        }

        enumerator->Release();
    }

    CoUninitialize();
}

void setMicMute(bool mute) {
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

    IMMDeviceEnumerator *enumerator = nullptr;
    IMMDevice *device = nullptr;
    IAudioEndpointVolume *volume = nullptr;

    if (SUCCEEDED(CoCreateInstance(
            __uuidof(MMDeviceEnumerator), nullptr,
            CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
            (void**)&enumerator)))
    {
        if (SUCCEEDED(enumerator->GetDefaultAudioEndpoint(
                eCapture, eMultimedia, &device)))
        {
            if (SUCCEEDED(device->Activate(
                    __uuidof(IAudioEndpointVolume),
                    CLSCTX_ALL, nullptr, (void**)&volume)))
            {
                volume->SetMute(mute, nullptr);
                volume->Release();
            }

            device->Release();
        }

        enumerator->Release();
    }

    CoUninitialize();
}

bool getMicMute() {
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

    IMMDeviceEnumerator *enumerator = nullptr;
    IMMDevice *device = nullptr;
    IAudioEndpointVolume *volume = nullptr;

    bool muted = false;

    if (SUCCEEDED(CoCreateInstance(
            __uuidof(MMDeviceEnumerator), nullptr,
            CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
            (void**)&enumerator)))
    {
        if (SUCCEEDED(enumerator->GetDefaultAudioEndpoint(
                eCapture, eMultimedia, &device)))
        {
            if (SUCCEEDED(device->Activate(
                    __uuidof(IAudioEndpointVolume),
                    CLSCTX_ALL, nullptr, (void**)&volume)))
            {
                BOOL state = FALSE;

                if (SUCCEEDED(volume->GetMute(&state)))
                    muted = state;

                volume->Release();
            }

            device->Release();
        }

        enumerator->Release();
    }

    CoUninitialize();
    return muted;
}

int lastMinute = -1;
int lastSpeakerMute = -1;
int lastMicMute = -1;
int lastSpeakerVolume = -1;
int lastMicVolume = -1;

int main(int argc, char *argv[]) {
    ///////////////////// Qml steps start
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("PicoStreamDeck");
    QCoreApplication::setApplicationName("ArduinoStreamDeck");

    // Allow only one running instance of the application.
    const QString lockPath =
        QDir::temp().filePath("ArduinoStreamDeck.lock");

    QLockFile singleInstanceLock(lockPath);

    // Do not automatically remove a lock just because it is old.
    // QLockFile can detect whether its owning process is still running.
    singleInstanceLock.setStaleLockTime(0);

    if (!singleInstanceLock.tryLock(100)) {
        qWarning() << "Another instance of ArduinoStreamDeck is already running.";
        return 0;
    }

    app.setQuitOnLastWindowClosed(false);

    QQmlApplicationEngine engine;

    ConfigManager configManager;
    engine.rootContext()->setContextProperty("configManager", &configManager);

    ActionHandler actionHandler;
    engine.rootContext()->setContextProperty("actionHandler", &actionHandler);

    engine.loadFromModule("AppStreamDeckURI", "Main");

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    auto *window = qobject_cast<QQuickWindow *>(engine.rootObjects().first());

    if (!window) {
        return -1;
    }

    // Check whether this is the first launch.
    QSettings settings;
    const bool firstLaunch =
        !settings.value("hasLaunchedBefore", false).toBool();

    // Mark the application as launched.
    if (firstLaunch) {
        settings.setValue("hasLaunchedBefore", true);
    }
    auto *tray = new QSystemTrayIcon(
        QApplication::style()->standardIcon(QStyle::SP_ComputerIcon), &app);
    auto *menu = new QMenu();

    auto *title = menu->addAction("Stream Deck");
    QFont font = title->font();
    font.setBold(true);
    title->setFont(font);
    title->setEnabled(false);

    menu->addSeparator();

    menu->addAction("Open", window, [window] {
        window->show();
        window->raise();
        window->requestActivate();
    });
    menu->addAction("Quit", &app, &QCoreApplication::quit);

    tray->setContextMenu(menu);
    tray->show();

    // Show the window only on the first launch.
    if (firstLaunch) {
        window->show();
    } else {
        window->hide();
    }
    //settings.setValue("hasLaunchedBefore", false);
    ///////////////////// Qml steps end

    ///////////////////// Connection
    QTimer reconnectTimer;
    reconnectTimer.setInterval(5000);

    QSerialPort serial;

    QObject::connect(&reconnectTimer, &QTimer::timeout, [&]() {
        //Already connected
        if(serial.isOpen()) {
            reconnectTimer.stop();
            lastSpeakerMute = -1;
            lastMicMute = -1;
            return;
        }

        ///////////////////// QSerialPortInfo start
        const auto ports = QSerialPortInfo::availablePorts();

        QSerialPortInfo targetPort;
        QString targetDescription = "PicoStreamDeck";

        for (const QSerialPortInfo &port : ports) {
            if(port.description() == targetDescription) {
                targetPort = port;
                /*qDebug() << port.portName()
                         << port.description()
                         << port.manufacturer();*/
                break;
            }
        }

        if(targetPort.isNull()) {
            qWarning() << "Target hardware not found";
            return;
        }
        ///////////////////// QSerialPortInfo end

        ///////////////////// QSerialPort start
        serial.setPort(targetPort);

        serial.setBaudRate(QSerialPort::Baud9600);
        serial.setDataBits(QSerialPort::Data8);
        serial.setParity(QSerialPort::NoParity);
        serial.setStopBits(QSerialPort::OneStop);
        serial.setFlowControl(QSerialPort::NoFlowControl);

        //Open port
        if (serial.open(QIODevice::ReadWrite)) {
            //qDebug() << "Port opened!";

            const bool dtrSet = serial.setDataTerminalReady(true);
            //qDebug() << "DTR set:" << dtrSet;

            QString message = "SVOL," + QString::number(getVolume()) + "\nMVOL," + QString::number(getMicVolume()) + "\nSMUTE," + QString::number(getMute()) + "\nMMUTE," + QString::number(getMicMute()) + "\n";
            serial.write(message.toUtf8());

            lastMinute = -1; //guaranteed time refresh

            reconnectTimer.stop();
        } else {
            qWarning() << "Failed:" << serial.errorString();
        }
        ///////////////////// QSerialPort end
    });

    //Try immediately, then retry every 5 seconds if needed
    reconnectTimer.start();
    QMetaObject::invokeMethod(&reconnectTimer, "timeout", Qt::DirectConnection);

    //Serial buffer, only print when full message arrived
    QByteArray buffer;

    //single line current line
    QByteArray message;

    //handle serial communication
    QObject::connect(&serial, &QSerialPort::readyRead, [&serial, &buffer, &message, &actionHandler, &configManager]() {
        buffer += serial.readAll(); //store in buffer

        while (buffer.contains('\n')) { //only print when the buffer has line end
            message = buffer.left(buffer.indexOf('\n'));
            buffer.remove(0, buffer.indexOf('\n') + 1);

            //qDebug() << "Original message: " << message.trimmed(); //remove any unnecessary characters and print

            QList<QByteArray> messageParts = message.split(',');

            QByteArray inputType    = 0;
            unsigned int inputID    = 0;
            unsigned int inputValue = 0;

            if(messageParts.size() == 3) {
                inputType  = messageParts[0].trimmed();
                inputID    = messageParts[1].trimmed().toUInt();
                inputValue = messageParts[2].trimmed().toUInt();
                //qDebug() << "Separated parts: " << inputType << inputID << inputValue;
            }

            //qDebug() << QTime::currentTime().hour() << ":" << QTime::currentTime().minute() << ":" << QTime::currentTime().second();

            ///Input actions handling
            if (inputType == "ENC") {
                if (inputID == 0) {
                    // Microphone encoder
                    setMicVolume(inputValue);
                    lastMicVolume = getMicVolume();

                } else if (inputID == 1) {
                    // Speaker encoder
                    setVolume(inputValue);
                    lastSpeakerVolume = getVolume();
                }
            } else if(inputType == "ESW") {
                if(inputID == 0 && inputValue == 1) {
                    //toggle microphone mute
                    //qDebug() << "mic mute toggle triggered" << inputType << inputID << inputValue;
                    setMicMute(!getMicMute());
                    QString message = "MMUTE," + QString::number(getMicMute());
                    serial.write(message.toUtf8());
                } else if(inputID == 1 && inputValue == 1) {
                    //toggle speaker mute
                    //qDebug() << "mute toggle triggered" << inputType << inputID << inputValue;
                    setMute(!getMute());
                    QString message = "SMUTE," + QString::number(getMute());
                    serial.write(message.toUtf8());
                }
            } else if (inputType == "BTN" && inputValue == 1) {
                // Make sure the physical button ID is valid.
                if (inputID >= 8) {
                    qWarning() << "Invalid button ID:" << inputID;
                    continue;
                }

                // Load the latest saved button configurations.
                const QVariantList configs = configManager.load();

                // Arduino IDs are 0-7; QML configuration indices are 0-7.
                const QVariantMap config = configs.at(inputID).toMap();

                const QString action = config.value("action").toString();
                const QString value = config.value("value").toString();

                qDebug() << "Button pressed:" << inputID
                         << "Action:" << action
                         << "Value:" << value;

                // Execute the action selected in the QML configuration.
                actionHandler.executeAction(action, value);
            } else {
                //invalid ID
            }
        }
    });

    QTimer everySecond;
    everySecond.setInterval(1000);
    everySecond.start();

    QObject::connect(&serial, &QSerialPort::errorOccurred,
        [&](QSerialPort::SerialPortError error) {
            if (error == QSerialPort::ResourceError) {
                qWarning() << "Serial port disconnected";
                serial.close();
                reconnectTimer.start();
            }
        });


    QObject::connect(&everySecond, &QTimer::timeout, [&]() {

        // Send the time when the minute changes
        if (QTime::currentTime().minute() != lastMinute) {
            lastMinute = QTime::currentTime().minute();

            QString timeMessage = "TIME," +
                                  QString("%1:%2")
                                      .arg(QTime::currentTime().hour(), 2, 10, QChar('0'))
                                      .arg(QTime::currentTime().minute(), 2, 10, QChar('0'));

            if (serial.isOpen()) {
                serial.write((timeMessage + "\n").toUtf8());
            }
        }

        // Only synchronize audio states while the Pico is connected
        if (!serial.isOpen()) {
            return;
        }

        // Read current Windows audio states
        const int speakerVolume = getVolume();
        const int micVolume = getMicVolume();

        const int speakerMute = getMute() ? 1 : 0;
        const int micMute = getMicMute() ? 1 : 0;

        // Send speaker volume only when it changes
        if (speakerVolume != lastSpeakerVolume) {
            lastSpeakerVolume = speakerVolume;

            const QString message =
                "SVOL," + QString::number(speakerVolume) + "\n";

            serial.write(message.toUtf8());
            // qDebug() << "Speaker volume:" << speakerVolume;
        }

        // Send microphone volume only when it changes
        if (micVolume != lastMicVolume) {
            lastMicVolume = micVolume;

            const QString message =
                "MVOL," + QString::number(micVolume) + "\n";

            serial.write(message.toUtf8());
            // qDebug() << "Microphone volume:" << micVolume;
        }

        // Send speaker mute state only when it changes
        if (speakerMute != lastSpeakerMute) {
            lastSpeakerMute = speakerMute;

            const QString message =
                "SMUTE," + QString::number(speakerMute) + "\n";

            serial.write(message.toUtf8());
        }

        // Send microphone mute state only when it changes
        if (micMute != lastMicMute) {
            lastMicMute = micMute;

            const QString message =
                "MMUTE," + QString::number(micMute) + "\n";

            serial.write(message.toUtf8());
        }
    });

    //execute the app and return the exit code when done
    return app.exec();
}