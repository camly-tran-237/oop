#ifndef SONGPLAYER_HPP
#define SONGPLAYER_HPP

#include <QObject>
#include <QTimer>
#include <QVector>
#include "Piano.hpp"
#include "Song.hpp"

class SongPlayer : public QObject {
    Q_OBJECT

public:
    SongPlayer(Piano* piano, QObject* parent = nullptr);
    ~SongPlayer();

    void addSong(const Song& song);
    void startPlaying(int songIndex);
    void stopPlaying();
    bool isPlaying() const;
    const QVector<Song>& getSongs() const;

signals:
    void songFinished();

private slots:
    void playNextNote();

private:
    void loadSongs();

    Piano* piano;
    QTimer* timer;
    QVector<Song> songs;
    int currentSongIndex;
    int currentNoteIndex;
    bool playing;
};

#endif // SONGPLAYER_HPP