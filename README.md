todo list:

HQH
1. Viết lại 3 monitor 
2. Cài delay khi gửi garbage 
3. Chỉnh lại rotate của chữ I.

77x17
1. Spin detect (bị mất effect đột ngột khi đặt khối mới)
2. Cài keyConfiguration.
3. Cài thêm time - lines.
4. Chỉnh lại số garbage gửi đi bên góc trái Infor là tổng của các garbage gửi đi trong combo chứ ko phải chỉ 1 mình nó.

hold problem ? # là răng (77x17) 

file CurrentBlockWithNetwork caanf code laij!
void CurrentBlockWithNetwork::compressWithSpin(sf::Packet &packet) {
    packet << block->getStateID() << posY << posX << shadowPosY << isJustSpin() << block->getStateID();
}
