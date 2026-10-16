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
    title: qsTr("Arduino Stream Deck Application")
    color: window.lightMode ? window.reallyLight : window.reallyDark
    property bool lightMode: Application.styleHints.colorScheme === Qt.Light
    property color reallyDark: "#1f1f1f"
    property color dark: "#262626"
    property color reallyLight: "#e7e7e7"
    property color light: "#e0e0e0"

    Rectangle {
        id: titleBar
        color: window.lightMode ? window.reallyLight : window.reallyDark
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        height: Math.max(30, parent.height * 0.08)

        Label {
            id: title
            color: window.lightMode ? window.dark : window.light
            font.pixelSize: titleBar.height * 0.80
            fontSizeMode: Text.Fit
            anchors.fill: parent
            anchors.topMargin: titleBar.height * 0.10
            anchors.bottomMargin: titleBar.height * 0.10
            anchors.leftMargin: titleBar.height * 0.20
            font.family: "Segoe UI"
            font.bold: true
            text: qsTr("Title")
        }

        Rectangle {
            id: titleBarIcons
            color: window.lightMode ? window.reallyLight : window.reallyDark
            anchors.right: titleBar.right
            anchors.top: titleBar.top
            anchors.bottom: titleBar.bottom
            anchors.rightMargin: titleBar.height * 0.20
            anchors.topMargin: titleBar.height * 0.10
            anchors.bottomMargin: titleBar.height * 0.10
            width: titleBar.width * 0.40
        }
    }

    Rectangle {
        id: mainDiv
        color: window.lightMode ? window.reallyLight : window.reallyDark
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: titleBar.bottom

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
}
