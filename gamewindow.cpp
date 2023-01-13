#include "gamewindow.h"
#include "ui_gamewindow.h"

GameWindow::GameWindow(Model& model, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::GameWindow)
    , valuePlotWindow(model)
{
    this->setFixedSize(860, 615);
    ui->setupUi(this);

    ui->winWidget->hide();
    ui->loseWidget->hide();
    ui->gameOverLabel->hide();
    ui->semiTranspGameOverLabel->hide();
    ui->blackJackAnimationLabel->hide();

    //when deal/hit/stand is clicked, send signal to model
    connect(this,
            &GameWindow::startGameDealSignal,
            &model,
            &Model::startGameDeal);
    connect(ui->hitButton,
            &QPushButton::clicked,
            &model,
            &Model::playerHitSlot);
    connect(ui->standButton,
            &QPushButton::clicked,
            &model,
            &Model::playerStandSlot);
    connect(ui->splitButton,
            &QPushButton::clicked,
            &model,
            &Model::playerSplitSlot);
    //when double down is clicked, one more card should be dealt
    connect(ui->doubleDownButton,
            &QPushButton::clicked,
            &model,
            &Model::playerHitSlot);

    //reset for another round
    connect(this,
            &GameWindow::resetRoundSignal,
            &model,
            &Model::resetRoundSlot);

    //FROM MODEL
    //signal to display cards
    connect(&model,
            &Model::cardSignal,
            this,
            &GameWindow::showCardSlot);
    //signal to dis/enable buttons
    connect(&model,
            &Model::hitButtonSignal,
            this,
            &GameWindow::hitButtonStateSlot);
    connect(&model,
            &Model::standButtonSignal,
            this,
            &GameWindow::standButtonStateSlot);
    connect(&model,
            &Model::whoWinsSignal,
            this,
            &GameWindow::winLoseDisplaySlot);
    connect(&model,
            &Model::splitButtonSignal,
            this,
            &GameWindow::splitButtonStateSlot);
    connect(&model,
            &Model::resetPlayerUISignal,
            this,
            &GameWindow::resetPlayerUI);
    //Signals to display advice
    connect(&model,
            &Model::moveFeedbackSignal,
            this,
            &GameWindow::showMoveAdviceSlot);
    //display split index
        connect(&model,
                &Model::updateSplitIndex,
                this,
                &GameWindow::updateSplitSlot);

    //Signal to start game
    connect(ui->StartButton,
            &QPushButton::clicked,
            this,
            &GameWindow::on_StartButton_clicked);

    //Signal to start game in rule page
    connect(ui->gameStartButton,
            &QPushButton::clicked,
            this,
            &GameWindow::on_StartButton_clicked);

    //Signal to navigate to the rule page with menubar
    connect(ui->actionGame_Rule,
            &QAction::triggered,
            this,
            &GameWindow::on_RuleButton_clicked);

    //Signal to start new game with menubar
    connect(ui->actionNew_Game,
            &QAction::triggered,
            this,
            &GameWindow::on_StartButton_clicked);

    //Signal to quit the game with menubar
    connect(ui->actionQuit,
            &QAction::triggered,
            this,
            &QCoreApplication::quit);
    connect(ui->backtogameButton,
            &QPushButton::clicked,
            this,
            &GameWindow::on_BackToGameButton_clicked);


    //for plotting values
    connect(ui->showValuePlotButton,
            &QPushButton::clicked,
            this,
            &GameWindow::on_showValuePlotButton_clicked);



    // Set up Black Jack Label
    QPixmap blackJackImage(":/new/images/blackJack.png");
    ui->blackJackAnimationLabel->setPixmap(blackJackImage.scaled(ui->blackJackAnimationLabel->width(),
                                                                 ui->blackJackAnimationLabel->height(),
                                                                 Qt::KeepAspectRatio));

    ui->chipsLabel->setPixmap(QPixmap(":/new/images/chips.png").scaled(ui->chipsLabel->width(),
                                                                       ui->chipsLabel->height(),
                                                                       Qt::KeepAspectRatio));

    gameOverImage = QPixmap(":/new/images/gameOver.png");
    ui->gameOverLabel->setPixmap(gameOverImage.scaled(ui->gameOverLabel->width(),
                                                      ui->gameOverLabel->height(),
                                                      Qt::KeepAspectRatio));

    semiTransparentGameOver = QPixmap(":/new/images/semiTranspGameOver.png");
    ui->semiTranspGameOverLabel->setPixmap(semiTransparentGameOver.scaled(ui->gameOverLabel->width(),
                                                                          ui->gameOverLabel->height(),
                                                                          Qt::KeepAspectRatio));


    landingPage = QPixmap(":/new/images/landingPage.jpg");

    // Set up timer
    connect(&animationTimer,
            &QTimer::timeout,
            this,
            &GameWindow::onTimeout);



    animationTimer.start(20);

    startRound();

}

