#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#include "MusicLibrary.h"
#include "PlaybackQueue.h"
#include "PlaybackHistory.h"

class MusicPlayer
{
private:
    MusicLibrary library;
    PlaybackQueue playbackQueue;
    PlaybackHistory history;
    Song *currentSong = nullptr;

public:
    // Xem bài hát hiện tại
    const Song* getCurrentSong() const;
    // Chọn và phát bài hát
    void selectAndPlaySong(int songID);
    // Phát bài hát kế tiếp
    void playNext();
   
};

#endif
