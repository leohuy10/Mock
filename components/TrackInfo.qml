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
    property string colorDim: parent.colorDim
    
    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: colorBg3 }
            GradientStop { position: 1.0; color: "#16213e" }
        }
        radius: 15
        
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 10
            
            Text {
                text: musicController.currentTitle
                color: colorPrimary
                font.pixelSize: 28
                font.bold: true
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.Wrap
            }
            
            Text {
                text: musicController.currentArtist
                color: colorAccent
                font.pixelSize: 20
                font.bold: true
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
            }
            
            Text {
                text: musicController.currentAlbum
                color: colorDim
                font.pixelSize: 16
                font.italic: true
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }
}
