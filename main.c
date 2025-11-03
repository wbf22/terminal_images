#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "dependencies/Map.h"
#include "dependencies/List.h"


#define STB_IMAGE_IMPLEMENTATION
#include "dependencies/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "dependencies/stb_image_write.h"


/*

valgrind --leak-check=full --show-leak-kinds=all ./ti

leaks -atExit -- ./ti test/wizards.jpg


*/

char* RESET = "\033[0m";
char* RED = "\033[252;3;3m";
char* YELLOW = "\033[252;186;3m";

int KMEANS_ITERATIONS = 3; // trades speed for color accuracy


Map* make_character_map() {

    // ▙▂▝╫┃╯╳▖▚▞ ╲▗▀▛▟█┳╭╱▒▘▜╋░

    Map* character_to_pixels = new_map();
    uint64_t* copy;
    uint64_t full[2] = {
        0b1111111111111111111111111111111111111111111111111111111111111111ULL,
        0b1111111111111111111111111111111111111111111111111111111111111111ULL,
    };
    copy = malloc(2 * sizeof(uint64_t));
    memcpy(copy, full, 2 * sizeof(uint64_t));
    m_put(character_to_pixels, "█", copy, sizeof(full));

    uint64_t empty[2] = {
        0b0000000000000000000000000000000000000000000000000000000000000000ULL,
        0b0000000000000000000000000000000000000000000000000000000000000000ULL,
    };
    copy = malloc(2 * sizeof(uint64_t));
    memcpy(copy, empty, 2 * sizeof(uint64_t));
    m_put(character_to_pixels, " ", copy, sizeof(empty));

    uint64_t dots[2] = {
        0b1000100000000000001000100000000010001000000000000010001000000000ULL,
        0b1000100000000000001000100000000010001000000000000010001000000000ULL,
    };
    copy = malloc(2 * sizeof(uint64_t));
    memcpy(copy, dots, 2 * sizeof(uint64_t));
    m_put(character_to_pixels, "░", copy, sizeof(dots));

    uint64_t l[2] = {
        0b1111000011110000111100001111000011110000111100001111000011110000ULL,
        0b1111111111111111111111111111111111111111111111111111111111111111ULL,
    };
    copy = malloc(2 * sizeof(uint64_t));
    memcpy(copy, l, 2 * sizeof(uint64_t));
    m_put(character_to_pixels, "▙", copy, sizeof(l));

    uint64_t slab[2] = {
        0b0000000000000000000000000000000000000000000000000000000000000000ULL,
        0b0000000000000000000000001111111111111111111111111111111111111111ULL,
    };
    copy = malloc(2 * sizeof(uint64_t));
    memcpy(copy, slab, 2 * sizeof(uint64_t));
    m_put(character_to_pixels, "▂", copy, sizeof(slab));

    uint64_t cross[2] = {
        0b0011110000111100001111000011110000111100001111001111111111111111ULL,
        0b1111111111111111001111000011110000111100001111000011110000111100ULL,
    };
    copy = malloc(2 * sizeof(uint64_t));
    memcpy(copy, cross, 2 * sizeof(uint64_t));
    m_put(character_to_pixels, "╋", copy, sizeof(cross));

    uint64_t pole[2] = {
        0b0011110000111100001111000011110000111100001111000011110000111100ULL,
        0b0011110000111100001111000011110000111100001111000011110000111100ULL,
    };
    copy = malloc(2 * sizeof(uint64_t));
    memcpy(copy, pole, 2 * sizeof(uint64_t));
    m_put(character_to_pixels, "┃", copy, sizeof(pole));

    uint64_t r[2] = {
        0b0000000000000000000000000000000000000000000000000000000000000000ULL,
        0b0000011100001100000110000001000000010000000100000001000000010000ULL,
    };
    copy = malloc(2 * sizeof(uint64_t));
    memcpy(copy, full, 2 * sizeof(uint64_t));
    m_put(character_to_pixels, "╭", copy, sizeof(r));

    uint64_t x_sym[2] = {
        0b1000000110000001110000110100001001100110001001000010010000011000ULL,
        0b0001100000100100001001000110011001000010110000111000000110000001ULL,
    };
    copy = malloc(2 * sizeof(uint64_t));
    memcpy(copy, x_sym, 2 * sizeof(uint64_t));
    m_put(character_to_pixels, "╳", copy, sizeof(x_sym));

    uint64_t j[2] = {
        0b0001000000010000000100000001000000010000001100000110000011000000ULL,
        0b0000000000000000000000000000000000000000000000000000000000000000ULL,
    };
    copy = malloc(2 * sizeof(uint64_t));
    memcpy(copy, j, 2 * sizeof(uint64_t));
    m_put(character_to_pixels, "╯", copy, sizeof(j));

    uint64_t crossed_poles[2] = {
        0b0010010000100100001001000010010000100100001001000010010011111111ULL,
        0b0010010000100100001001000010010000100100001001000010010000100100ULL,
    };
    copy = malloc(2 * sizeof(uint64_t));
    memcpy(copy, crossed_poles, 2 * sizeof(uint64_t));
    m_put(character_to_pixels, "╫", copy, sizeof(crossed_poles));

    uint64_t t[2] = {
        0b0000000000000000000000000000000000000000000000001111111111111111ULL,
        0b1111111100111100001111000011110000111100001111000011110000111100ULL,
    };
    copy = malloc(2 * sizeof(uint64_t));
    memcpy(copy, t, 2 * sizeof(uint64_t));
    m_put(character_to_pixels, "┳", copy, sizeof(t));

    uint64_t p[2] = {
        0b1111111111111111111111111111111111111111111111111111111111111111ULL,
        0b1111000011110000111100001111000011110000111100001111000011110000ULL,
    };
    copy = malloc(2 * sizeof(uint64_t));
    memcpy(copy, p, 2 * sizeof(uint64_t));
    m_put(character_to_pixels, "▛", copy, sizeof(p));

    uint64_t grid[2] = {
        0b1010101000000000010101010000000010101010000000000101010100000000ULL,
        0b1010101000000000010101010000000010101010000000000101010100000000ULL,
    };
    copy = malloc(2 * sizeof(uint64_t));
    memcpy(copy, grid, 2 * sizeof(uint64_t));
    m_put(character_to_pixels, "▒", copy, sizeof(grid));

    uint64_t right_corner[2] = {
        0b0000111100001111000011110000111100001111000011110000111100001111ULL,
        0b0000000000000000000000000000000000000000000000000000000000000000ULL,
    };
    copy = malloc(2 * sizeof(uint64_t));
    memcpy(copy, right_corner, 2 * sizeof(uint64_t));
    m_put(character_to_pixels, "▝", copy, sizeof(right_corner));

    uint64_t top_slab[2] = {
        0b1111111111111111111111111111111111111111111111111111111111111111ULL,
        0b0000000000000000000000000000000000000000000000000000000000000000ULL,
    };
    copy = malloc(2 * sizeof(uint64_t));
    memcpy(copy, top_slab, 2 * sizeof(uint64_t));
    m_put(character_to_pixels, "▀", copy, sizeof(top_slab));

    uint64_t corner_squares[2] = {
        0b1111000011110000111100001111000011110000111100001111000011110000ULL,
        0b0000111100001111000011110000111100001111000011110000111100001111ULL,
    };
    copy = malloc(2 * sizeof(uint64_t));
    memcpy(copy, corner_squares, 2 * sizeof(uint64_t));
    m_put(character_to_pixels, "▚", copy, sizeof(corner_squares));

    uint64_t corner_squares_2[2] = {
        0b0000111100001111000011110000111100001111000011110000111100001111ULL,
        0b1111000011110000111100001111000011110000111100001111000011110000ULL,
    };
    copy = malloc(2 * sizeof(uint64_t));
    memcpy(copy, corner_squares_2, 2 * sizeof(uint64_t));
    m_put(character_to_pixels, "▞", copy, sizeof(corner_squares_2));

    uint64_t forward_slash[2] = {
        0b0000000100000011000001100000010000001100000010000001100000010000ULL,
        0b0001000000110000001000000110000001100000110000001000000010000000ULL,
    };
    copy = malloc(2 * sizeof(uint64_t));
    memcpy(copy, forward_slash, 2 * sizeof(uint64_t));
    m_put(character_to_pixels, "╱", copy, sizeof(forward_slash));

    uint64_t back_slash[2] = {
        0b1000000010000000110000000100000001100000001000000010000000110000ULL,
        0b0001000000011000000010000000110000001100000001100000001000000011ULL,
    };
    copy = malloc(2 * sizeof(uint64_t));
    memcpy(copy, back_slash, 2 * sizeof(uint64_t));
    m_put(character_to_pixels, "╲", copy, sizeof(back_slash));

    uint64_t top_right_half[2] = {
        0b1111111111111111111111111111111111111111111111111111111111111111ULL,
        0b0000111100001111000011110000111100001111000011110000111100001111ULL,
    };
    copy = malloc(2 * sizeof(uint64_t));
    memcpy(copy, top_right_half, 2 * sizeof(uint64_t));
    m_put(character_to_pixels, "▜", copy, sizeof(top_right_half));

    uint64_t bottom_right_half[2] = {
        0b0000111100001111000011110000111100001111000011110000111100001111ULL,
        0b1111111111111111111111111111111111111111111111111111111111111111ULL,
    };
    copy = malloc(2 * sizeof(uint64_t));
    memcpy(copy, bottom_right_half, 2 * sizeof(uint64_t));
    m_put(character_to_pixels, "▟", copy, sizeof(bottom_right_half));

    uint64_t bottom_left[2] = {
        0b0000000000000000000000000000000000000000000000000000000000000000ULL,
        0b1111000011110000111100001111000011110000111100001111000011110000ULL,
    };
    copy = malloc(2 * sizeof(uint64_t));
    memcpy(copy, bottom_left, 2 * sizeof(uint64_t));
    m_put(character_to_pixels, "▖", copy, sizeof(bottom_left));

    uint64_t bottom_right[2] = {
        0b0000000000000000000000000000000000000000000000000000000000000000ULL,
        0b0000111100001111000011110000111100001111000011110000111100001111ULL,
    };
    copy = malloc(2 * sizeof(uint64_t));
    memcpy(copy, bottom_right, 2 * sizeof(uint64_t));
    m_put(character_to_pixels, "▗", copy, sizeof(bottom_right));

    uint64_t top_left[2] = {
        0b1111000011110000111100001111000011110000111100001111000011110000ULL,
        0b0000000000000000000000000000000000000000000000000000000000000000ULL,
    };
    copy = malloc(2 * sizeof(uint64_t));
    memcpy(copy, top_left, 2 * sizeof(uint64_t));
    m_put(character_to_pixels, "▘", copy, sizeof(top_left));



    // uint64_t square[2] = {
    //     0b0000000000000000000000000000000000000000111111111000000110000001ULL,
    //     0b1000000110000001100000011000000111111111000000000000000000000000ULL,
    // };
    // m_put(character_to_pixels, "□", square, sizeof(square));

    // uint64_t top_tick[2] = {
    //     0b0001100000011000000110000001100000011000000110000001100000011000ULL,
    //     0b0000000000000000000000000000000000000000000000000000000000000000ULL,
    // };
    // m_put(character_to_pixels, "╹", top_tick, sizeof(top_tick));

    // uint64_t right_stub[2] = {
    //     0b0000000000000000000000000000000000000000000000000000000000000000ULL,
    //     0b0000111100001111000000000000000000000000000000000000000000000000ULL,
    // };
    // m_put(character_to_pixels, "╺", right_stub, sizeof(right_stub));

    // uint64_t left_stub[2] = {
    //     0b0000000000000000000000000000000000000000000000000000000000000000ULL,
    //     0b1111000000000000000000000000000000000000000000000000000000000000ULL,
    // };
    // m_put(character_to_pixels, "╴", left_stub, sizeof(left_stub));

    // uint64_t note[2] = {
    //     0b0000000000000000000000000001000000011110000100100001001000010010ULL,
    //     0b0001001000010010011100100110011000000110000000000000000000000000ULL
    // };
    // m_put(character_to_pixels, "♫", note, sizeof(note));

    // uint64_t hash[2] = {
    //     0b0000000000000000000000000000100000101100001111000111010000100100ULL,
    //     0b0011110001110100001001000010000000000000000000000000000000000000ULL,
    // };
    // m_put(character_to_pixels, "♯", hash, sizeof(hash));

    // uint64_t recycle[2] = {
    //     0b0000000000000000000000000000000000000000000110000011110000111100ULL,
    //     0b0101101010011011100110110111111100000000000000000000000000000000ULL,
    // };
    // m_put(character_to_pixels, "♳", recycle, sizeof(recycle));

    // uint64_t plus[2] = {
    //     0b0000000000000000000000000000000000000000111111111110011111100111ULL,
    //     0b1000000111100111111001111111111100000000000000000000000000000000ULL,
    // };
    // m_put(character_to_pixels, "⚃", plus, sizeof(plus));
    
    // uint64_t pick[2] = {
    //     0b00000000000000000000000000001110001110000011100001111000100110ULL,
    //     0b110001100000011000000010000000000000000000000000000000000000000ULL,
    // };
    // m_put(character_to_pixels, "⛏", pick, sizeof(pick));

    // uint64_t hook[2] = {
    //     0b0000000000000000000000000111100010000100000001000000010000001100ULL,
    //     0b0001100000110000001100000111100000110000001100000000000000000000ULL,
    // };
    // m_put(character_to_pixels, "⚳", hook, sizeof(hook));

    // uint64_t spike_ball[2] = {
    //     0b0000000000000000000000000000100000001000000010000000100000010100ULL,
    //     0b0111011100101010000101000000100000000000000000000000000000000000ULL,
    // };
    // m_put(character_to_pixels, "⚶", spike_ball, sizeof(spike_ball));

    // uint64_t star[2] = {
    //     0b0000000000000000000000000000000000000100000011100111111111111111ULL,
    //     0b0001111100011111000110110011000100000000000000000000000000000000ULL,
    // };
    // m_put(character_to_pixels, "⭑", star, sizeof(star));

    // uint64_t moon[2] = {
    //     0b0000000000000000000000000011000001100000011000001110000011100000ULL,
    //     0b1110000001110000001111110001111100000000000000000000000000000000ULL,
    // };
    // m_put(character_to_pixels, "⏾", moon, sizeof(moon));


    // uint64_t circle[2] = {
    //     0b0000000000000000000000000000000000111000011111001111111011111111ULL,
    //     0b1111111001111100001110000000000000000000000000000000000000000000ULL,
    // };
    // m_put(character_to_pixels, "●", circle, sizeof(circle));

    // uint64_t little_circle[2] = {
    //     0b0000000000000000000000000000000000010000001110000111110011111110ULL,
    //     0b0111110000111000000100000000000000000000000000000000000000000000ULL,
    // };
    // m_put(character_to_pixels, "•", little_circle, sizeof(little_circle));

    // uint64_t snowman[2] = {
    //     0b0000000000000000000000000100010000010000011011001110100010101010ULL,
    //     0b0100010010000010010001000011100000000000000000000000000000000000ULL,
    // };
    // m_put(character_to_pixels, "☃", snowman, sizeof(snowman));

    // uint64_t heart[2] = {
    //     0b0000000000000000000000000000000001101110111111111111111101111110ULL,
    //     0b0111110000111000000100000000000000000000000000000000000000000000ULL,
    // };
    // m_put(character_to_pixels, "♥", heart, sizeof(heart));

    // uint64_t middle_stub[2] = {
    //     0b0000000000000000000000000000000000000000000000000000000000110000ULL,
    //     0b0000000000000000000000000000000000000000000000000000000000000000ULL,
    // };
    // m_put(character_to_pixels, "⠂", middle_stub, sizeof(middle_stub));

    // uint64_t dots_3[2] = {
    //     0b0000000000000000000000000000000000000000000000000000000000000000ULL,
    //     0b0000000000000000010101101101011000000000000000000000000000000000ULL,
    // };
    // m_put(character_to_pixels, "…", dots_3, sizeof(dots_3));

    // uint64_t dots_4[2] = {
    //     0b0000000000000000000000000000000000000000000000000000000000000000ULL,
    //     0b0000000000100100001101100000000000100100001101100000000000000000ULL,
    // };
    // m_put(character_to_pixels, "⣤", dots_4, sizeof(dots_4));

    // uint64_t semicolon[2] = {
    //     0b0000000000000000000000000000000000000000000110000011100000000000ULL,
    //     0b0000000000000000000110000011100000000000000000000000000000000000ULL,
    // };
    // m_put(character_to_pixels, ":", semicolon, sizeof(semicolon));



    return character_to_pixels;
}

