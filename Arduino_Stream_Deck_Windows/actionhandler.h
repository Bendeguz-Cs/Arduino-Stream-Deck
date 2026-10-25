
#pragma once

#include <QObject>
#include <QString>

class ActionHandler : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool capturing
                   READ isCapturing
                       NOTIFY capturingChanged)

    Q_PROPERTY(QString capturedShortcut
                   READ capturedShortcut
                       NOTIFY capturedShortcutChanged)

public:
    explicit ActionHandler(QObject *parent = nullptr);

    Q_INVOKABLE void executeAction(const QString &action,
                                   const QString &value = "");

    Q_INVOKABLE void startShortcutCapture();
    Q_INVOKABLE void captureShortcutKey(int key, int modifiers);
    Q_INVOKABLE void stopShortcutCapture();

    bool isCapturing() const { return m_capturing; }
    QString capturedShortcut() const { return m_capturedShortcut; }

signals:
    void capturingChanged();
    void capturedShortcutChanged();

private:
    bool m_capturing = false;
    QString m_capturedShortcut;
};
