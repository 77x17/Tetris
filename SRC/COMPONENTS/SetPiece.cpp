#include "SetPiece.hpp"

SetInPiece::SetInPiece(int numPlayer, uint16_t _shape):Piece(_shape) {
    cnt = numPlayer; nextPiece = nullptr;
}

SetInPiece::~SetInPiece() {}

SetPiece::SetPiece(int nPlayer) {
    nPlayer = nPlayer;
    head = tail = nullptr;
}

SetPiece::~SetPiece() {
    if (head == nullptr) return;
    while (head) {
        SetInPiece* p = head; head = head->nextPiece;
        delete p; 
    }
    head = tail = nullptr;
}

SetInPiece* SetPiece::getNewNode(int type) {
    uint16_t pieceType = -1;
    switch (type) {
    case 1:
        return new SetInPiece(nPlayer, LINE);
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
        SetInPiece* p = head; head = head->nextPiece;
        delete p;
    }
}

SetInPiece* SetPiece::get(int num) {
    SetInPiece* ans = this->head;
    while (num--) ans = ans->nextPiece;
    return ans;
}