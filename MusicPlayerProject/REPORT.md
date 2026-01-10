# BÁO CÁO MUSIC PLAYER PROJECT

## 1. GIỚI THIỆU

Hệ thống Music Player được xây dựng với 5 components: MusicLibrary, PlaybackQueue, PlaybackHistory, SmartPlaylist, và MusicPlayer. Dưới đây là báo cáo phân tích lựa chọn cấu trúc dữ liệu và độ phức tạp thuật toán.

## 2. PHÂN TÍCH CẤU TRÚC DỮ LIỆU

### 2.1. MusicLibrary

**Cấu trúc:**
```cpp
vector<Song> songs;                              // Storage O(1) access
unordered_map<int, Song*> songIndexByID;        // O(1) ID lookup
map<string, Song*> songIndexByTitle;            // O(log n) sorted
unordered_map<string, vector<Song*>> artistIndex; // O(1) artist lookup
```

**Lý do:** Vector cho storage chính với pointer stability (`reserve(50000)`). Mỗi index tối ưu cho use case riêng: unordered_map cho fast lookup O(1), map cho sorted order.

**Complexity:** addSong O(1), findByID O(1), findByTitle O(log n), findByArtist O(1)

**Chứng minh:** Multi-index trade memory cho performance, mỗi lookup có complexity tốt nhất.


### 2.2. PlaybackQueue (Sử dụng List)

**Cấu trúc:**
```cpp
list<Song> queue;
list<Song>::iterator current;
```

**So sánh với vector:**
- List: Remove O(1), iterator stable 
- Vector: Remove O(n), iterator invalidated 

Với iterator có sẵn

**Lý do:** Iterator stability critical cho music player. Khi xóa bài trong queue, current không bị invalidate.

**Complexity:** addSong O(1), playNext O(1), removeSong O(n), getCurrentSong O(1)

**Chứng minh:** List là lựa chọn duy nhất đảm bảo iterator stability khi remove elements.


### 2.3. PlaybackHistory (LIFO)

**Cấu trúc:**
```cpp
stack<Song> history;
```

**Lý do:** LIFO behavior - bài phát gần nhất quay lại trước. Stack là sự lựa chọn hợp lý nhất.

**Complexity:** push O(1), pop O(1), isEmpty O(1)

**Chứng minh:** Stack đơn giản và hiệu quả nhất cho LIFO, không có sự lựa chọn nào tốt hơn

---

### 2.4. SmartPlaylist (Sử dụng BFS)

**Cấu trúc:**
```cpp
queue<const Song*> bfsQueue;
set<int> visited;
```

**Complexity:** O(V + E·log V) với V = songs, E = artist connections

**Chứng minh:** BFS đảm bảo playlist quality tốt hơn trong ngữ cảnh này


### 2.5. MusicPlayer - Auto-Sync Controller

**Chức năng chính:**
```cpp
void playNext() {
    if (currentSong) history.pushSong(*currentSong);  // Auto-save
    playbackQueue.playNext();
    currentSong = library.findSongByID(...);          // Sync
}
```
**Ưu điểm:** Auto-sync history, state consistent, encapsulation tốt.

## 3. TỔNG KẾT

**Bảng complexity:**

| Component | Data Structure | Time (search) | Space |
|-----------|---------------|---------------|-------|
| MusicLibrary | Vector + Maps | O(1) - O(log n) | O(n) |
| PlaybackQueue | List | O(1) | O(k) |
| PlaybackHistory | Stack | O(1) | O(h) |
| SmartPlaylist | BFS | O(V+E) | O(V) |

**Total Space:** O(n + k + h + V) - Linear

**Trade-offs:**
- Chấp nhận: Memory overhead, O(log n) cho title, O(n) tìm trong queue
- Đạt được: O(1) ID/artist lookup, iterator stability, sorted order, auto-sync

**Kết luận:** Mỗi cấu trúc được chọn dựa trên requirements cụ thể với complexity tối ưu. Trade-offs đều justified và đáng giá.