void print_character_map(Map* character_map) {

    Element** charcters = map_elements(character_map);
    for (int i = 0; i < character_map->len; ++i) {
        Element* element = charcters[i];
        printf(element->key);
    }
    free(charcters);
    printf("\n");
}

int count_shared_bits(uint64_t* a, uint64_t* b) {
    uint64_t x_0 = ~(a[0] ^ b[0]);
    uint64_t x_1 = ~(a[1] ^ b[1]);

    int count = 0;
    while (x_0) {
        x_0 &= (x_0 - 1); // clears lowest set bit
        count++;
    }
    while (x_1) {
        x_1 &= (x_1 - 1);
        count++;
    }
    return count;
}

int x_y_to_index(int x, int y, int image_width, int channels) {
    return (x + y * image_width) * channels;
}

int clamp(int x, int min, int max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}


void lerp(
    uint8_t* res_r, 
    uint8_t* res_g, 
    uint8_t* res_b, 
    uint8_t* res_a, 

    uint8_t r_1, 
    uint8_t g_1, 
    uint8_t b_1, 
    uint8_t a_1, 
    
    uint8_t r_2, 
    uint8_t g_2, 
    uint8_t b_2, 
    uint8_t a_2, 
    
    float t
) {
    *res_r = (uint8_t)(r_1 + (r_2 - r_1) * t);
    *res_g = (uint8_t)(g_1 + (g_2 - g_1) * t);
    *res_b = (uint8_t)(b_1 + (b_2 - b_1) * t);
    *res_a = (uint8_t)(a_1 + (a_2 - a_1) * t);
}

