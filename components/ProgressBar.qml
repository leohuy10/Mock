import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    color: "transparent"

    // Colors từ parent
    property string colorBg2: parent.colorBg2
    property string colorBg3: parent.colorBg3
    property string colorPrimary: parent.colorPrimary
    property string colorAccent: parent.colorAccent
    property string colorText: parent.colorText

    ColumnLayout {
        anchors.fill: parent
        spacing: 6

        /* =======================
           PROGRESS SLIDER
        ======================== */
        Slider {
            id: progressSlider
            Layout.fillWidth: true
            Layout.preferredHeight: 22

            from: 0
            to: musicController.totalDuration > 0
                ? musicController.totalDuration
                : 100

            value: 0
            live: false

            /* --- Đồng bộ từ backend --- */
            Connections {
                target: musicController

                function onPositionChanged() {
                    // Không ghi đè khi user đang kéo
                    if (!progressSlider.pressed) {
                        progressSlider.value = musicController.currentPosition
                    }
                }

                function onDurationChanged() {
                    progressSlider.to = musicController.totalDuration > 0
                                        ? musicController.totalDuration
                                        : 100
                }
            }

            /* --- Seek khi người dùng kéo xong --- */
            onMoved: {
                musicController.seek(value)
            }

            /* =======================
               CUSTOM BACKGROUND
            ======================== */
            background: Rectangle {
                x: parent.leftPadding
                y: parent.topPadding + parent.availableHeight / 2 - height / 2
                width: parent.availableWidth
                height: 8
                radius: 4
                color: colorBg3

                // Thanh progress (theo slider, KHÔNG theo backend)
                Rectangle {
                    width: parent.width * progressSlider.visualPosition
                    height: parent.height
                    radius: 4

                    gradient: Gradient {
                        orientation: Gradient.Horizontal
                        GradientStop { position: 0.0; color: colorPrimary }
                        GradientStop { position: 1.0; color: colorAccent }
                    }
                }
            }

            /* =======================
               HANDLE
            ======================== */
            handle: Rectangle {
                width: 18
                height: 18
                radius: 9
                color: colorPrimary
                border.color: colorPrimary
                border.width: 2
                
                // Tự động update vị trí
                x: progressSlider.leftPadding + progressSlider.visualPosition * (progressSlider.availableWidth - width)
                y: progressSlider.topPadding + progressSlider.availableHeight / 2 - height / 2
            }
        }

        /* =======================
           TIME LABELS
        ======================== */
        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Text {
                text: musicController.formatTime(
                          progressSlider.pressed
                          ? progressSlider.value
                          : musicController.currentPosition
                      )
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
