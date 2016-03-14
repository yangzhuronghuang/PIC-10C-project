#include "gameboard.h"
#include "gameui.h"
#include "ui_gameboard.h"
#include <QDebug>

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator (seed);
std::uniform_int_distribution<int> distribution(0,3);

void Monster::update(){
    int px = monster_location->rx();
    int py = monster_location->ry();
    int b = distribution(generator);
    QLabel**labels = board->getLabels();
    if (alive){
        if (b == 0) { // turn up
            if (py > 0 &&!board->checkWalls(px,py-1) && !board->check_mines(px,py-1)) {
                labels[py*board->getBoardSize()+px]->clear();
                monster_location->setY(py-1);
            }
        }
        else{
            if (b == 1) { // turn down
                if (py < board->getBoardSize()-1 &&!board->checkWalls(px,py+1) && !board->check_mines(px,py+1)) {
                    labels[py*board->getBoardSize()+px]->clear();
                    monster_location->setY(py+1);
                }
            }
            else{
                if(b == 2) { //turn left
                    if(px > 0 && !board->checkWalls(px-1,py) && !board->check_mines(px-1,py)) {
                        labels[py*board->getBoardSize() + px]->clear();
                        monster_location->setX(px-1);
                    }
                }
                else { // turn right
                    if(px < board->getBoardSize()- 1 && !board->checkWalls(px+1,py) && !board->check_mines(px+1,py)) {
                        labels[py*board->getBoardSize() + px]->clear();
                        monster_location->setX(px+1);
                    }
                }
            }
        }

        if (board->checkPlayer(monster_location->rx(),monster_location->ry()))
            board->dec_lives();
    }
}


void Player::place_mine(){
    int x = player_location->rx();
    int y = player_location->ry();
    int count = 0;
    for (size_t i = 0; i<mines.size();i++){
        if (mines[i]->if_alive() == true)
            count++;
    }
    if (count <2){
        mines.push_back(new Mine(x,y,board));
        QLabel**labels = board->getLabels();
        labels[y*board->getBoardSize()+x]->setPixmap(*(board->get_mine_image()));
    }
}

void Player::update(int px, int py, int nx, int ny) {
    QLabel** labels  = board->getLabels();
    labels[py*board->getBoardSize()+px]->clear();
    player_location->setX(nx);
    player_location->setY(ny);
    //if player meet a monster, player loses one life
    if (board->checkMonsters(nx,ny)) {
        board->dec_lives();
    }
    //    return (px==nx)&&(py==ny);
}

void Player::recover_from_upset(){
    upset = false;
}

void Player::recover_from_happy(){
    happy = false;
}

void Mine::kill(int x, int y){
    QVector<Monster*> b = board->get_monsters();
    QLabel** labels  = board->getLabels();
    if (board->checkPlayer(x,y)) {
        board->dec_lives();
    }
    if (board->checkMonsters(x,y)){
        for (int i = 0; i<b.size();i++){
            if (b[i]->getLocation()->rx() == x && b[i]->getLocation()->ry() == y)
            {
                b[i]->setDead();
                labels[b[i]->getLocation()->ry()*board->getBoardSize()+b[i]->getLocation()->rx()]->clear();
                board->get_player()->set_happy();
                QTimer::singleShot(3000,board->get_player(),SLOT(recover_from_happy()));
            }
        }
    }
}

