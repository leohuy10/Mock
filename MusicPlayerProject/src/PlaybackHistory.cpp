#include "PlaybackHistory.h"
#include <stdexcept>

void PlaybackHistory::pushSong(const Song &song)
{
    // Thêm bài hát vào stack
    history.push(song);
}

Song PlaybackHistory::playPreviousSong()
{
    if (history.empty())
        throw std::runtime_error("No previous song");
    // Lấy ra phần từ đầu danh sách
    Song prev = history.top();
    history.pop();
    return prev;
}

bool PlaybackHistory::isEmpty() const
{
    // Check empty
    return history.empty();
}
