#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include "field.h"
#include "chesspiece.h"
#include <QVector>
#include <QVBoxLayout>
class Chessboard : public QGraphicsRectItem{
public:
    Chessboard();
    ~Chessboard();
    void setWhitePieces();
    void setBlackPieces();
    void drawFields() const;
    void drawChessPieces() const;
    Field * fieldAt(int i, int j) const;
private:
    std::array<std::array<Field *, 8>, 8> m_fields;
    QList <ChessPiece *> white;
    QList <ChessPiece *> black;
};

#endif // CHESSBOARD_H
