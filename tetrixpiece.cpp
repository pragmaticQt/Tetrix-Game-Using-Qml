#include "tetrixpiece.h"


const QPoint TetrixPiece::CoordinatesTable[TetrixShape::Count][TetrixPiece::TetrixPieceBlocks] = {
        { { 0, 0 },   { 0, 0 },   { 0, 0 },   { 0, 0 } },
        { { 0, 0 },   { 1, 0 },   { 0, 1 },   { 1, 1 } }, // Square

        { { 0, -1 },  { 0, 0 },   { -1, 0 },  { -1, 1 } },// Z
        { { 0, -1 },  { 0, 0 },   { 1, 0 },   { 1, 1 } }, // S
        { { 0, -1 },  { 0, 0 },   { 0, 1 },   { 0, 2 } }, // Line

        { { -1, 0 },  { 0, 0 },   { 0, 1 },   { 1, 1 } },// Z1
        { { -1, 0 },  { 0, 0 },   { 0, -1 },   { 1, -1 } }, // S1
        { { -1, 0 },  { 0, 0 },   { 1, 0 },   { 2, 0 } }, // Line1

        { { -1, 0 },  { 0, 0 },   { 1, 0 },   { 0, 1 } }, // T        
        { { -1, -1 }, { 0, -1 },  { 0, 0 },   { 0, 1 } }, // L
        { { 1, -1 },  { 0, -1 },  { 0, 0 },   { 0, 1 } },  // Mirrored L

        { { 0, 1 },  { 0, 0 },   { 0, -1 },   { 1, 0 } }, // T1
        { { -1, 1 }, { -1, 0 },  { 0, 0 },   { 1, 0 } }, // L1
        { { -1, -1 },  { -1, 0 },  { 0, 0 },   { 1, 0 } },  // Mirrored L1

        { { 1, 0 },  { 0, 0 },   { -1, 0 },   { 0, -1 } }, // T2
        { { 1, 1 }, { 0, 1 },  { 0, 0 },   { 0, -1 } }, // L2
        { { -1, 1 },  { 0, 1 },  { 0, 0 },   { 0, -1 } },  // Mirrored L2

        { { 0, -1 },  { 0, 0 },   { 0, 1 },   { -1, 0 } }, // T3
        { { 1, -1 }, { 1, 0 },  { 0, 0 },   { -1, 0 } }, // L3
        { { 1, 1 },  { 1, 0 },  { 0, 0 },   { -1, 0 } }  // Mirrored L3
    };
