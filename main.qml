import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: root
    visible: true
    width: 900
    height: 600
    title: "Car Music Player"
    
    // Dark theme colors
    readonly property string colorBg1: "#1a1a2e"
    readonly property string colorBg2: "#16213e"
    readonly property string colorBg3: "#0f3460"
    readonly property string colorPrimary: "#00d4ff"
    readonly property string colorAccent: "#e94560"
    readonly property string colorText: "#ffffff"
    readonly property string colorDim: "#aaaaaa"
    
    background: Rectangle {
        gradient: Gradient {
            GradientStop { position: 0.0; color: colorBg1 }
            GradientStop { position: 1.0; color: colorBg2 }
        }
    }
    
    contentData: ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20
        
        // Track Info Section
        Loader {
            Layout.fillWidth: true
            Layout.preferredHeight: 150
            sourceComponent: trackInfoComponent
        }
        
        // Progress Bar Section
        Loader {
            Layout.fillWidth: true
            Layout.preferredHeight: 80
            sourceComponent: progressBarComponent
        }
        
        // Player Controls
        Loader {
            Layout.fillWidth: true
            Layout.preferredHeight: 120
            sourceComponent: playerControlsComponent
        }
        
        // Volume Control
        Loader {
            Layout.fillWidth: true
            Layout.preferredHeight: 60
            sourceComponent: volumeControlComponent
        }
        
        // Add Music Button
        Button {
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            text: "➕ Add Music Files"
            
            background: Rectangle {
                color: pressed ? colorBg3 : colorBg2
                border.color: colorPrimary
                border.width: 2
                radius: 10
            }
            
            contentItem: Text {
                text: parent.text
                color: colorPrimary
                font.pixelSize: 16
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            
            onClicked: {
                // Placeholder for file selection
                // In real implementation, use FileDialog
            }
        }
        
        // Playlist Label
        Text {
            text: "📋 Playlist"
            color: colorPrimary
            font.pixelSize: 18
            font.bold: true
            Layout.fillWidth: true
        }
        
        // Playlist View
        Loader {
            Layout.fillWidth: true
            Layout.fillHeight: true
            sourceComponent: playlistViewComponent
        }
    }
    
    // ===== COMPONENT DEFINITIONS =====
    
    Component {
        id: trackInfoComponent
        Rectangle {
            color: "transparent"
            
            Rectangle {
                anchors.fill: parent
                gradient: Gradient {
                    GradientStop { position: 0.0; color: root.colorBg3 }
                    GradientStop { position: 1.0; color: "#16213e" }
                }
                radius: 15
                
                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 20
                    spacing: 10
                    
                    Text {
                        text: musicController.currentTitle
                        color: root.colorPrimary
                        font.pixelSize: 28
                        font.bold: true
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignHCenter
                        wrapMode: Text.Wrap
                    }
                    
                    Text {
                        text: musicController.currentArtist
                        color: root.colorAccent
                        font.pixelSize: 20
                        font.bold: true
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignHCenter
                    }
                    
                    Text {
                        text: musicController.currentAlbum
                        color: root.colorDim
                        font.pixelSize: 16
                        font.italic: true
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
            }
        }
    }
    
    Component {
        id: progressBarComponent
        Rectangle {
            color: "transparent"
            
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
                        color: root.colorBg3
                        
                        Rectangle {
                            width: parent.width * (musicController.currentPosition / (musicController.totalDuration > 0 ? musicController.totalDuration : 1))
                            height: parent.height
                            radius: 4
                            gradient: Gradient {
                                orientation: Gradient.Horizontal
                                GradientStop { position: 0.0; color: root.colorPrimary }
                                GradientStop { position: 1.0; color: root.colorAccent }
                            }
                        }
                    }
                    
                    handle: Rectangle {
                        x: parent.leftPadding + parent.visualPosition * (parent.availableWidth - width)
                        y: parent.topPadding + parent.availableHeight / 2 - height / 2
                        implicitWidth: 18
                        implicitHeight: 18
                        radius: 9
                        color: root.colorPrimary
                        border.color: root.colorPrimary
                        border.width: 2
                    }
                }
                
                // Time Labels
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10
                    
                    Text {
                        text: musicController.formatTime(musicController.currentPosition)
                        color: root.colorPrimary
                        font.pixelSize: 14
                        font.bold: true
                    }
                    
                    Item { Layout.fillWidth: true }
                    
                    Text {
                        text: musicController.formatTime(musicController.totalDuration)
                        color: root.colorPrimary
                        font.pixelSize: 14
                        font.bold: true
                    }
                }
            }
        }
    }
    
    Component {
        id: playerControlsComponent
        Rectangle {
            color: "transparent"
            
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
                        color: parent.checked ? root.colorPrimary : root.colorBg3
                        border.color: parent.hovered ? root.colorAccent : root.colorPrimary
                        border.width: 2
                        radius: 10
                    }
                    
                    contentItem: Text {
                        text: parent.text
                        color: parent.checked ? "#1a1a2e" : root.colorText
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
                        color: parent.pressed ? root.colorBg3 : root.colorBg3
                        border.color: parent.hovered ? root.colorAccent : root.colorPrimary
                        border.width: 2
                        radius: 10
                    }
                    
                    contentItem: Text {
                        text: parent.text
                        color: root.colorText
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
                        color: root.colorAccent
                        border.color: root.colorPrimary
                        border.width: 3
                        radius: 10
                    }
                    
                    contentItem: Text {
                        text: parent.text
                        color: root.colorText
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
                        color: parent.pressed ? root.colorBg3 : root.colorBg3
                        border.color: parent.hovered ? root.colorAccent : root.colorPrimary
                        border.width: 2
                        radius: 10
                    }
                    
                    contentItem: Text {
                        text: parent.text
                        color: root.colorText
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
                        color: parent.pressed ? root.colorBg3 : root.colorBg3
                        border.color: parent.hovered ? root.colorAccent : root.colorPrimary
                        border.width: 2
                        radius: 10
                    }
                    
                    contentItem: Text {
                        text: parent.text
                        color: root.colorText
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
                        color: parent.checked ? root.colorPrimary : root.colorBg3
                        border.color: parent.hovered ? root.colorAccent : root.colorPrimary
                        border.width: 2
                        radius: 10
                    }
                    
                    contentItem: Text {
                        text: parent.text
                        color: parent.checked ? "#1a1a2e" : root.colorText
                        font.pixelSize: 24
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    
                    onToggled: musicController.isRepeat = checked
                }
                
                Item { Layout.fillWidth: true }
            }
        }
    }
    
    Component {
        id: volumeControlComponent
        Rectangle {
            color: "transparent"
            
            RowLayout {
                anchors.fill: parent
                spacing: 10
                
                Item { Layout.fillWidth: true }
                
                Text {
                    text: "🔊"
                    font.pixelSize: 20
                    color: root.colorPrimary
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
                                GradientStop { position: 0.0; color: root.colorPrimary }
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
                        color: root.colorPrimary
                        border.color: root.colorPrimary
                        border.width: 2
                    }
                }
                
                Text {
                    text: musicController.volume + "%"
                    color: root.colorPrimary
                    font.pixelSize: 14
                    font.bold: true
                    Layout.preferredWidth: 40
                }
                
                Item { Layout.fillWidth: true }
            }
        }
    }
    
    Component {
        id: playlistViewComponent
        Rectangle {
            color: root.colorBg3
            radius: 10
            border.color: root.colorPrimary
            border.width: 2
            
            ListView {
                id: playlistView
                anchors.fill: parent
                anchors.margins: 5
                
                model: musicController.playlistItems
                
                delegate: Rectangle {
                    width: playlistView.width
                    height: 40
                    color: playlistView.currentIndex === index ? root.colorAccent : "transparent"
                    
                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        spacing: 10
                        
                        Text {
                            text: (index + 1) + "."
                            color: playlistView.currentIndex === index ? root.colorText : root.colorPrimary
                            font.pixelSize: 14
                            Layout.preferredWidth: 30
                        }
                        
                        Text {
                            text: modelData
                            color: playlistView.currentIndex === index ? root.colorText : root.colorText
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
    }
}
