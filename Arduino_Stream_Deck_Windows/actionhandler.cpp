#include <QDesktopServices>
#include <QProcess>
#include <QGuiApplication>
#include <QClipboard>
#include <QUrl>
#include <QDebug>
#include <windows.h>
#include <shellapi.h>
#include "actionhandler.h"

ActionHandler::ActionHandler(QObject *parent)
    : QObject(parent)
{
}


void ActionHandler::executeAction(const QString &action,const QString &value) {
    if (action == "none") {
        return;
    }

    else if (action == "application") {
        if (value.isEmpty()) {
            qWarning() << "No application selected";
            return;
        }

        const std::wstring path = value.toStdWString();

        const auto result = reinterpret_cast<INT_PTR>(
            ShellExecuteW(
                nullptr,
                L"open",
                path.c_str(),
                nullptr,
                nullptr,
                SW_SHOWNORMAL
                )
            );

        if (result <= 32) {
            qWarning() << "Failed to launch shortcut:"
                       << value
                       << "Error:" << result;
        }
    }
    else if (action == "website") {
        const QUrl url = QUrl::fromUserInput(value);

        if (!url.isValid() ||
            !QDesktopServices::openUrl(url)) {
            qWarning() << "Failed to open URL:" << value;
        }
    }
    else if (action == "copy" ||
             action == "cut" ||
             action == "paste" ||
             action == "forward" ||
             action == "backward" ||
             action == "playPause") {

        WORD key = 0;
        bool ctrl = false;
        bool shift = false;

        if (action == "copy") {
            key = 'C';
            ctrl = true;
        }
        else if (action == "cut") {
            key = 'X';
            ctrl = true;
        }
        else if (action == "paste") {
            key = 'V';
            ctrl = true;
        }
        else if (action == "forward") {
            key = VK_RIGHT;
            ctrl = true;
        }
        else if (action == "backward") {
            key = VK_LEFT;
            ctrl = true;
        }
        else if (action == "playPause") {
            key = VK_MEDIA_PLAY_PAUSE;
        }

        INPUT inputs[4] = {};
        int count = 0;

        if (ctrl) {
            inputs[count].type = INPUT_KEYBOARD;
            inputs[count++].ki.wVk = VK_CONTROL;
        }

        if (shift) {
            inputs[count].type = INPUT_KEYBOARD;
            inputs[count++].ki.wVk = VK_SHIFT;
        }

        inputs[count].type = INPUT_KEYBOARD;
        inputs[count++].ki.wVk = key;

        inputs[count].type = INPUT_KEYBOARD;
        inputs[count].ki.wVk = key;
        inputs[count++].ki.dwFlags = KEYEVENTF_KEYUP;

        if (shift) {
            inputs[count].type = INPUT_KEYBOARD;
            inputs[count++].ki.wVk = VK_SHIFT;
            inputs[count - 1].ki.dwFlags = KEYEVENTF_KEYUP;
        }

        if (ctrl) {
            inputs[count].type = INPUT_KEYBOARD;
            inputs[count++].ki.wVk = VK_CONTROL;
            inputs[count - 1].ki.dwFlags = KEYEVENTF_KEYUP;
        }

        SendInput(count, inputs, sizeof(INPUT));
    }
    else if (action == "screenshot") {
        // Windows screenshot shortcut: Win + Shift + S
        INPUT inputs[6] = {};

        const WORD keys[] = {
            VK_LWIN, VK_SHIFT, 'S',
            'S', VK_SHIFT, VK_LWIN
        };

        for (int i = 0; i < 6; ++i) {
            inputs[i].type = INPUT_KEYBOARD;
            inputs[i].ki.wVk = keys[i];

            if (i >= 3)
                inputs[i].ki.dwFlags = KEYEVENTF_KEYUP;
        }

        SendInput(6, inputs, sizeof(INPUT));
    }
    else if (action == "lock") {
        LockWorkStation();
    }
    else {
        qWarning() << "Unknown action:" << action;
    }
}
