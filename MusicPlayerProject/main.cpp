#include <iostream>
#include "MusicLibrary.h"
#include "MusicPlayer.h"
#include "SmartPlaylist.h"

int main()
{
    // Thêm bài hát vào thư viện
    MusicLibrary library;
    library.addSong({1, "Perfect", "Ed Sheeran", "Divide", 263});
    library.addSong({2, "Blank Space", "Taylor Swift", "1989", 219});
    library.addSong({3, "Yesterday", "The Beatles", "Help!", 169});
    library.addSong({4, "Hello", "Adele", "25", 223});
    library.addSong({5, "Rolling in the Deep", "Adele", "21", 285});
    library.addSong({6, "Someone Like You", "Adele", "21", 276});
    library.addSong({7, "Shape of You", "Ed Sheeran", "Divide", 233});
    library.addSong({8, "Castle on the Hill", "Ed Sheeran", "Divide", 272});
    library.addSong({9, "Photograph", "Ed Sheeran", "Divide", 243});
    library.addSong({10, "Creep", "Radiohead", "OK Computer", 301});
    
    std::cout << "\n TEST 1: Find by ID " << std::endl;
    Song* song = library.findSongByID(1);
    if (song) std::cout << "Found: " << song->title << std::endl;

    std::cout << "\n TEST 2: Find by Artist " << std::endl;
    auto songs = library.findSongByArtist("Adele");
    std::cout << "Adele songs: " << songs.size() << std::endl;
    for (auto* s : songs) {
        std::cout << "  - " << s->title << std::endl;
    }

    std::cout << "\n TEST 3: PlaybackQueue " << std::endl;
    PlaybackQueue queue;
    queue.addSong(*library.findSongByID(1));
    queue.addSong(*library.findSongByID(2));
    queue.addSong(*library.findSongByID(3));
    
    std::cout << "Current: " << queue.getCurrentSong().title << std::endl;
    queue.playNext();
    std::cout << "After next: " << queue.getCurrentSong().title << std::endl;
    queue.playNext();
    std::cout << "After next: " << queue.getCurrentSong().title << std::endl;

    std::cout << "\n TEST 4: Remove from Queue " << std::endl;
    queue.removeSong(2);
    std::cout << "After remove: " << queue.getCurrentSong().title << std::endl;

    std::cout << "\n TEST 5: Circular Queue " << std::endl;
    for (int i = 0; i < 5; i++) {
        std::cout << "Step " << i << ": " << queue.getCurrentSong().title << std::endl;
        queue.playNext();
    }

    std::cout << "\n TEST 6: PlaybackHistory " << std::endl;
    PlaybackHistory history;
    history.pushSong(*library.findSongByID(1));
    history.pushSong(*library.findSongByID(2));
    history.pushSong(*library.findSongByID(3));
    
    while (!history.isEmpty()) {
        Song prev = history.playPreviousSong();
        std::cout << "Previous: " << prev.title << std::endl;
    }

    std::cout << "\n TEST 7: Smart Playlist " << std::endl;
    Song* start = library.findSongByID(4);
    PlaybackQueue smart = generateSmartPlaylist(*start, library, 4);
    for (int i = 0; i < 4; i++) {
        try {
            Song curr = smart.getCurrentSong();
            std::cout << i+1 << ". " << curr.title << " - " << curr.artist << std::endl;
            smart.playNext();
        } catch (...) { break; }
    }

    std::cout << "\n TEST 8: Add Album to Queue " << std::endl;
    PlaybackQueue albumQueue;
    library.addAlbumToQueue("Divide", library, albumQueue);
    std::cout << "Current: " << albumQueue.getCurrentSong().title << std::endl;

    std::cout << "\n TEST 9: Edge Cases " << std::endl;
    // Not found
    Song* notFound = library.findSongByID(999);
    std::cout << "Find 999: " << (notFound ? "Found" : "Not found") << std::endl;
    
    // Không tìm thấy nghệ sỹ
    auto empty = library.findSongByArtist("Unknown");
    std::cout << "Unknown artist: " << empty.size() << " songs" << std::endl;
    
    // Lỗi khi queue rỗng
    PlaybackQueue emptyQ;
    try {
        emptyQ.getCurrentSong();
    } catch (const std::runtime_error& e) {
        std::cout << "Empty queue error: " << e.what() << std::endl;
    }

    std::cout << "\nAll tests completed!" << std::endl;
    return 0;
}
