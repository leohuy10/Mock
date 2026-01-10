#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QRandomGenerator>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentTrackIndex(0)
    , isPlaying(false)
    , isShuffle(false)
    , isRepeat(false)
{
    ui->setupUi(this);
    
    // Initialize media player
    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);
    
    // Setup UI components
    setupUI();
    applyCarTheme();
    loadDemoPlaylist();
    
    // Connect signals
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::onPositionChanged);
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::onDurationChanged);
    connect(player, &QMediaPlayer::playbackStateChanged, this, &MainWindow::onMediaPlayerStateChanged);
    connect(player, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            onPlaybackEnded();
        }
    });
    
    // Set window properties
    setWindowTitle("Car Music Player");
    resize(900, 600);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI()
{
    // Create central widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // ===== TRACK INFO SECTION =====
    QWidget *trackInfoWidget = new QWidget();
    trackInfoWidget->setObjectName("trackInfoWidget");
    QVBoxLayout *trackInfoLayout = new QVBoxLayout(trackInfoWidget);
    trackInfoLayout->setSpacing(10);
    
    lblTitle = new QLabel("No Track Playing");
    lblTitle->setObjectName("lblTitle");
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setWordWrap(true);
    
    lblArtist = new QLabel("Select a song to start");
    lblArtist->setObjectName("lblArtist");
    lblArtist->setAlignment(Qt::AlignCenter);
    
    lblAlbum = new QLabel("");
    lblAlbum->setObjectName("lblAlbum");
    lblAlbum->setAlignment(Qt::AlignCenter);
    
    trackInfoLayout->addWidget(lblTitle);
    trackInfoLayout->addWidget(lblArtist);
    trackInfoLayout->addWidget(lblAlbum);
    
    // ===== PROGRESS SECTION =====
    QWidget *progressWidget = new QWidget();
    QVBoxLayout *progressLayout = new QVBoxLayout(progressWidget);
    progressLayout->setSpacing(5);
    
    seekSlider = new QSlider(Qt::Horizontal);
    seekSlider->setObjectName("seekSlider");
    connect(seekSlider, &QSlider::sliderMoved, this, &MainWindow::onSeekSliderMoved);
    
    QHBoxLayout *timeLayout = new QHBoxLayout();
    lblCurrentTime = new QLabel("0:00");
    lblCurrentTime->setObjectName("lblTime");
    lblTotalTime = new QLabel("0:00");
    lblTotalTime->setObjectName("lblTime");
    lblTotalTime->setAlignment(Qt::AlignRight);
    
    timeLayout->addWidget(lblCurrentTime);
    timeLayout->addStretch();
    timeLayout->addWidget(lblTotalTime);
    
    progressLayout->addWidget(seekSlider);
    progressLayout->addLayout(timeLayout);
    
    // ===== CONTROL BUTTONS =====
    QWidget *controlWidget = new QWidget();
    QHBoxLayout *controlLayout = new QHBoxLayout(controlWidget);
    controlLayout->setSpacing(15);
    
    btnShuffle = new QPushButton("🔀");
    btnShuffle->setObjectName("btnShuffle");
    btnShuffle->setFixedSize(60, 60);
    btnShuffle->setCheckable(true);
    connect(btnShuffle, &QPushButton::clicked, this, &MainWindow::onShuffleClicked);
    
    btnPrevious = new QPushButton("⏮");
    btnPrevious->setObjectName("btnControl");
    btnPrevious->setFixedSize(70, 70);
    connect(btnPrevious, &QPushButton::clicked, this, &MainWindow::onPreviousClicked);
    
    btnPlayPause = new QPushButton("▶");
    btnPlayPause->setObjectName("btnPlayPause");
    btnPlayPause->setFixedSize(90, 90);
    connect(btnPlayPause, &QPushButton::clicked, this, &MainWindow::onPlayPauseClicked);
    
    btnStop = new QPushButton("⏹");
    btnStop->setObjectName("btnControl");
    btnStop->setFixedSize(70, 70);
    connect(btnStop, &QPushButton::clicked, this, &MainWindow::onStopClicked);
    
    btnNext = new QPushButton("⏭");
    btnNext->setObjectName("btnControl");
    btnNext->setFixedSize(70, 70);
    connect(btnNext, &QPushButton::clicked, this, &MainWindow::onNextClicked);
    
    btnRepeat = new QPushButton("🔁");
    btnRepeat->setObjectName("btnRepeat");
    btnRepeat->setFixedSize(60, 60);
    btnRepeat->setCheckable(true);
    connect(btnRepeat, &QPushButton::clicked, this, &MainWindow::onRepeatClicked);
    
    controlLayout->addStretch();
    controlLayout->addWidget(btnShuffle);
    controlLayout->addWidget(btnPrevious);
    controlLayout->addWidget(btnPlayPause);
    controlLayout->addWidget(btnStop);
    controlLayout->addWidget(btnNext);
    controlLayout->addWidget(btnRepeat);
    controlLayout->addStretch();
    
    // ===== ADD MUSIC BUTTON =====
    QWidget *addMusicWidget = new QWidget();
    QHBoxLayout *addMusicLayout = new QHBoxLayout(addMusicWidget);
    
    btnAddMusic = new QPushButton("➕ Add Music Files");
    btnAddMusic->setObjectName("btnControl");
    btnAddMusic->setFixedHeight(50);
    connect(btnAddMusic, &QPushButton::clicked, this, &MainWindow::onAddMusicClicked);
    
    addMusicLayout->addWidget(btnAddMusic);
    
    // ===== VOLUME CONTROL =====
    QWidget *volumeWidget = new QWidget();
    QHBoxLayout *volumeLayout = new QHBoxLayout(volumeWidget);
    volumeLayout->setSpacing(10);
    
    QLabel *lblVolume = new QLabel("🔊");
    lblVolume->setObjectName("lblVolume");
    
    volumeSlider = new QSlider(Qt::Horizontal);
    volumeSlider->setObjectName("volumeSlider");
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(50);
    volumeSlider->setFixedWidth(200);
    connect(volumeSlider, &QSlider::valueChanged, this, &MainWindow::onVolumeChanged);
    
    lblVolumePercent = new QLabel("50%");
    lblVolumePercent->setObjectName("lblVolume");
    lblVolumePercent->setFixedWidth(40);
    
    volumeLayout->addStretch();
    volumeLayout->addWidget(lblVolume);
    volumeLayout->addWidget(volumeSlider);
    volumeLayout->addWidget(lblVolumePercent);
    volumeLayout->addStretch();
    
    // ===== PLAYLIST =====
    QLabel *lblPlaylist = new QLabel("📋 Playlist");
    lblPlaylist->setObjectName("lblPlaylist");
    
    playlistWidget = new QListWidget();
    playlistWidget->setObjectName("playlistWidget");
    connect(playlistWidget, &QListWidget::currentRowChanged, this, &MainWindow::onPlaylistItemClicked);
    
    // Add all sections to main layout
    mainLayout->addWidget(trackInfoWidget);
    mainLayout->addWidget(progressWidget);
    mainLayout->addWidget(controlWidget);
    mainLayout->addWidget(volumeWidget);
    mainLayout->addWidget(addMusicWidget);
    mainLayout->addWidget(lblPlaylist);
    mainLayout->addWidget(playlistWidget, 1);
    
    // Set initial volume
    audioOutput->setVolume(0.5);
}

