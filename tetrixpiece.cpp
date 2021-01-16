#include "tetrixpiece.h"


const QPoint TetrixPiece::CoordinatesTable[TetrixShapeCount][TetrixPieceBlocks] = {
        { { 0, 0 },   { 0, 0 },   { 0, 0 },   { 0, 0 } },
        { { 0, -1 },  { 0, 0 },   { -1, 0 },  { -1, 1 } },// Z
        { { 0, -1 },  { 0, 0 },   { 1, 0 },   { 1, 1 } }, // S
        { { 0, -1 },  { 0, 0 },   { 0, 1 },   { 0, 2 } }, // Line
        { { -1, 0 },  { 0, 0 },   { 1, 0 },   { 0, 1 } }, // T
        { { 0, 0 },   { 1, 0 },   { 0, 1 },   { 1, 1 } }, // Square
        { { -1, -1 }, { 0, -1 },  { 0, 0 },   { 0, 1 } }, // L
        { { 1, -1 },  { 0, -1 },  { 0, 0 },   { 0, 1 } }  // Mirrored L
    };