GameWindow::~GameWindow()
{
    delete ui;
}


void GameWindow::onTimeout() {
    QPoint oldPos = ui->blackJackAnimationLabel->pos();
    QPoint newPos = oldPos;
    if(oldPos.x() <= 450 && movingTowardsRight == true) {
        newPos = QPoint(oldPos.x() + 3, oldPos.y());
    }
    else{
        movingTowardsRight = false;
        if(oldPos.x() >= 10) {
            newPos = QPoint(oldPos.x() - 3, oldPos.y());
        }
        else
            movingTowardsRight = true;
    }

    ui->blackJackAnimationLabel->move(newPos);
}


void GameWindow::on_showValuePlotButton_clicked(){
    valuePlotWindow.show();
}


void GameWindow::on_StartButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->gamePage);
    playerMoney = 100;
    betAmount = 0;
    startRound();
}


QString GameWindow::decideCardBackgroundImage(int suit)
{
    if(suit == 0){
        return "border-image: url(:/new/images/clubs.png);";
    }
    if(suit == 1){
        return "border-image: url(:/new/images/spades.png);";
    }
    if(suit == 2){
        return "border-image: url(:/new/images/hearts.png);";
    }
    //suit must be 3
    return "border-image: url(:/new/images/diamonds.png);";
}

void GameWindow::startRound()
{
    stop = true;
    resetUI();
    emit resetRoundSignal();
}


void GameWindow::resetUI()
{
    //if not checked Phase II
    if(!ui->dificulltyCheckBox->isChecked()){
        ui->doubleDownButton->setVisible(false);
        ui->splitButton->setVisible(false);
    }
    else{
        ui->doubleDownButton->setVisible(true);
        ui->splitButton->setVisible(true);
    }

    ui->adviceBrowser->setText("Advice Based on MIT's blackjack strategy.");
    ui->betAmountLabel->setText("Bet Amount: ");
    ui->playerMoneyLabel->setText("Player Money: $" + QString::number(playerMoney));

    //set up buttons for start
    ui->betButton->setEnabled(true);
    ui->dealButton->setEnabled(false);
    ui->hitButton->setEnabled(false);
    ui->standButton->setEnabled(false);
    ui->splitButton->setEnabled(false);
    ui->doubleDownButton->setEnabled(false);
    ui->NextButton->setEnabled(true);

    //restart cards UI
    ui->houseCard1->setStyleSheet("border-image: url(:/new/images/cardBack.png);");
    ui->houseCard1->raise();
    ui->playerCard1->setStyleSheet("border-image: url(:/new/images/cardBack.png);");
    ui->playerCard1->raise();

    //hide animation components
    ui->winWidget->hide();
    ui->loseWidget->hide();
    ui->blackJackAnimationLabel->hide();
    ui->gameOverLabel->hide();
    ui->semiTranspGameOverLabel->hide();

    ui->houseCard1->setText("");
    ui->playerCard1->setText("");

    ui->playerCard2->setVisible(false);
    ui->playerCard3->setVisible(false);
    ui->playerCard4->setVisible(false);
    ui->playerCard5->setVisible(false);

    ui->houseCard2->setVisible(false);
    ui->houseCard3->setVisible(false);
    ui->houseCard4->setVisible(false);
    ui->houseCard5->setVisible(false);
    ui->splitLabel->setVisible(false);
}


