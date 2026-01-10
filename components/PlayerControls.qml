import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    color: "transparent"
    
    // Access parent colors
    property string colorBg3: parent.colorBg3
    property string colorPrimary: parent.colorPrimary
    property string colorAccent: parent.colorAccent
    property string colorText: parent.colorText
    
    RowLayout {
        anchors.fill: parent
        spacing: 15
        
        Item { Layout.fillWidth: true }
        
        // Shuffle Button
        Button {
            Layout.preferredWidth: 60
            Layout.preferredHeight: 60
            text: "🔀"
            checkable: true
            checked: musicController.isShuffle
            
            background: Rectangle {
                color: parent.checked ? colorPrimary : colorBg3
                border.color: parent.hovered ? colorAccent : colorPrimary
                border.width: 2
                radius: 10
            }
            
            contentItem: Text {
                text: parent.text
                color: parent.checked ? "#1a1a2e" : colorText
                font.pixelSize: 24
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            
            onToggled: musicController.isShuffle = checked
        }
        
        // Previous Button
        Button {
            Layout.preferredWidth: 70
            Layout.preferredHeight: 70
            text: "⏮"
            
            background: Rectangle {
                color: parent.pressed ? colorBg3 : colorBg3
                border.color: parent.hovered ? colorAccent : colorPrimary
                border.width: 2
                radius: 10
            }
            
            contentItem: Text {
                text: parent.text
                color: colorText
                font.pixelSize: 28
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            
            onClicked: musicController.previous()
        }
        
        // Play/Pause Button
        Button {
            Layout.preferredWidth: 90
            Layout.preferredHeight: 90
            text: musicController.isPlaying ? "⏸" : "▶"
            
            background: Rectangle {
                color: colorAccent
                border.color: colorPrimary
                border.width: 3
                radius: 10
            }
            
            contentItem: Text {
                text: parent.text
                color: colorText
                font.pixelSize: 32
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            
            onClicked: musicController.playPause()
        }
        
        // Stop Button
        Button {
            Layout.preferredWidth: 70
            Layout.preferredHeight: 70
            text: "⏹"
            
            background: Rectangle {
                color: parent.pressed ? colorBg3 : colorBg3
                border.color: parent.hovered ? colorAccent : colorPrimary
                border.width: 2
                radius: 10
            }
            
            contentItem: Text {
                text: parent.text
                color: colorText
                font.pixelSize: 28
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            
            onClicked: musicController.stop()
        }
        
        // Next Button
        Button {
            Layout.preferredWidth: 70
            Layout.preferredHeight: 70
            text: "⏭"
            
            background: Rectangle {
                color: parent.pressed ? colorBg3 : colorBg3
                border.color: parent.hovered ? colorAccent : colorPrimary
                border.width: 2
                radius: 10
            }
            
            contentItem: Text {
                text: parent.text
                color: colorText
                font.pixelSize: 28
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            
            onClicked: musicController.next()
        }
        
        // Repeat Button
        Button {
            Layout.preferredWidth: 60
            Layout.preferredHeight: 60
            text: "🔁"
            checkable: true
            checked: musicController.isRepeat
            
            background: Rectangle {
                color: parent.checked ? colorPrimary : colorBg3
                border.color: parent.hovered ? colorAccent : colorPrimary
                border.width: 2
                radius: 10
            }
            
            contentItem: Text {
                text: parent.text
                color: parent.checked ? "#1a1a2e" : colorText
                font.pixelSize: 24
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            
            onToggled: musicController.isRepeat = checked
        }
        
        Item { Layout.fillWidth: true }
    }
}