void MainWindow::applyCarTheme()
{
    QString stylesheet = R"(
        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1a1a2e, stop:1 #16213e);
        }
        
        #trackInfoWidget {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #0f3460, stop:1 #16213e);
            border-radius: 15px;
            padding: 20px;
        }
        
        #lblTitle {
            color: #00d4ff;
            font-size: 28px;
            font-weight: bold;
            padding: 10px;
        }
        
        #lblArtist {
            color: #e94560;
            font-size: 20px;
            font-weight: bold;
        }
        
        #lblAlbum {
            color: #aaaaaa;
            font-size: 16px;
            font-style: italic;
        }
        
        #seekSlider, #volumeSlider {
            background: transparent;
        }
        
        #seekSlider::groove:horizontal, #volumeSlider::groove:horizontal {
            border: 1px solid #0f3460;
            height: 8px;
            background: #16213e;
            border-radius: 4px;
        }
        
        #seekSlider::handle:horizontal, #volumeSlider::handle:horizontal {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #00d4ff, stop:1 #0099cc);
            border: 2px solid #00d4ff;
            width: 18px;
            height: 18px;
            margin: -6px 0;
            border-radius: 9px;
        }
        
        #seekSlider::add-page:horizontal, #volumeSlider::add-page:horizontal {
            background: #16213e;
            border-radius: 4px;
        }
        
        #seekSlider::sub-page:horizontal, #volumeSlider::sub-page:horizontal {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #00d4ff, stop:1 #e94560);
            border-radius: 4px;
        }
        
        #lblTime, #lblVolume {
            color: #00d4ff;
            font-size: 14px;
            font-weight: bold;
        }
        
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #0f3460, stop:1 #16213e);
            color: white;
            border: 2px solid #00d4ff;
            border-radius: 10px;
            font-size: 24px;
            font-weight: bold;
        }
        
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #16213e, stop:1 #0f3460);
            border: 2px solid #e94560;
        }
        
        QPushButton:pressed {
            background: #0f3460;
        }
        
        #btnPlayPause {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #e94560, stop:1 #c13349);
            border: 3px solid #00d4ff;
            font-size: 32px;
        }
        
        #btnPlayPause:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #ff5577, stop:1 #e94560);
        }
        
        #btnShuffle:checked, #btnRepeat:checked {
            background: #00d4ff;
            color: #1a1a2e;
            border: 2px solid #00ff88;
        }
        
        #lblPlaylist {
            color: #00d4ff;
            font-size: 18px;
            font-weight: bold;
            padding: 5px;
        }
        
        #playlistWidget {
            background: #0f3460;
            color: white;
            border: 2px solid #00d4ff;
            border-radius: 10px;
            font-size: 14px;
            padding: 5px;
        }
        
        #playlistWidget::item {
            padding: 8px;
            border-bottom: 1px solid #16213e;
        }
        
        #playlistWidget::item:selected {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #e94560, stop:1 #00d4ff);
            color: white;
            font-weight: bold;
        }
        
        #playlistWidget::item:hover {
            background: #16213e;
        }
    )";
    
    setStyleSheet(stylesheet);
}

