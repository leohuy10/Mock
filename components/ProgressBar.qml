import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    color: "transparent"
    
    // Access parent colors
    property string colorBg2: parent.colorBg2
    property string colorBg3: parent.colorBg3
    property string colorPrimary: parent.colorPrimary
    property string colorAccent: parent.colorAccent
    property string colorText: parent.colorText
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 5
        
        // Progress Slider
        Slider {
            Layout.fillWidth: true
            Layout.preferredHeight: 20
            from: 0
            to: musicController.totalDuration
            value: musicController.currentPosition
            
            onMoved: musicController.seek(value)
            
            background: Rectangle {
                x: parent.leftPadding
                y: parent.topPadding + parent.availableHeight / 2 - height / 2
                implicitWidth: 200
                implicitHeight: 8
                width: parent.availableWidth
                height: implicitHeight
                radius: 4
                color: colorBg3
                
                Rectangle {
                    width: parent.width * (musicController.currentPosition / (musicController.totalDuration > 0 ? musicController.totalDuration : 1))
                    height: parent.height
                    radius: 4
                    gradient: Gradient {
                        orientation: Gradient.Horizontal
                        GradientStop { position: 0.0; color: colorPrimary }
                        GradientStop { position: 1.0; color: colorAccent }
                    }
                }
            }
            
            handle: Rectangle {
                x: parent.leftPadding + parent.visualPosition * (parent.availableWidth - width)
                y: parent.topPadding + parent.availableHeight / 2 - height / 2
                implicitWidth: 18
                implicitHeight: 18
                radius: 9
                color: colorPrimary
                border.color: colorPrimary
                border.width: 2
            }
        }
        
        // Time Labels
        RowLayout {
            Layout.fillWidth: true
            spacing: 10
            
            Text {
                text: musicController.formatTime(musicController.currentPosition)
                color: colorPrimary
                font.pixelSize: 14
                font.bold: true
            }
            
            Item { Layout.fillWidth: true }
            
            Text {
                text: musicController.formatTime(musicController.totalDuration)
                color: colorPrimary
                font.pixelSize: 14
                font.bold: true
            }
        }
    }
}
