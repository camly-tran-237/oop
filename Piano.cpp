#include <QPainter> //Để vẽ giao diện
#include <QKeyEvent> //Để xử lý sự kiện nhấn phím
#include <cmath> //Để tính toán
#include <QAudioOutput> //Để phát âm thanh
#include <QUrl> //Để tải âm thanh
#include <QSet> //Để lưu trữ các phím đang được nhấn
#include <QFile> //Để kiểm tra xem file có tồn tại hay không
#include <QDebug> //Để debug
#include "Piano.hpp" //Để sử dụng lớp Piano
#include <QMouseEvent> //Thêm include QMouseEvent
#include <QMessageBox> //Thêm include QMessageBox
#include <QTimer> //Thêm include QTimer
#include <QCoreApplication> //Thêm include QCoreApplication

Piano::Piano(QWidget *parent) : QWidget(parent) //Khởi tạo constructor
{
    setFixedSize(1000, 400); //Để đặt kích thước của cửa sổ
    setFocusPolicy(Qt::StrongFocus);  // Thêm dòng này để widget luôn nhận focus
    loadSounds(); //Để tải âm thanh
    createPianoKeys(); //Tạo các phím đàn
}

Piano::~Piano() //Khởi tạo destructor   
{
    //Dừng tất cả âm thanh và giải phóng bộ nhớ
    for (QMediaPlayer* player : sounds) {
        if (player) { //Nếu player tồn tại 
            player->stop(); //Dừng âm thanh
            delete player->audioOutput(); //Giải phóng audioOutput
            delete player; //Giải phóng player
        }
    }
    sounds.clear();
    
    //Giải phóng bộ nhớ cho các phím đàn
    for (PianoKey* key : pianoKeys) {
        delete key;
    }
    pianoKeys.clear();
}

void Piano::loadSounds()
{
    qDebug() << "Loading sounds";
    
    // Sử dụng file âm thanh piano_D4.wav
    QString soundPath = "piano/Sounds/piano_D4.wav";
    
    if (!QFile::exists(soundPath)) {
        qWarning() << "Sound file not found:" << soundPath;
        // Thử tìm file ở các vị trí khác
        QStringList possiblePaths = {
            "Sounds/piano_D4.wav",
            "./Sounds/piano_D4.wav",
            "../Sounds/piano_D4.wav",
            "../../Sounds/piano_D4.wav",
            QCoreApplication::applicationDirPath() + "/Sounds/piano_D4.wav"
        };
        
        for (const QString &path : possiblePaths) {
            if (QFile::exists(path)) {
                soundPath = path;
                qDebug() << "Found sound file at:" << path;
                break;
            }
        }
    } else {
        qDebug() << "Found sound file at:" << soundPath;
    }
    
    // Xóa các sound cũ
    for (QMediaPlayer* player : sounds) {
        if (player) {
            player->stop();
            if (player->audioOutput()) {
                delete player->audioOutput();
            }
            delete player;
        }
    }
    sounds.clear();
    
    // Tạo 24 player để phát âm thanh (14 phím trắng + 10 phím đen)
    for (int i = 0; i < 24; ++i) {
        QMediaPlayer *player = new QMediaPlayer(this);
        if (!player) {
            qDebug() << "Failed to create player" << i;
            continue;
        }
        
        QAudioOutput *audioOutput = new QAudioOutput(this);
        if (!audioOutput) {
            qDebug() << "Failed to create audio output" << i;
            delete player;
            continue;
        }

        player->setAudioOutput(audioOutput);
        player->setSource(QUrl::fromLocalFile(soundPath));
        audioOutput->setVolume(100);
        
        // Tính toán pitch dựa trên vị trí của phím
        // Sử dụng nốt D4 (Re) làm nốt cơ bản
        float pitchFactor = std::pow(2.0f, (i - 2) / 12.0f);
        player->setPlaybackRate(pitchFactor);
        
        qDebug() << "Created player" << i << "with pitch factor" << pitchFactor;
        sounds.append(player);
    }
    
    qDebug() << "Loaded" << sounds.size() << "sounds";
}

void Piano::paintEvent(QPaintEvent *) //Khởi tạo hàm paintEvent để vẽ giao diện
{
    qDebug() << "Painting piano with" << pianoKeys.size() << "keys";
    
    QPainter painter(this); //Khởi tạo painter
    
    // Vẽ các phím đàn
    for (PianoKey* key : pianoKeys) {
        key->draw(painter);
    }
}

