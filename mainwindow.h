#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QListWidget>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QTime>
#include <QVector>
#include <QFileDialog>
#include <QUrl>

// Include backend headers
#include "Song.h"
#include "MusicLibrary.h"
#include "MusicPlayer.h"
#include "PlaybackQueue.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

// Extended Song struct to include file path
struct ExtendedSong : public Song {
    std::string filePath;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onPlayPauseClicked();
    void onStopClicked();
    void onPreviousClicked();
    void onNextClicked();
    void onVolumeChanged(int value);
    void onPositionChanged(qint64 position);
    void onDurationChanged(qint64 duration);
    void onPlaylistItemClicked(int row);
    void onShuffleClicked();
    void onRepeatClicked();
    void onSeekSliderMoved(int position);
    void onAddMusicClicked();
    void onMediaPlayerStateChanged(QMediaPlayer::PlaybackState state);
    void onPlaybackEnded();

private:
    void setupUI();
    void applyCarTheme();
    void loadDemoPlaylist();
    void updateTrackInfo();
    void updateTimeLabels(qint64 position, qint64 duration);
    void playTrack(int index);
    void updatePlaylistWidget();
    QString formatTime(qint64 milliseconds);

    Ui::MainWindow *ui;
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    
    // Backend components
    MusicLibrary musicLibrary;
    MusicPlayer musicPlayer;
    PlaybackQueue playbackQueue;
    
    // UI Components
    QLabel *lblTitle;
    QLabel *lblArtist;
    QLabel *lblAlbum;
    QLabel *lblCurrentTime;
    QLabel *lblTotalTime;
    QLabel *lblVolumePercent;
    QSlider *seekSlider;
    QSlider *volumeSlider;
    QPushButton *btnPlayPause;
    QPushButton *btnStop;
    QPushButton *btnPrevious;
    QPushButton *btnNext;
    QPushButton *btnShuffle;
    QPushButton *btnRepeat;
    QPushButton *btnAddMusic;
    QListWidget *playlistWidget;
    
    QVector<ExtendedSong> playlist;
    int currentTrackIndex;
    bool isPlaying;
    bool isShuffle;
    bool isRepeat;
};
#endif // MAINWINDOW_H
