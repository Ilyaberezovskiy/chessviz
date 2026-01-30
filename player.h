#ifndef PLAYER_H
#define PLAYER_H

#include "pieces.h"
#include <string>

class Player {
public:
    Player(const std::string& name, Color color);

    const std::string& getName() const { return name_; }
    Color getColor() const { return color_; }

private:
    std::string name_;
    Color color_;
};

#endif
