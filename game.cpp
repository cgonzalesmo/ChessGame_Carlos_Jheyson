#include "game.h"
#include "king.h"
#include "queen.h"
#include "rook.h"
#include "bishop.h"
#include "knight.h"
#include <QTimer>
#include <QTextEdit>
#include <QDebug>
#include <QtMath>
#include <QGraphicsProxyWidget>

#define WINDOW_WIDTH 1325
#define WINDOW_HEIGHT 700
#define BUTTON_WIDTH 250
#define BUTTON_HEIGHT 86
#define BUTTON_MARGIN 25


Game::Game()
{
    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    setWindowTitle("Chess");
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setScene(scene);
    setFixedSize(WINDOW_WIDTH+1, WINDOW_HEIGHT+2);
}

Game::~Game()
{
    delete movesHistoryDisplay;
    delete board;
    delete scrollArea;
    delete scrollableHistoryWindow;
    delete movesHistoryDisplay;
    delete checkDisplay;
    delete panel;
    for(auto item : alivePieces)
        delete item;
    for(auto item : whiteDead)
        delete item;
    for(auto item : blackDead)
        delete item;
}

void Game::setTurn(const QString & turn)
{
    m_turn = turn;
}

QString Game::getTurn()
{
    return m_turn;
}

void Game::changeTurn()
{

    if(getTurn() == "WHITE")
        setTurn("BLACK");
    else
        setTurn("WHITE");
}

void Game::placeInDeadPlace(ChessPiece *piece)
{
    if(!moveValidationCheck)
        resetMoveCounter();
    if(piece->side() == "WHITE") {
        whiteDead.append(piece);
        displayDeadWhite();
    } else {
        blackDead.append(piece);
        displayDeadBlack();
    }
    alivePieces.removeAll(piece);
}

void Game::gameover(const QString & winner)
{
   disableMouseEventsWhenPanelIsShown();
   drawPanel(0, 0, width(), height(), Qt::black, 0.90);

   auto gameOverText = new QGraphicsPixmapItem();
   gameOverText->setPixmap(QPixmap(":/winners/images/Winners/" + winner + "Won.png"));
   gameOverText->setPos(width()/2.0-gameOverText->boundingRect().width()/2.0, 220);
   scene->addItem(gameOverText);

}

void Game::displayDeadWhite()
{
    int shift = 80;
    int j = 0;
    int k = 0;

    for(int i = 0, n = whiteDead.size(); i < n; i++) {
        if(j == 5) {
            k++;
            j = 0;
        }
        whiteDead[i]->setPos(shift * j++, scene->height() - shift*(k+1));
    }
}

void Game::displayDeadBlack()
{
    int shift = 80;
    int j = 0;
    int k = 0;

    for(int i = 0, n = blackDead.size(); i < n; i++) {
        if(j == 5) {
            k++;
            j = 0;
        }
        blackDead[i]->setPos(shift * j++, shift*k);
    }
}

void Game::checkForCheck()
{

     bool isBlackChecked = pawnAttack(blackKing)
                         || horizontalAttack(blackKing)
                         || verticalAttack(blackKing)
                         || diagonalAttack(blackKing)
                         || knightAttack(blackKing);
     if(isBlackChecked) {
         blackKing->setChecked(true);
         if(!moveValidationCheck) {
            blackKing->currentField()->setBrush(Qt::blue);
         }
     } else {
         blackKing->setChecked(false);
         if(!moveValidationCheck)
            blackKing->currentField()->resetOriginalColor();
     }


     bool whiteIsItChecked = pawnAttack(whiteKing)
                         || horizontalAttack(whiteKing)
                         || verticalAttack(whiteKing)
                         || diagonalAttack(whiteKing)
                         || knightAttack(whiteKing);

     if(whiteIsItChecked) {
         whiteKing->setChecked(true);
         if(!moveValidationCheck)
            whiteKing->currentField()->setBrush(Qt::blue);
     }  else {
         whiteKing->setChecked(false);
         if(!moveValidationCheck)
            whiteKing->currentField()->resetOriginalColor();
     }

     if(whiteIsItChecked || isBlackChecked) {
         checkDisplay->setVisible(true);
     } else {
         checkDisplay->setVisible(false);
     }

}


