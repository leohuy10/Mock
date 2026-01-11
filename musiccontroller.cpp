#include "musiccontroller.h"
#include <QFileInfo>
#include <QDir>
#include <QRandomGenerator>
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
    // --- ƯU TIÊN CAO NHẤT: HÀNG CHỜ THỦ CÔNG (Queue) ---
    if (!manualQueue.isEmpty()) {
        // Lấy bài hát đầu tiên trong hàng chờ ra để phát
        ExtendedSong nextInQueue = manualQueue.takeFirst();
        
        // Tìm vị trí bài hát này trong playlist hiện tại để đồng bộ UI (nếu có)
        bool found = false;
        for (int i = 0; i < playlist.size(); ++i) {
            if (playlist[i].id == nextInQueue.id) {
                currentTrackIndex = i;
                found = true;
                break;
            }
        }
        
        // Phát bài hát từ hàng chờ
        playTrack(currentTrackIndex);
        emit currentTrackIndexChanged();

        // Nếu người dùng đang nhìn tab Hàng chờ, cập nhật giao diện để xóa bài vừa phát
        if (currentMode == ShowQueue) {
            search(""); 
        }
        return; // Kết thúc hàm tại đây, không chạy xuống logic Repeat/Shuffle phía dưới
    }

    // --- LOGIC MẶC ĐỊNH (Chỉ chạy khi Queue trống) ---
    if (isRepeat) {
        playTrack(currentTrackIndex);
    } else if (isShuffle) {
        if (playlist.size() > 0) {
            currentTrackIndex = QRandomGenerator::global()->bounded(playlist.size());
            playTrack(currentTrackIndex);
            emit currentTrackIndexChanged();
        }
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
        // FIX: Không lọc theo 'text'. Luôn hiện đầy đủ những gì có trong manualQueue.
        for (const auto &song : manualQueue) {
            playlist.append(song);
        }
    } else {
        // --- CHẾ ĐỘ TẤT CẢ (LIBRARY) ---
        // Chỉ tìm kiếm (lọc) khi đang ở chế độ này
        for (size_t i = 0; i < musicLibrary.size(); ++i) {
            const Song* s = musicLibrary.getSongByIndex(i);
            if (!s) continue;

            QString title = QString::fromStdString(s->title).toLower();
            QString artist = QString::fromStdString(s->artist).toLower();

            // Thực hiện lọc theo text
            if (text.isEmpty() || title.contains(lowerQuery) || artist.contains(lowerQuery)) {
                ExtendedSong exSong;
                exSong.id = s->id;
                exSong.title = s->title;
                exSong.artist = s->artist;
                exSong.album = s->album;
                exSong.duration = s->duration;
                exSong.filePath = s->filePath;  // Copy đường dẫn file
                playlist.append(exSong);
            }
        }
    }

    emit playlistChanged(); // Cập nhật ListView trong QML
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