int Piano::mapKeyToIndex(int key)
{
    // Map các phím trắng (Q, W, E, R, T, Y, U, I, O, P, [, ], \, A)
    static const QMap<int, int> whiteKeyMap = {
        {Qt::Key_Q, 0},        // C
        {Qt::Key_W, 1},        // D
        {Qt::Key_E, 2},        // E
        {Qt::Key_R, 3},        // F
        {Qt::Key_T, 4},        // G
        {Qt::Key_Y, 5},        // A
        {Qt::Key_U, 6},        // B
        {Qt::Key_I, 7},        // C
        {Qt::Key_O, 8},        // D
        {Qt::Key_P, 9},        // E
        {Qt::Key_BracketLeft, 10},  // F
        {Qt::Key_BracketRight, 11}, // G
        {Qt::Key_Backslash, 12},    // A
        {Qt::Key_A, 13}             // B
    };
    
    // Map các phím đen (1-0)
    static const QMap<int, int> blackKeyMap = {
        {Qt::Key_1, 14},   // C#
        {Qt::Key_2, 15},   // D#
        {Qt::Key_3, 16},   // F#
        {Qt::Key_4, 17},   // G#
        {Qt::Key_5, 18},   // A#
        {Qt::Key_6, 19},   // C#
        {Qt::Key_7, 20},   // D#
        {Qt::Key_8, 21},   // F#
        {Qt::Key_9, 22},   // G#
        {Qt::Key_0, 23}    // A#
    };
    
    // Kiểm tra phím trắng trước
    if (whiteKeyMap.contains(key)) {
        return whiteKeyMap[key];
    }
    
    // Sau đó kiểm tra phím đen
    if (blackKeyMap.contains(key)) {
        return blackKeyMap[key];
    }
    
    return -1;
}

void Piano::keyPressEvent(QKeyEvent *event)
{
    // Bỏ qua phím lặp lại
    if (event->isAutoRepeat()) {
        return;
    }
    
    int keyIndex = mapKeyToIndex(event->key());
    if (keyIndex != -1 && !sounds.isEmpty()) {
        // Đánh dấu sự kiện đã được xử lý
        event->accept();
        
        // Thêm phím vào danh sách phím đang nhấn
        pressedKeys.insert(keyIndex);
        
        // Cập nhật trạng thái nhấn của phím đàn
        for (PianoKey* key : pianoKeys) {
            if (key->getKey() == keyIndex) {
                key->setPressed(true);
                break;
            }
        }
        
        // Phát âm thanh
        int soundIndex = keyIndex % sounds.size();
        QMediaPlayer* player = sounds[soundIndex];
        if (player && player->audioOutput()) {
            player->stop();
            player->play();
            
            // Reset note sau khi phát (tăng thời gian lên 300ms)
            QTimer::singleShot(300, this, [this, keyIndex]() {
                pressedKeys.remove(keyIndex);
                for (PianoKey* key : pianoKeys) {
                    if (key->getKey() == keyIndex) {
                        key->setPressed(false);
                        break;
                    }
                }
                update();
            });
        }
        
        // Cập nhật giao diện
        update();
    }
}

void Piano::keyReleaseEvent(QKeyEvent *event)
{
    // Bỏ qua phím lặp lại
    if (event->isAutoRepeat()) {
        return;
    }
    
    int keyIndex = mapKeyToIndex(event->key());
    if (keyIndex != -1) {
        // Xóa phím khỏi danh sách phím đang nhấn
        pressedKeys.remove(keyIndex);
        
        // Cập nhật trạng thái nhấn của phím đàn
        for (PianoKey* key : pianoKeys) {
            if (key->getKey() == keyIndex) {
                key->setPressed(false);
                break;
            }
        }
        
        // Cập nhật giao diện
        update();
    }
}

void Piano::createPianoKeys()
{
    qDebug() << "Creating piano keys";
    
    // Xóa các phím cũ trước khi tạo mới
    qDeleteAll(pianoKeys);
    pianoKeys.clear();
    
    float keyWidth = width() / 14.0f;
    float keyHeight = height();
    
    // Tạo các phím trắng (index 0-13)
    for (int i = 0; i < 14; ++i) {
        QRectF keyRect(i * keyWidth, 0, keyWidth, keyHeight);
        // Tính pitch dựa trên vị trí của phím, điều chỉnh cho phù hợp với thang âm của "Bạc Phận"
        // Sử dụng thang âm Dm (Re thứ) làm cơ sở
        double pitch = std::pow(2.0, (i - 2) / 12.0); // Điều chỉnh offset để bắt đầu từ nốt Re
        PianoKey* key = new PianoKey(keyRect, Qt::white, i, pitch, "piano/Sounds/piano_D4.wav");
        pianoKeys.append(key);
        qDebug() << "Created white key" << i << "with pitch" << pitch;
    }
    
    // Tạo các phím đen (index 14-23)
    float blackKeyWidth = keyWidth * 0.6f;
    float blackKeyHeight = keyHeight * 0.7f;
    int blackKeyPositions[] = {0, 1, 3, 4, 5, 7, 8, 10, 11, 12}; // Vị trí của các phím đen
    
    for (int i = 0; i < 10; ++i) {
        float xPos = (blackKeyPositions[i] + 0.75f) * keyWidth;
        QRectF blackKeyRect(xPos, 0, blackKeyWidth, blackKeyHeight);
        // Tính pitch cho phím đen, điều chỉnh cho phù hợp với thang âm của "Bạc Phận"
        double pitch = std::pow(2.0, (i + 14 - 2) / 12.0); // Điều chỉnh offset để bắt đầu từ nốt Re
        PianoKey* key = new PianoKey(blackKeyRect, Qt::black, i + 14, pitch, "piano/Sounds/piano_D4.wav");
        pianoKeys.append(key);
        qDebug() << "Created black key" << (i + 14) << "with pitch" << pitch;
    }
    
    qDebug() << "Created" << pianoKeys.size() << "piano keys";
}

