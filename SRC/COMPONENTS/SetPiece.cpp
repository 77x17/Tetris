#include "SetPiece.hpp"

// SetInPiece::SetInPiece(int numPlayer, uint16_t _shape):Piece(_shape) {
//     cnt = numPlayer; nextPiece = nullptr;
// }

// SetInPiece::~SetInPiece() {}

SetPiece::SetPiece(int nPlayer) {
    nPlayer = nPlayer;
    updateNext();
}

SetPiece::~SetPiece() {
    if (head == nullptr) return;
    while (head) {
        Piece* p = head; head = head->nextPiece;
        delete p; 
    }
    head = tail = nullptr;
}

Piece* SetPiece::getNewNode(int type) {
    uint16_t pieceType = -1;
    switch (type) {
    case 1:
        return new Piece(nPlayer, LINE);
    default:
        return nullptr;
    }
}

void SetPiece::updateNext() {
    if (tail == nullptr) {
        head = tail = getNewNode(1);
        for (int i = 0; i < 4; i++) {
            tail->nextPiece = getNewNode(1);
            tail = tail->nextPiece;
        }
        return;
    }
    tail->nextPiece = getNewNode(1);
    while (head->cnt == 0) {
        Piece* p = head; head = head->nextPiece;
        delete p;
    }
}

Piece* SetPiece::get(int num) {
    Piece* ans = this->head;
    while (num--) ans = ans->nextPiece;
    return ans;
}