int dist(
    uint8_t r_1,
    uint8_t g_1,
    uint8_t b_1,
    uint8_t a_1,
    uint8_t r_2,
    uint8_t g_2,
    uint8_t b_2,
    uint8_t a_2
) {
    return abs(r_1 - r_2) + abs(g_1 - g_2) + abs(b_1 - b_2) + abs(a_1 - a_2);
}


void set_bit(uint64_t* value, unsigned int n) {
    if (n >= 64) return;  // prevent shifting by 64 or more (undefined behavior)
    *value = *value | (1ULL << n);
}


int get_image_index(
    int cell_x, 
    int cell_y, 
    int inside_cell_pixel_x, 
    int inside_cell_pixel_y,
    int image_width_cells,
    int cell_width_pixels,
    int cell_height_pixels
) {
    
    int i = cell_y * image_width_cells * cell_height_pixels * cell_width_pixels + cell_x * cell_width_pixels; // top corner of cell in pixels
    // adjust for y level inside cell
    if (inside_cell_pixel_y != 0) {
        i += (image_width_cells - cell_x) * cell_width_pixels; 
        if (inside_cell_pixel_y > 1) {
            int mult = inside_cell_pixel_y - 2;
            i += mult * image_width_cells * cell_width_pixels;
        }
        i += cell_x * cell_width_pixels;
    }
    i += inside_cell_pixel_x; // adjust of x level inside cell
    i *= 4;

    return i;
}



