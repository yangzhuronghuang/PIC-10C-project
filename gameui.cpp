#include "gameui.h"
#include "ui_gameui.h"
#include "gameboard.h"
#include <QMessageBox>

GameUI::GameUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameUI)
{
    ui->setupUi(this);
}

GameUI::~GameUI()
{
    delete ui;
}

/**
 * @brief GameUI::on_pushButton_2_clicked will show a message box containing the description of the game
 */
void GameUI::on_pushButton_2_clicked()
{
    QMessageBox::information(this,tr("WTF"),tr("Basically it is a Bomber Man game. In the game there are some monsters, obstacles, etc. You can place mines which can kill monsters. Your goal is to kill all the monsters and go to exit, then you can go to next level. There are some bonus which you should find out by yourself. Good luck mate!"));
}

/**
 * @brief GameUI::easy_game_begin is to start the game in easy mode
 */
void GameUI::easy_game_begin() {
    board = new GameBoard(this,20,3,20,1000,1);
    this->setCentralWidget(board);
}

/**
 * @brief GameUI::medium_game_begin is to start the game in medium mode
 */
void GameUI::medium_game_begin() {
    board = new GameBoard(this,20,4,20,500,2);
    this->setCentralWidget(board);
}

/**
 * @brief GameUI::hard_game_begin is to start the game in hard mode
 */
void GameUI::hard_game_begin() {
    board = new GameBoard(this,20,4,20,300,3);
    this->setCentralWidget(board);
}

void GameUI::game_over() {

    // Take the current board game out of the central widget of MainWindow
    QWidget* wid = this->centralWidget();
    wid->setParent(NULL);

    // Line above will delete the widget gracefully (no memory errors)

    // Display something which says that you lost, maybe display high score.

    // Reset the MainWindow with the initial startup screen
    ui->setupUi(this);
    QMessageBox::information(this,tr("WTF"),tr("eh you died"));


}

void GameUI::quit(){

    // Take the current board game out of the central widget of MainWindow
    QWidget* wid = this->centralWidget();
    //wid->setParent(NULL);
    // Line above will delete the widget gracefully (no memory errors)

    // Display something which says that you lost, maybe display high score.
    wid->deleteLater();
    // Reset the MainWindow with the initial startup screen
    ui->setupUi(this);
    QMessageBox::information(this,tr("WTF"),tr("hope to see you again!"));


}

void GameUI::next_level(){

    // Take the current board game out of the central widget of MainWindow
    QWidget* wid = this->centralWidget();
    //wid->setParent(NULL);
    // Line above will delete the widget gracefully (no memory errors)

    // Display something which says that you lost, maybe display high score.

    // Reset the MainWindow with the initial startup screen
    //ui->setupUi(this);
    QMessageBox::information(this,tr("WTF"),tr("cong! go to next level"));
    board = new GameBoard(this,20,4,20,50,2);
    this->setCentralWidget(board);
    wid->deleteLater();
}


void GameUI::on_pushButton_clicked()
{
    QMessageBox::information(this,tr("WTF"),tr("use arrow keys to move, and space to place mines"));
}
