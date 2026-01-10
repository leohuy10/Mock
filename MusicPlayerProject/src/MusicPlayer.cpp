#include "MusicPlayer.h"
#include <stdexcept>

const Song* MusicPlayer::getCurrentSong() const
{
    return currentSong;
}    
void MusicPlayer::selectAndPlaySong(int songID)
{
    // Tìm bài hát theo ID
    Song *song = library.findSongByID(songID);
    if (!song)
        throw std::runtime_error("Song not found");

    // Thêm bài hát vào queue
    playbackQueue.addSong(*song);
    
    // Nếu đây là bài đầu tiên trong queue, set làm current
    if (!currentSong) {
        currentSong = song;
    }
}


void MusicPlayer::playNext()
{
    
    if (currentSong)
        history.pushSong(*currentSong);
    
    playbackQueue.playNext();
    
    // Cập nhật currentSong từ queue 
    try {
        Song nextSong = playbackQueue.getCurrentSong();
        currentSong = library.findSongByID(nextSong.id);
    } catch (...) {
        currentSong = nullptr; // Chỉ set nullptr nếu queue rỗng
    }
}