void set_terminal_color_rgb(int fr, int fg, int fb, int br, int bg, int bb) {
    // Foreground (text) = 38;2;r;g;b
    // Background         = 48;2;r;g;b
    printf("\033[38;2;%d;%d;%dm", fr, fg, fb);
    printf("\033[48;2;%d;%d;%dm", br, bg, bb);
}

void ansii_reset() {
    printf("\033[0m");
}


void kmeans_for_colors(
    uint8_t* new_image,
    int c_x,
    int c_y,
    int image_width_cells,
    int CURSOR_WIDTH,
    int CURSOR_HEIGHT,

    int* avg_1_r,
    int* avg_1_g,
    int* avg_1_b,
    int* avg_1_a,
    int* avg_2_r,
    int* avg_2_g,
    int* avg_2_b,
    int* avg_2_a

) {

    int avg_1_in = get_image_index(c_x, c_y, 0, 0, image_width_cells, CURSOR_WIDTH, CURSOR_HEIGHT);
    *avg_1_r = new_image[avg_1_in];
    *avg_1_g = new_image[avg_1_in+1];
    *avg_1_b = new_image[avg_1_in+2];
    *avg_1_a = new_image[avg_1_in+3];
    int avg_2_in = get_image_index(c_x, c_y, CURSOR_WIDTH-1, CURSOR_HEIGHT-1, image_width_cells, CURSOR_WIDTH, CURSOR_HEIGHT);
    *avg_2_r = new_image[avg_2_in];
    *avg_2_g = new_image[avg_2_in+1];
    *avg_2_b = new_image[avg_2_in+2];
    *avg_2_a = new_image[avg_2_in+3];
    List* avg_1 = new_list();
    List* avg_2 = new_list();
    for (int k = 0; k < KMEANS_ITERATIONS; k++) {
        
        // sort into groups
        for (int x = 0; x < CURSOR_WIDTH; x++) {
            for (int y = 0; y < CURSOR_HEIGHT; y++) {
                
                int i = get_image_index(c_x, c_y, x, y, image_width_cells, CURSOR_WIDTH, CURSOR_HEIGHT);
                uint8_t r = new_image[i];
                uint8_t g = new_image[i+1];
                uint8_t b = new_image[i+2];
                uint8_t a = new_image[i+3];
                int dist_1 = dist(*avg_1_r, *avg_1_g, *avg_1_g, *avg_1_a, r, g, b, a);
                int dist_2 = dist(*avg_2_r, *avg_2_g, *avg_2_g, *avg_2_a, r, g, b, a);
                int* coor = malloc(sizeof(int) * 2);
                coor[0] = x;
                coor[1] = y;
                if (dist_1 < dist_2) {
                    l_push(avg_1, coor);
                }
                else {
                    l_push(avg_2, coor);
                }
            }
        }

        // avoid empty groups
        if (avg_1->len == 0) {
            int* coor = l_pop(avg_2);
            l_push(avg_1, coor);
        }
        if (avg_2->len == 0) {
            int* coor = l_pop(avg_1);
            l_push(avg_2, coor);
        }

        // determine new averages
        *avg_1_r = 0;
        *avg_1_g = 0;
        *avg_1_b = 0;
        *avg_1_a = 0;
        *avg_2_r = 0;
        *avg_2_g = 0;
        *avg_2_b = 0;
        *avg_2_a = 0;
        for (int i = 0; i < avg_1->len; ++i) {
            int* coor = l_get(avg_1, i);
            int index = get_image_index(c_x, c_y, coor[0], coor[1], image_width_cells, CURSOR_WIDTH, CURSOR_HEIGHT);
            *avg_1_r += new_image[index];
            *avg_1_g += new_image[index+1];
            *avg_1_b += new_image[index+2];
            *avg_1_a += new_image[index+3];
        }
        *avg_1_r /= avg_1->len;
        *avg_1_g /= avg_1->len;
        *avg_1_b /= avg_1->len;
        *avg_1_a /= avg_1->len;
        l_clear(avg_1, 1);
        for (int i = 0; i < avg_2->len; ++i) {
            int* coor = l_get(avg_2, i);
            int index = get_image_index(c_x, c_y, coor[0], coor[1], image_width_cells, CURSOR_WIDTH, CURSOR_HEIGHT);
            *avg_2_r += new_image[index];
            *avg_2_g += new_image[index+1];
            *avg_2_b += new_image[index+2];
            *avg_2_a += new_image[index+3];
        }
        *avg_2_r /= avg_2->len;
        *avg_2_g /= avg_2->len;
        *avg_2_b /= avg_2->len;
        *avg_2_a /= avg_2->len;
        l_clear(avg_2, 1);

    }
    free_list(avg_1, 1);
    free_list(avg_2, 1);

}