void MainWindow::loadDemoPlaylist()
{
    // Load demo songs (for demonstration, add some default songs to backend)
    playlist.clear();
    
    ExtendedSong song1;
    song1.id = 1;
    song1.title = "Summer Nights";
    song1.artist = "The Waves";
    song1.album = "Sunset Drive";
    song1.duration = 210;
    song1.filePath = "";
    playlist.append(song1);
    musicLibrary.addSong(song1);
    
    ExtendedSong song2;
    song2.id = 2;
    song2.title = "Highway Dreams";
    song2.artist = "Road Kings";
    song2.album = "Journey";
    song2.duration = 195;
    song2.filePath = "";
    playlist.append(song2);
    musicLibrary.addSong(song2);
    
    // Update playlist widget
    updatePlaylistWidget();
    
    if (!playlist.isEmpty()) {
        playlistWidget->setCurrentRow(0);
        updateTrackInfo();
    }
}

void MainWindow::updatePlaylistWidget()
{
    playlistWidget->clear();
    for (const ExtendedSong &song : playlist) {
        QString displayText = QString("%1 - %2 [%3]")
            .arg(QString::fromStdString(song.title))
            .arg(QString::fromStdString(song.artist))
            .arg(formatTime(song.duration * 1000));
        playlistWidget->addItem(displayText);
    }
}

void MainWindow::onPlayPauseClicked()
{
    if (playlist.isEmpty()) return;
    
    if (isPlaying) {
        player->pause();
        btnPlayPause->setText("▶");
        isPlaying = false;
    } else {
        const ExtendedSong &currentSong = playlist[currentTrackIndex];
        
        // Check if there's a real file to play
        if (!currentSong.filePath.empty()) {
            QString filePath = QString::fromStdString(currentSong.filePath);
            player->setSource(QUrl::fromLocalFile(filePath));
            player->play();
        }
        
        btnPlayPause->setText("⏸");
        isPlaying = true;
        updateTrackInfo();
    }
}

void MainWindow::onStopClicked()
{
    player->stop();
    btnPlayPause->setText("▶");
    isPlaying = false;
    seekSlider->setValue(0);
    lblCurrentTime->setText("0:00");
}

void MainWindow::onPreviousClicked()
{
    if (playlist.isEmpty()) return;
    
    currentTrackIndex--;
    if (currentTrackIndex < 0) {
        currentTrackIndex = playlist.size() - 1;
    }
    
    playlistWidget->setCurrentRow(currentTrackIndex);
    updateTrackInfo();
    
    if (isPlaying) {
        // In real implementation: playTrack(currentTrackIndex);
    }
}

void MainWindow::onNextClicked()
{
    if (playlist.isEmpty()) return;
    
    if (isShuffle) {
        currentTrackIndex = QRandomGenerator::global()->bounded(playlist.size());
    } else {
        currentTrackIndex++;
        if (currentTrackIndex >= playlist.size()) {
            if (isRepeat) {
                currentTrackIndex = 0;
            } else {
                currentTrackIndex = playlist.size() - 1;
                onStopClicked();
                return;
            }
        }
    }
    
    playlistWidget->setCurrentRow(currentTrackIndex);
    updateTrackInfo();
    
    if (isPlaying) {
        // In real implementation: playTrack(currentTrackIndex);
    }
}

void MainWindow::onVolumeChanged(int value)
{
    float volume = value / 100.0f;
    audioOutput->setVolume(volume);
    lblVolumePercent->setText(QString::number(value) + "%");
}

