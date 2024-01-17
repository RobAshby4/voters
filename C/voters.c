#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define HEIGHT 24
#define WIDTH 80 
#define A_VOTE '@'
#define B_VOTE ' '

typedef struct Grid {
    char* voters[HEIGHT];
} Grid;

Grid* generate_grid() {
    Grid* new_grid = (Grid *) calloc(HEIGHT, sizeof(char *));
    for (int i = 0; i < HEIGHT; i++) {
        new_grid->voters[i] = (char*) calloc(WIDTH, sizeof(char));
    }
    return new_grid;
}

void populate_grid(Grid* voter_grid) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (rand() % 2 == 0) {
                voter_grid->voters[i][j] = B_VOTE;
            } else {
                voter_grid->voters[i][j] = A_VOTE;
            }
        }
    }
}

void print_grid(Grid* current_grid) {
    printf("\n");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c", current_grid->voters[i][j]);
        }
        printf("\n");
    }
}

void copy_grid(Grid* from, Grid* to) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            to->voters[i][j] = from->voters[i][j];
        }
    }
}

void free_grid(Grid* voter_grid) {
    for (int i = 0; i < HEIGHT; i++) {
        free(voter_grid->voters[i]);
    }
    free(voter_grid);
}

char replace_with(Grid* voter_grid, int y, int x) {
    int vote_counter = 0;
    int num_neighbors = 0;
    for (int i = y - 1; i <= y + 1; i++) {
        for (int j = x - 1; j <= x + 1; j++) {
            if (!(i < 0 || i >= HEIGHT || j < 0 || j >= WIDTH)) {
                num_neighbors++;
                if (voter_grid->voters[i][j] == A_VOTE) {
                    vote_counter++;
                }
            }
        }
    }

    // printf("\ny:%d x:%d, count:%d, neighbors:%d", y, x, vote_counter, num_neighbors);
    if (num_neighbors % 2 == 0) {
        if (vote_counter > num_neighbors / 2) {
            return A_VOTE;
        } else if (vote_counter == num_neighbors / 2) {
            return voter_grid->voters[y][x];
        }
        return B_VOTE;
    } 
    if (vote_counter > num_neighbors / 2) {
        return A_VOTE;
    } 
    return B_VOTE;
}

void calculate_voters(Grid* voter_grid, Grid* transfer_buffer) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            char new_char = replace_with(voter_grid, i, j);
            transfer_buffer->voters[i][j] = new_char;
        }
    }
}

int main() {
    Grid* voter_grid = generate_grid();
    Grid* transfer_buffer = generate_grid();
    srand(time(NULL));
    populate_grid(voter_grid);
    for (int i = 0; i < 10; i++) {
        print_grid(voter_grid);
        calculate_voters(voter_grid, transfer_buffer);
        copy_grid(transfer_buffer, voter_grid);
        usleep(500000);
    }
    print_grid(voter_grid);
    free_grid(voter_grid);
    free_grid(transfer_buffer);
}
