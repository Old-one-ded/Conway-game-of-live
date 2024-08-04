#include <stdio.h> // Include standard input/output library
#include <stdlib.h> // Include standard library for functions like atoi, rand, etc.
#include <unistd.h> // Include header file for miscellaneous system calls, including usleep

#define FOREACH_CELL(i, j, rows, cols) for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) // macro for iterating over cells

void display(const void *data, int width, int height) { // function to display the game board
    const unsigned char (*grid)[width] = data; // cast the data pointer to a 2D array of unsigned chars
    printf("\033[H"); // move cursor to the top-left corner of the terminal
    for (int row = 0; row < height; row++) { // iterate over rows
        for (int col = 0; col < width; col++) { // iterate over columns
            printf(grid[row][col] ? "\033[07m  \033[m" : "  "); // print a character based on cell state
        }
        printf("\033[E"); // move cursor to the next line
    }
    fflush(stdout); // delete the output buffer
}

void update(void *data, int width, int height) { // function to update the game state
    unsigned char (*grid)[width] = data; // cast the data pointer to a 2D array of unsigned chars
    unsigned char new_grid[height][width]; // create a new grid to store the next generation

    FOREACH_CELL(row, col, height, width) { // iterate over each cell
        int neighbors = 0; // count the number of live cels nearby
        for (int dy = -1; dy <= 1; dy++) { // check neighboring cells
            for (int dx = -1; dx <= 1; dx++) {
                neighbors += grid[(row + dy + height) % height][(col + dx + width) % width]; // boundaries
            }
        }
        neighbors -= grid[row][col]; // Subtract the current cell's state
        new_grid[row][col] = (neighbors == 3 || (neighbors == 2 && grid[row][col])); // apply the rules of life
    }
    FOREACH_CELL(row, col, height, width) grid[row][col] = new_grid[row][col]; // copy the new generation to the current grid
}

unsigned int get_speed() { // function to get the desired speed in mileseconds from the user
    unsigned int speed;
    printf("Enter desired speed (microseconds per frame, 100000-1000000): ");
    while (scanf("%u", &speed) != 1 || speed < 100000 || speed > 1000000) { // check input
        printf("Invalid input. Please enter a number between 100000 and 1000000: ");
        fflush(stdin);
    }
    return speed;
}

void simulate(int width, int height) { // function to run the game simulation
    unsigned char world[height][width]; // create the game world
    unsigned int speed = get_speed(); // get the desired speed

    FOREACH_CELL(row, col, height, width) world[row][col] = rand() < RAND_MAX / 10; // initialize the world randomly
    while (1) { // main game loop
        display(world, width, height); // display the current generation
        update(world, width, height); // update the game state
        usleep(speed); // pause for the specified speed
    }
}

int main(int argc, char **argv) { // main function
    int width = 80, height = 25; // default width and height
    simulate(width, height); // start the simulation
    return 0; // exit
}