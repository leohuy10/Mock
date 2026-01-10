#include "MusicLibrary.h"
#include "PlaybackQueue.h"

MusicLibrary::MusicLibrary()
{
    // Đảm bảo vector lưu trữ được 50000 phần tử Song, không bị thay đổi địa chỉ khi thêm/xoá
    songs.reserve(50000);
}

void MusicLibrary::addSong(const Song &song)
{
    songs.push_back(song);
    // SongPtr trỏ đến cuối cùng danh sách bài hát
    Song *songPtr = &songs.back();
    // Thêm key : value mới trong unorder_map
    songIndexByID[songPtr->id] = songPtr;
    songIndexByTitle[songPtr->title] = songPtr;
    // Push song vào danh sách bài hát của ca sỹ tương ứng
    artistIndex[songPtr->artist].push_back(songPtr);
}

void MusicLibrary::addAlbumToQueue(const std::string &albumName, const MusicLibrary &library, PlaybackQueue &queue)
{
    for (size_t i = 0; i < library.size(); i++) {
        const Song* song = library.getSongByIndex(i);
        if (song->album == albumName) {
            queue.addSong(*song);
        }
    }
}

Song *MusicLibrary::findSongByID(int id) const
{
    auto it = songIndexByID.find(id);
    if (it != songIndexByID.end())
    {
        return it->second;
    }
    return nullptr;
}



std::vector<Song *> MusicLibrary::findSongByArtist(const std::string &artist) const
{
    auto it = artistIndex.find(artist);
    if (it != artistIndex.end())
    {
        return it->second;
    }
    // Trả về Mảng rỗng
    return {};
}

const Song *MusicLibrary::getSongByIndex(size_t index) const
{
    if (index >= songs.size())
        return nullptr;
    return &songs[index];
};

size_t MusicLibrary::size() const
{
    return songs.size();
};