#include "Song.hpp"

Song::Song(const QString& name, const QVector<Note>& notes)
    : name(name), notes(notes) {
}

QString Song::getName() const {
    return name;
}

const QVector<Note>& Song::getNotes() const {
    return notes;
}