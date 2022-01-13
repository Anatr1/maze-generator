#include <iostream>
#include "SDL2/SDL.h"

//Creates the Room object which describes a single cell of the matrix which will become our maze
class Room {
private:
    int x, y, width; // Top left corner coordinates and room's width
    bool walls[4];   // The walls of the room
    bool visited;    // We use this to remember if the room has already been expanded

public:
    Room(int i, int j, int width);     // Public constructor

    //Utils
    void removeWalls(Room &r);         // Remove walls between this and an adjacent room
    void removeWall(int wall);         // Remove a wall in this room
    void printWalls();                 // Prints the walls (bools) on stdout. I'm not sure why
    void visit();                      // Marks the room as visited
    int getPositionInVector(int size) const; // I have no idea what is this for

    //Getters
    int getX() const;
    int getY() const;
    int getWidth() const;
    bool isVisited() const;

    //Renderer
    void show(SDL_Renderer* renderer);  // Renders the room
};