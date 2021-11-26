#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define WINDOW_WIDTH (double)200
#define WINDOW_HEIGHT (double)10

#define XMIN (double)-10
#define XMAX (double)10
#define YMIN (double)-10
#define YMAX (double)10

#define PLANE_WIDTH (double)fabs(XMAX - XMIN)
#define PLANE_HEIGHT (double)fabs(YMAX - YMIN)

typedef struct vec2 {double x, y;} vec2;
typedef struct Pixel{ vec2 abs_position; char display; } Pixel;

// FUNCTION TO GRAPH HERE //
// fabs(sin(pow(x, x)) / pow(2, (pow(x, x)-(3.1415/2))/3.1415)) //
double function(double x) { return pow(x, 2); }

bool close_to(double x, double y, double deviation) { return fabs(x-y) < deviation; }

void quantify_plane(Pixel **display) {
    double x_steps = (PLANE_WIDTH/WINDOW_WIDTH);
    double y_steps = (PLANE_HEIGHT/WINDOW_HEIGHT);
    for(int y = 0; y < WINDOW_HEIGHT; y++) {
        for(int x = 0; x < WINDOW_WIDTH; x++) {
            *&display[y][x].abs_position.x = (XMIN + (x_steps * x));
            *&display[y][x].abs_position.y = (YMAX - (y_steps * y));
        }
    }
}

Pixel **draw(Pixel **display) {
    double x_steps = (PLANE_WIDTH/WINDOW_WIDTH);
    double y_steps = (PLANE_HEIGHT/WINDOW_HEIGHT);
    double abs_x, abs_y;
    for(int y = 0; y < WINDOW_HEIGHT; y++) {
        for(int x = 0; x < WINDOW_WIDTH; x++) {
            Pixel *pixel = &display[y][x];
            abs_x = pixel -> abs_position.x;
            abs_y = pixel -> abs_position.y;
            if(close_to(abs_x, 0, x_steps/2.0)) {
                if(close_to(abs_y, 0, y_steps/2.0))
                    pixel -> display = '+';
                else
                    pixel -> display = '|';
            }
            else if(close_to(abs_y, 0, y_steps/2))
                pixel -> display = '-';
            else
                pixel -> display = ' ';

            if(close_to(function(abs_x), abs_y, y_steps/2.0))
                pixel -> display = '#';
        }
    }
    return display;
}

char snap_value(double x, double y, double range) {
    double test = fabs(y);
    char *table = "_,.-^*'`";
    int counter = 0;
    for(double i = 0; i < range; i += range/8) {
        if(i < test && (i + (range/8)) >= test)
            return table[counter];
        counter++;
    }
    return '#';
}

void print_plane(Pixel **display) {
    char **output = malloc(sizeof(char*) * WINDOW_HEIGHT + 1);
    for(int i = 0; i < WINDOW_HEIGHT; i++)
        output[i] = malloc(sizeof(char) * WINDOW_WIDTH + 1);

    for(int y = 0; y < WINDOW_HEIGHT; y++) {
        for(int x = 0; x < WINDOW_WIDTH; x++)
            output[y][x] = *&display[y][x].display;

        output[y][(int)WINDOW_WIDTH] = '\0';
    }

    for(int y = 0; y < WINDOW_HEIGHT; y++) {
        puts(output[y]);
    }

    for(int i = 0; i <= WINDOW_HEIGHT; i++)
        free(output[i]);
    free(output);
}

void clear_display(Pixel **display) {
    for(int i = 0; i < WINDOW_HEIGHT; i++)
        free(display[i]);
    free(display);
}

int main(void) {
    Pixel **display = malloc(sizeof(Pixel *) * WINDOW_HEIGHT);
    for(int i = 0; i < WINDOW_HEIGHT; i++)
        display[i] = malloc(sizeof(Pixel) * WINDOW_WIDTH);

    quantify_plane(display);
    draw(display);
    print_plane(display);
    clear_display(display);
}