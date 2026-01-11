#ifndef MUSICCONTROLLER_H
#define MUSICCONTROLLER_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QVector>
#include <QTime>
#include "Song.h"
#include "MusicLibrary.h"
#include "MusicPlayer.h"
#include "PlaybackQueue.h"
#include "PlaybackHistory.h"

// Extended Song struct to include file path
struct ExtendedSong : public Song {
    // Kế thừa tất cả từ Song (id, title, artist, album, duration, filePath)
};

class MusicController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentTitle READ getCurrentTitle NOTIFY currentTitleChanged)
    Q_PROPERTY(QString currentArtist READ getCurrentArtist NOTIFY currentArtistChanged)
    Q_PROPERTY(QString currentAlbum READ getCurrentAlbum NOTIFY currentAlbumChanged)
    Q_PROPERTY(qint64 currentPosition READ getCurrentPosition NOTIFY positionChanged)
    Q_PROPERTY(qint64 totalDuration READ getTotalDuration NOTIFY durationChanged)
    Q_PROPERTY(int volume READ getVolume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(bool isPlaying READ getIsPlaying NOTIFY playingStateChanged)
    Q_PROPERTY(bool isShuffle READ getIsShuffle WRITE setShuffle NOTIFY shuffleStateChanged)
    Q_PROPERTY(bool isRepeat READ getIsRepeat WRITE setRepeat NOTIFY repeatStateChanged)
    Q_PROPERTY(QStringList playlistItems READ getPlaylistItems NOTIFY playlistChanged)
    Q_PROPERTY(int currentTrackIndex READ getCurrentTrackIndex WRITE setCurrentTrackIndex NOTIFY currentTrackIndexChanged)
    Q_PROPERTY(int queueCount READ getQueueCount NOTIFY queueCountChanged)
    Q_PROPERTY(int currentMode READ getCurrentMode NOTIFY currentModeChanged)

    enum FilterMode {
        ShowAll,
        ShowQueue
    };

public:
    explicit MusicController(QObject *parent = nullptr);
    ~MusicController();

    // Getter properties
    QString getCurrentTitle() const;
    QString getCurrentArtist() const;
    QString getCurrentAlbum() const;
    qint64 getCurrentPosition() const;
    qint64 getTotalDuration() const;
    int getVolume() const;
    bool getIsPlaying() const;
    bool getIsShuffle() const;
    bool getIsRepeat() const;
    QStringList getPlaylistItems() const;
    int getCurrentTrackIndex() const;
    int getQueueCount() const;

    // Setter properties
    void setVolume(int value);
    void setShuffle(bool value);
    void setRepeat(bool value);
    void setCurrentTrackIndex(int index);

public slots:
    void search(const QString &text);
    void setFilterMode(int mode);
    void addToQueue(int songId);
    int getSongIdAt(int index) const;
    void removeFromQueue(int index);
    int getCurrentMode() const { return static_cast<int>(currentMode); }

    // Playback control
    void playPause();
    void stop();
    void next();
    void previous();
    void seek(qint64 position);
    
    // Playlist control
    void addMusicFiles(const QStringList &filePaths);
    void selectTrack(int index);
    void clearPlaylist();
    
    // Utility
    QString formatTime(qint64 milliseconds) const;

signals:
    void currentTitleChanged();
    void currentArtistChanged();
    void currentAlbumChanged();
    void positionChanged();
    void durationChanged();
    void volumeChanged();
    void playingStateChanged();
    void shuffleStateChanged();
    void repeatStateChanged();
    void playlistChanged();
    void currentTrackIndexChanged();
    void currentModeChanged();
    void queueCountChanged();

private slots:
    void onPositionChanged(qint64 position);
    void onDurationChanged(qint64 duration);
    void onMediaPlayerStateChanged(QMediaPlayer::PlaybackState state);
    void onPlaybackEnded();
    bool isSongInQueue(int songId) const;

private:
    void updateTrackInfo();
    void playTrack(int index);
    void playSongFile(const QString &filePath);  // Helper phát nhạc
    void loadDemoPlaylist();

    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    
    // Backend components
    MusicLibrary musicLibrary;
    MusicPlayer musicPlayer;
    PlaybackQueue playbackQueue;
    PlaybackHistory playbackHistory;
    
    // Playlist
    QVector<ExtendedSong> playlist;
    int currentTrackIndex;
    bool isPlaying;
    bool isShuffle;
    bool isRepeat;
    int currentVolume;

    // Filter
    FilterMode currentMode = ShowAll;
    QVector<ExtendedSong> queueList;
    QVector<ExtendedSong> manualQueue; // ds hang cho thu cong
    
    // Bài đang phát (lưu riêng để đồng bộ giữa các mode)
    ExtendedSong currentPlayingSong;
};

#endif // MUSICCONTROLLER_H
