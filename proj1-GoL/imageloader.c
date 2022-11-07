/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				Yotta Yottas - Fill up functions
**
**
** DATE:        2022-11-04
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"


//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("File not exist.");
		exit(-1);
	}
	char format[3];
	int depth;
	Image *image = (Image*) malloc(sizeof(Image));
	fscanf(fp, "%s %d %d %d", format, &image->cols, &image->rows, &depth);
	if (format[0] != 'P' || format[1] != '3' || depth != 255) {
		printf("Not a standard input.");
		exit(-1);
	}
	int resolution = image->cols * image->rows;
	image->image = (Color**) malloc(resolution * sizeof(Color*));
	for (int i = 0; i < resolution; i++) {
		*(image->image + i) = (Color*) malloc(sizeof(Color));
		Color *p = *(image->image + i);
		fscanf(fp, "%hhu %hhu %hhu", &p->R, &p->G, &p->B);
	}
	fclose(fp);
	return image;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	int rows = image->rows;
	int cols = image->cols;
	printf("P3\n%d %d\n255\n", cols, rows);
	Color **p = image->image;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols - 1; j++) {
			printf("%3hhu %3hhu %3hhu   ", (*p)->R, (*p)->G, (*p)->B);
			p++;
		}
		printf("%3hhu %3hhu %3hhu\n", (*p)->R, (*p)->G, (*p)->B);
		p++;
	}

}

//Frees an image
void freeImage(Image *image)
{
	int resolution = image->rows * image->cols;
	for (int i = 0; i < resolution; i++) {
		free(*(image->image + i));
	}
	free(image->image);
	free(image);
}