void GameWindow::resetPlayerUI()
{
    //set up buttons for start
    ui->betButton->setEnabled(false);
    ui->dealButton->setEnabled(false);
    //    ui->splitButton->setEnabled(false); for testing split
    ui->doubleDownButton->setEnabled(false);  //TODO can double down on split deck?

    ui->hitButton->setEnabled(true);
    ui->standButton->setEnabled(true);


    //restart player cards UI
    ui->playerCard1->setStyleSheet("border-image: url(:/new/images/cardBack.png);");
    ui->playerCard1->raise();


    //ui->playerCard1->setText("");  TODO need to call showCardSlot from split with first card in new deck
    ui->playerCard2->setVisible(false);
    ui->playerCard3->setVisible(false);
    ui->playerCard4->setVisible(false);
    ui->playerCard5->setVisible(false);

    ui->splitLabel->setVisible(false);
}


void GameWindow::showCardSlot(Card card, QString cardLabel)
{
    QLabel * lbl = this->findChild<QLabel *>(cardLabel);

    if(lbl)lbl->setText(card.getCardValue());
    if(lbl)lbl->setStyleSheet(decideCardBackgroundImage(card.getSuit()));
    if(lbl)lbl->raise();
    if(lbl)lbl->setVisible(true);
}


void GameWindow::on_dealButton_clicked()
{
    emit startGameDealSignal();
    ui->dealButton->setEnabled(false);
    ui->doubleDownButton->setEnabled(true);
    ui->hitButton->setEnabled(true);
    ui->standButton->setEnabled(true);
}



void GameWindow::on_RuleButton_clicked()
{
    savedPageData = ui->stackedWidget->currentWidget();
    ui->stackedWidget->setCurrentWidget(ui->rulePage);
    ui->cardValuesLable->setStyleSheet("border-image: url(:/new/images/CardValues.jpg);");

    //Execute the code depends on the current page
    if(savedPageData == ui->startPage){
        ui->backtogameButton->setVisible(false);
    }
    else if(savedPageData == ui->gamePage)
    {
        ui->backtogameButton->setVisible(true);
        ui->gameStartButton->setVisible(false);
    }
}


void GameWindow::on_BackToGameButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(savedPageData);
}


void GameWindow::on_NextButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->ruleDetailPage);
}


void GameWindow::on_PreviousButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->rulePage);
}


void GameWindow::hitButtonStateSlot(bool state)
{
    ui->hitButton->setEnabled(state);
    //as soon as hit or stand are clicked, should no longer be able to double down
    ui->doubleDownButton->setEnabled(false);
}


void GameWindow::standButtonStateSlot(bool state)
{
    ui->standButton->setEnabled(state);
    ui->doubleDownButton->setEnabled(false);
}


void GameWindow::splitButtonStateSlot(bool state)
{
    ui->splitButton->setEnabled(state);
}