void Mine::explode(){
    if(alive == true){
        alive = false;
        int x = mine_location->rx();
        int y = mine_location->ry();
        QLabel** labels = board->getLabels();
        //if x<3 explodes to the border
        if (x<2){
            for (int i = 0; i <= x+2; i++){
                if(!board->checkWalls(i,y))
                    labels[y*board->getBoardSize()+i]->setPixmap(*(board->get_mine_image()));

                kill(i,y);
            }
            if (y<2){
                for (int i = 0; i <= y+2; i++){
                    if(board->checkWalls(x,i))
                        break;
                    labels[i*board->getBoardSize()+x]->setPixmap(*(board->get_mine_image()));
                    kill(x,i);
                }
            }
            else if (y>board->getBoardSize()-3){
                for(int i = y-2; i<board->getBoardSize();i++){
                    if(board->checkWalls(x,i))
                        break;
                    labels[i*board->getBoardSize()+x]->setPixmap(*(board->get_mine_image()));
                    kill(x,i);
                }
            }
            else{
                for (int i = y-2; i<= y+2; i++){
                    if(board->checkWalls(x,i))
                        break;
                    labels[i*board->getBoardSize()+x]->setPixmap(*(board->get_mine_image()));
                    kill(x,i);
                }
            }
        }
        else{
            if (x>board->getBoardSize()-3){
                for(int i = x-2; i<board->getBoardSize();i++){
                    if(board->checkWalls(i,y))
                        break;
                    labels[y*board->getBoardSize()+i]->setPixmap(*(board->get_mine_image()));
                    kill(i,y);
                }
                if (y<2){
                    for (int i = 0; i <= y+2; i++){
                        if(board->checkWalls(x,i))
                            break;
                        labels[i*board->getBoardSize()+x]->setPixmap(*(board->get_mine_image()));
                        kill(x,i);
                    }
                }
                else{
                    if (y>board->getBoardSize()-3){
                        for(int i = y-2; i<board->getBoardSize();i++){
                            if(board->checkWalls(x,i))
                                break;
                            labels[i*board->getBoardSize()+x]->setPixmap(*(board->get_mine_image()));
                            kill(x,i);
                        }
                    }

                    else{
                        for (int i = y-2; i<= y+2; i++){
                            if(board->checkWalls(x,i))
                                break;
                            labels[i*board->getBoardSize()+x]->setPixmap(*(board->get_mine_image()));
                            kill(x,i);
                        }
                    }
                }
            }
            else{
                for (int i = x-2; i <= x+2; i++){
                    if(board->checkWalls(i,y))
                        break;
                    labels[y*board->getBoardSize()+i]->setPixmap(*(board->get_mine_image()));
                    kill(i,y);
                }
                if (y<2){
                    for (int i = 0; i <= y+2; i++){
                        if(board->checkWalls(x,i))
                            break;
                        labels[i*board->getBoardSize()+x]->setPixmap(*(board->get_mine_image()));
                        kill(x,i);
                    }
                }
                else{
                    if (y>board->getBoardSize()-3){
                        for(int i = y-2; i<board->getBoardSize();i++){
                            if(board->checkWalls(x,i))
                                break;
                            labels[i*board->getBoardSize()+x]->setPixmap(*(board->get_mine_image()));
                            kill(x,i);
                        }
                    }
                    else{
                        for (int i = y-2; i<= y+2; i++){
                            if(board->checkWalls(x,i))
                                break;
                            labels[i*board->getBoardSize()+x]->setPixmap(*(board->get_mine_image()));
                            kill(x,i);
                        }
                    }
                }
            }
        }
        board->all_dead();
        QTimer::singleShot(1000,this,SLOT(recover()));
    }
    //labels[y*board->getBoardSize()+x]->clear();
}

void Mine::recover(){
    QLabel** labels = board->getLabels();
    int x = mine_location->rx();
    int y = mine_location->ry();
    if (x<2){
        for (int i = 0; i <= x+2; i++){
            if(!board->checkWalls(i,y))
                break;
            labels[y*board->getBoardSize()+i]->clear();
        }
        if (y<2){
            for (int i = 0; i <= y+2; i++){
                if(board->checkWalls(x,i))
                    break;
                labels[i*board->getBoardSize()+x]->clear();
            }
        }
        else if (y>board->getBoardSize()-3){
            for(int i = y-2; i<board->getBoardSize();i++){
                if(board->checkWalls(x,i))
                    break;
                labels[i*board->getBoardSize()+x]->clear();
            }
        }
        else{
            for (int i = y-2; i<= y+2; i++){
                if(board->checkWalls(x,i))
                    break;
                labels[i*board->getBoardSize()+x]->clear();
            }
        }
    }
    else{
        if (x>board->getBoardSize()-3){
            for(int i = x-2; i<board->getBoardSize();i++){
                if(board->checkWalls(i,y))
                    break;
                labels[y*board->getBoardSize()+i]->clear();
            }
            if (y<2){
                for (int i = 0; i <= y+2; i++){
                    if(board->checkWalls(x,i))
                        break;
                    labels[i*board->getBoardSize()+x]->clear();
                }
            }
            else{
                if (y>board->getBoardSize()-3){
                    for(int i = y-2; i<board->getBoardSize();i++){
                        if(board->checkWalls(x,i))
                            break;
                        labels[i*board->getBoardSize()+x]->clear();
                    }
                }
                else{
                    for (int i = y-2; i<= y+2; i++){
                        if(board->checkWalls(x,i))
                            break;
                        labels[i*board->getBoardSize()+x]->clear();
                    }
                }
            }
        }
        else{
            for (int i = x-2; i <= x+2; i++){
                if(board->checkWalls(i,y))
                    break;
                labels[y*board->getBoardSize()+i]->clear();
            }
            if (y<2){
                for (int i = 0; i <= y+2; i++){
                    if(board->checkWalls(x,i))
                        break;
                    labels[i*board->getBoardSize()+x]->clear();
                }
            }
            else{
                if (y>board->getBoardSize()-3){
                    for(int i = y-2; i<board->getBoardSize();i++){
                        if(board->checkWalls(x,i))
                            break;
                        labels[i*board->getBoardSize()+x]->clear();
                    }
                }
                else{
                    for (int i = y-2; i<= y+2; i++){
                        if(board->checkWalls(x,i))
                            break;
                        labels[i*board->getBoardSize()+x]->clear();
                    }
                }
            }
        }
    }
}

