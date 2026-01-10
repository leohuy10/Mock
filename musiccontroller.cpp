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


// Clears existing data, registers 10 demo tracks, and triggers a UI refresh signal.
void MusicController::loadDemoPlaylist()
{
    playlist.clear();
    
    // Bài 1
    ExtendedSong song1;
    song1.id = 1;
    song1.title = "Summer Nights";
    song1.artist = "The Waves";
    song1.album = "Sunset Drive";
    song1.duration = 210;
    song1.filePath = "";
    playlist.append(song1);
    musicLibrary.addSong(song1);
    
    // Bài 2
    ExtendedSong song2;
    song2.id = 2;
    song2.title = "Highway Dreams";
    song2.artist = "Road Kings";
    song2.album = "Journey";
    song2.duration = 195;
    song2.filePath = "";
    playlist.append(song2);
    musicLibrary.addSong(song2);
   
    // Bài 3
    ExtendedSong song3;
    song3.id = 3;
    song3.title = "Neon Lights";
    song3.artist = "Synth City";
    song3.album = "Cyberpunk 2077";
    song3.duration = 245;
    song3.filePath = "";
    playlist.append(song3);
    musicLibrary.addSong(song3);

    // Bài 4
    ExtendedSong song4;
    song4.id = 4;
    song4.title = "Midnight City";
    song4.artist = "M83";
    song4.album = "Hurry Up";
    song4.duration = 243;
    song4.filePath = "";
    playlist.append(song4);
    musicLibrary.addSong(song4);

    // Bài 5
    ExtendedSong song5;
    song5.id = 5;
    song5.title = "Blinding Lights";
    song5.artist = "The Weeknd";
    song5.album = "After Hours";
    song5.duration = 200;
    song5.filePath = "";
    playlist.append(song5);
    musicLibrary.addSong(song5);

    // Bài 6
    ExtendedSong song6;
    song6.id = 6;
    song6.title = "Levitating";
    song6.artist = "Dua Lipa";
    song6.album = "Future Nostalgia";
    song6.duration = 203;
    song6.filePath = "";
    playlist.append(song6);
    musicLibrary.addSong(song6);

    // Bài 7
    ExtendedSong song7;
    song7.id = 7;
    song7.title = "Shape of You";
    song7.artist = "Ed Sheeran";
    song7.album = "Divide";
    song7.duration = 233;
    song7.filePath = "";
    playlist.append(song7);
    musicLibrary.addSong(song7);

    // Bài 8
    ExtendedSong song8;
    song8.id = 8;
    song8.title = "Heat Waves";
    song8.artist = "Glass Animals";
    song8.album = "Dreamland";
    song8.duration = 238;
    song8.filePath = "";
    playlist.append(song8);
    musicLibrary.addSong(song8);

    // Bài 9
    ExtendedSong song9;
    song9.id = 9;
    song9.title = "Cold Heart";
    song9.artist = "Elton John";
    song9.album = "The Lockdown";
    song9.duration = 202;
    song9.filePath = "";
    playlist.append(song9);
    musicLibrary.addSong(song9);

    // Bài 10
    ExtendedSong song10;
    song10.id = 10;
    song10.title = "Save Your Tears";
    song10.artist = "The Weeknd";
    song10.album = "After Hours";
    song10.duration = 215;
    song10.filePath = "";
    playlist.append(song10);
    musicLibrary.addSong(song10);

    emit playlistChanged();
}

QString MusicController::formatTime(qint64 milliseconds) const
{
    qint64 seconds = milliseconds / 1000;
    int minutes = seconds / 60;
    int secs = seconds % 60;
    return QString("%1:%2").arg(minutes).arg(secs, 2, 10, QChar('0'));
}
