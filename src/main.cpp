#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <stack>

#include "SDL2/SDL.h"
#include "Room.h"

#define WIDTH      1000 //600
#define HEIGHT     1000 //600
#define ROWS       100 //100
#define COLUMNS    100 //100
#define ROOM_WIDTH (WIDTH / ROWS)
#define FPS        512

using namespace std;

// Checks if there are still unvisited cells
bool unvisitedCells(const vector<Room>& mazeGrid) {
    for (auto & i : mazeGrid) { // I don't like this loop, I think it can be optimized
        if (!i.isVisited())
            return true;
    }
    return false;
}

// Randomly returns a cell that has not been visited and is adjacent to the given cell, if any.
int checkNeighbours(vector<Room> maze, Room r){
    int x = r.getX();
    int y = r.getY();
    vector<Room> neighbours;

    if (!maze[(x - 1) * ROWS + y].isVisited() && x > 0)
        neighbours.push_back(maze[(x - 1) * ROWS + y]);

    if (!maze[(x + 1) * ROWS + y].isVisited() && x < ROWS - 1)
        neighbours.push_back(maze[(x + 1) * ROWS + y]);

    if (!maze[x * ROWS + y + 1].isVisited() && y < COLUMNS - 1)
        neighbours.push_back(maze[x * ROWS + y + 1]);

    if (!maze[x * ROWS + y - 1].isVisited() && y > 0)
        neighbours.push_back(maze[x * ROWS + y - 1]);

    // If there are not unvisited adjacent rooms the check returns -1
    if (neighbours.empty())
        return -1;

    int randomIdx = rand() % neighbours.size();
    int next = neighbours[randomIdx].getY() + neighbours[randomIdx].getX() * ROWS;
    return next;
}

void cap_framerate(Uint32 starting_tick){
    if ( (1000/FPS) > SDL_GetTicks() - starting_tick ) {
        SDL_Delay( 1000 / FPS - (SDL_GetTicks() - starting_tick) );
    }
}

int main(int argc, char *argv[]){
    srand(time(NULL));
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer);

    if (window == NULL) {
        std::cout << "Error while initializing window!\n" << SDL_GetError();
        return 1;
    }

    Uint32 starting_tick;
    SDL_Event  event;
    bool running = true;

    vector<Room> mazeGrid;
    stack<Room*> roomStack;

    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLUMNS; j++){
            Room newRoom(i, j, ROOM_WIDTH);
            mazeGrid.push_back(newRoom);
        }
    }

    Room *current = &mazeGrid[0];
    current->visit();

    while (running) {
        starting_tick = SDL_GetTicks();

        while(SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){
                running = false;
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); // Yellow-ish 240 210 60
        SDL_RenderClear(renderer); // Paints everything

        int next = checkNeighbours(mazeGrid, *current);
        // If the current cell has any neighbour that has not been visited
        if (next != -1){
            // Choose randomly one of the neighbours
            Room &nextRoom = mazeGrid[next];
            // Push the current cell to the stack
            roomStack.push(current);
            // Remove the wall between the current cell and the chosen cell
            current->removeWalls(nextRoom);
            // Make the chosen cell the next current cell and mark it as visited
            current = &nextRoom;
            current->visit();
        } else if (!roomStack.empty()){ // If there are not unvisited neighbours we go back to the previous room
            Room &previousRoom = *roomStack.top();
            roomStack.pop();
            current = &previousRoom;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); // Pink-ish 230 30 120
        SDL_RenderClear(renderer); // Paints everything
        SDL_SetRenderDrawColor(renderer, 155, 155, 155, SDL_ALPHA_OPAQUE); // Yellow-ish

        // Cycle on the matrix cells: draws walls and background of the room accordingly to its state
        for (Uint32 i = 0; i < mazeGrid.size(); i++){
            if (!mazeGrid[i].isVisited()){
                SDL_SetRenderDrawColor(renderer, 155, 155, 155, SDL_ALPHA_OPAQUE); // Yellow-ish
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); // Pink-ish
            }

            SDL_Rect rect{
                mazeGrid[i].getX() * ROOM_WIDTH,
                mazeGrid[i].getY() * ROOM_WIDTH,
                ROOM_WIDTH,
                ROOM_WIDTH
            };

            SDL_RenderFillRect(renderer, &rect);
            SDL_SetRenderDrawColor(renderer, 155, 155, 155, SDL_ALPHA_OPAQUE); // Yellow-ish

            mazeGrid[i].show(renderer); // Draws the walls of the room
        }

        // Draws differently the current room
        SDL_SetRenderDrawColor(renderer, 200, 10, 10, SDL_ALPHA_OPAQUE); // Red-ish
        int xCoordCurrent = current->getX() * ROOM_WIDTH;
        int yCoordCurrent = current->getY() * ROOM_WIDTH;

        SDL_Rect rect{
            xCoordCurrent,
            yCoordCurrent,
            ROOM_WIDTH,
            ROOM_WIDTH
        };

        SDL_RenderFillRect(renderer, &rect);
        SDL_RenderPresent(renderer); // Actually updates the screen

        cap_framerate(starting_tick);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}