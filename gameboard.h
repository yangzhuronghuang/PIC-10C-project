#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QKeyEvent>
#include <QPoint>
#include <random>
#include <chrono>
#include <QTimer>
#include "features.h"

#include <iostream>

namespace Ui {
class GameBoard;
}

class GameBoard;

class Monster:public QObject
{
    Q_OBJECT
public:
    Monster();
    ~Monster(){
        delete monster_location;
        delete monster_clock;
    }
    Monster(int x, int y,GameBoard* which_board,size_t monster_speed):alive(true),board(which_board),movespeed(monster_speed){
        monster_location = new QPoint(x,y);
        monster_clock = new QTimer(this);
        connect(monster_clock, SIGNAL(timeout()), this, SLOT(update()));
        monster_clock->start(movespeed);
    }
    void setDead() {alive = false;}
    QPoint* getLocation() {return monster_location;}
    bool if_alive(){return alive;}

    friend class GameBoard;
public slots:
    void update();
private:
    QPoint* monster_location;
    bool alive;
    GameBoard* board;
    size_t movespeed;
    QTimer* monster_clock;
};

class Mine;

class Player:public QObject
{
    Q_OBJECT
public:
    Player();
    ~Player(){
        for (auto x:mines)
            delete x;
        delete player_location;
    }

    Player(int x, int y,GameBoard* which_board):alive(true),board(which_board){
        player_location = new QPoint(x,y);
        upset = false;
        happy = false;
    }
    void place_mine();
    void setDead(){alive =false;}
    void set_happy(){
        happy = true;
    }

    QPoint* getLocation(){return player_location;}


    void update(int px, int py, int nx, int ny);

    friend class GameBoard;
public slots:
    void recover_from_upset();
    void recover_from_happy();
private:
    QPoint* player_location;
    bool alive;
    bool upset;
    bool happy;
    int lives_remaining;
    GameBoard* board;
    QVector<Mine*> mines;
};

class Mine:QObject
{
    Q_OBJECT
public:
    Mine();
    Mine(int x, int y, GameBoard* which_board):alive(true),board(which_board){
        mine_location = new QPoint(x,y);
        mine_clock = new QTimer(this);
        connect(mine_clock, SIGNAL(timeout()), this, SLOT(explode()));
        mine_clock->start(3000);
    }
    ~Mine(){
        delete mine_location;
        delete mine_clock;
    }

    bool if_alive(){return alive;}
    void kill(int x, int y);
    friend class GameBoard;
public slots:
    void explode();
    void recover();

private:
    QPoint* mine_location;
    bool alive;
    GameBoard* board;
    QTimer* mine_clock;
};


class GameBoard : public QWidget
{
    Q_OBJECT

public:
    explicit GameBoard(QWidget *parent = 0, size_t board_size = 10, size_t number_monsters = 5, size_t wall_num = 20, size_t ogre_speed = 10, int gamemode = 1);
    ~GameBoard();

    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *e);
    void showEvent(QShowEvent *e);
    bool checkWalls(int x, int y);
    bool checkMonsters(int x, int y);
    bool check_mines(int x, int y);
    bool checkPlayer(int x, int y);
    //void updatePlayer(int px, int py, int nx, int ny);
    int check_num_mines();
    void placeMines(int px, int py);

    QLabel** getLabels() {return labels;}
    int getBoardSize() {return board_size;}
    QPixmap* get_mine_image() {return mine_image;}
    int get_lives(){return lives_remaining;}
    void dec_lives();
    bool all_dead();
    QLabel* get_life_number(){return life_number;}
    Player* get_player(){return player;}
    QVector<Monster*> get_monsters(){return monsters;}

public slots:
    //void updateMonsters();


signals:
    void game_over();
    void next_level();

private:
    Ui::GameBoard *ui;
    QWidget* Board;
    size_t board_size;
    size_t ogre_speed;
    size_t wall_number;
    int game_mode;
    QLabel** labels;
    int* values;
    int lives_remaining;

    QPixmap* player_image;
    QPixmap* player_dec;
    QPixmap* player_kill;
    QPixmap* monster_image;
    QPixmap* mine_image;
    size_t number_monsters;
    //QPoint* player_position;
    //QPoint* monster_positions;
    QPoint* wall_positions;

    QHBoxLayout* top_bar;
    QHBoxLayout* lifeContainer;
    QLabel* lives;
    QLabel* life_number;
    QVBoxLayout* score_and_level;
    QLabel* scores;
    QHBoxLayout* level;
    QLabel* level_text;
    QLabel* level_value;
    QGridLayout* SquareGrid;
    QVBoxLayout* piece_together;
    QPushButton* quit;


    Player* player;
    QVector<Monster*> monsters;
};

#endif // GAMEBOARD_H
