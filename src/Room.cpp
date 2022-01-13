#include "Room.h"

using namespace std;

Room::Room(int i, int j, int width) {
    this->x = i;
    this->y = j;
    this->width = width;
    walls[0] = true;
    walls[1] = true;
    walls[2] = true;
    walls[3] = true;
    visited = false;
}
 void Room::removeWalls(Room &r) {
    if (this->x - r.x == -1){
        this->removeWall(1);
        r.removeWall(3);
    }
     if (this->x - r.x == 1){
         this->removeWall(3);
         r.removeWall(1);
     }
     if (this->y - r.y == -1){
         this->removeWall(2);
         r.removeWall(0);
     }
     if (this->y - r.y == 1) {
         this->removeWall(0);
         r.removeWall(2);
     }
 }

 void Room::removeWall(int wall) {
    this->walls[wall] = false;
}

void Room::printWalls() {
    for (bool wall : walls){
        std::cout << wall << " ";
    }
    std::cout << "\n";
}

void Room::visit() {
    this->visited = true;
}

int Room::getPositionInVector(int size) const {
    return this->x * size + this->y;
}

int Room::getX() const {
    return this->x;
}

int Room::getY() const {
    return this->y;
}

int Room::getWidth() const  {
    return this->width;
}

bool Room::isVisited() const {
    return this->visited;
}

void Room::show(SDL_Renderer *renderer) {
    int xCoord = this->x * width;
    int yCoord = this->y * width;

    if (this->walls[0])
        SDL_RenderDrawLine(renderer, xCoord, yCoord,xCoord + this->width, yCoord);
    if (this->walls[1])
        SDL_RenderDrawLine(renderer, xCoord + this->width, yCoord, xCoord + this->width, yCoord + this->width);
    if (this->walls[2])
        SDL_RenderDrawLine(renderer, xCoord, yCoord + this->width, xCoord + this->width, yCoord + this->width);
    if (this->walls[3])
        SDL_RenderDrawLine(renderer, xCoord, yCoord, xCoord, yCoord + this->width);
}



