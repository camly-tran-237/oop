#include "SongPlayer.hpp"
#include <QDebug>

SongPlayer::SongPlayer(Piano* piano, QObject* parent)
    : QObject(parent), piano(piano), timer(new QTimer(this)),
    currentSongIndex(-1), currentNoteIndex(0), playing(false)
{
    connect(timer, &QTimer::timeout, this, &SongPlayer::playNextNote);
    loadSongs();
}

SongPlayer::~SongPlayer() {
    if (timer) {
        timer->stop();
        delete timer;
    }
}

void SongPlayer::addSong(const Song& song) {
    songs.append(song);
}

void SongPlayer::startPlaying(int songIndex) {
    if (songIndex < 0 || songIndex >= songs.size()) {
        qDebug() << "Invalid song index:" << songIndex;
        return;
    }

    currentSongIndex = songIndex;
    currentNoteIndex = 0;
    playing = true;
    
    qDebug() << "Starting to play:" << songs[currentSongIndex].getName();
    playNextNote();
}

void SongPlayer::stopPlaying() {
    playing = false;
    timer->stop();
    currentNoteIndex = 0;
    qDebug() << "Stopped playing";
}

bool SongPlayer::isPlaying() const {
    return playing;
}

const QVector<Song>& SongPlayer::getSongs() const {
    return songs;
}

void SongPlayer::playNextNote() {
    if (!playing || currentSongIndex < 0) {
        return;
    }

    const Song& currentSong = songs[currentSongIndex];
    const QVector<QPair<int, int>>& notes = currentSong.getNotes();

    if (currentNoteIndex >= notes.size()) {
        stopPlaying();
        emit songFinished();
        return;
    }

    const QPair<int, int>& note = notes[currentNoteIndex];
    piano->playKey(note.first);
    timer->start(note.second);
    currentNoteIndex++;
}

void SongPlayer::loadSongs() {
    songs.clear();

    // Happy Birthday
    QVector<QPair<int, int>> happyBirthday = {
        {4, 400}, {4, 400}, {6, 600}, {4, 600}, {9, 600}, {8, 1000},
        {4, 400}, {4, 400}, {6, 600}, {4, 600}, {11, 600}, {9, 1000},
        {4, 400}, {4, 400}, {16, 600}, {13, 600}, {9, 600}, {8, 600}, {6, 1000},
        {14, 400}, {14, 400}, {13, 600}, {9, 600}, {11, 600}, {9, 1000}
    };
    addSong(Song("Happy Birthday", happyBirthday));

    // Twinkle Twinkle Little Star
    QVector<QPair<int, int>> twinkle = {
    {7, 400}, {7, 400}, {14, 400}, {14, 400}, {16, 400}, {16, 400}, {14, 800},
    {12, 400}, {12, 400}, {11, 400}, {11, 400}, {9, 400}, {9, 400}, {7, 800},
    {14, 400}, {14, 400}, {12, 400}, {12, 400}, {11, 400}, {11, 400}, {9, 800},
    {14, 400}, {14, 400}, {12, 400}, {12, 400}, {11, 400}, {11, 400}, {9, 800}
};

    addSong(Song("Twinkle Twinkle Little Star", twinkle));

  QVector<QPair<int, int>> jingleBells = {
    // Jingle Bells, Jingle Bells, Jingle all the way
    {9, 400}, {9, 400}, {9, 800},
    {9, 400}, {9, 400}, {9, 800},
    // Oh what fun it is to ride in a one horse open sleigh
    {9, 400}, {12, 400}, {5, 400}, {7, 400},
    {9, 800},

    // Jingle Bells, Jingle Bells, Jingle all the way
    {10, 400}, {10, 400}, {10, 300}, {10, 500},
    {10, 500}, {9, 400}, {9, 400}, {9, 350},

    // Oh what fun it is to ride
    {9, 350}, {7, 400}, {7, 400}, {9, 400},
    // In a one horse open sleigh
    {7, 800}, {12, 800},

    
};
    addSong(Song("Jingle Bells", jingleBells));

    // Mary Had a Little Lamb
    QVector<QPair<int, int>> mary = {
    // Mary had a little lamb
    {5, 400}, {3, 400}, {1, 400}, {3, 400},
    {5, 400}, {5, 400}, {5, 800},

    // Little lamb, little lamb
    {3, 400}, {3, 400}, {3, 800},

    // Mary had a little lamb
    {5, 400}, {8, 400}, {8, 800},

    // Its fleece was white as snow
    {5, 400}, {3, 400}, {1, 400}, {3, 400},
    {5, 400}, {5, 400}, {5, 400}, {5, 400},
    {3, 400}, {3, 400}, {5, 400}, {3, 400},
    {1, 1000}
};



    addSong(Song("Mary Had a Little Lamb", mary));

    // Für Elise (một phần chuẩn hóa)
    QVector<QPair<int, int>> furElise = {
    {11, 300}, {18, 300}, {11, 300}, {18, 300}, {11, 300}, {15, 300}, {17, 300}, {16, 300},
    {10, 600}, {13, 300}, {15, 300}, {16, 300},
    {11, 600}, {13, 300}, {15, 300}, {16, 300},
    {11, 300}, {18, 300}, {11, 300}, {18, 300}, {11, 300}, {15, 300}, {17, 300}, {16, 300},
    {10, 600}, {13, 300}, {15, 300}, {16, 300},
    {11, 800}
};

    addSong(Song("Für Elise", furElise));

    qDebug() << "Loaded" << songs.size() << "songs:";
    for (const Song& song : songs) {
        qDebug() << "-" << song.getName();
    }
}


