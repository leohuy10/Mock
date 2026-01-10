import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: root
    visible: true
    width: 1200
    height: 800
    title: "Car Music Player Professional"
    
    // Theme Colors
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

    // MAIN LAYOUT
    RowLayout {
        anchors.fill: parent
        anchors.margins: 25
        spacing: 30

        // ==========================================
        // CỘT TRÁI: PLAYER (TRÌNH PHÁT NHẠC)
        // ==========================================
        ColumnLayout {
            Layout.fillWidth: true
            Layout.preferredWidth: 1 // Tỷ lệ 1:1 với bên phải
            spacing: 20

            // 1. Track Info Card
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 300
                color: colorBg3
                radius: 20
                border.color: colorPrimary
                border.width: 1
                clip: true

                ColumnLayout {
                    anchors.centerIn: parent
                    width: parent.width * 0.8
                    spacing: 15
                    
                    Text {
                        text: musicController.currentTitle || "No Track Selected"
                        color: colorText
                        font.pixelSize: 32
                        font.bold: true
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignHCenter
                        elide: Text.ElideRight
                    }
                    
                    Text {
                        text: musicController.currentArtist || "Unknown Artist"
                        color: colorPrimary
                        font.pixelSize: 20
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignHCenter
                    }

                    Rectangle {
                        Layout.alignment: Qt.AlignHCenter
                        width: 60; height: 4
                        radius: 2
                        color: colorAccent
                    }
                }
            }
            
            // 2. Progress Section
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 5

                Slider {
                    id: progressSlider
                    Layout.fillWidth: true
                    from: 0
                    to: musicController.totalDuration > 0 ? musicController.totalDuration : 100
                    value: musicController.currentPosition
                    onMoved: musicController.seek(value)
                    
                    background: Rectangle {
                        height: 8
                        width: progressSlider.availableWidth
                        radius: 4
                        color: colorBg3
                        Rectangle {
                            width: progressSlider.visualPosition * parent.width
                            height: parent.height
                            color: colorPrimary
                            radius: 4
                        }
                    }
                    handle: Rectangle {
                        x: progressSlider.leftPadding + progressSlider.visualPosition * (progressSlider.availableWidth - width)
                        y: progressSlider.topPadding + progressSlider.availableHeight / 2 - height / 2
                        width: 18; height: 18; radius: 9
                        color: colorPrimary
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    Text { text: musicController.formatTime(musicController.currentPosition); color: colorPrimary; font.bold: true; topPadding: 15 }
                    Item { Layout.fillWidth: true }
                    Text { text: musicController.formatTime(musicController.totalDuration); color: colorPrimary; font.bold: true; topPadding: 15 }
                }
            }
            
            // 3. Control Buttons
            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: 20

                Button {
                    text: "🔀"; checkable: true; checked: musicController.isShuffle
                    onToggled: musicController.isShuffle = checked
                    Layout.preferredWidth: 60; Layout.preferredHeight: 60
                    background: Rectangle { color: parent.checked ? colorPrimary : colorBg3; radius: 10 }
                }

                Button {
                    text: "⏮"; onClicked: musicController.previous()
                    Layout.preferredWidth: 70; Layout.preferredHeight: 70
                    background: Rectangle { color: colorBg3; radius: 10; border.color: colorPrimary }
                }

                Button {
                    text: musicController.isPlaying ? "⏸" : "▶"
                    onClicked: musicController.playPause()
                    Layout.preferredWidth: 90; Layout.preferredHeight: 90
                    background: Rectangle { color: colorAccent; radius: 45 } // Nút tròn
                    contentItem: Text { text: parent.text; font.pixelSize: 32; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter; color: "white" }
                }

                Button {
                    text: "⏭"; onClicked: musicController.next()
                    Layout.preferredWidth: 70; Layout.preferredHeight: 70
                    background: Rectangle { color: colorBg3; radius: 10; border.color: colorPrimary }
                }

                Button {
                    text: "🔁"; checkable: true; checked: musicController.isRepeat
                    onToggled: musicController.isRepeat = checked
                    Layout.preferredWidth: 60; Layout.preferredHeight: 60
                    background: Rectangle { color: parent.checked ? colorPrimary : colorBg3; radius: 10 }
                }
            }

            // 4. Volume Control
            RowLayout {
                Layout.fillWidth: true
                Layout.margins: 10
                spacing: 15
                Text { text: "🔊"; color: colorPrimary; font.pixelSize: 20 }
                Slider {
                    Layout.fillWidth: true
                    from: 0; to: 100
                    value: musicController.volume
                    onMoved: musicController.volume = value
                }
                Text { text: musicController.volume + "%"; color: colorPrimary; Layout.preferredWidth: 40 }
            }

            Item { Layout.fillHeight: true }
        }

        // Dòng kẻ ngăn cách
        Rectangle { Layout.fillHeight: true; width: 1; color: colorPrimary; opacity: 0.2 }

        // ==========================================
        // CỘT PHẢI: PLAYLIST & SEARCH
        // ==========================================
        ColumnLayout {
            Layout.fillWidth: true
            Layout.preferredWidth: 1
            spacing: 15

            // Text {
            //     text: "LIBRARY"
            //     color: colorPrimary
            //     font.pixelSize: 24
            //     font.bold: true
            //     font.letterSpacing: 2
            // }

            // Search Bar
            TextField {
                id: searchBar
                Layout.fillWidth: true
                placeholderText: "🔍 Search songs..."
                color: colorText
                font.pixelSize: 16
                padding: 12
                background: Rectangle {
                    color: colorBg3
                    radius: 12
                    border.color: searchBar.activeFocus ? colorPrimary : "transparent"
                }
            }

            // Playlist View
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#0a192f"
                radius: 15
                clip: true
                border.color: colorBg3

                ListView {
                    id: playlistView
                    anchors.fill: parent
                    anchors.margins: 10
                    model: musicController.playlistItems
                    spacing: 5

                    currentIndex: musicController.currentTrackIndex

                    onCurrentIndexChanged: {
                        playlistView.positionViewAtIndex(currentIndex, ListView.Beginning)
                    }

                    delegate: ItemDelegate {
                        width: playlistView.width
                        height: 50

                        background: Rectangle {
                            color: playlistView.currentIndex === index ? colorPrimary : (hovered ? colorBg3 : "transparent")
                            opacity: playlistView.currentIndex === index ? 0.2 : 1
                            radius: 8
                        }
                        contentItem: RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 15
                            anchors.rightMargin: 15
                            spacing: 15
                            Text { text: (index + 1); color: colorPrimary; opacity: 0.5; Layout.preferredWidth: 25; verticalAlignment: Text.AlignVCenter }
                            Text { text: modelData; color: colorText; Layout.fillWidth: true; elide: Text.ElideRight; verticalAlignment: Text.AlignVCenter }
                            Text { 
                                text: "..."; visible: playlistView.currentIndex === index; color: colorPrimary; font.pixelSize: 32;
                                bottomPadding: 20
                            }
                        }
                        onClicked: {
                            playlistView.currentIndex = index
                            musicController.selectTrack(index)
                        }
                    }
                    ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }
                }
            }

            // Bottom Button
            Button {
                Layout.fillWidth: true
                Layout.preferredHeight: 50
                text: "ADD MUSIC FILES"
                onClicked: console.log("Open File Dialog")
                background: Rectangle {
                    color: "transparent"
                    border.color: colorAccent
                    border.width: 2
                    radius: 12
                }
                contentItem: Text { text: parent.text; color: colorText; font.bold: true; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
            }
        }
    }
}