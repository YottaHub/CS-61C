/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				Yotta Yottas - Fill out functions
**
**
** DATE:        2022-11-05
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

#define DEAD 0
#define ALIVE_1 128
#define ALIVE_2 192
#define ALIVE_3 255

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	Color *new_pixel = (Color *) malloc(sizeof(Color));
	int row_set_off[9] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};
	int col_set_off[9] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
	int num_of_alive_neighbors_R = 0;
	Color pixel = **(image->image + row * image->cols + col);
	if (pixel.R) {
		num_of_alive_neighbors_R += 8;
	}
	int num_of_alive_neighbors_G = 0;
	if (pixel.G) {
		num_of_alive_neighbors_G += 8;
	}
	int num_of_alive_neighbors_B = 0;
	if (pixel.B) {
		num_of_alive_neighbors_B += 8;
	}
	for (int i = 0; i < 9; i++) {
		int nrow = (row + row_set_off[i] + image->rows) % image->rows;
		int ncol = (col + col_set_off[i] + image->cols) % image->cols;
		if ((**(image->image + nrow * image->cols + ncol)).R > DEAD) {
			num_of_alive_neighbors_R++;
		}
		if ((**(image->image + nrow * image->cols + ncol)).G > DEAD) {
			num_of_alive_neighbors_G++;
		}
		if ((**(image->image + nrow * image->cols + ncol)).B > DEAD) {
			num_of_alive_neighbors_B++;
		}
	}
	if((rule >> num_of_alive_neighbors_R) & 1) {
		//this cell will be alive in next R state
		if (pixel.R >= ALIVE_2) {
			new_pixel->R = ALIVE_3;
		} else if (pixel.R >= ALIVE_1) {
			new_pixel->R = ALIVE_2;
		} else {
			new_pixel->R = ALIVE_1;
		}
	} else {
		//this cell will be dead in next R state
		new_pixel->R = DEAD;
	}
	if((rule >> num_of_alive_neighbors_G) & 1) {
		//this cell will be alive in next G state
		if (pixel.G >= ALIVE_2) {
			new_pixel->G = ALIVE_3;
		} else if (pixel.G >= ALIVE_1) {
			new_pixel->G = ALIVE_2;
		} else {
			new_pixel->G = ALIVE_1;
		}
	} else {
		//this cell will be dead in next G state
		new_pixel->G = DEAD;
	}
	if((rule >> num_of_alive_neighbors_B) & 1) {
		//this cell will be alive in next B state
		if (pixel.B >= ALIVE_2) {
			new_pixel->B = ALIVE_3;
		} else if (pixel.B >= ALIVE_1) {
			new_pixel->B = ALIVE_2;
		} else {
			new_pixel->B = ALIVE_1;
		}
	} else {
		//this cell will be dead in next G state
		new_pixel->B = DEAD;
	}
	return new_pixel;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	Image *np = (Image*) malloc(sizeof(Image));
	np->rows = image->rows;
	np->cols = image->cols;
	np->image = (Color**) malloc(np->rows * np->cols * sizeof(Color*));
	Color **p = np->image;
	for (int i = 0; i < np->rows; i++) {
		for (int j = 0; j < np->cols; j++) {
			*p = evaluateOneCell(image, i, j, rule);
			p++;
		}
	}
	return np;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	if (argc != 3 || argv[2][0] != '0' || argv[2][1] != 'x') {
		printf("usage: %s filename rule\n",argv[0]);
		printf("filename is an ASCII PPM file (type P3) with maximum value 255.\n");
		printf("rule is a hex number beginning with 0x; Life is 0x1808.\n");
		exit(-1);
	}
	char *filename = argv[1];
	uint32_t rule = (uint32_t) strtol(argv[2], NULL, 0);
	Image *image = readData(filename);
	Image *new_image = life(image, rule);
	writeData(new_image);
	freeImage(image);
	freeImage(new_image);
}
