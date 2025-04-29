#ifndef SONG_HPP
#define SONG_HPP

#include <QString>
#include <QVector>
#include <QPair>

typedef QPair<int, int> Note;

class Song {
public:
    Song(const QString& name, const QVector<Note>& notes);
    QString getName() const;
    const QVector<Note>& getNotes() const;

private:
    QString name;
    QVector<Note> notes;
};

#endif // SONG_HPP