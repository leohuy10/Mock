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
    
    // Load demo playlist
    loadDemoPlaylist();
    
    // Khởi tạo bài đầu tiên (không thêm vào queue)
    if (!playlist.isEmpty()) {
        currentPlayingSong = playlist[0];
        currentTrackIndex = 0;
    }
    
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
    if (playlist.isEmpty()) {
        qDebug() << "⚠️ playPause: playlist is empty!";
        return;
    }
    
    if (isPlaying) {
        player->pause();
        isPlaying = false;
        qDebug() << "⏸ Paused";
        emit playingStateChanged();
    } else {
        // Đảm bảo index hợp lệ
        if (currentTrackIndex < 0 || currentTrackIndex >= playlist.size()) {
            currentTrackIndex = 0;
        }
        
        const ExtendedSong &currentSong = playlist[currentTrackIndex];
        currentPlayingSong = currentSong;
        
        if (!currentSong.filePath.empty()) {
            playSongFile(QString::fromStdString(currentSong.filePath));
        } else {
            qDebug() << "⚠️ filePath is empty for:" << QString::fromStdString(currentSong.title);
        }
        
        emit currentTrackIndexChanged();
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
    
    // ===== ƯU TIÊN 1: HÀNG CHỜ (Queue) =====
    if (!manualQueue.isEmpty()) {
        ExtendedSong nextSong = manualQueue.takeFirst();
        qDebug() << "Playing from Queue:" << QString::fromStdString(nextSong.title);
        
        // Tìm bài trong playlist hiện tại
        for (int i = 0; i < playlist.size(); ++i) {
            if (playlist[i].id == nextSong.id) {
                currentTrackIndex = i;
                currentPlayingSong = playlist[i];
                
                // PHÁT NHẠC
                player->stop();
                player->setSource(QUrl::fromLocalFile(QString::fromStdString(currentPlayingSong.filePath)));
                player->play();
                isPlaying = true;
                
                updateTrackInfo();
                emit currentTrackIndexChanged();
                emit playingStateChanged();
                
                qDebug() << "Now playing index:" << i;
                
                if (currentMode == ShowQueue) {
                    search("");
                }
                return;
            }
        }
        qDebug() << "Song not found in playlist!";
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
            if (isRepeat) {
                newIndex = 0;
            } else {
                stop();
                return;
            }
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
    
    if (playlist.isEmpty()) {
        qDebug() << "Playlist is empty!";
        return;
    }
    
    // Tính index mới
    int newIndex = currentTrackIndex - 1;
    if (newIndex < 0) {
        newIndex = playlist.size() - 1;
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
        ExtendedSong selectedSong = playlist[index];
        
        // Xóa bài này khỏi queue nếu đã có (tránh duplicate)
        for (int i = 0; i < manualQueue.size(); ++i) {
            if (manualQueue[i].id == selectedSong.id) {
                manualQueue.removeAt(i);
                break;
            }
        }
        
        qDebug() << "▶ Selected:" << QString::fromStdString(selectedSong.title);
        
        currentTrackIndex = index;
        currentPlayingSong = selectedSong;
        
        // Phát bằng helper
        if (!selectedSong.filePath.empty()) {
            playSongFile(QString::fromStdString(selectedSong.filePath));
        }
        
        emit currentTrackIndexChanged();
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
    // ===== ƯU TIÊN 1: HÀNG CHỜ TẠM THỜI (User Queue) =====
    if (!manualQueue.isEmpty()) {
        ExtendedSong nextInQueue = manualQueue.takeFirst();
        
        qDebug() << "▶ Auto Next from Queue:" << QString::fromStdString(nextInQueue.title) 
                 << "| Queue còn:" << manualQueue.size();
        
        currentPlayingSong = nextInQueue;
        
        if (!nextInQueue.filePath.empty()) {
            playSongFile(QString::fromStdString(nextInQueue.filePath));
        }
        
        // Đồng bộ UI
        for (int i = 0; i < playlist.size(); ++i) {
            if (playlist[i].id == nextInQueue.id) {
                currentTrackIndex = i;
                break;
            }
        }
        emit currentTrackIndexChanged();
        
        if (currentMode == ShowQueue) {
            search("");
        }
        return;
    }

    // ===== ƯU TIÊN 2: DANH SÁCH NGUỒN (musicLibrary) =====
    size_t librarySize = musicLibrary.size();
    if (librarySize == 0) {
        stop();
        return;
    }
    
    // Tìm vị trí hiện tại trong library
    int currentLibraryIndex = -1;
    for (size_t i = 0; i < librarySize; ++i) {
        const Song* s = musicLibrary.getSongByIndex(i);
        if (s && s->id == currentPlayingSong.id) {
            currentLibraryIndex = static_cast<int>(i);
            break;
        }
    }
    
    if (isRepeat) {
        // Phát lại bài hiện tại
        if (!currentPlayingSong.filePath.empty()) {
            playSongFile(QString::fromStdString(currentPlayingSong.filePath));
        }
    } else if (isShuffle) {
        int newIndex = currentLibraryIndex;
        do {
            newIndex = QRandomGenerator::global()->bounded(static_cast<int>(librarySize));
        } while (newIndex == currentLibraryIndex && librarySize > 1);
        
        const Song* nextSong = musicLibrary.getSongByIndex(newIndex);
        if (nextSong) {
            currentPlayingSong.id = nextSong->id;
            currentPlayingSong.title = nextSong->title;
            currentPlayingSong.artist = nextSong->artist;
            currentPlayingSong.album = nextSong->album;
            currentPlayingSong.duration = nextSong->duration;
            currentPlayingSong.filePath = nextSong->filePath;
            
            if (!currentPlayingSong.filePath.empty()) {
                playSongFile(QString::fromStdString(currentPlayingSong.filePath));
            }
            
            for (int i = 0; i < playlist.size(); ++i) {
                if (playlist[i].id == currentPlayingSong.id) {
                    currentTrackIndex = i;
                    break;
                }
            }
            emit currentTrackIndexChanged();
        }
    } else {
        // Phát tuần tự
        int newIndex = currentLibraryIndex + 1;
        if (newIndex < static_cast<int>(librarySize)) {
            const Song* nextSong = musicLibrary.getSongByIndex(newIndex);
            if (nextSong) {
                currentPlayingSong.id = nextSong->id;
                currentPlayingSong.title = nextSong->title;
                currentPlayingSong.artist = nextSong->artist;
                currentPlayingSong.album = nextSong->album;
                currentPlayingSong.duration = nextSong->duration;
                currentPlayingSong.filePath = nextSong->filePath;
                
                if (!currentPlayingSong.filePath.empty()) {
                    playSongFile(QString::fromStdString(currentPlayingSong.filePath));
                }
                
                for (int i = 0; i < playlist.size(); ++i) {
                    if (playlist[i].id == currentPlayingSong.id) {
                        currentTrackIndex = i;
                        break;
                    }
                }
                emit currentTrackIndexChanged();
            }
        } else {
            stop();  // Hết playlist
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
    if (index >= 0 && index < playlist.size()) {
        currentTrackIndex = index;
        const ExtendedSong &song = playlist[index];
        
        // Lưu bài đang phát
        currentPlayingSong = song;
        
        if (!song.filePath.empty()) {
            QString filePath = QString::fromStdString(song.filePath);
            playSongFile(filePath);
        }
        
        emit currentTrackIndexChanged();
    }
}


// Clears existing data, registers 10 demo tracks, and triggers a UI refresh signal.
void MusicController::loadDemoPlaylist()
{
    playlist.clear();
    // musicLibrary.clear(); // Bỏ comment nếu bạn đã thêm hàm clear() vào MusicLibrary

    std::string musicFolderPath = "C:/Users/Asus/Downloads/Playlist"; 

    try {
        if (!fs::exists(musicFolderPath)) return;

        int idCounter = 1;
        for (const auto& entry : fs::directory_iterator(musicFolderPath)) {
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
                        song.artist = "Unknown Artist";
                    }

                    playlist.append(song);
                    musicLibrary.addSong(song);
                }
            }
        }
    } catch (const std::exception& e) {
        qDebug() << "Lỗi quét thư mục:" << e.what();
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
    // 1. Kiểm tra xem bài hát đã tồn tại trong manualQueue chưa
    for (const auto &song : manualQueue) {
        if (song.id == songId) {
            qDebug() << "Bài hát ID:" << songId << " đã tồn tại trong hàng chờ.";
            return; // Thoát hàm, không thêm nữa
        }
    }

    // 2. Tìm bài hát trong Library bằng ID
    Song* s = musicLibrary.findSongByID(songId);
    
    if (s) {
        ExtendedSong ex;
        ex.id = s->id;
        ex.title = s->title;
        ex.artist = s->artist;
        ex.album = s->album;
        ex.duration = s->duration;
        ex.filePath = s->filePath;  // Copy đường dẫn file

        // 3. Thêm vào danh sách chờ thủ công
        manualQueue.append(ex);

        // 4. Đồng bộ với class PlaybackQueue (nếu bạn đang dùng nó để quản lý logic lõi)
        playbackQueue.addSong(ex);

        // 5. Cập nhật giao diện nếu đang ở tab "Hàng chờ"
        if (currentMode == ShowQueue) {
            search(""); 
        }
        
        qDebug() << "Đã thêm mới vào hàng chờ ID:" << songId << "| Tổng cộng:" << manualQueue.size();
    } else {
        qDebug() << "Không tìm thấy bài hát trong Library với ID:" << songId;
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
    if (currentMode == ShowQueue) {
        if (index >= 0 && index < manualQueue.size()) {
            manualQueue.removeAt(index); // Xóa khỏi danh sách chờ thực tế
            
            // Cập nhật lại danh sách hiển thị (playlist)
            search(""); 
            
            qDebug() << "Removed song at index:" << index << " Remaining in queue:" << manualQueue.size();
        }
    }
}

// Trong MusicController.cpp
bool MusicController::isSongInQueue(int songId) const {
    for (const auto &song : manualQueue) {
        if (song.id == songId) return true;
    }
    return false;
}