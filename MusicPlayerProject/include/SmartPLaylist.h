#ifndef SMART_PLAYLIST_H
#define SMART_PLAYLIST_H

#include "MusicLibrary.h"
#include "PlaybackQueue.h"

PlaybackQueue generateSmartPlaylist(
    const Song &startSong,
    const MusicLibrary &library,
    int maxSize);

#endif
