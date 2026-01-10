#include "musiccontroller.h"
#include <QFileInfo>
#include <QDir>
#include <QRandomGenerator>

MusicController::MusicController(QObject *parent)
    : QObject(parent)
    , currentTrackIndex(0)
    , isPlaying(false)
    , isShuffle(false)
    , isRepeat(false)
    , currentVolume(50)
{
    // Initialize media player
    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);
    
    // Connect signals
    connect(player, &QMediaPlayer::positionChanged, this, &MusicController::onPositionChanged);
    connect(player, &QMediaPlayer::durationChanged, this, &MusicController::onDurationChanged);
    connect(player, &QMediaPlayer::playbackStateChanged, this, &MusicController::onMediaPlayerStateChanged);
    connect(player, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            onPlaybackEnded();
        }
    });
    
    // Load demo playlist
    loadDemoPlaylist();
    
    // Set initial volume
    audioOutput->setVolume(currentVolume / 100.0f);
}

MusicController::~MusicController()
{
}

// ===== GETTER PROPERTIES =====

QString MusicController::getCurrentTitle() const
{
    if (currentTrackIndex >= 0 && currentTrackIndex < playlist.size()) {
        return QString::fromStdString(playlist[currentTrackIndex].title);
    }
    return "No Track Playing";
}

QString MusicController::getCurrentArtist() const
{
    if (currentTrackIndex >= 0 && currentTrackIndex < playlist.size()) {
        return QString::fromStdString(playlist[currentTrackIndex].artist);
    }
    return "Select a song to start";
}

QString MusicController::getCurrentAlbum() const
{
    if (currentTrackIndex >= 0 && currentTrackIndex < playlist.size()) {
        return QString::fromStdString(playlist[currentTrackIndex].album);
    }
    return "";
}

qint64 MusicController::getCurrentPosition() const
{
    return player->position();
}

qint64 MusicController::getTotalDuration() const
{
    return player->duration();
}

int MusicController::getVolume() const
{
    return currentVolume;
}

bool MusicController::getIsPlaying() const
{
    return isPlaying;
}

bool MusicController::getIsShuffle() const
{
    return isShuffle;
}

bool MusicController::getIsRepeat() const
{
    return isRepeat;
}

QStringList MusicController::getPlaylistItems() const
{
    QStringList items;
    for (const ExtendedSong &song : playlist) {
        QString displayText = QString("%1 - %2 [%3]")
            .arg(QString::fromStdString(song.title))
            .arg(QString::fromStdString(song.artist))
            .arg(formatTime(song.duration * 1000));
        items.append(displayText);
    }
    return items;
}

int MusicController::getCurrentTrackIndex() const
{
    return currentTrackIndex;
}

// ===== SETTER PROPERTIES =====

void MusicController::setVolume(int value)
{
    if (value < 0) value = 0;
    if (value > 100) value = 100;
    
    currentVolume = value;
    audioOutput->setVolume(value / 100.0f);
    emit volumeChanged();
}

void MusicController::setShuffle(bool value)
{
    if (isShuffle != value) {
        isShuffle = value;
        emit shuffleStateChanged();
    }
}

void MusicController::setRepeat(bool value)
{
    if (isRepeat != value) {
        isRepeat = value;
        emit repeatStateChanged();
    }
}

void MusicController::setCurrentTrackIndex(int index)
{
    if (index >= 0 && index < playlist.size() && currentTrackIndex != index) {
        currentTrackIndex = index;
        updateTrackInfo();
        emit currentTrackIndexChanged();
    }
}

// ===== PLAYBACK CONTROL SLOTS =====

void MusicController::playPause()
{
    if (playlist.isEmpty()) return;
    
    if (isPlaying) {
        player->pause();
        isPlaying = false;
    } else {
        const ExtendedSong &currentSong = playlist[currentTrackIndex];
        
        if (!currentSong.filePath.empty()) {
            QString filePath = QString::fromStdString(currentSong.filePath);
            player->setSource(QUrl::fromLocalFile(filePath));
        }
        
        player->play();
        isPlaying = true;
        updateTrackInfo();
    }
    
    emit playingStateChanged();
}

void MusicController::stop()
{
    player->stop();
    isPlaying = false;
    emit playingStateChanged();
    emit positionChanged();
}

