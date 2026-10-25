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

    property int selectedIndex: 0

    property var buttonConfigs: [
        { action: "none", value: "" },
        { action: "none", value: "" },
        { action: "none", value: "" },
        { action: "none", value: "" },
        { action: "none", value: "" },
        { action: "none", value: "" },
        { action: "none", value: "" },
        { action: "none", value: "" }
    ]

    Component.onCompleted: {
        buttonConfigs = configManager.load()
    }

    function setAction(action) {
        var updated = buttonConfigs.slice()
        updated[selectedIndex] = {
            action: action,
            value: updated[selectedIndex].value
        }
        buttonConfigs = updated
        saveConfigs()
    }

    function setValue(value) {
        var updated = buttonConfigs.slice()
        updated[selectedIndex] = {
            action: updated[selectedIndex].action,
            value: value
        }
        buttonConfigs = updated
        saveConfigs()
    }
    function saveConfigs() {
        configManager.save(buttonConfigs)
    }

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

        RowLayout {
            anchors.fill: parent
            spacing: 12

            // Main content area
            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.margins: 12

                GridLayout {
                    columns: 4
                    rowSpacing: 8
                    columnSpacing: 8
                    Layout.alignment: Qt.AlignTop

                    ButtonGroup {
                        id: buttonGroup
                        exclusive: true
                    }

                    Repeater {
                        model: [
                            "Button 1", "Button 2",
                            "Button 3", "Button 4",
                            "Button 5", "Button 6",
                            "Button 7", "Button 8"
                        ]

                        Button {
                            required property int index
                            required property string modelData

                            text: modelData
                            checkable: true
                            checked: index === selectedIndex

                            ButtonGroup.group: buttonGroup

                            onClicked: {
                                selectedIndex = index
                            }

                            Layout.fillWidth: true
                        }
                    }
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


        Frame {
            anchors.fill: parent
            anchors.margins: 12

            ScrollView {
                anchors.fill: parent
                clip: true
                contentWidth: availableWidth
                ScrollBar.vertical.policy: ScrollBar.AsNeeded

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 12

                    Label {
                        text: "Button " + (window.selectedIndex + 1)
                              + " Settings"
                        font.pixelSize: 22
                        font.bold: true
                    }

                    Label {
                        text: "Choose an action"
                        font.bold: true
                    }

                    ButtonGroup {
                        id: actionGroup
                        exclusive: true
                    }

                    RadioButton {
                        text: "Do nothing"
                        ButtonGroup.group: actionGroup
                        checked: window.buttonConfigs[window.selectedIndex].action
                                 === "none"
                        onClicked: window.setAction("none")
                    }

                    RadioButton {
                        text: "Open application"
                        ButtonGroup.group: actionGroup
                        checked: window.buttonConfigs[window.selectedIndex].action
                                 === "application"
                        onClicked: window.setAction("application")
                    }

                    RadioButton {
                        text: "Open website"
                        ButtonGroup.group: actionGroup
                        checked: window.buttonConfigs[window.selectedIndex].action
                                 === "website"
                        onClicked: window.setAction("website")
                    }

                    RadioButton {
                        text: "Copy"
                        ButtonGroup.group: actionGroup
                        checked: window.buttonConfigs[window.selectedIndex].action
                                 === "copy"
                        onClicked: window.setAction("copy")
                    }

                    RadioButton {
                        text: "Cut"
                        ButtonGroup.group: actionGroup
                        checked: window.buttonConfigs[window.selectedIndex].action
                                 === "cut"
                        onClicked: window.setAction("cut")
                    }

                    RadioButton {
                        text: "Paste"
                        ButtonGroup.group: actionGroup
                        checked: window.buttonConfigs[window.selectedIndex].action
                                 === "paste"
                        onClicked: window.setAction("paste")
                    }

                    RadioButton {
                        text: "Keyboard shortcut"
                        ButtonGroup.group: actionGroup
                        checked: window.buttonConfigs[window.selectedIndex].action
                                 === "shortcut"
                        onClicked: window.setAction("shortcut")
                    }

                    RadioButton {
                        text: "Screenshot"
                        ButtonGroup.group: actionGroup
                        checked: window.buttonConfigs[window.selectedIndex].action
                                 === "screenshot"
                        onClicked: window.setAction("screenshot")
                    }

                    RadioButton {
                        text: "Play / pause"
                        ButtonGroup.group: actionGroup
                        checked: window.buttonConfigs[window.selectedIndex].action
                                 === "playPause"
                        onClicked: window.setAction("playPause")
                    }

                    RadioButton {
                        text: "Lock PC"
                        ButtonGroup.group: actionGroup
                        checked: window.buttonConfigs[window.selectedIndex].action
                                 === "lock"
                        onClicked: window.setAction("lock")
                    }

                    RadioButton {
                        text: "Forward"
                        ButtonGroup.group: actionGroup
                        checked: window.buttonConfigs[window.selectedIndex].action
                                 === "forward"
                        onClicked: window.setAction("forward")
                    }

                    RadioButton {
                        text: "Backward"
                        ButtonGroup.group: actionGroup
                        checked: window.buttonConfigs[window.selectedIndex].action
                                 === "backward"
                        onClicked: window.setAction("backward")
                    }


                    // Extra settings: only show the field for the selected action.


                    ColumnLayout {
                        id: applicationSettings

                        Layout.fillWidth: true
                        spacing: 6

                        visible: window.buttonConfigs[window.selectedIndex].action
                                 === "application"

                        ComboBox {
                            id: applicationCombo

                            Layout.fillWidth: true

                            model: configManager.availableApplications()

                            textRole: "name"
                            valueRole: "path"

                            // Restore the saved selection for this button.
                            currentIndex: indexOfValue(
                                window.buttonConfigs[window.selectedIndex].value
                            )

                            displayText: currentIndex >= 0
                                         ? currentText
                                         : "Select an application..."

                            onActivated: function(index) {
                                window.setValue(valueAt(index))
                            }

                            // Keep the dropdown usable if the list is empty.
                            enabled: count > 0
                        }

                        Label {
                            Layout.fillWidth: true

                            visible: applicationCombo.count === 0

                            text: "No Start Menu shortcuts found."
                            wrapMode: Text.WordWrap
                            opacity: 0.7
                        }
                    }

                    // Website URL
                    TextField {
                        id: websiteUrlField

                        Layout.fillWidth: true

                        visible: window.buttonConfigs[window.selectedIndex].action
                                 === "website"

                        placeholderText: "https://example.com"

                        text: window.buttonConfigs[window.selectedIndex].action
                              === "website"
                              ? window.buttonConfigs[window.selectedIndex].value
                              : ""

                        onTextEdited: window.setValue(text)
                    }

                    ColumnLayout {
                        id: shortcutSettings

                        Layout.fillWidth: true
                        spacing: 8

                        visible: window.buttonConfigs[window.selectedIndex].action
                                 === "shortcut"

                        Label {
                            text: "Keyboard shortcut"
                            font.bold: true
                        }

                        Rectangle {
                            id: shortcutCaptureArea

                            Layout.fillWidth: true
                            height: 48

                            radius: 6
                            border.width: 1
                            border.color: window.lightMode ? "#777777" : "#aaaaaa"
                            color: window.lightMode ? "#eeeeee" : "#303030"

                            focus: actionHandler.capturing

                            Keys.enabled: actionHandler.capturing

                            Keys.onPressed: function(event) {
                                if (!actionHandler.capturing)
                                    return

                                actionHandler.captureShortcutKey(
                                    event.key, event.modifiers)

                                event.accepted = true
                            }

                            Label {
                                anchors.fill: parent
                                anchors.margins: 8

                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter

                                color: window.lightMode ? "#222222" : "#eeeeee"

                                text: actionHandler.capturing
                                      ? (actionHandler.capturedShortcut.length > 0
                                         ? actionHandler.capturedShortcut
                                         : "Press a key combination...")
                                      : (window.buttonConfigs[window.selectedIndex].value
                                         || "No shortcut configured")
                            }

                            MouseArea {
                                anchors.fill: parent
                                enabled: !actionHandler.capturing

                                onClicked: {
                                    actionHandler.startShortcutCapture()
                                    shortcutCaptureArea.forceActiveFocus()
                                }
                            }
                        }

                        RowLayout {
                            Layout.fillWidth: true

                            Button {
                                text: actionHandler.capturing ? "Accept" : "Record"

                                onClicked: {
                                    if (!actionHandler.capturing) {
                                        actionHandler.startShortcutCapture()
                                        shortcutCaptureArea.forceActiveFocus()
                                    } else if (actionHandler.capturedShortcut.length > 0) {
                                        window.setValue(actionHandler.capturedShortcut)
                                        actionHandler.stopShortcutCapture()
                                    }
                                }
                            }

                            Button {
                                text: "Cancel"
                                enabled: actionHandler.capturing

                                onClicked: {
                                    actionHandler.stopShortcutCapture()
                                }
                            }
                        }

                        Label {
                            Layout.fillWidth: true

                            text: "Click Record, press a shortcut, then click Accept."
                            wrapMode: Text.WordWrap
                            opacity: 0.7
                        }
                    }

                }
            }
        }
    }
}
