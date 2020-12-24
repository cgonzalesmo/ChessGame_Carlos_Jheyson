#include "knight.h"
#include "game.h"
extern Game * game;
Knight::Knight(QString side, bool promotionPiece) : ChessPiece(side, promotionPiece)
{
    setImage();
}


QString Knight::type()
{
    return "KNIGHT";
}

void Knight::setImage()
{
    if(m_side == "WHITE") {
        setPixmap(QPixmap(":/chesspiece/images/ChessPieces/WhiteKnight.png"));
    } else {
        setPixmap(QPixmap(":/chesspiece/images/ChessPieces/BlackKnight.png"));
    }
}

void Knight::generatePossibleMoves()
{
    m_possibleMoves.clear();
    int row = this->currentField()->row();
    int col = this->currentField()->column();

    int i, j;
    //arriba arriba izquierda
    i = row - 2;
    j = col - 1;
    if(i >= 0 && j >= 0 && (game->board->fieldAt(i, j)->isItFree()
                            || (!game->board->fieldAt(i, j)->isItFree() && game->board->fieldAt(i, j)->currentPiece->side() != this->side()))) {
        m_possibleMoves.append(game->board->fieldAt(i, j));
        if(!isLegalMove(game->board->fieldAt(i, j))) {
            m_possibleMoves.removeLast();
        } else {
            colorAvailableFields(m_possibleMoves.last());
        }
    }

    //arriba arriba derecha
    i = row - 2;
    j = col + 1;
    if(i >= 0 && j <= 7 && (game->board->fieldAt(i, j)->isItFree()
                            || (!game->board->fieldAt(i, j)->isItFree() && game->board->fieldAt(i, j)->currentPiece->side() != this->side()))) {
        m_possibleMoves.append(game->board->fieldAt(i, j));
        if(!isLegalMove(game->board->fieldAt(i, j))) {
            m_possibleMoves.removeLast();
        } else {
            colorAvailableFields(m_possibleMoves.last());
        }
    }

    //arriba derecha derecha
    i = row - 1;
    j = col + 2;
    if(i >= 0 && j <= 7 && (game->board->fieldAt(i, j)->isItFree()
                            || (!game->board->fieldAt(i, j)->isItFree() && game->board->fieldAt(i, j)->currentPiece->side() != this->side()))) {
        m_possibleMoves.append(game->board->fieldAt(i, j));
        if(!isLegalMove(game->board->fieldAt(i, j))) {
            m_possibleMoves.removeLast();
        } else {
            colorAvailableFields(m_possibleMoves.last());
        }
    }

    //arriba izquierda izquierda
    i = row - 1;
    j = col - 2;
    if(i >= 0 && j >= 0 && (game->board->fieldAt(i, j)->isItFree()
                            || (!game->board->fieldAt(i, j)->isItFree() && game->board->fieldAt(i, j)->currentPiece->side() != this->side()))) {
        m_possibleMoves.append(game->board->fieldAt(i, j));
        if(!isLegalMove(game->board->fieldAt(i, j))) {
            m_possibleMoves.removeLast();
        } else {
            colorAvailableFields(m_possibleMoves.last());
        }
    }

    //abajo izquierda izquierda
    i = row + 1;
    j = col - 2;
    if(i <= 7 && j >= 0 && (game->board->fieldAt(i, j)->isItFree()
                            || (!game->board->fieldAt(i, j)->isItFree() && game->board->fieldAt(i, j)->currentPiece->side() != this->side()))) {
        m_possibleMoves.append(game->board->fieldAt(i, j));
        if(!isLegalMove(game->board->fieldAt(i, j))) {
            m_possibleMoves.removeLast();
        } else {
            colorAvailableFields(m_possibleMoves.last());
        }
    }

    //abajo derecha derecha
    i = row + 1;
    j = col + 2;
    if(i <= 7 && j <= 7 && (game->board->fieldAt(i, j)->isItFree()
                            || (!game->board->fieldAt(i, j)->isItFree() && game->board->fieldAt(i, j)->currentPiece->side() != this->side()))) {
        m_possibleMoves.append(game->board->fieldAt(i, j));
        if(!isLegalMove(game->board->fieldAt(i, j))) {
            m_possibleMoves.removeLast();
        } else {
            colorAvailableFields(m_possibleMoves.last());
        }
    }

    //abajo abajo derecha
    i = row + 2;
    j = col + 1;
    if(i <= 7 && j <= 7 && (game->board->fieldAt(i, j)->isItFree()
                            || (!game->board->fieldAt(i, j)->isItFree() && game->board->fieldAt(i, j)->currentPiece->side() != this->side()))) {
        m_possibleMoves.append(game->board->fieldAt(i, j));
        if(!isLegalMove(game->board->fieldAt(i, j))) {
            m_possibleMoves.removeLast();
        } else {
            colorAvailableFields(m_possibleMoves.last());
        }
    }

    //abajo abajo izquierda
    i = row + 2;
    j = col - 1;
    if(i <= 7 && j >= 0 && (game->board->fieldAt(i, j)->isItFree()
                            || (!game->board->fieldAt(i, j)->isItFree() && game->board->fieldAt(i, j)->currentPiece->side() != this->side()))) {
        m_possibleMoves.append(game->board->fieldAt(i, j));
        if(!isLegalMove(game->board->fieldAt(i, j))) {
            m_possibleMoves.removeLast();
        } else {
            colorAvailableFields(m_possibleMoves.last());
        }
    }


}
