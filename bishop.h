#ifndef BISHOP_H
#define BISHOP_H
#include "chesspiece.h"
class Bishop : public ChessPiece {
public:
    Bishop(QString side, bool promotionPiece = false);
    ~Bishop() = default;
    void setImage();
    void generatePossibleMoves();
    QString type();
};

#endif // BISHOP_H
