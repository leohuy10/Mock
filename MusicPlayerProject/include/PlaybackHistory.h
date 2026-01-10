#ifndef PLAYBACK_HISTORY_H
#define PLAYBACK_HISTORY_H

#include "Song.h"
#include <stack>

class PlaybackHistory
{
private:
    // Stack lưu danh sách những bài hát đã phát
    std::stack<Song> history;

public:
    // Thêm vào stack
    void pushSong(const Song &song);
    // Back lại bài hát trước đó
    Song playPreviousSong();
    // Check nếu stack empty
    bool isEmpty() const;
};

#endif
