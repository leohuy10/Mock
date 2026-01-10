import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    color: "#0f3460"
    radius: 10
    border.color: parent.colorPrimary
    border.width: 2
    
    // Access parent colors
    property string colorPrimary: parent.colorPrimary
    property string colorAccent: parent.colorAccent
    property string colorText: parent.colorText
    property string colorDim: parent.colorDim
    
    ListView {
        id: playlistView
        anchors.fill: parent
        anchors.margins: 5
        
        model: musicController.playlistItems
        
        delegate: Rectangle {
            width: playlistView.width
            height: 40
            color: playlistView.currentIndex === index ? colorAccent : "transparent"
            
            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                spacing: 10
                
                Text {
                    text: (index + 1) + "."
                    color: playlistView.currentIndex === index ? colorText : colorPrimary
                    font.pixelSize: 14
                    Layout.preferredWidth: 30
                }
                
                Text {
                    text: modelData
                    color: playlistView.currentIndex === index ? colorText : colorText
                    font.pixelSize: 14
                    Layout.fillWidth: true
                    elide: Text.ElideRight
                }
            }
            
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    musicController.selectTrack(index)
                }
            }
        }
        
        ScrollBar.vertical: ScrollBar {
            active: true
            policy: ScrollBar.AsNeeded
        }
    }
}
