#include "king.h"
#include "game.h"
#include <QDebug>
extern Game * game;
King::King(QString side) : ChessPiece (side), m_isChecked(false)
{
    setImage();
}


QString King::type()
{
    return "KING";
}

void King::setImage()
{
    if(m_side == "WHITE") {
        setPixmap(QPixmap(":/chesspiece/images/ChessPieces/WhiteKing.png"));
    } else {
        setPixmap(QPixmap(":/chesspiece/images/ChessPieces/BlackKing.png"));
    }
}

void King::generatePossibleMoves()
{
    m_possibleMoves.clear();
    int row = this->currentField()->row();
    int col = this->currentField()->column();
    int i, j;
    QString opponentSide;
    if(side() == "WHITE")
        opponentSide = "BLACK";
    else {
        opponentSide = "WHITE";
    }

    //arriba
    i = row - 1;
    j = col;
    if(i >= 0 && ((game->board->fieldAt(i, j)->isItFree() && !game->nextToKing(game->board->fieldAt(i, j), opponentSide))
                            || (!game->board->fieldAt(i, j)->isItFree() && game->board->fieldAt(i, j)->currentPiece->side() != this->side()))) {

        m_possibleMoves.append(game->board->fieldAt(i, j));
        if(!isLegalMove(game->board->fieldAt(i, j))) {
            m_possibleMoves.removeLast();
        } else {
            colorAvailableFields(m_possibleMoves.last());
        }
    }

    //abajo
    i = row + 1;
    j = col;
    if(i <= 7 && ((game->board->fieldAt(i, j)->isItFree() && !game->nextToKing(game->board->fieldAt(i, j), opponentSide))
                            || (!game->board->fieldAt(i, j)->isItFree() && game->board->fieldAt(i, j)->currentPiece->side() != this->side()))) {
        m_possibleMoves.append(game->board->fieldAt(i, j));
        if(!isLegalMove(game->board->fieldAt(i, j))) {
            m_possibleMoves.removeLast();
        } else {
            colorAvailableFields(m_possibleMoves.last());
        }
    }

    //izquierda
    i = row;
    j = col - 1;
    if(j >= 0 && ((game->board->fieldAt(i, j)->isItFree() && !game->nextToKing(game->board->fieldAt(i, j), opponentSide))
                            || (!game->board->fieldAt(i, j)->isItFree() && game->board->fieldAt(i, j)->currentPiece->side() != this->side()))) {
        m_possibleMoves.append(game->board->fieldAt(i, j));
        if(!isLegalMove(game->board->fieldAt(i, j))) {
            m_possibleMoves.removeLast();
        } else {
            colorAvailableFields(m_possibleMoves.last());
        }
    }

    //derecha
    i = row;
    j = col + 1;
    if(j <= 7 && ((game->board->fieldAt(i, j)->isItFree() && !game->nextToKing(game->board->fieldAt(i, j), opponentSide))
                            || (!game->board->fieldAt(i, j)->isItFree() && game->board->fieldAt(i, j)->currentPiece->side() != this->side()))) {
        m_possibleMoves.append(game->board->fieldAt(i, j));
        if(!isLegalMove(game->board->fieldAt(i, j))) {
            m_possibleMoves.removeLast();
        } else {
            colorAvailableFields(m_possibleMoves.last());
        }
    }


    //arriba izquierda
    i = row - 1;
    j = col - 1;
    if(i >= 0 && j >= 0 && ((game->board->fieldAt(i, j)->isItFree() && !game->nextToKing(game->board->fieldAt(i, j), opponentSide))
                            || (!game->board->fieldAt(i, j)->isItFree() && game->board->fieldAt(i, j)->currentPiece->side() != this->side()))) {

        m_possibleMoves.append(game->board->fieldAt(i, j));
        if(!isLegalMove(game->board->fieldAt(i, j))) {
            m_possibleMoves.removeLast();
        } else {
            colorAvailableFields(m_possibleMoves.last());
        }
    }

    //arriba derecha
    i = row - 1;
    j = col + 1;
    if(i >= 0 && j <= 7 && ((game->board->fieldAt(i, j)->isItFree() && !game->nextToKing(game->board->fieldAt(i, j), opponentSide))
                            || (!game->board->fieldAt(i, j)->isItFree() && game->board->fieldAt(i, j)->currentPiece->side() != this->side()))) {
        m_possibleMoves.append(game->board->fieldAt(i, j));
        if(!isLegalMove(game->board->fieldAt(i, j))) {
            m_possibleMoves.removeLast();
        } else {
            colorAvailableFields(m_possibleMoves.last());
        }
    }

    //abajo derecha
    i = row + 1;
    j = col + 1;
    if(i <= 7 && j <= 7 && ((game->board->fieldAt(i, j)->isItFree() && !game->nextToKing(game->board->fieldAt(i, j), opponentSide))
                            || (!game->board->fieldAt(i, j)->isItFree() && game->board->fieldAt(i, j)->currentPiece->side() != this->side()))) {
        m_possibleMoves.append(game->board->fieldAt(i, j));
        if(!isLegalMove(game->board->fieldAt(i, j))) {
            m_possibleMoves.removeLast();
        } else {
            colorAvailableFields(m_possibleMoves.last());
        }
    }

    //abajo izquierda
    i = row + 1;
    j = col - 1;
    if(i <= 7 && j >= 0 && ((game->board->fieldAt(i, j)->isItFree() && !game->nextToKing(game->board->fieldAt(i, j), opponentSide))
                            || (!game->board->fieldAt(i, j)->isItFree() && game->board->fieldAt(i, j)->currentPiece->side() != this->side()))) {
        m_possibleMoves.append(game->board->fieldAt(i, j));
        if(!isLegalMove(game->board->fieldAt(i, j))) {
            m_possibleMoves.removeLast();
        } else {
            colorAvailableFields(m_possibleMoves.last());
        }
    }
    //enrroque corto
    if(firstMove()
            && !m_isChecked
            && !game->board->fieldAt(row, 7)->isItFree()
            && game->board->fieldAt(row, 7)->currentPiece->type() == "ROOK"
            && game->board->fieldAt(row, 7)->currentPiece->firstMove()
            && m_possibleMoves.contains(game->board->fieldAt(row, 5))
            && game->board->fieldAt(row, 5)->isItFree()
            && game->board->fieldAt(row, 6)->isItFree()) {
        m_possibleMoves.append(game->board->fieldAt(row, 6));
        if(!isLegalMove(game->board->fieldAt(row, 6))) {
            m_possibleMoves.removeLast();
        } else {
            m_possibleMoves.removeLast();
            m_possibleMoves.append(game->board->fieldAt(row, 7));
            colorAvailableFields(m_possibleMoves.last());
        }
    }
    //enrroque largo
    if(firstMove()
            && !m_isChecked
            && !game->board->fieldAt(row, 0)->isItFree()
            && game->board->fieldAt(row, 0)->currentPiece->type() == "ROOK"
            && game->board->fieldAt(row, 0)->currentPiece->firstMove()
            && m_possibleMoves.contains(game->board->fieldAt(row, 3))
            && game->board->fieldAt(row, 1)->isItFree()
            && game->board->fieldAt(row, 2)->isItFree()
            && game->board->fieldAt(row, 3)->isItFree()) {
        m_possibleMoves.append(game->board->fieldAt(row, 2));
        if(!isLegalMove(game->board->fieldAt(row, 2))) {
            m_possibleMoves.removeLast();
        } else {
            m_possibleMoves.removeLast();
            m_possibleMoves.append(game->board->fieldAt(row, 0));
            colorAvailableFields(m_possibleMoves.last());
        }
    }

}

bool King::checked()
{
    return m_isChecked;
}

void King::setChecked(bool value)
{
    m_isChecked = value;
}
