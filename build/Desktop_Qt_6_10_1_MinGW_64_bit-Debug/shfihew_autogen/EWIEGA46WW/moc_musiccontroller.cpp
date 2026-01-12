/****************************************************************************
** Meta object code from reading C++ file 'musiccontroller.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../musiccontroller.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'musiccontroller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN15MusicControllerE_t {};
} // unnamed namespace

template <> constexpr inline auto MusicController::qt_create_metaobjectdata<qt_meta_tag_ZN15MusicControllerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MusicController",
        "currentTitleChanged",
        "",
        "currentArtistChanged",
        "currentAlbumChanged",
        "positionChanged",
        "durationChanged",
        "volumeChanged",
        "playingStateChanged",
        "shuffleStateChanged",
        "repeatStateChanged",
        "playlistChanged",
        "currentTrackIndexChanged",
        "currentModeChanged",
        "queueCountChanged",
        "search",
        "text",
        "setFilterMode",
        "mode",
        "addToQueue",
        "songId",
        "getSongIdAt",
        "index",
        "removeFromQueue",
        "getCurrentMode",
        "playPause",
        "stop",
        "next",
        "previous",
        "seek",
        "position",
        "addMusicFiles",
        "filePaths",
        "selectTrack",
        "clearPlaylist",
        "loadMusicFolder",
        "folderPath",
        "formatTime",
        "milliseconds",
        "onPositionChanged",
        "onDurationChanged",
        "duration",
        "onMediaPlayerStateChanged",
        "QMediaPlayer::PlaybackState",
        "state",
        "onPlaybackEnded",
        "isSongInQueue",
        "currentTitle",
        "currentArtist",
        "currentAlbum",
        "currentPosition",
        "totalDuration",
        "volume",
        "isPlaying",
        "isShuffle",
        "isRepeat",
        "playlistItems",
        "currentTrackIndex",
        "queueCount",
        "currentMode"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'currentTitleChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'currentArtistChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'currentAlbumChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'positionChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'durationChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'volumeChanged'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'playingStateChanged'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'shuffleStateChanged'
        QtMocHelpers::SignalData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'repeatStateChanged'
        QtMocHelpers::SignalData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'playlistChanged'
        QtMocHelpers::SignalData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'currentTrackIndexChanged'
        QtMocHelpers::SignalData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'currentModeChanged'
        QtMocHelpers::SignalData<void()>(13, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'queueCountChanged'
        QtMocHelpers::SignalData<void()>(14, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'search'
        QtMocHelpers::SlotData<void(const QString &)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 16 },
        }}),
        // Slot 'setFilterMode'
        QtMocHelpers::SlotData<void(int)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Slot 'addToQueue'
        QtMocHelpers::SlotData<void(int)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 20 },
        }}),
        // Slot 'getSongIdAt'
        QtMocHelpers::SlotData<int(int) const>(21, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::Int, 22 },
        }}),
        // Slot 'removeFromQueue'
        QtMocHelpers::SlotData<void(int)>(23, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 22 },
        }}),
        // Slot 'getCurrentMode'
        QtMocHelpers::SlotData<int() const>(24, 2, QMC::AccessPublic, QMetaType::Int),
        // Slot 'playPause'
        QtMocHelpers::SlotData<void()>(25, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'stop'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'next'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'previous'
        QtMocHelpers::SlotData<void()>(28, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'seek'
        QtMocHelpers::SlotData<void(qint64)>(29, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::LongLong, 30 },
        }}),
        // Slot 'addMusicFiles'
        QtMocHelpers::SlotData<void(const QStringList &)>(31, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QStringList, 32 },
        }}),
        // Slot 'selectTrack'
        QtMocHelpers::SlotData<void(int)>(33, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 22 },
        }}),
        // Slot 'clearPlaylist'
        QtMocHelpers::SlotData<void()>(34, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'loadMusicFolder'
        QtMocHelpers::SlotData<void(const QString &)>(35, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 36 },
        }}),
        // Slot 'formatTime'
        QtMocHelpers::SlotData<QString(qint64) const>(37, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::LongLong, 38 },
        }}),
        // Slot 'onPositionChanged'
        QtMocHelpers::SlotData<void(qint64)>(39, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::LongLong, 30 },
        }}),
        // Slot 'onDurationChanged'
        QtMocHelpers::SlotData<void(qint64)>(40, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::LongLong, 41 },
        }}),
        // Slot 'onMediaPlayerStateChanged'
        QtMocHelpers::SlotData<void(QMediaPlayer::PlaybackState)>(42, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 43, 44 },
        }}),
        // Slot 'onPlaybackEnded'
        QtMocHelpers::SlotData<void()>(45, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'isSongInQueue'
        QtMocHelpers::SlotData<bool(int) const>(46, 2, QMC::AccessPrivate, QMetaType::Bool, {{
            { QMetaType::Int, 20 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'currentTitle'
        QtMocHelpers::PropertyData<QString>(47, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'currentArtist'
        QtMocHelpers::PropertyData<QString>(48, QMetaType::QString, QMC::DefaultPropertyFlags, 1),
        // property 'currentAlbum'
        QtMocHelpers::PropertyData<QString>(49, QMetaType::QString, QMC::DefaultPropertyFlags, 2),
        // property 'currentPosition'
        QtMocHelpers::PropertyData<qint64>(50, QMetaType::LongLong, QMC::DefaultPropertyFlags, 3),
        // property 'totalDuration'
        QtMocHelpers::PropertyData<qint64>(51, QMetaType::LongLong, QMC::DefaultPropertyFlags, 4),
        // property 'volume'
        QtMocHelpers::PropertyData<int>(52, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 5),
        // property 'isPlaying'
        QtMocHelpers::PropertyData<bool>(53, QMetaType::Bool, QMC::DefaultPropertyFlags, 6),
        // property 'isShuffle'
        QtMocHelpers::PropertyData<bool>(54, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable, 7),
        // property 'isRepeat'
        QtMocHelpers::PropertyData<bool>(55, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable, 8),
        // property 'playlistItems'
        QtMocHelpers::PropertyData<QStringList>(56, QMetaType::QStringList, QMC::DefaultPropertyFlags, 9),
        // property 'currentTrackIndex'
        QtMocHelpers::PropertyData<int>(57, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 10),
        // property 'queueCount'
        QtMocHelpers::PropertyData<int>(58, QMetaType::Int, QMC::DefaultPropertyFlags, 12),
        // property 'currentMode'
        QtMocHelpers::PropertyData<int>(59, QMetaType::Int, QMC::DefaultPropertyFlags, 11),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MusicController, qt_meta_tag_ZN15MusicControllerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MusicController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15MusicControllerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15MusicControllerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN15MusicControllerE_t>.metaTypes,
    nullptr
} };

void MusicController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MusicController *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->currentTitleChanged(); break;
        case 1: _t->currentArtistChanged(); break;
        case 2: _t->currentAlbumChanged(); break;
        case 3: _t->positionChanged(); break;
        case 4: _t->durationChanged(); break;
        case 5: _t->volumeChanged(); break;
        case 6: _t->playingStateChanged(); break;
        case 7: _t->shuffleStateChanged(); break;
        case 8: _t->repeatStateChanged(); break;
        case 9: _t->playlistChanged(); break;
        case 10: _t->currentTrackIndexChanged(); break;
        case 11: _t->currentModeChanged(); break;
        case 12: _t->queueCountChanged(); break;
        case 13: _t->search((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 14: _t->setFilterMode((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 15: _t->addToQueue((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 16: { int _r = _t->getSongIdAt((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 17: _t->removeFromQueue((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 18: { int _r = _t->getCurrentMode();
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 19: _t->playPause(); break;
        case 20: _t->stop(); break;
        case 21: _t->next(); break;
        case 22: _t->previous(); break;
        case 23: _t->seek((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1]))); break;
        case 24: _t->addMusicFiles((*reinterpret_cast<std::add_pointer_t<QStringList>>(_a[1]))); break;
        case 25: _t->selectTrack((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 26: _t->clearPlaylist(); break;
        case 27: _t->loadMusicFolder((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 28: { QString _r = _t->formatTime((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 29: _t->onPositionChanged((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1]))); break;
        case 30: _t->onDurationChanged((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1]))); break;
        case 31: _t->onMediaPlayerStateChanged((*reinterpret_cast<std::add_pointer_t<QMediaPlayer::PlaybackState>>(_a[1]))); break;
        case 32: _t->onPlaybackEnded(); break;
        case 33: { bool _r = _t->isSongInQueue((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (MusicController::*)()>(_a, &MusicController::currentTitleChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (MusicController::*)()>(_a, &MusicController::currentArtistChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (MusicController::*)()>(_a, &MusicController::currentAlbumChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (MusicController::*)()>(_a, &MusicController::positionChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (MusicController::*)()>(_a, &MusicController::durationChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (MusicController::*)()>(_a, &MusicController::volumeChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (MusicController::*)()>(_a, &MusicController::playingStateChanged, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (MusicController::*)()>(_a, &MusicController::shuffleStateChanged, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (MusicController::*)()>(_a, &MusicController::repeatStateChanged, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (MusicController::*)()>(_a, &MusicController::playlistChanged, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (MusicController::*)()>(_a, &MusicController::currentTrackIndexChanged, 10))
            return;
        if (QtMocHelpers::indexOfMethod<void (MusicController::*)()>(_a, &MusicController::currentModeChanged, 11))
            return;
        if (QtMocHelpers::indexOfMethod<void (MusicController::*)()>(_a, &MusicController::queueCountChanged, 12))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->getCurrentTitle(); break;
        case 1: *reinterpret_cast<QString*>(_v) = _t->getCurrentArtist(); break;
        case 2: *reinterpret_cast<QString*>(_v) = _t->getCurrentAlbum(); break;
        case 3: *reinterpret_cast<qint64*>(_v) = _t->getCurrentPosition(); break;
        case 4: *reinterpret_cast<qint64*>(_v) = _t->getTotalDuration(); break;
        case 5: *reinterpret_cast<int*>(_v) = _t->getVolume(); break;
        case 6: *reinterpret_cast<bool*>(_v) = _t->getIsPlaying(); break;
        case 7: *reinterpret_cast<bool*>(_v) = _t->getIsShuffle(); break;
        case 8: *reinterpret_cast<bool*>(_v) = _t->getIsRepeat(); break;
        case 9: *reinterpret_cast<QStringList*>(_v) = _t->getPlaylistItems(); break;
        case 10: *reinterpret_cast<int*>(_v) = _t->getCurrentTrackIndex(); break;
        case 11: *reinterpret_cast<int*>(_v) = _t->getQueueCount(); break;
        case 12: *reinterpret_cast<int*>(_v) = _t->getCurrentMode(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 5: _t->setVolume(*reinterpret_cast<int*>(_v)); break;
        case 7: _t->setShuffle(*reinterpret_cast<bool*>(_v)); break;
        case 8: _t->setRepeat(*reinterpret_cast<bool*>(_v)); break;
        case 10: _t->setCurrentTrackIndex(*reinterpret_cast<int*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *MusicController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MusicController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15MusicControllerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MusicController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 34)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 34;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 34)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 34;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void MusicController::currentTitleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void MusicController::currentArtistChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void MusicController::currentAlbumChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void MusicController::positionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void MusicController::durationChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void MusicController::volumeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void MusicController::playingStateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void MusicController::shuffleStateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void MusicController::repeatStateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void MusicController::playlistChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void MusicController::currentTrackIndexChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void MusicController::currentModeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void MusicController::queueCountChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}
QT_WARNING_POP