void Game::checkForCheckmateOrStalemate()
{
    bool whiteIsChecked = whiteKing->checked();
    bool blackIsChecked = blackKing->checked();

    if(numOfMovesWithoutEating == 50)
        gameover("none");

    if(alivePieces.size() == 2) {
        gameover("none");
    }

    if(alivePieces.size() == 3) {
        if(std::any_of(alivePieces.begin(), alivePieces.end(), [](ChessPiece * a){return a->type() == "KNIGHT" || a->type() == "BISHOP";}))
            gameover("none");
    }

    if(alivePieces.size() == 4) {
        if(std::count_if(alivePieces.begin(), alivePieces.end(), [](ChessPiece * a) {return a->type() == "KNIGHT" && a->side() == "WHITE";}) == 2
         || std::count_if(alivePieces.begin(), alivePieces.end(), [](ChessPiece * a) {return a->type() == "KNIGHT" && a->side() == "BLACK";}) == 2)
            gameover("none");
    }


    bool areTherePossibleMoves = false;
    moveValidationCheck = false;

    auto currentAlivePieces = alivePieces;
    for(auto item : currentAlivePieces) {
        if(item->side() == getTurn()) {
            item->generatePossibleMoves();
            item->decolorAvailableFields();
            if(!item->possibleMoves().empty()) {
                areTherePossibleMoves = true;
                break;
            }
        }
    }
    if(!areTherePossibleMoves) {
        if(getTurn() == "WHITE" && whiteIsChecked) {
            gameover("black");
        } else if(getTurn() == "BLACK" && blackIsChecked) {
            gameover("white");
        } else {
            gameover("none");
        }
    }
}

void Game::disableMouseEventsWhenPanelIsShown()
{

    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            board->fieldAt(i, j)->setEnabled(false);
        }
    }

    for(auto item : alivePieces)
        item->setEnabled(false);
    for(auto item : whiteDead)
        item->setEnabled(false);
    for(auto item : blackDead)
        item->setEnabled(false);

}

void Game::enableMouseEventsAfterClosingPanel()
{

    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            board->fieldAt(i, j)->setEnabled(true);
        }
    }

    for(auto item : alivePieces)
        item->setEnabled(true);
    for(auto item : whiteDead)
        item->setEnabled(true);
    for(auto item : blackDead)
        item->setEnabled(true);
}

void Game::updateHistory(const QString & move)
{
    QTextCursor cursor = movesHistoryDisplay->textCursor();
    if(getTurn() == "WHITE") {
        cursor.movePosition(QTextCursor::End);
        cursor.insertText(move + "\n");
        movesHistoryDisplay->setTextCursor(cursor);
    } else {
        cursor.movePosition(QTextCursor::End);
        cursor.insertText(move + "\t");
        movesHistoryDisplay->setTextCursor(cursor);
    }
}

void Game::updateMoveCounter()
{
    numOfMovesWithoutEating++;
}

void Game::resetMoveCounter()
{
    numOfMovesWithoutEating = 0;
}

bool Game::isWhiteKingChecked()
{
    return whiteKing->checked();
}

bool Game::isBlackKingChecked()
{
    return blackKing->checked();
}

bool Game::pawnAttack(King *a)
{
    bool isItChecked = false;
    int row = a->currentField()->row();
    int col = a->currentField()->column();
    if(a->side() == "BLACK") {
        if(row + 1 < 8
                && col - 1 >= 0
                && !board->fieldAt(row+1, col-1)->isItFree()
                && board->fieldAt(row+1, col-1)->currentPiece->side() =="WHITE"
                && board->fieldAt(row+1, col-1)->currentPiece->type() == "PAWN")
            isItChecked = true;

        if(row + 1 < 8
                && col + 1 < 8
                && !board->fieldAt(row+1, col+1)->isItFree()
                && board->fieldAt(row+1, col+1)->currentPiece->side() == "WHITE"
                && board->fieldAt(row+1, col+1)->currentPiece->type() == "PAWN")
            isItChecked = true;
    } else {
        if(row - 1 >= 0
                && col - 1 >= 0
                && !board->fieldAt(row-1, col-1)->isItFree()
                && board->fieldAt(row-1, col-1)->currentPiece->side() == "BLACK"
                && board->fieldAt(row-1, col-1)->currentPiece->type() == "PAWN")
            isItChecked = true;
        if(row - 1 >= 0
                && col + 1 < 8
                && !board->fieldAt(row-1, col+1)->isItFree()
                && board->fieldAt(row-1, col+1)->currentPiece->side() == "BLACK"
                && board->fieldAt(row-1, col+1)->currentPiece->type() == "PAWN")
            isItChecked = true;
    }

    return isItChecked;
}