void MusicController::next()
{
    if (playlist.isEmpty()) return;
    
    if (isShuffle) {
        currentTrackIndex = QRandomGenerator::global()->bounded(playlist.size());
    } else {
        currentTrackIndex++;
        if (currentTrackIndex >= playlist.size()) {
            if (isRepeat) {
                currentTrackIndex = 0;
            } else {
                currentTrackIndex = playlist.size() - 1;
                stop();
                return;
            }
        }
    }
    
    updateTrackInfo();
    if (isPlaying) {
        playTrack(currentTrackIndex);
    }
    
    emit currentTrackIndexChanged();
}

void MusicController::previous()
{
    if (playlist.isEmpty()) return;
    
    currentTrackIndex--;
    if (currentTrackIndex < 0) {
        currentTrackIndex = playlist.size() - 1;
    }
    
    updateTrackInfo();
    if (isPlaying) {
        playTrack(currentTrackIndex);
    }
    
    emit currentTrackIndexChanged();
}

void MusicController::seek(qint64 position)
{
    player->setPosition(position);
}

// ===== PLAYLIST CONTROL SLOTS =====

void MusicController::addMusicFiles(const QStringList &filePaths)
{
    for (const QString &filePath : filePaths) {
        QFileInfo fileInfo(filePath);
        
        ExtendedSong song;
        song.id = playlist.size() + 1;
        song.title = fileInfo.completeBaseName().toStdString();
        song.artist = "Unknown Artist";
        song.album = "Unknown Album";
        song.duration = 180;
        song.filePath = filePath.toStdString();
        
        playlist.append(song);
        musicLibrary.addSong(song);
        playbackQueue.addSong(song);
    }
    
    emit playlistChanged();
}

void MusicController::selectTrack(int index)
{
    if (index >= 0 && index < playlist.size()) {
        currentTrackIndex = index;
        updateTrackInfo();
        emit currentTrackIndexChanged();
        
        if (isPlaying) {
            playTrack(index);
        }
    }
}

void MusicController::clearPlaylist()
{
    stop();
    playlist.clear();
    currentTrackIndex = 0;
    emit playlistChanged();
}

// ===== PRIVATE SLOTS =====

void MusicController::onPositionChanged(qint64 position)
{
    emit positionChanged();
}

void MusicController::onDurationChanged(qint64 duration)
{
    emit durationChanged();
}

void MusicController::onMediaPlayerStateChanged(QMediaPlayer::PlaybackState state)
{
    switch (state) {
        case QMediaPlayer::PlayingState:
            isPlaying = true;
            break;
        case QMediaPlayer::PausedState:
        case QMediaPlayer::StoppedState:
            isPlaying = false;
            break;
    }
    emit playingStateChanged();
}

void MusicController::onPlaybackEnded()
{
    if (isRepeat) {
        playTrack(currentTrackIndex);
    } else if (isShuffle) {
        currentTrackIndex = QRandomGenerator::global()->bounded(playlist.size());
        playTrack(currentTrackIndex);
        emit currentTrackIndexChanged();
    } else {
        if (currentTrackIndex < playlist.size() - 1) {
            next();
        } else {
            stop();
        }
    }
}

// ===== PRIVATE FUNCTIONS =====

void MusicController::updateTrackInfo()
{
    emit currentTitleChanged();
    emit currentArtistChanged();
    emit currentAlbumChanged();
}

void MusicController::playTrack(int index)
{
    if (index >= 0 && index < playlist.size()) {
        currentTrackIndex = index;
        const ExtendedSong &song = playlist[index];
        
        if (!song.filePath.empty()) {
            QString filePath = QString::fromStdString(song.filePath);
            player->setSource(QUrl::fromLocalFile(filePath));
            player->play();
            isPlaying = true;
        }
        
        updateTrackInfo();
        emit currentTrackIndexChanged();
        emit playingStateChanged();
    }
}

void MusicController::loadDemoPlaylist()
{
    playlist.clear();
    
    ExtendedSong song1;
    song1.id = 1;
    song1.title = "Summer Nights";
    song1.artist = "The Waves";
    song1.album = "Sunset Drive";
    song1.duration = 210;
    song1.filePath = "";
    playlist.append(song1);
    musicLibrary.addSong(song1);
    
    ExtendedSong song2;
    song2.id = 2;
    song2.title = "Highway Dreams";
    song2.artist = "Road Kings";
    song2.album = "Journey";
    song2.duration = 195;
    song2.filePath = "";
    playlist.append(song2);
    musicLibrary.addSong(song2);
    
    emit playlistChanged();
}

QString MusicController::formatTime(qint64 milliseconds) const
{
    qint64 seconds = milliseconds / 1000;
    int minutes = seconds / 60;
    int secs = seconds % 60;
    return QString("%1:%2").arg(minutes).arg(secs, 2, 10, QChar('0'));
}
