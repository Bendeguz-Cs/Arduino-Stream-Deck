#include <QDesktopServices>
#include <QProcess>
#include <QGuiApplication>
#include <QClipboard>
#include <QUrl>
#include <QDebug>
#include <windows.h>
#include <shellapi.h>
#include "actionhandler.h"

#include <QKeySequence>
#include <Qt>
#include <windows.h>

// Convert a Qt key code to a Windows virtual-key code.
static WORD toWindowsKey(int key)
{
    // Letters and digits have matching Qt/Windows codes.
    if (key >= Qt::Key_A && key <= Qt::Key_Z)
        return WORD(key);

    if (key >= Qt::Key_0 && key <= Qt::Key_9)
        return WORD(key);

    if (key >= Qt::Key_F1 && key <= Qt::Key_F24)
        return WORD(VK_F1 + key - Qt::Key_F1);

    switch (key) {
    case Qt::Key_Space:     return VK_SPACE;
    case Qt::Key_Tab:       return VK_TAB;
    case Qt::Key_Return:
    case Qt::Key_Enter:     return VK_RETURN;
    case Qt::Key_Escape:    return VK_ESCAPE;
    case Qt::Key_Backspace: return VK_BACK;
    case Qt::Key_Delete:    return VK_DELETE;
    case Qt::Key_Insert:    return VK_INSERT;
    case Qt::Key_Home:      return VK_HOME;
    case Qt::Key_End:       return VK_END;
    case Qt::Key_PageUp:    return VK_PRIOR;
    case Qt::Key_PageDown:  return VK_NEXT;
    case Qt::Key_Left:      return VK_LEFT;
    case Qt::Key_Right:     return VK_RIGHT;
    case Qt::Key_Up:        return VK_UP;
    case Qt::Key_Down:      return VK_DOWN;
    case Qt::Key_Print:     return VK_SNAPSHOT;
    case Qt::Key_Pause:     return VK_PAUSE;
    case Qt::Key_CapsLock:  return VK_CAPITAL;
    case Qt::Key_NumLock:   return VK_NUMLOCK;
    case Qt::Key_ScrollLock:return VK_SCROLL;
    case Qt::Key_Minus:     return VK_OEM_MINUS;
    case Qt::Key_Equal:     return VK_OEM_PLUS;
    case Qt::Key_BracketLeft:  return VK_OEM_4;
    case Qt::Key_BracketRight: return VK_OEM_6;
    case Qt::Key_Backslash:    return VK_OEM_5;
    case Qt::Key_Semicolon:    return VK_OEM_1;
    case Qt::Key_Apostrophe:   return VK_OEM_7;
    case Qt::Key_Comma:        return VK_OEM_COMMA;
    case Qt::Key_Period:       return VK_OEM_PERIOD;
    case Qt::Key_Slash:        return VK_OEM_2;
    case Qt::Key_QuoteLeft:    return VK_OEM_3;
    default:
        return 0;
    }
}

static bool sendShortcut(const QString &text)
{
    const QKeySequence sequence =
        QKeySequence::fromString(text, QKeySequence::PortableText);

    if (sequence.isEmpty())
        return false;

    // This implementation supports one shortcut chord, not a sequence.
    const int combined = sequence[0];
    const int key = combined & ~int(Qt::KeyboardModifierMask);
    const int modifiers = combined & int(Qt::KeyboardModifierMask);

    const WORD mainKey = toWindowsKey(key);

    if (!mainKey) {
        qWarning() << "Unsupported shortcut key:" << text;
        return false;
    }

    WORD modifierKeys[4];
    int modifierCount = 0;

    if (modifiers & Qt::ControlModifier)
        modifierKeys[modifierCount++] = VK_CONTROL;

    if (modifiers & Qt::AltModifier)
        modifierKeys[modifierCount++] = VK_MENU;

    if (modifiers & Qt::ShiftModifier)
        modifierKeys[modifierCount++] = VK_SHIFT;

    if (modifiers & Qt::MetaModifier)
        modifierKeys[modifierCount++] = VK_LWIN;

    INPUT inputs[10] = {};
    int count = 0;

    // Press modifiers first.
    for (int i = 0; i < modifierCount; ++i) {
        inputs[count].type = INPUT_KEYBOARD;
        inputs[count].ki.wVk = modifierKeys[i];
        ++count;
    }

    // Press and release the main key.
    inputs[count].type = INPUT_KEYBOARD;
    inputs[count].ki.wVk = mainKey;
    ++count;

    inputs[count].type = INPUT_KEYBOARD;
    inputs[count].ki.wVk = mainKey;
    inputs[count].ki.dwFlags = KEYEVENTF_KEYUP;
    ++count;

    // Release modifiers in reverse order.
    for (int i = modifierCount - 1; i >= 0; --i) {
        inputs[count].type = INPUT_KEYBOARD;
        inputs[count].ki.wVk = modifierKeys[i];
        inputs[count].ki.dwFlags = KEYEVENTF_KEYUP;
        ++count;
    }

    const UINT sent = SendInput(count, inputs, sizeof(INPUT));

    if (sent != UINT(count)) {
        qWarning() << "SendInput did not send all shortcut events."
                   << "Sent:" << sent << "Expected:" << count
                   << "Windows error:" << GetLastError();
        return false;
    }

    return true;
}


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
    else if (action == "shortcut") {
        if (value.isEmpty()) {
            qWarning() << "No keyboard shortcut configured";
            return;
        }

        sendShortcut(value);
    }

    else {
        qWarning() << "Unknown action:" << action;
    }
}

void ActionHandler::startShortcutCapture()
{
    m_capturedShortcut.clear();
    m_capturing = true;

    emit capturedShortcutChanged();
    emit capturingChanged();
}

void ActionHandler::captureShortcutKey(int key, int modifiers)
{
    if (!m_capturing)
        return;

    // Ignore a modifier key pressed on its own.
    if (key == Qt::Key_Control ||
        key == Qt::Key_Shift ||
        key == Qt::Key_Alt ||
        key == Qt::Key_Meta ||
        key == Qt::Key_AltGr)
        return;

    const QKeySequence sequence(
        key | (modifiers & int(Qt::KeyboardModifierMask)));

    if (sequence.isEmpty())
        return;

    m_capturedShortcut =
        sequence.toString(QKeySequence::PortableText);

    emit capturedShortcutChanged();
}

void ActionHandler::stopShortcutCapture()
{
    if (!m_capturing)
        return;

    m_capturing = false;
    emit capturingChanged();
}
