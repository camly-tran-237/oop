#include <QApplication>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QWidget>
#include "Piano.hpp"
#include "SongPlayer.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Tạo cửa sổ chính
    QWidget mainWindow;
    mainWindow.setWindowTitle("Virtual Piano");
    mainWindow.setFixedSize(1000, 500);
    
    // Tạo layout chính
    QVBoxLayout* mainLayout = new QVBoxLayout(&mainWindow);
    
    // Tạo piano
    Piano* piano = new Piano(&mainWindow);
    mainLayout->addWidget(piano);
    
    // Tạo song player
    SongPlayer* songPlayer = new SongPlayer(piano);
    
    // Tạo layout cho controls
    QHBoxLayout* controlsLayout = new QHBoxLayout();
    
    // Tạo combo box để chọn bài hát
    QComboBox* songComboBox = new QComboBox(&mainWindow);
    for (const Song& song : songPlayer->getSongs()) {
        songComboBox->addItem(song.getName());
    }
    controlsLayout->addWidget(songComboBox);
    
    // Tạo nút play/stop
    QPushButton* playButton = new QPushButton("Play", &mainWindow);
    controlsLayout->addWidget(playButton);
    
    mainLayout->addLayout(controlsLayout);
    
    // Kết nối các sự kiện
    QObject::connect(playButton, &QPushButton::clicked, [&]() {
        if (songPlayer->isPlaying()) {
            songPlayer->stopPlaying();
            playButton->setText("Play");
        } else {
            songPlayer->startPlaying(songComboBox->currentIndex());
            playButton->setText("Stop");
        }
    });
    
    // Kết nối sự kiện khi bài hát kết thúc
    QObject::connect(songPlayer, &SongPlayer::songFinished, [&]() {
        playButton->setText("Play");
    });
    
    mainWindow.show();
    return app.exec();
}