bool Game::horizontalAttack(King *a)
{

    bool isItChecked = false;
    int row = a->currentField()->row();
    int col = a->currentField()->column();
    for(int i = col + 1; i < 8; i++) {
        if(!board->fieldAt(row, i)->isItFree()
                && board->fieldAt(row, i)->currentPiece->side() == a->side())
           break;

        if(!board->fieldAt(row, i)->isItFree()
                && board->fieldAt(row, i)->currentPiece->side() != a->side()) {
            if(board->fieldAt(row, i)->currentPiece->type() == "QUEEN"
                || board->fieldAt(row, i)->currentPiece->type() == "ROOK") {
                isItChecked = true;
                break;
            } else {
                break;
            }
        }

    }

    for(int i = col - 1; i >= 0; i--) {
        if(!board->fieldAt(row, i)->isItFree()
                && board->fieldAt(row, i)->currentPiece->side() == a->side())
           break;

        if(!board->fieldAt(row, i)->isItFree()
                && board->fieldAt(row, i)->currentPiece->side() != a->side()) {
            if(board->fieldAt(row, i)->currentPiece->type() == "QUEEN"
                || board->fieldAt(row, i)->currentPiece->type() == "ROOK") {
                isItChecked = true;
                break;
            } else {
                break;
            }
        }
    }

    return isItChecked;
}

bool Game::verticalAttack(King *a)
{
    bool isItChecked = false;
    int row = a->currentField()->row();
    int col = a->currentField()->column();

    for(int i = row + 1; i < 8; i++) {
        if(!board->fieldAt(i, col)->isItFree()
                && board->fieldAt(i, col)->currentPiece->side() == a->side())
           break;

        if(!board->fieldAt(i, col)->isItFree()
                && board->fieldAt(i, col)->currentPiece->side() != a->side()) {
            if((board->fieldAt(i, col)->currentPiece->type() == "QUEEN"
                    || board->fieldAt(i, col)->currentPiece->type() == "ROOK")) {
                isItChecked = true;
                break;
            } else {
                break;
            }
        }
    }

    for(int i = row - 1; i >= 0; i--) {
        if(!board->fieldAt(i, col)->isItFree()
                && board->fieldAt(i, col)->currentPiece->side() == a->side())
           break;

        if(!board->fieldAt(i, col)->isItFree()
                && board->fieldAt(i, col)->currentPiece->side() != a->side()) {
            if(board->fieldAt(i, col)->currentPiece->type() == "QUEEN"
                   || board->fieldAt(i, col)->currentPiece->type() == "ROOK") {
                isItChecked = true;
                break;
            } else {
                break;
            }
        }

    }

    return isItChecked;
}

bool Game::diagonalAttack(King *a)
{
    bool isItChecked = false;
    int row = a->currentField()->row();
    int col = a->currentField()->column();

    for(int i = row + 1, j = col + 1; i < 8 && j < 8; i++, j++) {
        if(!board->fieldAt(i, j)->isItFree()
                && board->fieldAt(i, j)->currentPiece->side() ==  a->side())
           break;
        if(!board->fieldAt(i, j)->isItFree()
                && board->fieldAt(i, j)->currentPiece->side() != a->side()) {
            if(board->fieldAt(i, j)->currentPiece->type() == "QUEEN"
                || board->fieldAt(i, j)->currentPiece->type() == "BISHOP") {
                isItChecked = true;
                break;
            } else {
                break;
            }
        }
    }

    for(int i = row + 1, j = col - 1; i < 8 && j >= 0; i++, j--) {
        if(!board->fieldAt(i, j)->isItFree()
                && board->fieldAt(i, j)->currentPiece->side() == a->side())
           break;
        if(!board->fieldAt(i, j)->isItFree()
                && board->fieldAt(i, j)->currentPiece->side() != a->side()) {
            if(board->fieldAt(i, j)->currentPiece->type() == "QUEEN"
                || board->fieldAt(i, j)->currentPiece->type() == "BISHOP") {
                isItChecked = true;
                break;
            } else {
                break;
            }
        }

    }

    for(int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--) {
        if(!board->fieldAt(i, j)->isItFree()
                && board->fieldAt(i, j)->currentPiece->side() == a->side())
           break;
        if(!board->fieldAt(i, j)->isItFree()
                && board->fieldAt(i, j)->currentPiece->side() != a->side()) {
            if(board->fieldAt(i, j)->currentPiece->type() == "QUEEN"
                || board->fieldAt(i, j)->currentPiece->type() == "BISHOP") {
                isItChecked = true;
                break;
            } else {
                break;
            }
        }
    }

    for(int i = row - 1, j = col + 1; i >= 0 && j < 8; i--, j++) {
        if(!board->fieldAt(i, j)->isItFree()
                && board->fieldAt(i, j)->currentPiece->side() == a->side())
           break;
        if(!board->fieldAt(i, j)->isItFree()
                && board->fieldAt(i, j)->currentPiece->side() != a->side()) {
            if(board->fieldAt(i, j)->currentPiece->type() == "QUEEN"
                || board->fieldAt(i, j)->currentPiece->type() == "BISHOP") {
                isItChecked = true;
                break;
            } else {
                break;
            }
        }
    }

    return isItChecked;
}

