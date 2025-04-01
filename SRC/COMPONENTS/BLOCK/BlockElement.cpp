#include "BlockElement.hpp"

BlockElement::BlockElement() : block(nullptr), next(nullptr) {}
BlockElement::BlockElement(Block* tmp) : block(tmp), next(nullptr) {}