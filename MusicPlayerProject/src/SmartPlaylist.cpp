#include "SmartPLaylist.h"

#include <queue>
#include <set>

PlaybackQueue generateSmartPlaylist(const Song& startSong,
                                    const MusicLibrary& library, int maxSize) {
    PlaybackQueue result;
    std::queue<const Song*> bfsQueue;
    std::set<int> visited;

    bfsQueue.push(&startSong);
    visited.insert(startSong.id);
    result.addSong(startSong);

    while (!bfsQueue.empty() && visited.size() < static_cast<size_t>(maxSize)) {
        const Song* current = bfsQueue.front();
        bfsQueue.pop();

        auto sameArtistSongs = library.findSongByArtist(current->artist);
        for (Song* s : sameArtistSongs) {
            if (visited.insert(s->id).second) {
                result.addSong(*s);
                bfsQueue.push(s);
                if (visited.size() >= static_cast<size_t>(maxSize)) break;
            }
        }
    }
    return result;
}
