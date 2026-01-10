# 🎵 Music Player Project

## 📋 Giới Thiệu

Đây là một ứng dụng Music Player hoàn chỉnh được xây dựng bằng C++ và Qt Framework, có giao diện đẹp mắt theo phong cách xe hơi (Car Theme) và hệ thống backend mạnh mẽ với các thuật toán tối ưu.

## 🛠️ Công Nghệ Sử Dụng

### 1. **Qt Framework (Qt5/Qt6)**
- **Qt Widgets**: Xây dựng giao diện đồ họa người dùng (GUI)
- **Qt Multimedia**: Xử lý phát nhạc, âm thanh
- **QMediaPlayer**: Trình phát media chính
- **QAudioOutput**: Điều khiển đầu ra âm thanh

### 2. **C++ Standard Library (C++17)**
- **STL Containers**: Vector, List, Stack, Queue, Map, Unordered_map
- **Smart Pointers**: Quản lý bộ nhớ tự động
- **Modern C++ Features**: Auto, lambda expressions

### 3. **CMake**
- Hệ thống build đa nền tảng
- Tự động quản lý dependencies
- Hỗ trợ MinGW, GCC và các compiler khác

## 🏗️ Kiến Trúc Hệ Thống

### Cấu Trúc Thư Mục

```
Mock/
├── MusicPlayerProject/          # Backend Core
│   ├── include/                 # Header files
│   │   ├── Song.h              # Cấu trúc dữ liệu bài hát
│   │   ├── MusicLibrary.h      # Quản lý thư viện nhạc
│   │   ├── MusicPlayer.h       # Trình phát nhạc chính
│   │   ├── PlaybackQueue.h     # Hàng đợi phát nhạc
│   │   ├── PlaybackHistory.h   # Lịch sử phát nhạc
│   │   └── SmartPlaylist.h     # Playlist thông minh
│   └── src/                    # Implementation files
│       ├── MusicLibrary.cpp
│       ├── MusicPlayer.cpp
│       ├── PlaybackQueue.cpp
│       ├── PlaybackHistory.cpp
│       └── SmartPlaylist.cpp
├── mainwindow.h                # GUI Header
├── mainwindow.cpp              # GUI Implementation
├── mainwindow.ui               # Qt Designer UI file
├── main.cpp                    # Entry point
└── CMakeLists.txt              # Build configuration
```

## 🎯 Tính Năng Chính

### 1. **Giao Diện Người Dùng (GUI)**