void GameWindow::winLoseDisplaySlot(int whosWinner, int wins, int rounds)
{
    //allow improved difficulty(Phase II)
    ui->dificulltyCheckBox->setEnabled(true);    

    if(whosWinner == 100){
        //blackjack gets 1.5 original bet
        playerMoney += originalBetAmount*1.5;
        ui->blackJackAnimationLabel->show();
    }
    else if(whosWinner > 0){
        playerMoney += betAmount*whosWinner;
        ui->winWidget->raise();
        ui->winWidget->show();
    }
    else if(whosWinner < 0){
        stop = false;
        if(playerMoney > -(originalBetAmount*whosWinner)){
            playerMoney += betAmount*whosWinner;
            std::cout<< "PLAYER LOSE"<<std::endl;
            ui->loseWidget->raise();
            ui->loseWidget->show();
        }
        else {
            //out of money
            playerMoney = 0;
            gameOverFlash();
        }
    }
    else{
        //ITS A TIE. WIN CODE HERE AS PLACEHOLDER
        std::cout<< "Tie"<<std::endl;
        startAnotherRound();
        return;
    }

    ui->playerMoneyLabel->setText("Player Money: $" + QString::number(playerMoney));

    //After 2D physics Display, if still has money, play another round?
    if(playerMoney > 0){
        timer.singleShot(2000, this, &GameWindow::startAnotherRound);
    }
    //subtract 99 so black jack is read normally
    if(whosWinner >= 100){whosWinner -= 99;}

    QString results = "In total, you've played " + QString::number(rounds) + " round(s), and won " + QString::number(wins) + " time(s).";
    ui->adviceBrowser->append(results);
}


void GameWindow::startAnotherRound(){
    QMessageBox::StandardButton playAgain;
    playAgain = QMessageBox::question(this, "Kimochi Casino", "Play another round?",
                                      QMessageBox::Yes|QMessageBox::No);
    if (playAgain == QMessageBox::Yes) {
        startRound();
    } else {
        ui->stackedWidget->setCurrentIndex(0);
    }
}


void GameWindow::showMoveAdviceSlot(QString advice)
{
    ui->adviceBrowser->setText(advice);
}


void GameWindow::on_betButton_clicked()
{
    ui->dificulltyCheckBox->setEnabled(false);

    bool ok;

    betAmount = QInputDialog::getDouble(this, tr("Bet Amount"),
                                          tr("Choose Bet Amount:"), 10, 1, playerMoney, 2, &ok,
                                          Qt::WindowFlags(), 1);
    //Value of bet when they first bet. This value will not be updated after initial bet
    originalBetAmount = betAmount;
     if (ok){
         ui->betAmountLabel->setText("Bet Amount: $" + QString::number(betAmount));
         ui->betButton->setEnabled(false);
         ui->dealButton->setEnabled(true);
    }
    //else nothing is enabled until player places a bet
}


void GameWindow::paintEvent(QPaintEvent *pe) {
    QPainter pt(this);
    int width = ui->startPage->width();
    int height = ui->startPage->height();
    landingPage = landingPage.scaled(width, height, Qt::KeepAspectRatioByExpanding);
    QPoint center = ui->startPage->rect().center();
    QRect rectOfImage = landingPage.rect();
    rectOfImage.moveCenter(center);
    pt.drawPixmap(rectOfImage.topLeft(), landingPage);
}


void GameWindow::gameOverFlash() {
        ui->gameOverLabel->show();
        ui->semiTranspGameOverLabel->hide();
        gameOverTimer.singleShot(500, this, &GameWindow::gameOverLabelShow);
}


void GameWindow::gameOverLabelShow() {
    if(!stop) {
        ui->gameOverLabel->hide();
        ui->semiTranspGameOverLabel->show();
        gameOverTimer.singleShot(500, this, &GameWindow::gameOverFlash);
    }
}


void GameWindow::on_dificulltyCheckBox_stateChanged(int arg1)
{
    if(arg1){
        ui->doubleDownButton->setVisible(true);
        ui->splitButton->setVisible(true);
    }
    else {
        ui->doubleDownButton->setVisible(false);
        ui->splitButton->setVisible(false);
    }
}


void GameWindow::on_doubleDownButton_clicked()
{
    if(betAmount*2 <= playerMoney){
        betAmount *= 2;
    }
    ui->betAmountLabel->setText("Bet Amount: $" + QString::number(betAmount));
    ui->doubleDownButton->setEnabled(false);
    ui->hitButton->setEnabled(false);
    //doubledown button connected to game deal so should end
}

void GameWindow::updateSplitSlot(int currentPlayerHandIndex)
{
    //ui->splitLable->setVisible(true);
    ui->splitLable->setText("Split Deck! Index: " + QString::number(currentPlayerHandIndex));
}


