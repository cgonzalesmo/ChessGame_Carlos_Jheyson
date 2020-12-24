#include "bishop.h"
#include "game.h"
extern Game * game;
Bishop::Bishop(QString side, bool promotionPiece) : ChessPiece (side, promotionPiece)
{
    setImage();
}


QString Bishop::type()
{
    return "BISHOP";
}

void Bishop::setImage()
{
    if(m_side == "WHITE") {
        setPixmap(QPixmap(":/chesspiece/images/ChessPieces/WhiteBishop.png"));
    } else {
        setPixmap(QPixmap(":/chesspiece/images/ChessPieces/BlackBishop.png"));
    }
}

void Bishop::generatePossibleMoves()
{
    m_possibleMoves.clear();
    int row = this->currentField()->row();
    int col = this->currentField()->column();

    //arriba derecha
    for(int i = row - 1, j = col + 1; i >= 0 && j <= 7; i--, j++) {
        if(!game->board->fieldAt(i, j)->isItFree()
                && game->board->fieldAt(i, j)->currentPiece->side() == this->side()) {
            break;
        }
        m_possibleMoves.append(game->board->fieldAt(i, j));
        if(!isLegalMove(game->board->fieldAt(i, j))) {
            m_possibleMoves.removeLast();
            if(!game->board->fieldAt(i, j)->isItFree())
                break;
        } else {
            if(colorAvailableFields(m_possibleMoves.last()))
                break;
        }

    }

    //arriba izquierda
    for(int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--) {
        if(!game->board->fieldAt(i, j)->isItFree()
                && game->board->fieldAt(i, j)->currentPiece->side() == this->side()) {
            break;
        }
        m_possibleMoves.append(game->board->fieldAt(i, j));
        if(!isLegalMove(game->board->fieldAt(i, j))) {
            m_possibleMoves.removeLast();
            if(!game->board->fieldAt(i, j)->isItFree())
                break;
        } else {
            if(colorAvailableFields(m_possibleMoves.last()))
                break;
        }

    }

    //abajo izquierda
    for(int i = row + 1, j = col - 1; i <= 7 && j >= 0; i++, j--) {
        if(!game->board->fieldAt(i, j)->isItFree()
                && game->board->fieldAt(i, j)->currentPiece->side() == this->side()) {
            break;
        }
        m_possibleMoves.append(game->board->fieldAt(i, j));
        if(!isLegalMove(game->board->fieldAt(i, j))) {
            m_possibleMoves.removeLast();
        } else {
            if(colorAvailableFields(m_possibleMoves.last()))
                break;
        }

    }

    //abajo derecha
    for(int i = row + 1, j = col + 1; i <= 7 && j <= 7; i++, j++) {
        if(!game->board->fieldAt(i, j)->isItFree()
                && game->board->fieldAt(i, j)->currentPiece->side() == this->side()){
            break;
        }
        m_possibleMoves.append(game->board->fieldAt(i, j));
        if(!isLegalMove(game->board->fieldAt(i, j))) {
            m_possibleMoves.removeLast();
            if(!game->board->fieldAt(i, j)->isItFree())
                break;
        } else {
            if(colorAvailableFields(m_possibleMoves.last()))
                break;
        }

    }
}
