#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "dependencies/Map.h"
#define STB_IMAGE_IMPLEMENTATION
#include "dependencies/stb_image.h"



char* RESET = "\033[0m";
char* RED = "\033[252;3;3m";
char* YELLOW = "\033[252;186;3m";

Map* make_character_map() {

    Map* character_to_pixels = new_map();
    uint64_t note[2] = {
        0b0000000000000000000000000001000000011110000100100001001000010010ULL,
        0b0001001000010010011100100110011000000110000000000000000000000000ULL
    };
    insert(character_to_pixels, "♫", note, sizeof(note));

    uint64_t hash[2] = {
        0b0000000000000000000000000000100000101100001111000111010000100100ULL,
        0b0011110001110100001001000010000000000000000000000000000000000000ULL,
    };
    insert(character_to_pixels, "♯", hash, sizeof(hash));

    uint64_t recycle[2] = {
        0b0000000000000000000000000000000000000000000110000011110000111100ULL,
        0b0101101010011011100110110111111100000000000000000000000000000000ULL,
    };
    insert(character_to_pixels, "♳", recycle, sizeof(recycle));

    uint64_t plus[2] = {
        0b0000000000000000000000000000000000000000111111111110011111100111ULL,
        0b1000000111100111111001111111111100000000000000000000000000000000ULL,
    };
    insert(character_to_pixels, "⚃", plus, sizeof(plus));
    
    uint64_t pick[2] = {
        0b00000000000000000000000000001110001110000011100001111000100110ULL,
        0b110001100000011000000010000000000000000000000000000000000000000ULL,
    };
    insert(character_to_pixels, "⛏", pick, sizeof(pick));

    uint64_t hook[2] = {
        0b0000000000000000000000000111100010000100000001000000010000001100ULL,
        0b0001100000110000001100000111100000110000001100000000000000000000ULL,
    };
    insert(character_to_pixels, "⚳", hook, sizeof(hook));

    uint64_t spike_ball[2] = {
        0b0000000000000000000000000000100000001000000010000000100000010100ULL,
        0b0111011100101010000101000000100000000000000000000000000000000000ULL,
    };
    insert(character_to_pixels, "⚶", spike_ball, sizeof(spike_ball));

    uint64_t star[2] = {
        0b0000000000000000000000000000000000000100000011100111111111111111ULL,
        0b0001111100011111000110110011000100000000000000000000000000000000ULL,
    };
    insert(character_to_pixels, "⭑", star, sizeof(star));

    uint64_t moon[2] = {
        0b0000000000000000000000000011000001100000011000001110000011100000ULL,
        0b1110000001110000001111110001111100000000000000000000000000000000ULL,
    };
    insert(character_to_pixels, "⏾", moon, sizeof(moon));

    uint64_t full[2] = {
        0b1111111111111111111111111111111111111111111111111111111111111111ULL,
        0b1111111111111111111111111111111111111111111111111111111111111111ULL,
    };
    insert(character_to_pixels, "█", full, sizeof(full));

    uint64_t empty[2] = {
        0b0000000000000000000000000000000000000000000000000000000000000000ULL,
        0b0000000000000000000000000000000000000000000000000000000000000000ULL,
    };
    insert(character_to_pixels, " ", empty, sizeof(empty));

    uint64_t dots[2] = {
        0b1000100000000000001000100000000010001000000000000010001000000000ULL,
        0b1000100000000000001000100000000010001000000000000010001000000000ULL,
    };
    insert(character_to_pixels, "░", dots, sizeof(dots));

    uint64_t l[2] = {
        0b1111000011110000111100001111000011110000111100001111000011110000ULL,
        0b1111111111111111111111111111111111111111111111111111111111111111ULL,
    };
    insert(character_to_pixels, "▙", l, sizeof(l));

    uint64_t slab[2] = {
        0b0000000000000000000000000000000000000000000000000000000000000000ULL,
        0b0000000000000000000000001111111111111111111111111111111111111111ULL,
    };
    insert(character_to_pixels, "▂", slab, sizeof(slab));

    uint64_t cross[2] = {
        0b0011110000111100001111000011110000111100001111001111111111111111ULL,
        0b1111111111111111001111000011110000111100001111000011110000111100ULL,
    };
    insert(character_to_pixels, "╋", cross, sizeof(cross));

    uint64_t pole[2] = {
        0b0011110000111100001111000011110000111100001111000011110000111100ULL,
        0b0011110000111100001111000011110000111100001111000011110000111100ULL,
    };
    insert(character_to_pixels, "┃", pole, sizeof(pole));

    uint64_t r[2] = {
        0b0000000000000000000000000000000000000000000000000000000000000000ULL,
        0b0000011100001100000110000001000000010000000100000001000000010000ULL,
    };
    insert(character_to_pixels, "╭", r, sizeof(r));

    uint64_t x_sym[2] = {
        0b1000000110000001110000110100001001100110001001000010010000011000ULL,
        0b0001100000100100001001000110011001000010110000111000000110000001ULL,
    };
    insert(character_to_pixels, "╳", x_sym, sizeof(x_sym));

    uint64_t j[2] = {
        0b0001000000010000000100000001000000010000001100000110000011000000ULL,
        0b0000000000000000000000000000000000000000000000000000000000000000ULL,
    };
    insert(character_to_pixels, "╯", j, sizeof(j));

    uint64_t crossed_poles[2] = {
        0b0010010000100100001001000010010000100100001001000010010011111111ULL,
        0b0010010000100100001001000010010000100100001001000010010000100100ULL,
    };
    insert(character_to_pixels, "╫", crossed_poles, sizeof(crossed_poles));

    uint64_t t[2] = {
        0b0000000000000000000000000000000000000000000000001111111111111111ULL,
        0b1111111100111100001111000011110000111100001111000011110000111100ULL,
    };
    insert(character_to_pixels, "┳", t, sizeof(t));

    uint64_t p[2] = {
        0b1111111111111111111111111111111111111111111111111111111111111111ULL,
        0b1111000011110000111100001111000011110000111100001111000011110000ULL,
    };
    insert(character_to_pixels, "▛", p, sizeof(p));

    uint64_t grid[2] = {
        0b1010101000000000010101010000000010101010000000000101010100000000ULL,
        0b1010101000000000010101010000000010101010000000000101010100000000ULL,
    };
    insert(character_to_pixels, "▒", grid, sizeof(grid));

    uint64_t right_corner[2] = {
        0b0000111100001111000011110000111100001111000011110000111100001111ULL,
        0b0000000000000000000000000000000000000000000000000000000000000000ULL,
    };
    insert(character_to_pixels, "▝", right_corner, sizeof(right_corner));

    uint64_t top_slab[2] = {
        0b1111111111111111111111111111111111111111111111111111111111111111ULL,
        0b0000000000000000000000000000000000000000000000000000000000000000ULL,
    };
    insert(character_to_pixels, "▀", top_slab, sizeof(top_slab));

    uint64_t square[2] = {
        0b0000000000000000000000000000000000000000111111111000000110000001ULL,
        0b1000000110000001100000011000000111111111000000000000000000000000ULL,
    };
    insert(character_to_pixels, "□", square, sizeof(square));

    return character_to_pixels;
}

