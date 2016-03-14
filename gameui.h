#ifndef GAMEUI_H
#define GAMEUI_H

#include <QMainWindow>
#include <gameboard.h>

namespace Ui {
class GameUI;
}

class GameUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameUI(QWidget *parent = 0);
    ~GameUI();
public slots:
    void easy_game_begin();
    void medium_game_begin();
    void hard_game_begin();
    void game_over();
    void quit();
    void next_level();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::GameUI *ui;
    GameBoard* board;
};

#endif // GAMEUI_H