bool Game::knightAttack(King *a)
{
    bool isItChecked = false;
    int row = a->currentField()->row();
    int col = a->currentField()->column();

    int i = row - 2;
    int j = col + 1;

    if(i >= 0
            && j < 8
            && !board->fieldAt(i, j)->isItFree()
            && board->fieldAt(i, j)->currentPiece->side() != a->side()
            && board->fieldAt(i, j)->currentPiece->type() == "KNIGHT") {
        isItChecked = true;
    }

    i = row - 2;
    j = col - 1;

    if(i >= 0
            && j >= 0
            && !board->fieldAt(i, j)->isItFree()
            && board->fieldAt(i, j)->currentPiece->side() != a->side()
            && board->fieldAt(i, j)->currentPiece->type() == "KNIGHT") {
        isItChecked = true;
    }

    i = row - 1;
    j = col + 2;

    if(i >= 0
            && j < 8
            && !board->fieldAt(i, j)->isItFree()
            && board->fieldAt(i, j)->currentPiece->side() != a->side()
            && board->fieldAt(i, j)->currentPiece->type() == "KNIGHT") {
        isItChecked = true;
    }


    i = row - 1;
    j = col - 2;

    if(i >= 0
            && j >= 0
            && !board->fieldAt(i, j)->isItFree()
            && board->fieldAt(i, j)->currentPiece->side() != a->side()
            && board->fieldAt(i, j)->currentPiece->type() == "KNIGHT") {
        isItChecked = true;
    }


    i = row + 1;
    j = col - 2;

    if(i < 8
            && j >= 0
            && !board->fieldAt(i, j)->isItFree()
            && board->fieldAt(i, j)->currentPiece->side() != a->side()
            && board->fieldAt(i, j)->currentPiece->type() == "KNIGHT") {
        isItChecked = true;
    }

    i = row + 1;
    j = col + 2;

    if(i < 8
            && j < 8
            && !board->fieldAt(i, j)->isItFree()
            && board->fieldAt(i, j)->currentPiece->side() != a->side()
            && board->fieldAt(i, j)->currentPiece->type() == "KNIGHT") {
        isItChecked = true;
    }

    i = row + 2;
    j = col - 1;

    if(i < 8
            && j >= 0
            && !board->fieldAt(i, j)->isItFree()
            && board->fieldAt(i, j)->currentPiece->side() != a->side()
            && board->fieldAt(i, j)->currentPiece->type() == "KNIGHT") {
        isItChecked = true;
    }

    i = row + 2;
    j = col + 1;

    if(i < 8
            && j < 8
            && !board->fieldAt(i, j)->isItFree()
            && board->fieldAt(i, j)->currentPiece->side() != a->side()
            && board->fieldAt(i, j)->currentPiece->type() == "KNIGHT") {
        isItChecked = true;
    }

    return isItChecked;
}