int main(int argc, char **argv) {
    if (argc < 2 || argv[1] == NULL) {
        printf("%sPlease provide a single path to and image file you'd like to display%s", RED, RESET);
        exit(-1);
    }


    // get file path
    char* path;

    int info = 0;
    if (argc > 2) {
        char* flag = argv[1];
        info = strcmp(flag, "-i") == 0;
        path = argv[2];
    }
    else {
        path = argv[1];
    }


    // load map
    Map* character_to_pixels = make_character_map();
    // print_character_map(character_to_pixels);

    // get window size
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        perror("ioctl");
        return 1;
    }
    int terminal_width = w.ws_col - 2;
    int terminal_height = w.ws_row - 2;

    if (info) {
        terminal_height -= 4;
    }


    // LOAD image
    int image_width, image_height, channels;
    uint8_t *image = stbi_load(path, &image_width, &image_height, &channels, 0);
    if (!image) {
        printf("Failed to load image: %s\n", stbi_failure_reason());
        exit(-1);
    }

    int CURSOR_WIDTH = 8;
    int CURSOR_HEIGHT = 16;
    double image_ratio = (double)image_width / (double)image_height;
    double terminal_ratio = (double)(terminal_width * CURSOR_WIDTH) / (double)(terminal_height * CURSOR_HEIGHT);

    // DETERMINE pixels per cell
    double pixels_per_cell_pixel;
    if (image_ratio > terminal_ratio) {
        // if image is wider than terminal portionally, use the width to determine the pixel ratio
        pixels_per_cell_pixel = (double)image_width / (terminal_width * CURSOR_WIDTH);
    }
    else {
        // if the image is taller than terminal portionally, use the height to determine the pixel ratio
        pixels_per_cell_pixel = (double)image_height / (terminal_height * CURSOR_HEIGHT);
    }


    // SCALE image with bilinear interpolation
    int new_width = image_width / pixels_per_cell_pixel;
    int new_height = image_height / pixels_per_cell_pixel;

    // ensure multiples of 8
    while(new_width % 8 != 0) {
        --new_width;
    }
    while(new_height % 8 != 0) {
        --new_height;
    }


    int new_image_length = new_height * new_width * 4;
    uint8_t* new_image = malloc(sizeof(uint8_t) * new_image_length);
    for (int x = 0; x < new_width; x++) {
        for (int y = 0; y < new_height; y++) {
            // find coordinates in the source image
            float gx = (x + 0.5f) * image_width / new_width  - 0.5f;
            float gy = (y + 0.5f) * image_height / new_height - 0.5f;

            int x0 = floor(gx);
            int x1 = x0 + 1;
            int y0 = floor(gy);
            int y1 = y0 + 1;

            float dx = gx - x0;
            float dy = gy - y0;

            // clamp coordinates to image boundaries
            x0 = clamp(x0, 0, image_width-1);
            x1 = clamp(x1, 0, image_width-1);
            y0 = clamp(y0, 0, image_height-1);
            y1 = clamp(y1, 0, image_height-1);

            // sample the four neighboring pixels
            uint8_t top_left_r = image[y0 * image_width * channels + x0 * channels];
            uint8_t top_left_g = image[y0 * image_width * channels + x0 * channels + 1];
            uint8_t top_left_b = image[y0 * image_width * channels + x0 * channels + 2];
            uint8_t top_left_a = 255;
            if (channels > 3) top_left_a = image[y0 * image_width * channels + x0 * channels + 3];

            uint8_t top_right_r = image[y0 * image_width * channels + x1 * channels];
            uint8_t top_right_g = image[y0 * image_width * channels + x1 * channels + 1];
            uint8_t top_right_b = image[y0 * image_width * channels + x1 * channels + 2];
            uint8_t top_right_a = 255;
            if (channels > 3) top_right_a = image[y0 * image_width * channels + x1 * channels + 3];

            uint8_t bottom_left_r = image[y1 * image_width * channels + x0 * channels];
            uint8_t bottom_left_g = image[y1 * image_width * channels + x0 * channels + 1];
            uint8_t bottom_left_b = image[y1 * image_width * channels + x0 * channels + 2];
            uint8_t bottom_left_a = 255;
            if (channels > 3) bottom_left_a = image[y1 * image_width * channels + x0 * channels + 3];

            uint8_t bottom_right_r = image[y1 * image_width * channels + x1 * channels];
            uint8_t bottom_right_g = image[y1 * image_width * channels + x1 * channels + 1];
            uint8_t bottom_right_b = image[y1 * image_width * channels + x1 * channels + 2];
            uint8_t bottom_right_a = 255;
            if (channels > 3) bottom_right_a = image[y1 * image_width * channels + x1 * channels + 3];

            // interpolate horizontally then vertically
            uint8_t top_r;
            uint8_t top_g;
            uint8_t top_b;
            uint8_t top_a = 255;
            lerp(
                &top_r,
                &top_g,
                &top_b,
                &top_a,
                top_left_r,
                top_left_g,
                top_left_b,
                top_left_a,
                top_right_r,
                top_right_g,
                top_right_b,
                top_right_a,
                dx
            );
            uint8_t bottom_r;
            uint8_t bottom_g;
            uint8_t bottom_b;
            uint8_t bottom_a = 255;
            lerp(
                &bottom_r,
                &bottom_g,
                &bottom_b,
                &bottom_a,
                bottom_left_r,
                bottom_left_g,
                bottom_left_b,
                bottom_left_a,
                bottom_right_r,
                bottom_right_g,
                bottom_right_b,
                bottom_right_a,
                dx
            );
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a = 255;
            lerp(
                &r,
                &g,
                &b,
                &a,
                top_r,
                top_g,
                top_b,
                top_a,
                bottom_r,
                bottom_g,
                bottom_b,
                bottom_a,
                dy
            );
            int index = (y * new_width + x) * 4;
            new_image[index] = r;
            new_image[index+1] = g;
            new_image[index+2] = b;
            new_image[index+3] = a;
        }
    }
    // stbi_write_png("test/resized.png", new_width, new_height, 4, new_image, new_width * 4);


    // DETERMINE characters and colors for each cell
    int image_width_cells = ceil((double)new_width / (double)CURSOR_WIDTH);
    int image_height_cells = ceil((double)new_height / (double)CURSOR_HEIGHT);
    for (int c_y = 0; c_y < image_height_cells; c_y++) {
        for (int c_x = 0; c_x < image_width_cells; c_x++) {

            // kmeans determine color pair for cells
            int avg_1_r, avg_1_g, avg_1_b, avg_1_a;
            int avg_2_r, avg_2_g, avg_2_b, avg_2_a;
            kmeans_for_colors(
                new_image,
                c_x,
                c_y,
                image_width_cells,
                CURSOR_WIDTH,
                CURSOR_HEIGHT,
                &avg_1_r, 
                &avg_1_g, 
                &avg_1_b, 
                &avg_1_a,
                &avg_2_r, 
                &avg_2_g, 
                &avg_2_b, 
                &avg_2_a
            );

            // determine character that matches the pixels the best
            uint64_t first_variation[2] = {0, 0}; // avg_1 is set or is text
            uint64_t second_variation[2] = {0, 0}; // avg_2 is set or is text
            for (int x = 0; x < CURSOR_WIDTH; x++) {
                for (int y = 0; y < CURSOR_HEIGHT; y++) {

                    int i = get_image_index(c_x, c_y, x, y, image_width_cells, CURSOR_WIDTH, CURSOR_HEIGHT);

                    // index = (x + y*width) * 4
                    int pixel_x = (i / 4) % new_width;
                    int pixel_y = ((i / 4) - x) / new_width;
                    int cursor_x = pixel_x / CURSOR_WIDTH;
                    int cursor_y = pixel_y / CURSOR_HEIGHT;
                    uint8_t r = new_image[i];
                    uint8_t g = new_image[i+1];
                    uint8_t b = new_image[i+2];
                    uint8_t a = new_image[i+3];
                    int dist_1 = dist(avg_1_r, avg_1_g, avg_1_b, avg_1_a, r, g, b, a);
                    int dist_2 = dist(avg_2_r, avg_2_g, avg_2_b, avg_2_a, r, g, b, a);

                    int index = x + y * CURSOR_WIDTH;
                    int is_second_uint = 0;
                    if (index > 64) {
                        is_second_uint = 1;
                        index %= 64;
                    }
                    
                    if (dist_1 < dist_2) {
                        set_bit(
                            is_second_uint? &first_variation[1] : &first_variation[0],
                            63-index
                        );
                    }
                    else {
                        set_bit(
                            is_second_uint? &second_variation[1] : &second_variation[0],
                            63-index
                        );
                    }
                }
            }


            int is_first_best = 1;
            int best_variation_closeness = 0;
            char* element_key = "";

            Element** elements = map_elements(character_to_pixels);
            for (int i = 0; i < character_to_pixels->len; ++i) {
                Element* element = elements[i];
                uint64_t* bits = element->data;
                
                int closeness = count_shared_bits(bits, first_variation);
                if (closeness > best_variation_closeness) {
                    is_first_best = 1;
                    best_variation_closeness = closeness;
                    element_key = element->key;
                }

                closeness = count_shared_bits(bits, second_variation);
                if (closeness > best_variation_closeness) {
                    is_first_best = 0;
                    best_variation_closeness = closeness;
                    element_key = element->key;
                }
            }
            free(elements);
            



            // print
            if (is_first_best) {
                set_terminal_color_rgb(avg_1_r, avg_1_g, avg_1_b, avg_2_r, avg_2_g, avg_2_b);
            }
            else {
                set_terminal_color_rgb(avg_2_r, avg_2_g, avg_2_b, avg_1_r, avg_1_g, avg_1_b);
            }
            printf(element_key);
        }

        // new line or flush for last
        if (c_y != image_height_cells) {
            ansii_reset();
            printf("\n");
        }
        else {
            ansii_reset();
            fflush(stdout);  
        }
    }


    free(image);
    free(new_image);
    free_map(character_to_pixels, 1);


    if (info) {

        printf("image_width %d\n", image_width);
        printf("image_height %d\n", image_height);
        printf("channels %d\n", channels);

        FILE *f = fopen(path, "rb");
        fseek(f, 0, SEEK_END);
        long size_bytes = ftell(f);
        fclose(f);
        double size_mb = (double)size_bytes / (1000.0 * 1000.0);
        printf("size %.2f MB\n", size_mb);

    }
    
    return 0;
}
