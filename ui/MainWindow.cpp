#include "ui/MainWindow.h"
#include "ui/GameWidget.h"
#include "sprite/UISpriteSheet.h"
#include "screen/MainMenuScreen.h"
#include "screen/GameOverlayScreen.h"
#include "systems/ScoreManager.h"
#include "screen/QuizDialog.h"
#include "core/Constants.h"
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setFixedSize(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT);
    m_game     = new Game(this);
    m_gameLoop = new GameLoop(this);
    buildUI();
    connectSignals();
    UISpriteSheet::init(":/assets/sprites/ui/ui_theme.png", ":/assets/sprites/ui/ui_theme.json");
}

void MainWindow::buildUI() {
    m_stack = new QStackedWidget(this);
    m_menuScreen = new MainMenuScreen(this);
    m_gameWidget = new GameWidget(m_game, this);
    m_pauseScreen = new GameOverlayScreen(m_game, GameOverlayScreen::Type::Pause, this);
    m_gameOverScreen = new GameOverlayScreen(m_game, GameOverlayScreen::Type::GameOver, this);
    m_winScreen = new GameOverlayScreen(m_game, GameOverlayScreen::Type::Victory, this);
    m_quizScreen = new QuizDialog(m_game, this);

    m_stack->addWidget(m_menuScreen);
    m_stack->addWidget(m_gameWidget);
    m_stack->addWidget(m_pauseScreen);
    m_stack->addWidget(m_gameOverScreen);
    m_stack->addWidget(m_winScreen);
    m_stack->addWidget(m_quizScreen);
    m_stack->setCurrentIndex(0);
    setCentralWidget(m_stack);

    QMenuBar* nativeMenuBar = this->menuBar();
    QMenu* helpMenu = nativeMenuBar->addMenu(tr("&Help"));
    QAction* aboutAction = helpMenu->addAction(tr("About Scavenger Hunt"));
    aboutAction->setMenuRole(QAction::AboutRole);
    connect(aboutAction, &QAction::triggered, this, [this]{
        AboutDialog dialog(this);
        dialog.exec();
    });
}
void MainWindow::connectSignals() {

    connect(m_game, &Game::stateChanged, this, &MainWindow::onStateChanged);

    connect(m_game, &Game::puzzleRequired,
            this, &MainWindow::onPuzzleRequired);

    connect(m_quizScreen, &QuizDialog::quizFinished,
            this, &MainWindow::onQuizFinished);

    connect(m_gameLoop, &GameLoop::tick, this, &MainWindow::onGameTick);

    connect(m_menuScreen, &MainMenuScreen::startRequested,
            this, [this](){ m_game->startNewGame(0); m_gameLoop->start(); });

    connect(m_menuScreen, &MainMenuScreen::levelSelected,
            this, [this](int levelNumber){ m_game->startNewGame(levelNumber); m_gameLoop->start(); });

    connect(m_pauseScreen, &GameOverlayScreen::resumeRequested,
            this, [this](){ m_game->resumeGame(); m_gameLoop->start(); });
    connect(m_pauseScreen, &GameOverlayScreen::restartRequested,
            this, [this](){ m_game->restartLevel(); m_gameLoop->start(); });
    connect(m_pauseScreen, &GameOverlayScreen::exitRequested,
            this, [this](){ m_gameLoop->stop(); m_game->exitToMenu(); });

    connect(m_gameOverScreen, &GameOverlayScreen::restartRequested,
            this, [this](){
                m_game->restartLevel();
                m_game->resumeGame();
                m_gameLoop->start();
            });
    connect(m_gameOverScreen, &GameOverlayScreen::exitRequested,
            this, [this](){ m_gameLoop->stop(); m_game->exitToMenu(); });

    connect(m_winScreen, &GameOverlayScreen::nextLevelRequested,
            this, [this](){ m_game->startNextLevel(); m_gameLoop->start(); });
    connect(m_winScreen, &GameOverlayScreen::exitRequested,
            this, [this](){ m_gameLoop->stop(); m_game->exitToMenu(); });

    connect(m_winScreen, &GameOverlayScreen::restartRequested,
            this, [this](){
                m_game->restartLevel();
                m_game->resumeGame();
                m_gameLoop->start();
            });

}

void MainWindow::onGameTick(int deltaTimeMs) {
    m_game->update(deltaTimeMs);
    m_gameWidget->update();
}

void MainWindow::onStateChanged(GameState oldState, GameState newState) {
    m_gameLoop->stop();
    switch(newState) {
    case GameState::MAIN_MENU:
        m_stack->setCurrentIndex(0);
        break;
    case GameState::PLAYING:
        m_stack->setCurrentIndex(1);
        m_gameWidget->setFocus();
        m_gameLoop->start();
        break;
    case GameState::PAUSED:
        m_stack->setCurrentIndex(2);
        m_pauseScreen->setFocus();
        break;
    case GameState::GAME_OVER:
        m_stack->setCurrentIndex(3);
        m_gameOverScreen->setFocus();
        break;
    case GameState::WIN:
        m_stack->setCurrentIndex(4);
        m_winScreen->setFocus();
        break;
    }
}
void MainWindow::onPuzzleRequired(const PuzzleData& puzzle) {
    if (!m_game || !m_gameLoop || !m_quizScreen) return;

    m_gameLoop->stop();
    if (m_game->score()) {
        m_game->score()->stopTimer();
    }

    m_quizScreen->startQuiz(puzzle);
    m_stack->setCurrentWidget(m_quizScreen);
    m_quizScreen->setFocus();
}

void MainWindow::onQuizFinished(bool passed, const QString& objectId) {
    if (!m_game || !m_gameLoop) return;

    if (passed) {
        m_game->onPuzzleSolved(objectId);
    } else {
        m_game->onPuzzleFailed(objectId);
    }

    if (m_game->currentState() == GameState::PLAYING) {
        if (m_game->score()) {
            m_game->score()->startTimer(m_game->score()->timeLeft());
        }
        m_stack->setCurrentWidget(m_gameWidget);
        m_gameWidget->setFocus();
        m_gameLoop->start();
    }
}