void Game::displayPromotion(const QString & side)
{
    panel = drawPanel(0, 0, width(), height(), Qt::black, 0.90);
    scene->addItem(panel);
    disableMouseEventsWhenPanelIsShown();
    auto pickPromotionText = new QGraphicsPixmapItem();
    pickPromotionText->setPixmap(QPixmap(":/moves/images/pickPromotion.png"));
    pickPromotionText->setPos(width()/2.0-pickPromotionText->boundingRect().width()/2.0, 220);
    pickPromotionText->setParentItem(panel);
    scene->addItem(pickPromotionText);

    promotionPieces.append(new Queen(side, true));
    scene->addItem(promotionPieces[0]);

    promotionPieces[0]->setPos(width()/2.0 - 175, height()/2.0 + 50);
    promotionPieces.append(new Rook(side, true));

    scene->addItem(promotionPieces[1]);
    promotionPieces[1]->setPos(width()/2.0 - 175 + 90, height()/2.0 + 50);

    promotionPieces.append(new Bishop(side, true));
    scene->addItem(promotionPieces[2]);

    promotionPieces[2]->setPos(width()/2.0 - 175 + 180, height()/2.0 + 50);
    promotionPieces.append(new Knight(side, true));

    scene->addItem(promotionPieces[3]);
    promotionPieces[3]->setPos(width()/2.0 - 175 + 270, height()/2.0 + 50);
}

bool Game::isAlive(ChessPiece *piece)
{
    if(piece->side() == "WHITE" && whiteDead.contains(piece))
        return false;
    if(piece->side() == "BLACK" && blackDead.contains(piece))
        return false;
    return true;
}

bool Game::nextToKing(Field *field, const QString &kingSide)
{
    King * currentKing = nullptr;
    if(kingSide == "WHITE")
        currentKing = whiteKing;
    else
        currentKing = blackKing;

    if(field->row() >= 0
            && field->row() <= 7
            && field->column() >= 0
            && field->column() <= 7
            && qFabs(currentKing->currentField()->row() - field->row()) <= 1
            && qFabs(currentKing->currentField()->column() - field->column()) <= 1)
        return true;
    return false;
}

QGraphicsRectItem *Game::drawPanel(int x, int y, int width, int height, const QColor & color, double opacity)
{

       auto panel = new QGraphicsRectItem(x, y, width, height);
       QBrush brush;
       brush.setStyle(Qt::SolidPattern);
       brush.setColor(color);
       panel->setBrush(brush);
       panel->setOpacity(opacity);
       scene->addItem(panel);


       auto panel2 = new QGraphicsPixmapItem();
       panel2->setPixmap(QPixmap(":/textures/images/Textures/panelBackground.png").scaled(600, 400));
       panel2->setPos(x+this->width()/2.0-300, y+this->height()/2.0-200);
       panel2->setOpacity(1);

       scene->addItem(panel2);
       panel2->setParentItem(panel);

       return panel;

}

