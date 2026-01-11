#include "PlaybackQueue.h"
#include <stdexcept>

// Khởi tạo cho current trỏ đến vị trí kết thúc của queue
PlaybackQueue::PlaybackQueue()
    : current(queue.end())
{
}

void PlaybackQueue::addSong(const Song &song)
{
    // [Fix] Kiem tra xem bai hat day trong Queue da ton tai hay chua
    queue.push_back(song);
    // Nếu danh sách có 1 bài, điều chỉnh con trỏ về phần tử đầu tiên
    if (queue.size() == 1)
        current = queue.begin();
}

void PlaybackQueue::removeSong(int songID)
{
    for (auto it = queue.begin(); it != queue.end(); ++it)
    {
        if (it->id == songID)
        {
            // Nếu xoá bài hát mà con trỏ đang trỏ tới
            if (it == current)
                // Chơi bài hát kế tiếp
                playNext();
            // Xoá
            queue.erase(it);
            return;
        }
    }
}

Song PlaybackQueue::getCurrentSong() const
{
    if (current == queue.end())
        throw std::runtime_error("Queue is empty");
    // trả về current
    return *current;
}

void PlaybackQueue::playNext()
{
    if (current != queue.end())
    {
        ++current;
        // Nếu đến cuối danh sách và queue chưa rỗng
        if (current == queue.end() && !queue.empty())
            // current trỏ về đầu danh sách
            current = queue.begin();
    }
}

