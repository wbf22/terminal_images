#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>


#include "dependencies/TerminalImages.h"

/*

valgrind --leak-check=full --show-leak-kinds=all ./ti

leaks -atExit -- ./ti test/wizards.jpg


*/

void set_terminal_color_rgb(int fr, int fg, int fb, int br, int bg, int bb) {
    // Foreground (text) = 38;2;r;g;b
    // Background         = 48;2;r;g;b
    printf("\033[38;2;%d;%d;%dm", fr, fg, fb);
    printf("\033[48;2;%d;%d;%dm", br, bg, bb);
}

void ansii_reset() {
    printf("\033[0m");
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

    TImageCell** cells = convert_image_to_ansii_cells(path, terminal_width, terminal_height);
    for (int y = 0; y < terminal_height; ++y) {
        for (int x = 0; x < terminal_width; ++x) {

            int i = x + y * terminal_width;
            TImageCell* cell = cells[i];

            if (cell != NULL) {
                set_terminal_color_rgb(
                    cell->text_color.r,
                    cell->text_color.g,
                    cell->text_color.b,
                    cell->background_color.r,
                    cell->background_color.g,
                    cell->background_color.b
                );
                printf(cell->unicode);
            }
            else {
                ansii_reset();
                printf(" ");
            }
        
        }
        // new line or flush for last
        if (y != terminal_height) {
            ansii_reset();
            printf("\n");
        }
        else {
            ansii_reset();
            fflush(stdout);  
        }
    }
    free_image_cells(cells, terminal_width, terminal_height);
    


    if (info) {
        int image_width, image_height, channels;
        uint8_t *image = stbi_load(path, &image_width, &image_height, &channels, 0);
        free(image);

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
