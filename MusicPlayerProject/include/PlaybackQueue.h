#ifndef PLAYBACK_QUEUE_H
#define PLAYBACK_QUEUE_H

#include "Song.h"
#include <list>

//  Lớp quản lý danh phát phát
class PlaybackQueue
{
private:
    // Lưu trữ danh sách phát
    std::list<Song> queue;
    // Con trỏ đến bài hát hiện tại
    std::list<Song>::iterator current;

public:
    // Constructor
    PlaybackQueue();
    // Thêm bài hát vào danh sách phát
    void addSong(const Song &song);
    // Xoá bài hát khởi danh sách phát
    void removeSong(int songID);
    // Lấy ra bài hát hiện tại
    Song getCurrentSong() const;
    // Hàm chọn bài hát kế tiếp
    void playNext();
};

#endif