GameBoard::GameBoard(QWidget *parent, size_t board_sz, size_t monster_number,size_t wall_num,size_t ogre_spd, int gamemode):
QWidget(parent),ui(new Ui::GameBoard), board_size(board_sz), number_monsters(monster_number), wall_number(wall_num), ogre_speed(ogre_spd), game_mode(gamemode){

    ui->setupUi(this);

    //set the top bar contents, like life, score and level
    top_bar = new QHBoxLayout;
    //initialize number of life
    lives = new QLabel;
    lives->setText("life: ");
    lives_remaining = 3;
    life_number = new QLabel(QString::number(lives_remaining));
    lifeContainer = new QHBoxLayout;
    lifeContainer->addWidget(lives);
    lifeContainer->addWidget(life_number);
    lifeContainer->setSpacing(0);
    top_bar->addLayout(lifeContainer);
    // It seems that it may cause a crash, will figure out later
     //initialize score and level
    scores = new QLabel;
    scores->setText("Score:");

    level_text = new QLabel;
    level_text->setText("Level: ");
    level_value = new QLabel(QString::number(game_mode));

    level = new QHBoxLayout;
    level->addWidget(level_text);
    level->addWidget(level_value);
    top_bar->addLayout(level);

    score_and_level = new QVBoxLayout;
    score_and_level->addWidget(scores);
    score_and_level->addLayout(level);

    top_bar->addLayout(score_and_level);
    //*/

    //create widget board, which we want to be a big grid,with the board size given in the constructor of GameBoard.
    Board = new QWidget;
    labels = new QLabel*[board_size*board_size];
    values = new int[board_size*board_size];
    //set the grid layout
    QGridLayout *SquareGrid = new QGridLayout(Board);
    SquareGrid->setGeometry(QRect());
    SquareGrid->setSpacing(0);
    for(size_t i=0;i<board_size;i++) {
        for(size_t j=0;j<board_size;j++) {

            // Create label and set properties.
            labels[i*board_size+j] = new QLabel;
            labels[i*board_size+j]->setMinimumSize(30,30);
            labels[i*board_size+j]->setMaximumSize(20,20);
            labels[i*board_size+j]->setStyleSheet("QLabel { background-color : white; color : white;border:none }");
            /*
             * if ((i*board_size+j) % 3 == 0)
                labels[i*board_size+j]->setStyleSheet("QLabel { background-color : rgb(28,55,232); color : white;border:none }");
            else{
                if ((i*board_size+j) % 3 == 1)
                    labels[i*board_size+j]->setStyleSheet("QLabel { background-color : rgb(19,155,159); color : white;border:none }");
                else
                    labels[i*board_size+j]->setStyleSheet("QLabel { background-color : rgb(40,134,228); color : white;border:none }");

            }
            */

            labels[i*board_size+j]->setFrameStyle(2);
            labels[i*board_size+j]->setAlignment(Qt::AlignCenter);

            // Add label to the layout
            SquareGrid -> addWidget(labels[i*board_size+j] ,i,j);

        }
    }



    QString player_filename(":/images/Player.jpg");
    player_image = new QPixmap(player_filename);
    QString player_happy(":/images/yeah.jpeg");
    player_kill = new QPixmap(player_happy);
    QString player_wtf(":/images/wtf.jpeg");
    player_dec = new QPixmap(player_wtf);


    //store image for mines
    QString minename(":/images/mine.jpg");
    mine_image = new QPixmap(minename);

    //create player for different modes
    if (game_mode == 1)
        player = new Player(8,8,this);
    else {
        if (game_mode == 2)
            player = new Player(5,16,this);
        else
            player = new Player(8,13,this);
    }
    labels[player->player_location->ry()*board_size + player->player_location->rx()]->setPixmap(*player_image);

    //create monsters
    //monster_positions = new QPoint[number_monsters];

    QString monster_filename(":/images/monster.jpg");
    monster_image = new QPixmap(monster_filename);
    for (size_t i = 0; i < number_monsters; i++) {
        monsters.push_back(new Monster(10,10,this,ogre_speed));
        labels[monsters[i]->monster_location->ry()*board_size + monsters[i]->monster_location->rx()]->setPixmap(*monster_image);
    }


    //create walls
    QString wallname(":/images/walls.jpeg");
    QPixmap* wall_image = new QPixmap(wallname);

    //set walls for different modes
    wall_positions = new QPoint[wall_number];
    if (game_mode == 1) {
        for (size_t i = 0; i < board_size; i++) {
            wall_positions[i].setX(i);
            wall_positions[i].setY(18);
            labels[18*board_size+i]->setPixmap(*wall_image);
        }
    }
    else {
        if (game_mode == 2) {
            wall_positions = new QPoint[20];
            for (size_t i = 0; i < board_size; i++) {
                wall_positions[i].setX(i);
                wall_positions[i].setY(18);
                labels[18*board_size+i]->setPixmap(*wall_image);
            }
        }
        else {
            wall_positions = new QPoint[20];
            for (size_t i = 0; i < board_size; i++) {
                wall_positions[i].setX(i);
                wall_positions[i].setY(18);
                labels[18*board_size+i]->setPixmap(*wall_image);
            }
        }
    }

    Board->setFixedSize(600,600);

    //piece everything together
    setFixedSize(700,700);
    piece_together = new QVBoxLayout;
    piece_together->addLayout(top_bar);
    piece_together->addWidget(Board,0,Qt::AlignCenter);
    this->setLayout(piece_together);
    // set back to main menu
    quit = new QPushButton("back");
    connect(this->quit, SIGNAL(clicked(bool)), parent, SLOT(quit()));
    piece_together->addWidget(quit);
/*
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateMonsters()));
    timer->start(ogre_speed);
    */

    connect(this,SIGNAL(game_over()), parent, SLOT(game_over()));
    connect(this,SIGNAL(next_level()),parent,SLOT(next_level()));

}

