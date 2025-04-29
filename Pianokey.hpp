#ifndef PIANOKEY_HPP
#define PIANOKEY_HPP

#include <QRectF>
#include <QColor>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QPainter>

// Lớp PianoKey quản lý thông tin và hoạt động của một phím đàn piano
class PianoKey {
public:
    // Hàm khởi tạo và hủy
    PianoKey(const QRectF &rect,      // Hình dạng và kích thước phím
             const QColor &color,      // Màu sắc phím (trắng/đen)
             int key,                  // Số thứ tự phím
             double pitch,             // Cao độ của nốt nhạc
             const QString &soundPath); // Đường dẫn tới file âm thanh
    ~PianoKey();
    
    // Các hàm xử lý âm thanh và hiển thị
    void playSound();                  // Phát âm thanh của phím
    void draw(QPainter &painter) const; // Vẽ hình dạng phím đàn
    bool contains(const QPointF &point) const; // Kiểm tra tọa độ chuột có nằm trên phím không
    
    // Các hàm truy xuất và cập nhật trạng thái
    int getKey() const;                // Lấy số thứ tự phím
    void setPressed(bool isPressed);   // Cập nhật trạng thái nhấn
    bool isPressed() const;            // Kiểm tra trạng thái nhấn

private:
    // Các thuộc tính của phím đàn
    QRectF rect;         // Hình dạng và vị trí phím
    QColor color;        // Màu sắc phím
    int key;            // Số thứ tự phím
    double pitch;       // Cao độ nốt nhạc
    bool pressed;       // Trạng thái nhấn
    QMediaPlayer *player; // Đối tượng phát âm thanh
};

#endif // PIANOKEY_HPP