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
    color: window.windowBackground

    // true = follow system theme; false = use manual selection
    property bool followSystemTheme: true

    // Used only when followSystemTheme is false
    property bool manualLightMode: false

    readonly property bool lightMode: followSystemTheme
        ? Application.styleHints.colorScheme === Qt.Light
        : manualLightMode

    readonly property color windowBackground:
        lightMode ? "#F5F6F8" : "#1F1F1F"

    readonly property color titleBarBackground:
        lightMode ? "#E7E9ED" : "#171717"

    readonly property color sidebarBackground:
        lightMode ? "#FFFFFF" : "#1B1B1B"

    readonly property color surfaceBackground:
        lightMode ? "#FFFFFF" : "#292929"

    readonly property color primaryText:
        lightMode ? "#202124" : "#F0F0F0"

    readonly property color secondaryText:
        lightMode ? "#5F6368" : "#B8B8B8"

    readonly property color borderColor:
        lightMode ? "#D0D4DA" : "#454545"

    readonly property color buttonBackground:
        lightMode ? "#E8EBF0" : "#303030"

    readonly property color selectedBackground:
        lightMode ? "#D5E4FA" : "#354D6B"

    readonly property color selectedText:
        lightMode ? "#163A67" : "#FFFFFF"

    readonly property color inputBackground:
        lightMode ? "#FFFFFF" : "#303030"

    readonly property color placeholderText:
        lightMode ? "#777777" : "#AAAAAA"

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

    property bool loadingSettings: false

    Component.onCompleted: {
        loadingSettings = true

        buttonConfigs = configManager.load()
        followSystemTheme = configManager.loadFollowSystemTheme()
        manualLightMode = configManager.loadManualLightMode()

        loadingSettings = false
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
        configManager.saveFollowSystemTheme(followSystemTheme)
        configManager.saveManualLightMode(manualLightMode)
    }

    onFollowSystemThemeChanged: {
        if (!loadingSettings)
            saveConfigs()
    }

    onManualLightModeChanged: {
        if (!loadingSettings)
            saveConfigs()
    }

    onClosing: function(close) {
        close.accepted = false
        hide()
    }

    Rectangle {
        id: titleBar
        color: window.titleBarBackground
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        height: Math.max(30, parent.height * 0.08)

        Label {
            id: title
            color: window.primaryText
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
            color: window.titleBarBackground
            anchors.right: titleBar.right
            anchors.top: titleBar.top
            anchors.bottom: titleBar.bottom
            anchors.rightMargin: titleBar.height * 0.20
            anchors.topMargin: titleBar.height * 0.10
            anchors.bottomMargin: titleBar.height * 0.10
            width: titleBar.width * 0.40

            Switch {
                id: themeModeSwitch

                anchors.verticalCenter: parent.verticalCenter
                anchors.right: lightModeSwitch.left
                anchors.rightMargin: parent.width * 0.05

                checked: !window.followSystemTheme

                onToggled: {
                    window.followSystemTheme = !checked
                }

                text: checked ? "Manual" : "System"

                palette.windowText: window.primaryText
                palette.buttonText: window.primaryText

                indicator: Rectangle {
                    implicitWidth: 40
                    implicitHeight: 22

                    x: themeModeSwitch.leftPadding
                    y: parent.height / 2 - height / 2

                    radius: height / 2

                    color: themeModeSwitch.checked
                           ? "#2E9E55"
                           : (window.lightMode ? "#C9CDD2" : "#505050")

                    border.width: 1
                    border.color: themeModeSwitch.checked
                                  ? "#258346"
                                  : window.borderColor

                    Rectangle {
                        width: 16
                        height: 16
                        radius: height / 2

                        y: (parent.height - height) / 2

                        x: themeModeSwitch.checked
                           ? parent.width - width - 3
                           : 3

                        color: "#FFFFFF"

                        Behavior on x {
                            NumberAnimation { duration: 120 }
                        }
                    }
                }

                contentItem: Text {
                    text: themeModeSwitch.text
                    font: themeModeSwitch.font
                    color: window.primaryText
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: themeModeSwitch.indicator.width
                                 + themeModeSwitch.spacing
                }
            }

            Button {
                id: lightModeSwitch

                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: parent.width * 0.05

                height: parent.height * 0.8
                width: parent.width * 0.4

                text: window.lightMode ? "\u263D Dark mode" : "\u263C Light mode"

                palette.buttonText: window.primaryText

                background: Rectangle {
                    radius: parent.height * 0.15
                    color: parent.down
                           ? window.borderColor
                           : parent.hovered
                             ? window.buttonBackground
                             : window.surfaceBackground

                    border.width: 1
                    border.color: window.borderColor
                }

                enabled: !window.followSystemTheme

                onClicked: {
                    window.manualLightMode = !window.manualLightMode
                }
            }
        }
    }

    Rectangle {
        id: mainDiv
        color: window.windowBackground
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
                            checked: index === window.selectedIndex

                            ButtonGroup.group: buttonGroup

                            Layout.fillWidth: true

                            contentItem: Text {
                                text: parent.text
                                color: parent.checked
                                       ? window.selectedText
                                       : window.primaryText
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                            }

                            background: Rectangle {
                                color: parent.checked
                                       ? window.selectedBackground
                                       : parent.down
                                         ? window.borderColor
                                         : parent.hovered
                                           ? window.buttonBackground
                                           : window.surfaceBackground

                                border.width: 1
                                border.color: parent.checked
                                              ? window.selectedBackground
                                              : window.borderColor
                            }

                            onClicked: {
                                window.selectedIndex = index
                            }
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        id: sidebarDiv
        color: window.sidebarBackground
        anchors.left: mainDiv.right
        anchors.top: titleBar.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom


        Frame {
            anchors.fill: parent
            anchors.margins: 12

            background: Rectangle {
                color: "transparent"
                border.width: 0
            }

            ScrollView {
                id: settingsScrollView

                anchors.fill: parent
                clip: false
                contentWidth: availableWidth

                ScrollBar.vertical: ScrollBar {
                    id: settingsScrollBar

                    parent: settingsScrollView
                    anchors.top: settingsScrollView.top
                    anchors.right: settingsScrollView.right
                    anchors.bottom: settingsScrollView.bottom

                    policy: ScrollBar.AlwaysOn
                    width: 12

                    background: Rectangle {
                        color: window.lightMode ? "#E1E4E8" : "#252525"
                        radius: width / 2
                    }

                    contentItem: Rectangle {
                        implicitWidth: 8
                        radius: width / 2

                        color: settingsScrollBar.pressed
                               ? (window.lightMode ? "#606770" : "#B0B0B0")
                               : settingsScrollBar.hovered
                                 ? (window.lightMode ? "#858C96" : "#909090")
                                 : (window.lightMode ? "#A0A5AD" : "#686868")
                    }
                }

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 12

                    Label {
                        text: "Button " + (window.selectedIndex + 1)
                              + " Settings"
                        font.pixelSize: 22
                        font.bold: true
                        color: window.primaryText
                    }

                    Label {
                        text: "Choose an action"
                        font.bold: true
                        color: window.primaryText
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
                        contentItem: Text {
                            text: parent.text
                            color: window.primaryText
                            font: parent.font
                            verticalAlignment: Text.AlignVCenter
                            leftPadding: parent.indicator.width + parent.spacing
                            wrapMode: Text.WordWrap
                        }
                        onClicked: window.setAction("none")
                    }

                    RadioButton {
                        text: "Open application"
                        ButtonGroup.group: actionGroup
                        checked: window.buttonConfigs[window.selectedIndex].action
                                 === "application"
                        contentItem: Text {
                            text: parent.text
                            color: window.primaryText
                            font: parent.font
                            verticalAlignment: Text.AlignVCenter
                            leftPadding: parent.indicator.width + parent.spacing
                            wrapMode: Text.WordWrap
                        }
                        onClicked: window.setAction("application")
                    }

                    RadioButton {
                        text: "Open website"
                        ButtonGroup.group: actionGroup
                        checked: window.buttonConfigs[window.selectedIndex].action
                                 === "website"
                        contentItem: Text {
                            text: parent.text
                            color: window.primaryText
                            font: parent.font
                            verticalAlignment: Text.AlignVCenter
                            leftPadding: parent.indicator.width + parent.spacing
                            wrapMode: Text.WordWrap
                        }
                        onClicked: window.setAction("website")
                    }

                    RadioButton {
                        text: "Copy"
                        ButtonGroup.group: actionGroup
                        checked: window.buttonConfigs[window.selectedIndex].action
                                 === "copy"
                        contentItem: Text {
                            text: parent.text
                            color: window.primaryText
                            font: parent.font
                            verticalAlignment: Text.AlignVCenter
                            leftPadding: parent.indicator.width + parent.spacing
                            wrapMode: Text.WordWrap
                        }
                        onClicked: window.setAction("copy")
                    }

                    RadioButton {
                        text: "Cut"
                        ButtonGroup.group: actionGroup
                        checked: window.buttonConfigs[window.selectedIndex].action
                                 === "cut"
                        contentItem: Text {
                            text: parent.text
                            color: window.primaryText
                            font: parent.font
                            verticalAlignment: Text.AlignVCenter
                            leftPadding: parent.indicator.width + parent.spacing
                            wrapMode: Text.WordWrap
                        }
                        onClicked: window.setAction("cut")
                    }

                    RadioButton {
                        text: "Paste"
                        ButtonGroup.group: actionGroup
                        checked: window.buttonConfigs[window.selectedIndex].action
                                 === "paste"
                        contentItem: Text {
                            text: parent.text
                            color: window.primaryText
                            font: parent.font
                            verticalAlignment: Text.AlignVCenter
                            leftPadding: parent.indicator.width + parent.spacing
                            wrapMode: Text.WordWrap
                        }
                        onClicked: window.setAction("paste")
                    }

                    RadioButton {
                        text: "Keyboard shortcut"
                        ButtonGroup.group: actionGroup
                        checked: window.buttonConfigs[window.selectedIndex].action
                                 === "shortcut"
                        contentItem: Text {
                            text: parent.text
                            color: window.primaryText
                            font: parent.font
                            verticalAlignment: Text.AlignVCenter
                            leftPadding: parent.indicator.width + parent.spacing
                            wrapMode: Text.WordWrap
                        }
                        onClicked: window.setAction("shortcut")
                    }

                    RadioButton {
                        text: "Screenshot"
                        ButtonGroup.group: actionGroup
                        checked: window.buttonConfigs[window.selectedIndex].action
                                 === "screenshot"
                        contentItem: Text {
                            text: parent.text
                            color: window.primaryText
                            font: parent.font
                            verticalAlignment: Text.AlignVCenter
                            leftPadding: parent.indicator.width + parent.spacing
                            wrapMode: Text.WordWrap
                        }
                        onClicked: window.setAction("screenshot")
                    }

                    RadioButton {
                        text: "Play / pause"
                        ButtonGroup.group: actionGroup
                        checked: window.buttonConfigs[window.selectedIndex].action
                                 === "playPause"
                        contentItem: Text {
                            text: parent.text
                            color: window.primaryText
                            font: parent.font
                            verticalAlignment: Text.AlignVCenter
                            leftPadding: parent.indicator.width + parent.spacing
                            wrapMode: Text.WordWrap
                        }
                        onClicked: window.setAction("playPause")
                    }

                    RadioButton {
                        text: "Lock PC"
                        ButtonGroup.group: actionGroup
                        checked: window.buttonConfigs[window.selectedIndex].action
                                 === "lock"
                        contentItem: Text {
                            text: parent.text
                            color: window.primaryText
                            font: parent.font
                            verticalAlignment: Text.AlignVCenter
                            leftPadding: parent.indicator.width + parent.spacing
                            wrapMode: Text.WordWrap
                        }
                        onClicked: window.setAction("lock")
                    }

                    RadioButton {
                        text: "Forward"
                        ButtonGroup.group: actionGroup
                        checked: window.buttonConfigs[window.selectedIndex].action
                                 === "forward"
                        contentItem: Text {
                            text: parent.text
                            color: window.primaryText
                            font: parent.font
                            verticalAlignment: Text.AlignVCenter
                            leftPadding: parent.indicator.width + parent.spacing
                            wrapMode: Text.WordWrap
                        }
                        onClicked: window.setAction("forward")
                    }

                    RadioButton {
                        text: "Backward"
                        ButtonGroup.group: actionGroup
                        checked: window.buttonConfigs[window.selectedIndex].action
                                 === "backward"
                        contentItem: Text {
                            text: parent.text
                            color: window.primaryText
                            font: parent.font
                            verticalAlignment: Text.AlignVCenter
                            leftPadding: parent.indicator.width + parent.spacing
                            wrapMode: Text.WordWrap
                        }
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

                            currentIndex: indexOfValue(
                                window.buttonConfigs[window.selectedIndex].value
                            )

                            displayText: currentIndex >= 0
                                         ? currentText
                                         : "Select an application..."

                            contentItem: Text {
                                leftPadding: 8
                                rightPadding: applicationCombo.indicator.width + 8

                                text: applicationCombo.displayText
                                color: applicationCombo.currentIndex >= 0
                                       ? window.primaryText
                                       : window.placeholderText

                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                            }

                            background: Rectangle {
                                radius: 5
                                color: window.inputBackground
                                border.width: 1
                                border.color: applicationCombo.activeFocus
                                              ? (window.lightMode ? "#316AC5" : "#8AB4F8")
                                              : window.borderColor
                            }

                            onActivated: function(index) {
                                window.setValue(valueAt(index))
                            }

                            enabled: count > 0
                        }

                        Label {
                            Layout.fillWidth: true

                            visible: applicationCombo.count === 0

                            text: "No Start Menu shortcuts found."
                            wrapMode: Text.WordWrap
                            opacity: 0.7
                            color: window.secondaryText
                        }
                    }

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

                        color: window.primaryText
                        placeholderTextColor: window.placeholderText
                        selectionColor: window.lightMode ? "#B7D1F5" : "#425D80"

                        background: Rectangle {
                            radius: 5
                            color: window.inputBackground
                            border.width: 1
                            border.color: websiteUrlField.activeFocus
                                          ? (window.lightMode ? "#316AC5" : "#8AB4F8")
                                          : window.borderColor
                        }

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
                            color: window.primaryText
                        }

                        Rectangle {
                            id: shortcutCaptureArea

                            Layout.fillWidth: true
                            height: 48

                            radius: 6
                            border.width: 1
                            border.color: window.borderColor
                            color: window.inputBackground

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

                                //color: window.lightMode ? "#222222" : "#eeeeee"
                                color: window.primaryText

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

                                contentItem: Text {
                                    text: parent.text
                                    color: window.primaryText
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    elide: Text.ElideRight
                                }

                                background: Rectangle {
                                    radius: 6

                                    color: parent.down
                                           ? window.borderColor
                                           : parent.hovered
                                             ? window.buttonBackground
                                             : window.surfaceBackground

                                    border.width: 1
                                    border.color: window.borderColor
                                }

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

                                contentItem: Text {
                                    text: parent.text
                                    color: parent.enabled
                                           ? window.primaryText
                                           : window.secondaryText

                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    elide: Text.ElideRight
                                }

                                background: Rectangle {
                                    radius: 6

                                    color: !parent.enabled
                                           ? window.windowBackground
                                           : parent.down
                                             ? window.borderColor
                                             : parent.hovered
                                               ? window.buttonBackground
                                               : window.surfaceBackground

                                    border.width: 1
                                    border.color: window.borderColor
                                }

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
                            color: window.secondaryText
                        }
                    }
                }
            }
        }
    }
}