void MainWindow::onPositionChanged(qint64 position)
{
    if (!seekSlider->isSliderDown()) {
        seekSlider->setValue(position);
    }
    updateTimeLabels(position, player->duration());
}

void MainWindow::onDurationChanged(qint64 duration)
{
    seekSlider->setRange(0, duration);
    updateTimeLabels(player->position(), duration);
}

void MainWindow::onPlaylistItemClicked(int row)
{
    if (row >= 0 && row < playlist.size()) {
        currentTrackIndex = row;
        updateTrackInfo();
        
        if (isPlaying) {
            // In real implementation: playTrack(currentTrackIndex);
        }
    }
}

void MainWindow::onShuffleClicked()
{
    isShuffle = !isShuffle;
}

void MainWindow::onRepeatClicked()
{
    isRepeat = !isRepeat;
}

void MainWindow::onSeekSliderMoved(int position)
{
    player->setPosition(position);
}

void MainWindow::updateTrackInfo()
{
    if (currentTrackIndex >= 0 && currentTrackIndex < playlist.size()) {
        const ExtendedSong &song = playlist[currentTrackIndex];
        lblTitle->setText(QString::fromStdString(song.title));
        lblArtist->setText(QString::fromStdString(song.artist));
        lblAlbum->setText(QString::fromStdString(song.album));
        
        // Update time labels with track duration
        lblTotalTime->setText(formatTime(song.duration * 1000));
        lblCurrentTime->setText("0:00");
        seekSlider->setRange(0, song.duration * 1000);
        seekSlider->setValue(0);
    }
}

void MainWindow::updateTimeLabels(qint64 position, qint64 duration)
{
    lblCurrentTime->setText(formatTime(position));
    lblTotalTime->setText(formatTime(duration));
}

void MainWindow::playTrack(int index)
{
    if (index >= 0 && index < playlist.size()) {
        currentTrackIndex = index;
        const ExtendedSong &song = playlist[index];
        
        // Play the actual file if it exists
        if (!song.filePath.empty()) {
            QString filePath = QString::fromStdString(song.filePath);
            player->setSource(QUrl::fromLocalFile(filePath));
            player->play();
            isPlaying = true;
            btnPlayPause->setText("⏸");
        }
        
        updateTrackInfo();
        playlistWidget->setCurrentRow(index);
    }
}

QString MainWindow::formatTime(qint64 milliseconds)
{
    qint64 seconds = milliseconds / 1000;
    int minutes = seconds / 60;
    int secs = seconds % 60;
    return QString("%1:%2").arg(minutes).arg(secs, 2, 10, QChar('0'));
}

void MainWindow::onAddMusicClicked()
{
    // Open file dialog to select audio files
    QStringList files = QFileDialog::getOpenFileNames(
        this,
        "Select Music Files",
        QDir::homePath(),
        "Audio Files (*.mp3 *.wav *.flac *.m4a *.ogg *.wma);;All Files (*.*)"
    );
    
    if (files.isEmpty()) return;
    
    // Add each selected file to the playlist
    for (const QString &filePath : files) {
        QFileInfo fileInfo(filePath);
        
        ExtendedSong song;
        song.id = playlist.size() + 1;
        song.title = fileInfo.completeBaseName().toStdString();  // Use filename as title
        song.artist = "Unknown Artist";
        song.album = "Unknown Album";
        song.duration = 180;  // Default duration, will be updated when played
        song.filePath = filePath.toStdString();
        
        playlist.append(song);
        musicLibrary.addSong(song);
        playbackQueue.addSong(song);
    }
    
    // Update the playlist widget
    updatePlaylistWidget();
    
    // If this is the first song, select it
    if (playlist.size() == files.size()) {
        playlistWidget->setCurrentRow(0);
        currentTrackIndex = 0;
        updateTrackInfo();
    }
}

void MainWindow::onMediaPlayerStateChanged(QMediaPlayer::PlaybackState state)
{
    switch (state) {
        case QMediaPlayer::PlayingState:
            isPlaying = true;
            btnPlayPause->setText("⏸");
            break;
        case QMediaPlayer::PausedState:
        case QMediaPlayer::StoppedState:
            isPlaying = false;
            btnPlayPause->setText("▶");
            break;
    }
}

void MainWindow::onPlaybackEnded()
{
    // Automatically play next song when current song ends
    if (isRepeat) {
        // Replay current song
        playTrack(currentTrackIndex);
    } else if (isShuffle) {
        // Play random song
        currentTrackIndex = QRandomGenerator::global()->bounded(playlist.size());
        playTrack(currentTrackIndex);
    } else {
        // Play next song
        if (currentTrackIndex < playlist.size() - 1) {
            onNextClicked();
        } else {
            // End of playlist
            onStopClicked();
        }
    }
}