GameBoard::~GameBoard() {
    //std::cout <<"I am called"<<std::endl;

    for (int i = 0; i < monsters.size();i++)
        delete monsters[i];
    std::cout<< monsters.size() << std::endl;
    delete player;
    delete []labels;
    delete ui;

}

/* check if there is a wall at point(x,y)
 * x is the x coordinate of the point
 * y is the y coordinte of the point
 * return true if there is a wall at point(x,y), false otherwise
 */
bool GameBoard::checkWalls(int x, int y) {
    for (size_t i = 0; i < wall_number; i++) {
        if (x == wall_positions[i].rx() && y == wall_positions[i].ry())
            return true;
    }
    return false;
}

/* check if there is a monster at point(x,y)
 * x is the x coordinate of the point
 * y is the y coordinte of the point
 * return true if there is a monster at point(x,y), false otherwise
 */
bool GameBoard::checkMonsters(int x, int y) {
    for (size_t i = 0; i < monsters.size(); i++) {
        if (x == monsters[i]->monster_location->rx() && y == monsters[i]->monster_location->ry()&&monsters[i]->alive == true)
            return true;
    }
    return false;
}

bool GameBoard::check_mines(int x, int y){
    for (size_t i = 0; i < player->mines.size(); i++){
        if (player->mines[i]->alive == true)
            if ( x == player->mines[i]->mine_location->rx() && y == player->mines[i]->mine_location->ry())
                return true;
    }
    return false;
}

void GameBoard::keyPressEvent(QKeyEvent *event){

    int x = player->player_location->rx();
    int y = player->player_location->ry();

    switch (event->key()) {
        case Qt::Key_Left:
            //player cannot go out of board or through the walls
            if(x!= 0 && !checkWalls(x-1,y) && !check_mines(x-1,y)) {
                player->update(x,y,x-1,y);
            }

            break;
        case Qt::Key_Right:
            if(x != board_size-1 && !checkWalls(x+1,y) && !check_mines(x+1,y))
            player->update(x,y,x+1,y);

            break;
        case Qt::Key_Up:
            if(y!= 0 && !checkWalls(x,y-1) && !check_mines(x,y-1))
            player->update(x,y,x,y-1);

            break;
        case Qt::Key_Down:
            if(y!= board_size-1 && !checkWalls(x,y+1) && !check_mines(x,y+1))
            player->update(x,y,x,y+1);

            break;


        case Qt::Key_Space:
            player->place_mine();
            break;

        default:
            QWidget::keyPressEvent(event);
    }

    // Need both lines to force a repaint.

    // This line forces processor to process all previously promised events.
    //QCoreApplication::processEvents();

    // This one QUEUES up a repaint
    //repaint();


    return;
}