int count_shared_bits(uint64_t* a, uint64_t* b) {

    uint64_t x_0 = a[0] & b[0];
    uint64_t x_1 = a[1] & b[1];

    int count = 0;
    while (x_0) {
        x_0 &= (x_0 - 1); // clears the lowest set bit
        count++;
    }
    while (x_1) {
        x_1 &= (x_1 - 1); // clears the lowest set bit
        count++;
    }
    return count;
}

int x_y_to_index(int x, int y, int image_width, int channels) {
    return (x + y * image_width) * channels;
}


int main(int argc, char **argv) {

    if (argc != 2 || argv[1] == NULL) {
        printf("%sPlease provide a single path to and image file you'd like to display%s", RED, RESET);
        exit(-1);
    }


    // get file path
    char* path = argv[1];


    // load map
    Map* character_to_pixels = make_character_map();


    // get window size
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        perror("ioctl");
        return 1;
    }
    int terminal_width = w.ws_row;
    int terminal_height = w.ws_col;


    // load image
    int image_width, image_height, channels;
    uint8_t *image = stbi_load(path, &image_width, &image_height, &channels, 0);

    int CURSOR_WIDTH = 8;
    int CURSOR_HEIGHT = 16;
    int pixels_per_cell_width = (image_width / terminal_width);
    int pixels_per_cell_height = (image_height / terminal_height);

    if (pixels_per_cell_width < CURSOR_WIDTH) {
        pixels_per_cell_width = CURSOR_WIDTH;
    }
    if (pixels_per_cell_height < CURSOR_HEIGHT) {

    }

    for (int c_y = 0; c_y < terminal_height; c_y++) {
        for (int c_x = 0; c_x < terminal_width; c_x++) {

            // kmeans determine color pair for cells


            // convert cells to color pair values
            
        }
    }


    free(image);
    
    return 0;
}
