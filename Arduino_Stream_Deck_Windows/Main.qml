import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic


ApplicationWindow {
    id: window
    width: 640
    height: 480
    minimumWidth: 640
    minimumHeight: 480
    visible: true
    title: qsTr("Arduino Stream Deck Configuration")
    color: window.lightMode ? window.mainLight : window.mainDark
    property bool lightMode: Application.styleHints.colorScheme === Qt.Light
    property color titlebarDark: "#171717"
    property color sidebarDark: "#1b1b1b"
    property color mainDark: "#1f1f1f"
    property color buttonDark: "#222222"
    property color darkText: "#262626"
    property color titlebarLight: "#e7e7e7"
    property color sidebarLight: "#808080"
    property color mainLight: "#111111"
    property color lightText: "#e0e0e0"
    property color buttonLight: "#000000"

    onClosing: function(close) {
        close.accepted = false
        hide()
    }

    Rectangle {
        id: titleBar
        color: window.lightMode ? window.titlebarLight : window.titlebarDark
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        height: Math.max(30, parent.height * 0.08)

        Label {
            id: title
            color: window.lightMode ? window.darkText : window.lightText
            font.pixelSize: titleBar.height * 0.80
            fontSizeMode: Text.Fit
            anchors.fill: parent
            anchors.topMargin: titleBar.height * 0.10
            anchors.bottomMargin: titleBar.height * 0.10
            anchors.leftMargin: titleBar.height * 0.20
            font.family: "Segoe UI"
            font.bold: true
            text: qsTr("Stream Deck Options")
        }

        Rectangle {
            id: titleBarIcons
            color: window.lightMode ? window.titlebarLight : window.titlebarDark
            anchors.right: titleBar.right
            anchors.top: titleBar.top
            anchors.bottom: titleBar.bottom
            anchors.rightMargin: titleBar.height * 0.20
            anchors.topMargin: titleBar.height * 0.10
            anchors.bottomMargin: titleBar.height * 0.10
            width: titleBar.width * 0.40

            Button {
                id: lightModeSwitch
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: parent.width * 0.05
                height: parent.height * 0.8
                width: parent.width * 0.4

                background: Rectangle {
                    color: window.lightMode ? window.buttonLight : window.buttonDark
                    radius: parent.height * 0.15
                }

                text: window.lightMode ? "\u263D Dark mode" : "\u263C Light Mode"
                palette.buttonText: window.lightMode ? window.darkText : window.lightText

                onClicked: {
                    window.lightMode = !window.lightMode
                }
            }
        }
    }

    Rectangle {
        id: mainDiv
        color: window.lightMode ? window.mainLight : window.mainDark
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.top: titleBar.bottom
        width: parent.width * 0.65

        Label {
            id: sysTime
            anchors.left: parent.left
            anchors.top: parent.top
            text: Qt.formatTime(new Date(), "hh:mm:ss")

            Timer {
                interval: 1000
                running: true
                repeat: true

                onTriggered: {
                    sysTime.text = Qt.formatTime(new Date, "hh:mm:ss")
                }
            }
        }
    }

    Rectangle {
        id: sidebarDiv
        color: window.lightMode ? window.sidebarLight : window.sidebarDark
        anchors.left: mainDiv.right
        anchors.top: titleBar.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }
}
