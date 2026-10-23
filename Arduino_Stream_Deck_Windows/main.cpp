#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QDebug>
#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

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

    //single line current line
    QByteArray message;

    //handle the data stream
    QObject::connect(&serial, &QSerialPort::readyRead, [&serial, &buffer, &message]()
                     {
                         buffer += serial.readAll(); //store in buffer

                         while (buffer.contains('\n')) { //only print when the buffer has line end
                             message = buffer.left(buffer.indexOf('\n'));
                             buffer.remove(0, buffer.indexOf('\n') + 1);

                             qDebug() << "Original message: " << message.trimmed(); //remove any unnecessary characters and print

                             QList<QByteArray> messageParts = message.split(',');

                             QByteArray inputType    = 0;
                             unsigned int inputID    = 0;
                             unsigned int inputValue = 0;

                             if(messageParts.size() == 3) {
                                 inputType  = messageParts[0].trimmed();
                                 inputID    = messageParts[1].trimmed().toUInt();
                                 inputValue = messageParts[2].trimmed().toUInt();
                                 qDebug() << "Separated parts: " << inputType << inputID << inputValue;
                             }

                             ///Input actions handling
                             if(inputType == "ENC") {
                                 if(inputID == 0) {
                                     //change microphone volume
                                     setMicVolume(inputValue);
                                 } else if(inputID == 1) {
                                     //change speaker volume
                                     setVolume(inputValue);
                                 }
                             } else if(inputType == "ESW") {
                                 if(inputID == 0 && inputValue == 1) {
                                     //toggle microphone mute
                                     qDebug() << "mic mute toggle triggered" << inputType << inputID << inputValue;
                                     setMicMute(!getMicMute());
                                 } else if(inputID == 1 && inputValue == 1) {
                                     //toggle speaker mute
                                     qDebug() << "mute toggle triggered" << inputType << inputID << inputValue;
                                     setMute(!getMute());
                                 }
                             } else if(inputType == "BTN") {
                                 if(inputID == 0 && inputValue == 1) {
                                     //button0
                                 } else if(inputID == 1 && inputValue == 1) {
                                     //button1
                                 } else if(inputID == 2 && inputValue == 1) {
                                     //button2
                                 } else if(inputID == 3 && inputValue == 1) {
                                     //button3
                                 } else if(inputID == 4 && inputValue == 1) {
                                     //button4
                                 } else if(inputID == 5 && inputValue == 1) {
                                     //button5
                                 } else if(inputID == 6 && inputValue == 1) {
                                     //button6
                                 } else if(inputID == 7 && inputValue == 1) {
                                     //button7
                                 }
                             } else {
                                 //invalid ID
                             }
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