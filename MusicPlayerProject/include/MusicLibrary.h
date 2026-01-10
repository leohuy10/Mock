#ifndef MUSIC_LIBRARY_H
#define MUSIC_LIBRARY_H

#include "Song.h"
#include <vector>
#include <map>
#include <unordered_map>
#include <string>

// Forward declaration
class PlaybackQueue;

// Lớp quản lý danh sách bài hát
class MusicLibrary
{
private:
    // Lưu trữ playlist quản lý bài hát
    std::vector<Song> songs;
    // Lưu các bài hát theo key : value
    std::unordered_map<int, Song *> songIndexByID;
    // Lưu các bài hát theo tên (tự sắp xếp theo thứ tự)
    std::map<std::string, Song *> songIndexByTitle;
    // Lưu nghệ sĩ : các bài hát của nghệ sĩ đó
    std::unordered_map<std::string, std::vector<Song *>> artistIndex;

public:
    // Constructor
    MusicLibrary();

    // Thêm bài hát vào thư viện
    void addSong(const Song &song);

    // Tìm bài hát theo id
    Song *findSongByID(int id) const;

    // Tìm bài hát theo tên
    Song *findSongByTitle(const std::string &title) const;

    void addAlbumToQueue(const std::string &albumName, const MusicLibrary &library, PlaybackQueue &queue);

    // Tìm các bài hát theo ca sỹ cụ thể
    std::vector<Song *> findSongByArtist(const std::string &artist) const;

    // Tìm bài hát theo index
    const Song *getSongByIndex(size_t index) const;

    // Hàm trả về số lượng bài hát
    size_t size() const;
};

#endif
