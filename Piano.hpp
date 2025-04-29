#ifndef PIANO_HPP
#define PIANO_HPP

#include "Pianokey.hpp"
#include <QVector>
#include <QSet>
#include <QKeyEvent>
#include <QWidget>
#include <QMediaPlayer>
#include <QPainter>
#include <QMouseEvent>
#include <QFocusEvent>

// Lớp Piano quản lý toàn bộ giao diện và hoạt động của đàn piano
class Piano : public QWidget {
    Q_OBJECT

public:
    // Hàm khởi tạo và hủy
    Piano(QWidget *parent = nullptr);
    ~Piano();
    
    // Các hàm xử lý tương tác từ người dùng
    void keyPressEvent(QKeyEvent *event) override;     // Xử lý khi nhấn phím
    void keyReleaseEvent(QKeyEvent *event) override;   // Xử lý khi thả phím
    void paintEvent(QPaintEvent *event) override;      // Vẽ giao diện đàn piano
    void mousePressEvent(QMouseEvent *event) override; // Xử lý khi nhấn chuột
    void mouseReleaseEvent(QMouseEvent *event) override; // Xử lý khi thả chuột
    void focusInEvent(QFocusEvent *event) override;    // Xử lý khi cửa sổ được chọn
    void focusOutEvent(QFocusEvent *event) override;   // Xử lý khi cửa sổ mất focus
    
    // Hàm phát âm thanh
    void playKey(int key);                            // Phát âm thanh cho phím được chỉ định

private:
    // Các thành phần chính của đàn piano
    QVector<QMediaPlayer*> sounds;     // Danh sách âm thanh các nốt nhạc
    QSet<int> pressedKeys;            // Danh sách các phím đang được nhấn
    QVector<PianoKey*> pianoKeys;     // Danh sách các phím đàn
    
    // Các hàm hỗ trợ xử lý
    int mapKeyToIndex(int key);       // Chuyển đổi mã phím thành số thứ tự phím đàn
    void loadSounds();                // Tải các file âm thanh
    void createPianoKeys();           // Khởi tạo các phím đàn
};

#endif // PIANO_HPP
