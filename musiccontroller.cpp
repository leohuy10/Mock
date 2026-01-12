#include "musiccontroller.h"
#include <QFileInfo>
#include <QDir>
#include <QRandomGenerator>
#include <QCoreApplication>
#include <QEventLoop>
#include <filesystem> // Thêm thư viện này ở đầu file
namespace fs = std::filesystem;

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
    
    // Set initial volume
    audioOutput->setVolume(currentVolume / 100.0f);
}

MusicController::~MusicController()
{
}

// ===== GETTER PROPERTIES =====

QString MusicController::getCurrentTitle() const
{
    if (!currentPlayingSong.title.empty()) {
        return QString::fromStdString(currentPlayingSong.title);
    }
    return "No Track Playing";
}

QString MusicController::getCurrentArtist() const
{
    if (!currentPlayingSong.artist.empty()) {
        return QString::fromStdString(currentPlayingSong.artist);
    }
    return "Select a song to start";
}

QString MusicController::getCurrentAlbum() const
{
    if (!currentPlayingSong.album.empty()) {
        return QString::fromStdString(currentPlayingSong.album);
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
        QString displayText = QString("%1")
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

int MusicController::getQueueCount() const
{
    return manualQueue.size();
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
    if (playlist.isEmpty()) {
        qDebug() << "playPause: playlist is empty!";
        return;
    }
    
    if (isPlaying) {
        // Đang phát → Pause
        player->pause();
        isPlaying = false;
        qDebug() << "Paused";
        emit playingStateChanged();
    } else {
        // Đang pause/stop → Play
        if (currentTrackIndex < 0 || currentTrackIndex >= playlist.size()) {
            currentTrackIndex = 0;
        }
        
        currentPlayingSong = playlist[currentTrackIndex];
        
        // Nếu player đang pause (có source) thì chỉ cần play()
        // Nếu player đang stop (source rỗng) thì phải setSource lại
        if (player->playbackState() == QMediaPlayer::PausedState) {
            player->play();
        } else {
            player->stop();
            player->setSource(QUrl::fromLocalFile(QString::fromStdString(currentPlayingSong.filePath)));
            player->play();
        }
        
        isPlaying = true;
        qDebug() << "Playing:" << QString::fromStdString(currentPlayingSong.title);
        
        updateTrackInfo();
        emit currentTrackIndexChanged();
        emit playingStateChanged();
    }
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
    qDebug() << ">>> NEXT button pressed";
    
    // Thêm bài hát hiện tại vào history
    if (!currentPlayingSong.title.empty()) {
        playbackHistory.pushSong(currentPlayingSong);
    }
    
    // ===== ƯU TIÊN 0: REPEAT (Phát lại) =====
    if (isRepeat) {
        qDebug() << "Repeat mode: replaying current song";
        player->stop();
        player->setSource(QUrl::fromLocalFile(QString::fromStdString(currentPlayingSong.filePath)));
        player->play();
        isPlaying = true;
        emit playingStateChanged();
        return;
    }
    
    // ===== ƯU TIÊN 1: HÀNG CHỜ (Queue) =====
    if (!manualQueue.isEmpty()) {
        ExtendedSong nextSong = manualQueue.takeFirst();
        emit queueCountChanged();  // Cập nhật số lượng queue
        qDebug() << "Playing from Queue:" << QString::fromStdString(nextSong.title);
        
        // Phát trực tiếp từ queue (không cần tìm trong playlist)
        currentPlayingSong = nextSong;
        
        player->stop();
        player->setSource(QUrl::fromLocalFile(QString::fromStdString(nextSong.filePath)));
        player->play();
        isPlaying = true;
        
        // Cập nhật currentTrackIndex nếu bài này có trong playlist hiện tại
        for (int i = 0; i < playlist.size(); ++i) {
            if (playlist[i].id == nextSong.id) {
                currentTrackIndex = i;
                break;
            }
        }
        
        updateTrackInfo();
        emit currentTrackIndexChanged();
        emit playingStateChanged();
        
        if (currentMode == ShowQueue) {
            search("");
        }
        return;
    }
    
    // ===== ƯU TIÊN 2: PLAYLIST (My Songs) =====
    if (playlist.isEmpty()) {
        qDebug() << "Playlist is empty!";
        return;
    }
    
    // Tính index mới
    int newIndex = currentTrackIndex;
    if (isShuffle) {
        do {
            newIndex = QRandomGenerator::global()->bounded(playlist.size());
        } while (newIndex == currentTrackIndex && playlist.size() > 1);
    } else {
        newIndex = currentTrackIndex + 1;
        if (newIndex >= playlist.size()) {
            // Quay về đầu playlist (loop)
            newIndex = 0;
        }
    }
    
    qDebug() << "Playing from Playlist, index:" << currentTrackIndex << "->" << newIndex;
    
    // Cập nhật và phát
    currentTrackIndex = newIndex;
    currentPlayingSong = playlist[currentTrackIndex];
    
    // PHÁT NHẠC
    player->stop();
    player->setSource(QUrl::fromLocalFile(QString::fromStdString(currentPlayingSong.filePath)));
    player->play();
    isPlaying = true;
    
    updateTrackInfo();
    emit currentTrackIndexChanged();
    emit playingStateChanged();
    
    qDebug() << "Now playing:" << QString::fromStdString(currentPlayingSong.title);
}

void MusicController::previous()
{
    qDebug() << ">>> PREVIOUS button pressed";
    
    // Kiểm tra xem có bài trong history không
    if (playbackHistory.isEmpty()) {
        qDebug() << "No previous songs in history!";
        return;
    }
    
    try {
        // Lấy bài hát từ history (LIFO - bài vừa phát gần đây nhất)
        Song prevSong = playbackHistory.playPreviousSong();
        
        qDebug() << "Playing from History:" << QString::fromStdString(prevSong.title);
        
        // Cập nhật bài hát hiện tại
        currentPlayingSong.id = prevSong.id;
        currentPlayingSong.title = prevSong.title;
        currentPlayingSong.artist = prevSong.artist;
        currentPlayingSong.album = prevSong.album;
        currentPlayingSong.duration = prevSong.duration;
        currentPlayingSong.filePath = prevSong.filePath;
        
        // Tìm index trong playlist để cập nhật currentTrackIndex
        for (int i = 0; i < playlist.size(); ++i) {
            if (playlist[i].id == prevSong.id) {
                currentTrackIndex = i;
                break;
            }
        }
        
        // PHÁT NHẠC
        player->stop();
        player->setSource(QUrl::fromLocalFile(QString::fromStdString(currentPlayingSong.filePath)));
        player->play();
        isPlaying = true;
        
        updateTrackInfo();
        emit currentTrackIndexChanged();
        emit playingStateChanged();
        
        qDebug() << "Now playing:" << QString::fromStdString(currentPlayingSong.title);
    } catch (const std::exception &e) {
        qDebug() << "Error in previous():" << e.what();
    }
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
    qDebug() << ">>> selectTrack called, index:" << index;
    
    if (index < 0 || index >= playlist.size()) {
        qDebug() << "Invalid index!";
        return;
    }
    
    // Thêm bài hát hiện tại vào history trước khi chuyển
    if (!currentPlayingSong.title.empty()) {
        playbackHistory.pushSong(currentPlayingSong);
    }
    
    // Xóa bài này khỏi queue nếu đã có (tránh duplicate)
    for (int i = 0; i < manualQueue.size(); ++i) {
        if (manualQueue[i].id == playlist[index].id) {
            manualQueue.removeAt(i);
            emit queueCountChanged();
            break;
        }
    }
    
    // Cập nhật index và bài đang phát
    currentTrackIndex = index;
    currentPlayingSong = playlist[index];
    
    qDebug() << "Playing:" << QString::fromStdString(currentPlayingSong.title);
    
    // PHÁT NHẠC (cùng logic với next/previous)
    player->stop();
    player->setSource(QUrl::fromLocalFile(QString::fromStdString(currentPlayingSong.filePath)));
    player->play();
    isPlaying = true;
    
    updateTrackInfo();
    emit currentTrackIndexChanged();
    emit playingStateChanged();
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
    qDebug() << ">>> Song ended, auto next";
    
    // ===== ƯU TIÊN 1: HÀNG CHỜ (Queue) =====
    if (!manualQueue.isEmpty()) {
        ExtendedSong nextSong = manualQueue.takeFirst();
        emit queueCountChanged();  // Cập nhật số lượng queue
        qDebug() << "Auto playing from Queue:" << QString::fromStdString(nextSong.title);
        
        // Phát trực tiếp từ queue
        currentPlayingSong = nextSong;
        
        player->stop();
        player->setSource(QUrl::fromLocalFile(QString::fromStdString(nextSong.filePath)));
        player->play();
        isPlaying = true;
        
        // Cập nhật currentTrackIndex nếu có trong playlist
        for (int i = 0; i < playlist.size(); ++i) {
            if (playlist[i].id == nextSong.id) {
                currentTrackIndex = i;
                break;
            }
        }
        
        updateTrackInfo();
        emit currentTrackIndexChanged();
        emit playingStateChanged();
        
        if (currentMode == ShowQueue) {
            search("");
        }
        return;
    }
    
    // ===== ƯU TIÊN 2: PLAYLIST (My Songs) =====
    if (playlist.isEmpty()) {
        stop();
        return;
    }
    
    // Repeat bài hiện tại
    if (isRepeat) {
        player->stop();
        player->setSource(QUrl::fromLocalFile(QString::fromStdString(currentPlayingSong.filePath)));
        player->play();
        isPlaying = true;
        emit playingStateChanged();
        return;
    }
    
    // Tính index mới
    int newIndex = currentTrackIndex;
    if (isShuffle) {
        do {
            newIndex = QRandomGenerator::global()->bounded(playlist.size());
        } while (newIndex == currentTrackIndex && playlist.size() > 1);
    } else {
        newIndex = currentTrackIndex + 1;
        if (newIndex >= playlist.size()) {
            // Quay về đầu playlist (loop)
            newIndex = 0;
        }
    }
    
    qDebug() << "Auto playing from Playlist, index:" << currentTrackIndex << "->" << newIndex;
    
    // Cập nhật và phát
    currentTrackIndex = newIndex;
    currentPlayingSong = playlist[currentTrackIndex];
    
    player->stop();
    player->setSource(QUrl::fromLocalFile(QString::fromStdString(currentPlayingSong.filePath)));
    player->play();
    isPlaying = true;
    
    updateTrackInfo();
    emit currentTrackIndexChanged();
    emit playingStateChanged();
}

// ===== PRIVATE FUNCTIONS =====

void MusicController::updateTrackInfo()
{
    emit currentTitleChanged();
    emit currentArtistChanged();
    emit currentAlbumChanged();
}

// Helper: Phát file nhạc từ đường dẫn
void MusicController::playSongFile(const QString &filePath)
{
    qDebug() << ">> playSongFile called";
    qDebug() << "   Path:" << filePath;
    
    if (filePath.isEmpty()) {
        qDebug() << "⚠️ ERROR: filePath is empty!";
        return;
    }
    
    // QUAN TRỌNG: Stop và reset hoàn toàn trước khi đổi source
    qDebug() << "   Stopping current playback...";
    if (player->playbackState() != QMediaPlayer::StoppedState) {
        player->stop();
    }
    
    // Reset position về 0
    player->setPosition(0);
    
    // Process events để đảm bảo stop hoàn tất
    QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
    
    qDebug() << "   Setting new source...";
    QUrl url = QUrl::fromLocalFile(filePath);
    qDebug() << "   URL:" << url.toString();
    
    // Set source mới (clear source cũ trước)
    player->setSource(QUrl());  // Clear source cũ
    QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
    
    player->setSource(url);  // Set source mới
    
    // Process events để source được load
    QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
    
    qDebug() << "   Starting playback...";
    qDebug() << "   Player state before play:" << player->playbackState();
    qDebug() << "   Media status:" << player->mediaStatus();
    
    player->play();
    isPlaying = true;
    
    // Process events để play được thực thi
    QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
    
    qDebug() << "   Player state after play:" << player->playbackState();
    qDebug() << "   ✓ Playback command sent";
    
    updateTrackInfo();
    emit playingStateChanged();
}

void MusicController::playTrack(int index)
{
    if (index < 0 || index >= playlist.size()) return;
    
    // Thêm bài hát hiện tại vào history trước khi chuyển sang bài mới
    if (!currentPlayingSong.title.empty()) {
        playbackHistory.pushSong(currentPlayingSong);
    }
    
    currentTrackIndex = index;
    currentPlayingSong = playlist[index];
    
    qDebug() << "playTrack:" << QString::fromStdString(currentPlayingSong.title);
    
    // PHÁT NHẠC
    player->stop();
    player->setSource(QUrl::fromLocalFile(QString::fromStdString(currentPlayingSong.filePath)));
    player->play();
    isPlaying = true;
    
    updateTrackInfo();
    emit currentTrackIndexChanged();
    emit playingStateChanged();
}


// Clears existing data, registers 10 demo tracks, and triggers a UI refresh signal.
void MusicController::loadMusicFolder(const QString &folderPath)
{
    playlist.clear();

    std::string musicFolderPathStr = folderPath.toStdString();

    try {
        if (!fs::exists(musicFolderPathStr)) {
            qDebug() << "Folder does not exist:" << folderPath;
            return;
        }

        int idCounter = 1;
        for (const auto& entry : fs::directory_iterator(musicFolderPathStr)) {
            if (entry.is_regular_file()) {
                const auto& path = entry.path();
                std::string ext = path.extension().string();
                std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

                if (ext == ".mp3" || ext == ".wav" || ext == ".m4a" || ext == ".flac") {
                    
                    // 1. Lấy tên file không bao gồm đuôi (ví dụ: "Nang Thuy Tinh - Khanh Ly")
                    QString fullFileName = QString::fromStdString(path.stem().string());
                    
                    // 2. Tách chuỗi dựa trên dấu gạch ngang "-"
                    QStringList parts = fullFileName.split("-");
                    
                    ExtendedSong song;
                    song.id = idCounter++;
                    song.filePath = path.string();
                    song.duration = 180; // Mặc định 3 phút
                    song.album = "Local Folder"; 

                    if (parts.size() >= 2) {
                        // Nếu file có dạng "Ten Bai Hat - Ten Tac Gia"
                        song.title = parts.at(0).trimmed().toStdString();
                        song.artist = parts.at(1).trimmed().toStdString();
                    } else {
                        // Nếu file chỉ có tên (không có dấu -)
                        song.title = fullFileName.trimmed().toStdString();
                    }

                    playlist.append(song);
                    musicLibrary.addSong(song);
                }
            }
        }
        
        // Khởi tạo bài đầu tiên nếu có nhạc
        if (!playlist.isEmpty()) {
            currentPlayingSong = playlist[0];
            currentTrackIndex = 0;
        }
    } catch (const std::exception& e) {
        qDebug() << "Error scanning folder:" << e.what();
    }

    emit playlistChanged();
}

QString MusicController::formatTime(qint64 milliseconds) const
{
    qint64 seconds = milliseconds / 1000;
    int minutes = seconds / 60;
    int secs = seconds % 60;
    return QString("%1:%2").arg(minutes).arg(secs, 2, 10, QChar('0'));
}

void MusicController::search(const QString &text) {
    // Luôn xóa danh sách hiển thị trước khi làm mới
    playlist.clear();
    QString lowerQuery = text.toLower();

    if (currentMode == ShowQueue) {
        // --- CHẾ ĐỘ HÀNG CHỜ ---
        for (const auto &song : manualQueue) {
            playlist.append(song);
        }
    } else {
        // --- CHẾ ĐỘ TẤT CẢ (LIBRARY) ---
        for (size_t i = 0; i < musicLibrary.size(); ++i) {
            const Song* s = musicLibrary.getSongByIndex(i);
            if (!s) continue;

            QString title = QString::fromStdString(s->title).toLower();
            QString artist = QString::fromStdString(s->artist).toLower();

            if (text.isEmpty() || title.contains(lowerQuery) || artist.contains(lowerQuery)) {
                ExtendedSong exSong;
                exSong.id = s->id;
                exSong.title = s->title;
                exSong.artist = s->artist;
                exSong.album = s->album;
                exSong.duration = s->duration;
                exSong.filePath = s->filePath;
                playlist.append(exSong);
            }
        }
    }

    // QUAN TRỌNG: Đồng bộ currentTrackIndex với bài đang phát
    // Tìm lại vị trí của bài đang phát trong playlist mới
    if (currentPlayingSong.id > 0) {
        for (int i = 0; i < playlist.size(); ++i) {
            if (playlist[i].id == currentPlayingSong.id) {
                currentTrackIndex = i;
                break;
            }
        }
    }

    emit playlistChanged();
    emit currentTrackIndexChanged();  // Cập nhật highlight
}

void MusicController::setFilterMode(int mode) {
    currentMode = static_cast<FilterMode>(mode);
    emit currentModeChanged(); // QUAN TRỌNG
    search(""); 
}

void MusicController::addToQueue(int songId) {
    if (isSongInQueue(songId)) return;
    Song* s = musicLibrary.findSongByID(songId);
    if(s) {
        playbackQueue.addSong(*s);

        ExtendedSong ex = *static_cast<ExtendedSong*>(s);
        manualQueue.append(ex);

        emit queueCountChanged();  // Cập nhật số lượng queue
        if (currentMode == ShowQueue) search("");
    }
}

int MusicController::getSongIdAt(int index) const {
    if (index >= 0 && index < playlist.size()) {
        return playlist[index].id;
    }
    return -1; // Không tìm thấy
}

void MusicController::removeFromQueue(int index) {
    // Chỉ xử lý nếu đang ở chế độ ShowQueue
    if (currentMode == ShowQueue && index >= 0 && index < manualQueue.size()) {
        int idToRemove = getSongIdAt(index);

        playbackQueue.removeSong(idToRemove);
        manualQueue.removeAt(index);

        emit queueCountChanged();  // Cập nhật số lượng queue
        search(""); // Làm mới danh sách hiển thị
    }
}

// Trong MusicController.cpp
bool MusicController::isSongInQueue(int songId) const {
    for (const auto &song : manualQueue) {
        if (song.id == songId) return true;
    }
    return false;
}