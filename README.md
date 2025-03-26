todo list:

HQH
1. Viết lại 3 monitor 
2. Cài delay khi gửi garbage 

77x17
1. Spin detect (bị mất effect đột ngột khi đặt khối mới)
2. Cài keyConfiguration.
3. Cài thêm time - lines.
hold problem ?

file CurrentBlockWithNetwork caanf code laij!
void CurrentBlockWithNetwork::compressWithSpin(sf::Packet &packet) {
    packet << block->getStateID() << posY << posX << shadowPosY << isJustSpin() << block->getStateID();
}
