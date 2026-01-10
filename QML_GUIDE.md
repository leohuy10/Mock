# Qt Quick QML Guide - Extending Your Music Player

## Overview
This guide shows how to customize and extend your new QML-based music player.

## Understanding the Project Structure

```
.
├── main.cpp                    # Application entry point (Qt Quick)
├── musiccontroller.h/cpp       # C++ backend exposed to QML
├── resources.qrc               # QML resource file
├── main.qml                    # Root UI window
├── components/
│   ├── TrackInfo.qml          # Current song display
│   ├── ProgressBar.qml        # Seek bar
│   ├── PlayerControls.qml     # Play/pause/etc buttons
│   ├── VolumeControl.qml      # Volume slider
│   └── PlaylistView.qml       # Song list
└── MusicPlayerProject/         # Backend logic (unchanged)
    ├── include/
    └── src/
```

## How QML Property Binding Works

### Example 1: Displaying a Value
```qml
Text {
    text: musicController.currentTitle  // Automatically updates when property changes
    color: colorPrimary
}
```

When `musicController.currentTitle` changes, the Text automatically re-renders.

### Example 2: Button Click
```qml
Button {
    onClicked: musicController.playPause()  // Call C++ method
}
```

### Example 3: Property Binding
```qml
Button {
    checked: musicController.isShuffle  // Always reflects C++ state
    onToggled: musicController.isShuffle = checked
}
```

## Adding New Features

### 1. Add a New C++ Signal/Property

In `musiccontroller.h`:
```cpp
class MusicController : public QObject {
    Q_OBJECT
    Q_PROPERTY(float rating READ getRating WRITE setRating NOTIFY ratingChanged)
    
public:
    float getRating() const { return currentRating; }
    void setRating(float value) { currentRating = value; emit ratingChanged(); }
    
signals:
    void ratingChanged();
    
private:
    float currentRating = 0.0f;
};
```

In `musiccontroller.cpp`:
```cpp
void MusicController::setRating(float value) {
    if (currentRating != value) {
        currentRating = value;
        // Store to database or file
        emit ratingChanged();
    }
}
```

### 2. Use the New Property in QML

Create `components/RatingControl.qml`:
```qml
Rectangle {
    RowLayout {
        Repeater {
            model: 5
            delegate: Text {
                text: "⭐"
                color: index < musicController.rating ? colorAccent : colorDim
                
                MouseArea {
                    anchors.fill: parent
                    onClicked: musicController.rating = index + 1
                }
            }
        }
    }
}
```

Add to `main.qml`:
```qml
ColumnLayout {
    // ... other components
    RatingControl {
        Layout.fillWidth: true
        Layout.preferredHeight: 40
    }
}
```

## Example: Adding a Search Feature

### Step 1: Extend MusicController (musiccontroller.h)
```cpp
Q_PROPERTY(QStringList searchResults READ getSearchResults NOTIFY searchResultsChanged)

public slots:
    void searchPlaylist(const QString &query);

signals:
    void searchResultsChanged();

private:
    QStringList getSearchResults() const;
    QStringList currentSearchResults;
```

### Step 2: Implement Search (musiccontroller.cpp)
```cpp
void MusicController::searchPlaylist(const QString &query) {
    currentSearchResults.clear();
    for (const ExtendedSong &song : playlist) {
        QString title = QString::fromStdString(song.title);
        QString artist = QString::fromStdString(song.artist);
        
        if (title.contains(query, Qt::CaseInsensitive) || 
            artist.contains(query, Qt::CaseInsensitive)) {
            currentSearchResults.append(QString("%1 - %2")
                .arg(title).arg(QString::fromStdString(song.artist)));
        }
    }
    emit searchResultsChanged();
}
```

### Step 3: Create SearchBar.qml
```qml
Rectangle {
    RowLayout {
        TextField {
            placeholderText: "Search songs..."
            color: colorText
            
            onTextChanged: musicController.searchPlaylist(text)
        }
    }
}
```

### Step 4: Add to main.qml
```qml
SearchBar {
    Layout.fillWidth: true
    Layout.preferredHeight: 40
}
```

## Common QML Patterns

### 1. Conditional Styling
```qml
Rectangle {
    color: isSelected ? colorAccent : colorBg3
}
```

### 2. Animations
```qml
Rectangle {
    color: colorAccent
    
    ColorAnimation on color {
        duration: 500
        from: colorBg3
        to: colorAccent
    }
}
```

### 3. Layouts
```qml
ColumnLayout {       // Vertical stacking
    spacing: 10
}

RowLayout {          // Horizontal stacking
    spacing: 10
}

GridLayout {         // Grid arrangement
    columns: 3
}
```

### 4. List Delegation
```qml
ListView {
    model: musicController.playlistItems
    
    delegate: Rectangle {
        width: parent.width
        height: 40
        
        Text {
            text: modelData
        }
    }
}
```

### 5. Pop-ups
```qml
Dialog {
    title: "Add Music"
    
    Button {
        text: "OK"
        onClicked: accept()
    }
}
```

## Styling Guidelines

### Color System
Define all colors in `main.qml`:
```qml
readonly property string colorBg1: "#1a1a2e"      // Darkest
readonly property string colorBg2: "#16213e"      // Dark
readonly property string colorBg3: "#0f3460"      // Medium
readonly property string colorPrimary: "#00d4ff"  // Cyan
readonly property string colorAccent: "#e94560"   // Red
readonly property string colorText: "#ffffff"     // White
readonly property string colorDim: "#aaaaaa"      // Gray
```

### Common Component Styling
```qml
Rectangle {
    color: colorBg3
    border.color: colorPrimary
    border.width: 2
    radius: 10
}

Text {
    color: colorText
    font.pixelSize: 16
    font.bold: true
}
```

## Performance Tips

1. **Use Layouts**: Let QML handle sizing
```qml
Rectangle {
    Layout.fillWidth: true
    Layout.preferredHeight: 60
}
```

2. **Lazy Loading**: Load components only when needed
```qml
Loader {
    sourceComponent: heavyComponent
    active: visible
}
```

3. **Item Pooling**: Reuse delegate items
```qml
ListView {
    cacheBuffer: 200
}
```

4. **Avoid Binding Loops**: Bad ↓
```qml
Text { text: label.text }  // If label.text also depends on this
```

## Debugging QML

### Enable Debug Output
In `musiccontroller.cpp`:
```cpp
qDebug() << "Current track:" << getCurrentTitle();
```

View in application output.

### QML Console
Right-click → "Show QML Console" in some Qt Creator versions.

### Print Statements
```qml
Component.onCompleted: console.log("Component loaded")
```

## File Dialog Integration

Replace placeholder in `main.qml`:
```qml
import QtQuick.Dialogs

Button {
    text: "➕ Add Music Files"
    onClicked: fileDialog.open()
}

FolderDialog {
    id: fileDialog
    onAccepted: {
        // Extract file paths and call musicController.addMusicFiles()
    }
}
```

## Deployment

### For Desktop
- Includes QML files in executable
- `resources.qrc` bundles everything

### For Mobile
- Add `QtPlatformHeaders`
- Test touch interactions
- Adjust button sizes for fingers

## Resources

- Qt Quick Documentation: https://doc.qt.io/qt-6/qtquick-index.html
- QML Syntax: https://doc.qt.io/qt-6/qml-typesystem.html
- Property System: https://doc.qt.io/qt-6/qtqml-syntax-propertybinding.html

