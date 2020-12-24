#ifndef ROOK_H
#define ROOK_H
#include "chesspiece.h"
class Rook : public ChessPiece {
public:
    Rook(QString side, bool promotionPiece = false);
    ~Rook() = default;
    void setImage();
    void generatePossibleMoves();
    QString type();

};

#endif // ROOK_H
