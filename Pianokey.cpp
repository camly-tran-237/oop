#include "Pianokey.hpp"
#include <QPainter>
#include <QUrl>
#include <QAudioOutput>
#include <QFile>
#include <QDebug>
#include <QMessageBox>

//Hàm khởi tạo:constructor
PianoKey::PianoKey(const QRectF &rect, const QColor &color, int key, double pitch, const QString &soundPath)
    : rect(rect), color(color), key(key), pitch(pitch), pressed(false), player(nullptr)
{
    qDebug() << "Creating PianoKey" << key << "with sound path:" << soundPath;
    
    // Không tạo player nếu không có đường dẫn âm thanh
    if (soundPath.isEmpty()) {
        qDebug() << "No sound path provided for key" << key;
        return;
    }

    if (pitch <= 0.0) {
        qWarning() << "Invalid pitch value" << pitch << "for key" << key << "- using default 1.0";
        pitch = 1.0;
    }
    
    //Khởi tạo player để phát âm thanh  
    player = new QMediaPlayer;
    QAudioOutput *audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);
    
    //Kiểm tra file âm thanh có tồn tại không   
    if (QFile::exists(soundPath)) {
        qDebug() << "Sound file found for key" << key;
        //Nếu tồn tại thì thiết lập nguồn âm thanh
        player->setSource(QUrl::fromLocalFile(soundPath));
        
        // Check if the source was set successfully
        if (player->mediaStatus() == QMediaPlayer::InvalidMedia) {
            qWarning() << "Failed to load sound file for key" << key << "- invalid media";
            delete audioOutput;
            delete player;
            player = nullptr;
            return;
        }
        
        //Thiết lập tốc độ phát âm thanh
        player->setPlaybackRate(pitch);
        //Thiết lập âm lượng phát âm thanh
        audioOutput->setVolume(50);
        qDebug() << "Successfully initialized sound for key" << key;
    } else {
        //Nếu không tồn tại thì hiển thị thông báo lỗi
        qWarning() << "Sound file not found:" << soundPath;
        delete audioOutput;
        delete player;
        player = nullptr;
    }
}
//Hàm hủy:destructor
PianoKey::~PianoKey()
{
    //Nếu player tồn tại thì dừng âm thanh và giải phóng bộ nhớ
    if (player) {
        player->stop();
        if (player->audioOutput()) { //Nếu player có audioOutput thì giải phóng audioOutput 
            delete player->audioOutput();
        }
        delete player; //Giải phóng player
    }
}
//Hàm phát âm thanh:playSound
void PianoKey::playSound()
{
    qDebug() << "Playing sound for key" << key;
    
    if (player) {
        player->stop(); //Dừng âm thanh 
        player->play(); //Phát âm thanh
    } else {
        qWarning() << "Cannot play sound for key" << key << "- no sound loaded";
    }
}
//Hàm vẽ khóa piano:draw
void PianoKey::draw(QPainter &painter) const
{   
    // Thiết lập màu sắc của khóa piano
    if (pressed) {
        if (color == Qt::white) {
            // Nếu phím trắng được nhấn, đổi màu thành xanh nhạt
            painter.setBrush(QColor(0, 170, 255));
        } else if (color == Qt::black) {
            // Nếu phím đen được nhấn, đổi màu thành xám đậm
            painter.setBrush(QColor(40, 40, 40));
        } else {
            painter.setBrush(color);
        }
    } else {
        painter.setBrush(color);
    }
    
    // Thiết lập màu sắc của viền khóa piano
    painter.setPen(Qt::black);
    
    // Vẽ khóa piano
    painter.drawRect(rect);
}
//Hàm kiểm tra xem điểm có nằm trong khóa piano hay không:contains
bool PianoKey::contains(const QPointF &point) const
{
    bool result = rect.contains(point);
    qDebug() << "Checking if point" << point << "is in key" << key << ":" << result;
    return result;
}
//Hàm lấy key của khóa piano:getKey
int PianoKey::getKey() const
{
    return key; //Trả về key của khóa piano
}

void PianoKey::setPressed(bool isPressed)
{
    pressed = isPressed;
}

bool PianoKey::isPressed() const
{
    return pressed;
}
