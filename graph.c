#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// window boundaries/resolution.
#define WINDOW_WIDTH (float) 200
#define WINDOW_HEIGHT (float) 100

// plane boundaries.
#define XMIN (float) -10
#define XMAX (float) 10
#define YMIN (float) -10
#define YMAX (float) 10

#define PLANE_WIDTH fabs(XMAX - XMIN)
#define PLANE_HEIGHT fabs(YMAX - YMIN)

static float x_steps = (PLANE_WIDTH/WINDOW_WIDTH);
static float y_steps = (PLANE_HEIGHT/WINDOW_HEIGHT);

typedef struct Pixel{ float x, y; char display; } Pixel;

float function(float x) { return pow(x, 2); }

// calculate the derivative of the function given abofe at any given input.
static float delta = 0.0001;
float derive(float x) {
    return (function(x+delta)-function(x-delta)) / ((x+delta)-(x-delta));
}

// return whether or not a value is close to another value based off of a certain deviation.
bool close_to(float x, float y, float deviation) { return fabs(x-y) < deviation; }

// converts indices of pixels in a 2d array from absolute positions to positions relative to the origin on the x - y plane.
void quantify_plane(Pixel **display) {
    for(int y = 0; y < WINDOW_HEIGHT; y++) {
        for(int x = 0; x < WINDOW_WIDTH; x++) {
            *&display[y][x].x = (XMIN + (x_steps * x));
            *&display[y][x].y = (YMAX - (y_steps * y));
        }
    }
}

// returns a different ascii character based on how close a value is to the end of a range of values.
char ycompress(float num, float pixel, float range) {
    char *table = "_,.-~*'`";
    float steps = range/8;
    float goal = num - (pixel - (range/2) );
    int counter = 0;
    float step = 0;
    while(step < goal) {
        step += steps;
        counter++;
    }
    return table[counter - 1];
}

// sets the display of every pixel to the correct ascii character.
void draw(Pixel **display, float (*func)(float)) {
    float rel_x, rel_y;
    for(int y = 0; y < WINDOW_HEIGHT; y++) {
        for(int x = 0; x < WINDOW_WIDTH; x++) {
            Pixel *pixel = &display[y][x];
            rel_x = pixel -> x;
            rel_y = pixel -> y;
            if(close_to(rel_x, 0, x_steps/2.0)) {
                if(close_to(rel_y, 0, y_steps/2.0))
                    pixel -> display = '+';
                else
                    pixel -> display = '|';
            }
            else if(close_to(rel_y, 0, y_steps/2))
                pixel -> display = '-';
            else
                pixel -> display = ' ';

            if(close_to(func(rel_x), rel_y, y_steps/2.0))
                pixel -> display = ycompress(func(rel_x), rel_y, y_steps);
        }
    }
}

// prints the display.
void print_plane(Pixel **display) {
    char **output = malloc(sizeof(char*) * WINDOW_HEIGHT + 1);
    for(int i = 0; i < WINDOW_HEIGHT; i++)
        output[i] = malloc(sizeof(char) * WINDOW_WIDTH + 1);

    for(int y = 0; y < WINDOW_HEIGHT; y++) {
        for(int x = 0; x < WINDOW_WIDTH; x++)
            output[y][x] = *&display[y][x].display;

        output[y][(int)WINDOW_WIDTH] = '\0';
    }

    for(int y = 0; y < WINDOW_HEIGHT; y++)
        puts(output[y]);

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
    // initialize display as multidimensional array of pixels.
    Pixel **display = malloc(sizeof(Pixel *) * WINDOW_HEIGHT);
    for(int i = 0; i < WINDOW_HEIGHT; i++)
        display[i] = malloc(sizeof(Pixel) * WINDOW_WIDTH);
    
    // choice to graph derivative or function.
    char *input = malloc(1);
    do{
        printf("Would you like to graph the derivative? (y, n) \n");
        scanf("%[^\n]%*c", input);
    } while(input[0] != 'y' && input[0] != 'n');

    quantify_plane(display);
    draw(display, input[0] == 'y'? &derive:&function);
    free(input);
    print_plane(display);
    clear_display(display);
}