#### 🎨 Car-Inspired Theme
- Thiết kế hiện đại theo phong cách xe hơi
- Dark theme với accent màu cam (#FF8C00)
- Hiệu ứng gradient và shadows
- Responsive design

#### 🎛️ Các Thành Phần Giao Diện
- **Track Info Display**: Hiển thị thông tin bài hát (tên, nghệ sĩ, album)
- **Progress Bar**: Thanh tiến trình với khả năng tua
- **Time Display**: Hiển thị thời gian hiện tại/tổng thời gian
- **Playlist Widget**: Danh sách phát với highlight bài đang phát
- **Control Buttons**: Các nút điều khiển phát nhạc
- **Volume Control**: Điều chỉnh âm lượng với slider

### 2. **Chức Năng Phát Nhạc**

#### ▶️ Điều Khiển Cơ Bản
- **Play/Pause**: Phát và tạm dừng nhạc
- **Stop**: Dừng hoàn toàn
- **Next/Previous**: Chuyển bài tiếp theo/trước đó
- **Seek**: Tua nhanh đến vị trí bất kỳ

#### 🔀 Điều Khiển Nâng Cao
- **Shuffle Mode**: Phát ngẫu nhiên
  - Sử dụng QRandomGenerator
  - Tránh lặp lại bài vừa phát
- **Repeat Mode**: Lặp lại playlist
  - Auto replay khi hết playlist
- **Volume Control**: Điều chỉnh âm lượng 0-100%

#### 📁 Quản Lý File
- **Add Music**: Thêm file nhạc từ máy tính
- **Supported Formats**: MP3, WAV, FLAC, OGG, M4A
- **File Browser**: Dialog chọn file với filter

### 3. **Backend System**

#### 📚 MusicLibrary - Thư Viện Nhạc
**Cấu trúc dữ liệu đa chỉ mục (Multi-index):**
```cpp
vector<Song> songs;                              // Storage chính
unordered_map<int, Song*> songIndexByID;        // Tra cứu theo ID: O(1)
map<string, Song*> songIndexByTitle;            // Tra cứu theo tên: O(log n)
unordered_map<string, vector<Song*>> artistIndex; // Tra cứu theo nghệ sĩ: O(1)
```

**Ưu điểm:**
- ✅ Tìm kiếm cực nhanh theo ID: O(1)
- ✅ Tìm kiếm theo nghệ sĩ: O(1)
- ✅ Tìm kiếm có thứ tự theo tên: O(log n)
- ✅ Quản lý hiệu quả cho thư viện lớn (50,000+ bài hát)

#### 🎼 PlaybackQueue - Hàng Đợi Phát
**Sử dụng std::list:**
```cpp
list<Song> queue;
list<Song>::iterator current;
```

**Lý do chọn List thay vì Vector:**
- ✅ Iterator stability: Iterator không bị invalid khi xóa phần tử
- ✅ Xóa phần tử: O(1) (vector là O(n))
- ✅ Phù hợp cho thao tác thêm/xóa thường xuyên

**Chức năng:**
- Thêm bài vào queue: O(1)
- Phát bài tiếp theo: O(1)
- Xóa bài khỏi queue: O(1) (nếu có iterator)
- Lấy bài hiện tại: O(1)

#### 📜 PlaybackHistory - Lịch Sử Phát
**Sử dụng std::stack (LIFO):**
```cpp
stack<Song> history;
```

**Lý do chọn Stack:**
- ✅ LIFO behavior: Bài phát gần nhất được quay lại trước
- ✅ Push/Pop: O(1)
- ✅ Đơn giản và hiệu quả nhất cho use case này

**Chức năng:**
- Lưu bài vừa phát: O(1)
- Quay lại bài trước: O(1)
- Kiểm tra history rỗng: O(1)

#### 🎲 SmartPlaylist - Playlist Thông Minh
**Sử dụng BFS (Breadth-First Search):**
```cpp
queue<const Song*> bfsQueue;
set<int> visited;
```

**Thuật toán:**
- Tạo playlist dựa trên mối quan hệ nghệ sĩ
- Đảm bảo diversity trong playlist
- Complexity: O(V + E·log V)
  - V = số bài hát
  - E = số kết nối giữa các nghệ sĩ

**Ưu điểm:**
- ✅ Tạo playlist có liên kết nhau
- ✅ Không lặp lại bài hát (sử dụng visited set)
- ✅ Quality playlist cao

#### 🎮 MusicPlayer - Controller Tổng
**Auto-sync system:**
```cpp
void playNext() {
    if (currentSong) history.pushSong(*currentSong);  // Tự động lưu
    playbackQueue.playNext();
    currentSong = library.findSongByID(...);          // Đồng bộ
}
```

**Đặc điểm:**
- ✅ Tự động đồng bộ history
- ✅ State consistency
- ✅ Encapsulation tốt
- ✅ Single source of truth

## 📊 Độ Phức Tạp Thuật Toán

| Component | Cấu Trúc Dữ Liệu | Tìm Kiếm | Thêm/Xóa | Space |
|-----------|------------------|----------|----------|-------|
| MusicLibrary | Vector + Maps | O(1) - O(log n) | O(1) | O(n) |
| PlaybackQueue | List | O(n) | O(1) | O(k) |
| PlaybackHistory | Stack | O(1) | O(1) | O(h) |
| SmartPlaylist | BFS Graph | O(V+E) | O(1) | O(V) |

**Tổng Space Complexity:** O(n + k + h + V) - Linear

## 🎨 UI/UX Features

### Theme System
```css
/* Dark Background với Gradient */
background: linear-gradient(135deg, #1e1e1e, #2d2d2d)

/* Accent Color */
primary: #FF8C00 (Dark Orange)

/* Hover Effects */
- Button hover: Scale transform + glow effect
- Slider hover: Color transition
```

### Responsive Elements
- Window size: 900x600 (có thể resize)
- Auto-adjust layout
- Proper spacing và margins
- Font scaling

## 🚀 Cách Sử Dụng

### Build Project

```bash
# Tạo build directory
mkdir build && cd build

# Configure với CMake
cmake ..

# Build
cmake --build .

# Hoặc với MinGW
mingw32-make
```

### Chạy Ứng Dụng

```bash
# Windows
.\shfihew.exe

# Linux/Mac
./shfihew
```

### Thêm Nhạc
1. Click nút "📁 Add Music"
2. Chọn file nhạc (MP3, WAV, FLAC, OGG, M4A)
3. Bài hát sẽ được thêm vào playlist
4. Double-click hoặc click nút Play để phát

### Điều Khiển
- **Space/Click Play**: Play/Pause
- **Slider**: Tua nhạc
- **Volume Slider**: Điều chỉnh âm lượng
- **Shuffle**: Bật/tắt chế độ phát ngẫu nhiên
- **Repeat**: Bật/tắt chế độ lặp lại

## 💡 Điểm Nổi Bật

### 1. **Hiệu Suất Cao**
- Multi-index cho tìm kiếm nhanh
- Iterator stability với List
- Memory efficient với pointers
- O(1) cho hầu hết operations

### 2. **Thiết Kế Tốt**
- Separation of concerns (GUI vs Backend)
- SOLID principles
- Modern C++ practices
- Clean code structure

### 3. **User Experience**
- Beautiful car-themed UI
- Smooth animations
- Intuitive controls
- Real-time updates

### 4. **Mở Rộng Dễ Dàng**
- Modular architecture
- Clear interfaces
- Well-documented code
- Easy to add features

## 📝 Trade-offs & Decisions

### Chấp Nhận
- ❌ Memory overhead (multiple indices)
- ❌ O(log n) cho tìm kiếm theo title
- ❌ O(n) cho tìm kiếm trong queue

### Đạt Được
- ✅ O(1) ID/artist lookup
- ✅ Iterator stability
- ✅ Sorted order
- ✅ Auto-sync
- ✅ High performance

## 🔧 Yêu Cầu Hệ Thống

### Software
- **CMake**: ≥ 3.16
- **Qt**: Qt5 hoặc Qt6 (Qt Widgets, Qt Multimedia)
- **C++ Compiler**: Support C++17 (GCC, MinGW, MSVC, Clang)

### Hardware
- **RAM**: Tối thiểu 2GB
- **Disk**: 100MB cho ứng dụng
- **OS**: Windows, Linux, macOS

## 📚 Dependencies

```cmake
# Qt Modules
Qt::Widgets          # GUI components
Qt::Multimedia       # Audio playback

# C++ Standard
C++17               # Modern C++ features
```

## 🐛 Known Issues & Future Improvements

### Planned Features
- [ ] Equalizer
- [ ] Lyrics display
- [ ] Playlist import/export
- [ ] Online streaming
- [ ] Visualizer
- [ ] Keyboard shortcuts
- [ ] Playlist editing
- [ ] Search functionality

### Bug Fixes
- [ ] Handle corrupted audio files
- [ ] Improve shuffle algorithm
- [ ] Better error messages

## 👨‍💻 Development

### Code Structure
```cpp
// Frontend (Qt)
main.cpp           → Entry point
mainwindow.h/cpp   → Main GUI controller
mainwindow.ui      → Qt Designer file

// Backend (C++)
Song.h             → Data structure
MusicLibrary.*     → Song management
PlaybackQueue.*    → Queue management
PlaybackHistory.*  → History tracking
SmartPlaylist.*    → Smart algorithm
MusicPlayer.*      → Main controller
```

### Coding Standards
- **Naming**: camelCase cho functions, PascalCase cho classes
- **Comments**: Vietnamese cho logic, English cho technical terms
- **Indentation**: 4 spaces
- **Max line**: 100 characters

## 📄 License

Đây là project học tập/cá nhân. Free to use and modify.

## 🤝 Contributing

Contributions are welcome! Please feel free to:
- Report bugs
- Suggest features
- Submit pull requests
- Improve documentation

## 📧 Contact

Nếu có câu hỏi hoặc góp ý, vui lòng liên hệ qua GitHub Issues.

---

**Phát triển với ❤️ bằng C++ và Qt Framework**

*Last updated: January 2026*