void GameBoard::paintEvent(QPaintEvent *e) {

    int x = player->player_location->rx();
    int y = player->player_location->ry();
    if(player->happy)
        labels[y*board_size+x]->setPixmap(*player_kill);
    else{
        if (player->upset )
            labels[y*board_size+x]->setPixmap(*player_dec);
        else
            labels[y*board_size+x]->setPixmap(*player_image);
    }

    for(size_t i=0;i< monsters.size();i++) {
        int px = monsters[i]->monster_location->rx();
        int py = monsters[i]->monster_location->ry();
        if (monsters[i]->alive)
            if(px >= 0 && py >= 0 && px < (int)board_size && py < (int)board_size)
                labels[py*board_size+px]->setPixmap(*monster_image);
    }
    for (size_t i = 0; i<player->mines.size();i++) {
        if (player->mines[i]->alive == true)
            labels[player->mines[i]->mine_location->ry()*board_size + player->mines[i]->mine_location->rx()]->setPixmap(*mine_image);
    }
    /*
    for (int i = 0; i < board_size; i++){
        for (int j = 0; j<board_size; j++){
           if(!checkMonsters(i,j) && !checkPlayer(i,j)&& !checkWalls(i,j) && !check_mines(i,j))
               labels[i,j]->clear();
        }
    }
    */
}


void GameBoard::showEvent(QShowEvent *e) {

    this->activateWindow();
    this->setFocus();
    QWidget::showEvent(e);
}






bool GameBoard::checkPlayer(int x, int y) {
    if (x == player->player_location->rx() && y == player->player_location->ry())
        return true;
    else
        return false;
}

void GameBoard::dec_lives(){
    if (get_lives() > 1)
    {
        lives_remaining--;
        get_life_number()->setText(QString::number(get_lives()));
        player->upset = true;
        QTimer::singleShot(3000,this->player,SLOT(recover_from_upset()));
        //labels[player->player_location->ry()*board_size + player->player_location->rx()]->setPixmap(*player_dec);
    }
    else
         emit game_over();
}

bool GameBoard::all_dead(){
    for (int i = 0; i< monsters.size(); i++){
        if (monsters[i]->if_alive())
            return false;
    }
    emit next_level();
    return true;
}

/*
void GameBoard::updateMonsters(){
    for(size_t i=0; i<monsters.size(); i++) {
        int px = monsters[i]->monster_location->rx();
        int py = monsters[i]->monster_location->ry();
        //randomly find a direction
        //detect whether we can go that direction
        //if monsters can go that way, then update, else stay there
        int b = distribution(generator);
        if (b == 0) { // turn up
            if (py > 0 &&!checkWalls(px,py-1) && !check_mines(px,py-1)) {
                labels[py*board_size + px]->clear();
                monsters[i]->monster_location->setY(py-1);
            }
        }
        else {
            if (b == 1) { //turn down
                if (py < board_size -1 && !checkWalls(px,py+1) && !check_mines(px,py+1)){
                    labels[py*board_size + px]->clear();
                    monsters[i]->monster_location->setY(py+1);
                }
            }
            else {
                if(b == 2) { //turn left
                    if(px > 0 && !checkWalls(px-1,py) && !check_mines(px-1,py)) {
                        labels[py*board_size + px]->clear();
                        monsters[i]->monster_location->setX(px-1);
                    }
                }
                else { // turn right
                    if(px < board_size - 1 && !checkWalls(px+1,py) && !check_mines(px+1,py)) {
                        labels[py*board_size + px]->clear();
                        monsters[i]->monster_location->setX(px+1);
                    }
                }
            }
        }
        // if the monster touches the player, the player loses one life
        if (checkPlayer(monsters[i]->monster_location->rx(),monsters[i]->monster_location->ry())) {
            if (lives_remaining > 1)
            {
                lives_remaining --;
                life_number->setText(QString::number(lives_remaining));
            }
            else
                this->game_over();
        }
    }
}
*/
/*
int GameBoard::check_num_mines (){

    return mine_positions.size();
}
*/
/*
void GameBoard::placeMines(int x, int y) {
    //if cannot place too many mines, don't do anything
    if (check_num_mines() == 2)
        return;
    //else create the mine
    mine_positions.push_back(new QPoint(x,y));
    labels[y*board_size + x]->setPixmap(*mine_image);

    std::cout << "lol" <<std::endl;
}
*/
/*
void GameBoard::explode(){
    for (int i = 0; i<board_size*board_size; i++)
        if ( i / )

}
*/