void Game::previousState()
{
    if(!moves.isEmpty()) {

        if(!moveValidationCheck) {
            QTextCursor cursor = movesHistoryDisplay->textCursor();
            cursor.movePosition(QTextCursor::End);
            if(getTurn() == "BLACK") {
                cursor.movePosition(QTextCursor::End);
                cursor.select(QTextCursor::LineUnderCursor);
                cursor.removeSelectedText();
                movesHistoryDisplay->setTextCursor(cursor);
            } else {
                QString newText = movesHistoryDisplay->toPlainText();
                newText.truncate(newText.lastIndexOf('\t'));
                movesHistoryDisplay->clear();
                movesHistoryDisplay->setText(newText + '\t');
            }
        }


        GameMove * lastMove = moves.last();

        setTurn(lastMove->m_movedPiece->side());
        lastMove->m_from->placePiece(lastMove->m_movedPiece);
        lastMove->m_movedPiece->setFirstMove(lastMove->m_wasItFirstMove);

        if(!moveValidationCheck) {
            if(selectedPiece) {
                selectedPiece->decolorAvailableFields();
                selectedPiece->currentField()->resetOriginalColor();
                selectedPiece = nullptr;
            }
        }

        lastMove->m_to->setIsItFree(true);
        lastMove->m_to->currentPiece = nullptr;

        checkDisplay->setVisible(lastMove->m_wasMovedInCheck);
        if(lastMove->m_wasMovedInCheck) {
            if(lastMove->m_movedPiece->side() == "WHITE") {
                whiteKing->currentField()->setBrush(Qt::blue);
             } else {
                blackKing->currentField()->setBrush(Qt::blue);
            }
        } else if(!lastMove->m_wasMovedInCheck) {
            if(lastMove->m_movedPiece->side() == "WHITE") {
                whiteKing->currentField()->resetOriginalColor();
             } else {
                blackKing->currentField()->resetOriginalColor();
            }
        }


        if(lastMove->m_hasEatenSomething) {
            if(lastMove->m_deadPiece->side() == "WHITE") {
                whiteDead.removeAll(lastMove->m_deadPiece);
            } else {
                blackDead.removeAll(lastMove->m_deadPiece);
            }
            alivePieces.push_back(lastMove->m_deadPiece);
            lastMove->m_deadPiece->setFirstMove(lastMove->m_eatenPieceFirstMove);
            lastMove->m_deadPieceField->placePiece(lastMove->m_deadPiece);
            lastMove->m_deadPiece->setPos(lastMove->m_deadPieceField->x(), lastMove->m_deadPieceField->y());
        }


        if(lastMove->m_wasOpponentInCheck) {
            if(lastMove->m_movedPiece->side() == "WHITE") {
                blackKing->currentField()->resetOriginalColor();
                blackKing->setChecked(false);
            } else {
                whiteKing->currentField()->resetOriginalColor();
                whiteKing->setChecked(false);
            }
        }


        if(moveValidationCheck) {
            if(lastMove->m_movedPiece->type() == "KING") {
                King * p = dynamic_cast<King*>(lastMove->m_movedPiece);
                if(p->checked())
                    p->setChecked(false);
            }
        }




        if(lastMove->m_smallCastling) {
            int row = lastMove->m_movedPiece->currentField()->row();
            board->fieldAt(row, 5)->setIsItFree(true);
            board->fieldAt(row, 6)->setIsItFree(true);
        }


        if(lastMove->m_largeCastling) {
            int row = lastMove->m_movedPiece->currentField()->row();
            board->fieldAt(row, 1)->setIsItFree(true);
            board->fieldAt(row, 2)->setIsItFree(true);
            board->fieldAt(row, 3)->setIsItFree(true);
        }


        if(lastMove->m_movedPiece->type() == "PAWN"
                && (lastMove->m_to->row() == 7
                    || lastMove->m_to->row() == 0)
                && !moveValidationCheck){
            alivePieces.removeAll(lastMove->promotionPiece);
            if(lastMove->m_movedPiece->side() == "WHITE")
                whiteDead.removeAll(lastMove->m_movedPiece);
            else {
                blackDead.removeAll(lastMove->m_movedPiece);
            }
            scene->removeItem(lastMove->promotionPiece);
            promotionPieces.clear();
        }
        moves.removeLast();
    }
}

void Game::start()
{
    scene->clear();
    moveValidationCheck = false;
    enpassantAvailable = false;
    numOfMovesWithoutEating = 0;

    checkDisplay = new QGraphicsPixmapItem();
    checkDisplay->setPixmap(QPixmap(":/moves/images/Moves/check.png"));
    checkDisplay->setPos(WINDOW_WIDTH / 2 , WINDOW_HEIGHT - checkDisplay->boundingRect().height() + 1);
    checkDisplay->setVisible(false);
    scene->addItem(checkDisplay);

    auto movesBackground = new QGraphicsPixmapItem();
    movesBackground->setPixmap(QPixmap(":/background/images/movesBackground.png"));
    movesBackground->setPos(width() - BUTTON_WIDTH - BUTTON_MARGIN ,BUTTON_MARGIN);
    scene->addItem(movesBackground);

    scrollArea = new QScrollArea();
    movesHistoryDisplay = new QTextEdit();
    movesHistoryDisplay->setReadOnly(true);
    movesHistoryDisplay->setFixedWidth(190);
    movesHistoryDisplay->setFixedHeight(550);
    movesHistoryDisplay->setFrameShape(QFrame::NoFrame);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setWidget(movesHistoryDisplay);
    scrollArea->setFixedWidth(190);
    scrollArea->setFixedHeight(550);
    scrollableHistoryWindow = scene->addWidget(scrollArea);
    scrollableHistoryWindow->setPos(width() - scrollArea->width() - BUTTON_MARGIN - 14, BUTTON_MARGIN + 66);

    board = new Chessboard();
    board->drawFields();
    board->drawChessPieces();
    selectedPiece = nullptr;
    whiteKing = dynamic_cast<King*>(board->fieldAt(7, 4)->currentPiece);
    blackKing = dynamic_cast<King*>(board->fieldAt(0, 4)->currentPiece);


    setTurn("WHITE");
}

