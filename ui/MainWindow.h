#pragma once
#include <QMainWindow>
#include <QStackedWidget>
#include <QString>
#include "core/Game.h"
#include "core/GameLoop.h"
#include "screen/AboutDialog.h"

class GameWidget;
class MainMenuScreen;
class GameOverlayScreen;
class QuizDialog;
struct PuzzleData;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
private slots:
    void onStateChanged(GameState oldState, GameState newState);
    void onGameTick(int deltaTimeMs);
    void onPuzzleRequired(const PuzzleData& puzzle);
    void onQuizFinished(bool passed, const QString& objectId);
private:
    Game* m_game;
    GameLoop* m_gameLoop;
    QStackedWidget* m_stack;
    GameWidget* m_gameWidget;
    MainMenuScreen* m_menuScreen;

    GameOverlayScreen* m_pauseScreen;
    GameOverlayScreen* m_gameOverScreen;
    GameOverlayScreen* m_winScreen;
    QuizDialog* m_quizScreen;

    void buildUI();
    void connectSignals();
};