void Piano::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "Mouse pressed at:" << event->pos();
    
    QPointF pos = event->pos();
    
    // Kiểm tra phím đen trước (index 14-23)
    for (int i = 14; i < pianoKeys.size(); ++i) {
        PianoKey* key = pianoKeys[i];
        if (key->contains(pos)) {
            int keyIndex = key->getKey();
            if (!pressedKeys.contains(keyIndex)) {
                pressedKeys.insert(keyIndex);
                key->setPressed(true);
                int soundIndex = keyIndex % sounds.size();
                if (soundIndex >= 0 && soundIndex < sounds.size()) {
                    QMediaPlayer* player = sounds[soundIndex];
                    if (player && player->audioOutput()) {
                        player->stop();
                        player->play();
                    }
                }
            }
            update();
            return;
        }
    }
    
    // Nếu không tìm thấy phím đen, kiểm tra phím trắng (index 0-13)
    for (int i = 0; i < 14; ++i) {
        PianoKey* key = pianoKeys[i];
        if (key->contains(pos)) {
            int keyIndex = key->getKey();
            if (!pressedKeys.contains(keyIndex)) {
                pressedKeys.insert(keyIndex);
                key->setPressed(true);
                int soundIndex = keyIndex % sounds.size();
                if (soundIndex >= 0 && soundIndex < sounds.size()) {
                    QMediaPlayer* player = sounds[soundIndex];
                    if (player && player->audioOutput()) {
                        player->stop();
                        player->play();
                    }
                }
            }
            update();
            return;
        }
    }
}

void Piano::mouseReleaseEvent(QMouseEvent *event)
{
    QPointF pos = event->pos();
    
    // Kiểm tra phím đen trước (index 14-23)
    for (int i = 14; i < pianoKeys.size(); ++i) {
        PianoKey* key = pianoKeys[i];
        if (key->contains(pos)) {
            int keyIndex = key->getKey();
            if (pressedKeys.contains(keyIndex)) {
                pressedKeys.remove(keyIndex);
                key->setPressed(false);
                update();
            }
            return;
        }
    }
    
    // Nếu không tìm thấy phím đen, kiểm tra phím trắng (index 0-13)
    for (int i = 0; i < 14; ++i) {
        PianoKey* key = pianoKeys[i];
        if (key->contains(pos)) {
            int keyIndex = key->getKey();
            if (pressedKeys.contains(keyIndex)) {
                pressedKeys.remove(keyIndex);
                key->setPressed(false);
                update();
            }
            return;
        }
    }
}

void Piano::playKey(int key) {
    if (key >= 0 && key < sounds.size()) {
        // Thêm phím vào danh sách phím đang nhấn
        pressedKeys.insert(key);
        
        // Cập nhật trạng thái nhấn của phím đàn
        for (PianoKey* pianoKey : pianoKeys) {
            if (pianoKey->getKey() == key) {
                pianoKey->setPressed(true);
                break;
            }
        }
        
        // Phát âm thanh
        int soundIndex = key % sounds.size();
        QMediaPlayer* player = sounds[soundIndex];
        if (player && player->audioOutput()) {
            player->stop();
            player->play();
        }
        
        update();
        
        // Tự động nhả phím sau một khoảng thời gian ngắn
        QTimer::singleShot(300, this, [this, key]() {
            pressedKeys.remove(key);
            for (PianoKey* pianoKey : pianoKeys) {
                if (pianoKey->getKey() == key) {
                    pianoKey->setPressed(false);
                    break;
                }
            }
            update();
        });
    }
}

// Thêm hàm focusInEvent để đảm bảo widget nhận được sự kiện bàn phím
void Piano::focusInEvent(QFocusEvent *event)
{
    QWidget::focusInEvent(event);
    setFocusPolicy(Qt::StrongFocus);
}

// Thêm hàm focusOutEvent để xử lý khi widget mất focus
void Piano::focusOutEvent(QFocusEvent *event)
{
    QWidget::focusOutEvent(event);
    // Reset tất cả các phím đang nhấn khi mất focus
    pressedKeys.clear();
    for (PianoKey* key : pianoKeys) {
        key->setPressed(false);
    }
    update();
}
