import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    color: "transparent"
    
    // Access parent colors
    property string colorBg2: parent.colorBg2
    property string colorPrimary: parent.colorPrimary
    property string colorText: parent.colorText
    
    RowLayout {
        anchors.fill: parent
        spacing: 10
        
        Item { Layout.fillWidth: true }
        
        Text {
            text: "🔊"
            font.pixelSize: 20
            color: colorPrimary
        }
        
        Slider {
            Layout.preferredWidth: 200
            Layout.fillHeight: true
            from: 0
            to: 100
            value: musicController.volume
            
            onMoved: musicController.volume = value
            
            background: Rectangle {
                x: parent.leftPadding
                y: parent.topPadding + parent.availableHeight / 2 - height / 2
                implicitWidth: 200
                implicitHeight: 8
                width: parent.availableWidth
                height: implicitHeight
                radius: 4
                color: "#16213e"
                
                Rectangle {
                    width: parent.width * (musicController.volume / 100)
                    height: parent.height
                    radius: 4
                    gradient: Gradient {
                        orientation: Gradient.Horizontal
                        GradientStop { position: 0.0; color: colorPrimary }
                        GradientStop { position: 1.0; color: "#e94560" }
                    }
                }
            }
            
            handle: Rectangle {
                x: parent.leftPadding + parent.visualPosition * (parent.availableWidth - width)
                y: parent.topPadding + parent.availableHeight / 2 - height / 2
                implicitWidth: 16
                implicitHeight: 16
                radius: 8
                color: colorPrimary
                border.color: colorPrimary
                border.width: 2
            }
        }
        
        Text {
            text: musicController.volume + "%"
            color: colorPrimary
            font.pixelSize: 14
            font.bold: true
            Layout.preferredWidth: 40
        }
        
        Item { Layout.fillWidth: true }
    }
}
