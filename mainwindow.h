#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QElapsedTimer>
#include <QSoundEffect>
#include <QVector>
#include <QPixmap>
#include <QPointF>
#include <QLineEdit> // [CHANGE] Added for name input
#include <QPushButton> // [CHANGE] Added for menu buttons

#include "leaderboardmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// In mainwindow.h

struct Egg {
    QPointF pos;
    float prevY;
    float yVelocity;
    QString state;
    float animTimer = 0;
    float scale = 1.0f;
    float alpha = 1.0f;
    QString type;    // "normal", "bad", "life"
    QColor color;    // visual tint
};

struct Particle {
    QPoint pos;      // integer position (grid units scaled)
    QPoint velocity; // integer velocity (scaled)
    int lifetime;    // in "ticks" (e.g., 60 = 1 second at 60 FPS)
    int alpha;       // 0-255
    QColor color;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void gameTick();
    void startGameButtonClicked();
    void showLeaderboardButtonClicked();

private:

    LeaderboardManager leaderboardManager;
    QString playerName = "Player"; // [CHANGE] Default player name

    // [CHANGE] UI elements for the menu/leaderboard
    QPushButton *playButton;
    QPushButton *leaderboardButton;
    QLineEdit *nameInput;
    // [CHANGE] Button to start game from leaderboard screen
    QPushButton *backToMenuButton;

    QVector<Particle> particles;

    int highScore;

    Ui::MainWindow *ui;
    QTimer *gameTimer;
    QElapsedTimer frameClock;

    QPixmap background;

    int grid_box;
    int grid_size;
    int cols;
    int rows;

    float globalTime;
    float globalSpawnTimer;
    float spawnInterval;
    float lastEdgeSpawnTime;
    float edgeSpawnCooldown;

    int currentColumnIndex;

    QVector<int> dropColumns;
    QVector<float> columnTimers;
    QVector<float> columnDelays;

    QVector<Egg> eggs;

    QPointF basket;
    float prevBasketX;
    float basketXVelocity;
    float basketTargetVel;
    float basketAccel;
    float basketMaxVel;

    bool moveLeft;
    bool moveRight;
    float flashAlpha;
    float flashFadeSpeed;
    QColor flashColor;
    float flashTimer;

    float fixedDelta;
    float accumulator;

    float scoreScale = 1.0f;
    float scoreAnimTimer = 0.0f;
    bool scoreChanged = false;

    float livesPulseTimer = 0.0f;
    bool livesChanged = false;

    bool gameOver;
    bool gameRunning;
    bool showMenu;
    bool showLeaderboard;

    int score;
    int lives;

    QSoundEffect soundCatch;
    QSoundEffect soundLose;

    // ---- Wind system ----
    bool windActive;
    float windStrength;      // grid cells per second, +/- for left/right
    float windTimer;         // remaining time for current wind event
    float windCooldown;      // minimum time between wind events
    float timeSinceLastWind; // time since last wind ended

    // ---- Focus mode ----
    bool focusMode;          // true when in focus mode (score-based cycles)

    // ---- Utility Methods ----
    void resetGame();
    void updatePhysics(float dt);
    void drawGame(float alpha);
    void drawGameOver();
    void drawMenu();
    void drawLeaderboard();
    void drawStartScreen();
    void drawEggShape(QPainter &p, const Egg &egg, float cellSize);
    void loadHighScore();
    void saveHighScore();
    void handleGameOver();
};

#endif // MAINWINDOW_H
