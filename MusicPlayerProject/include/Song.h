#ifndef SONG_H
#define SONG_H

#include <string>

// Lớp Song để lưu trữ thông tin bài hát
struct Song
{
    int id;
    std::string title;
    std::string artist;
    std::string album;
    int duration;
    std::string filePath;  // Đường dẫn file nhạc
};

#endif
