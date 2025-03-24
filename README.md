# Install prerequisites:

    sudo apt install libncurses5-dev libncursesw5-dev

hold problem ?

file CurrentBlockWithNetwork caanf code laij!
void CurrentBlockWithNetwork::compressWithSpin(sf::Packet &packet) {
    packet << block->getStateID() << posY << posX << shadowPosY << isJustSpin() << block->getStateID();
}