#ifndef PAWN_H
#define PAWN_H
#include "chesspiece.h"
class Pawn : public ChessPiece {
public:
    Pawn(QString side);
    ~Pawn() = default;
    void setImage();
    void generatePossibleMoves();
    QString type();
};

#endif // PAWN_H
