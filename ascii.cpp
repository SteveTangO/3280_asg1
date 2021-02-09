/*

CSCI 3280, Introduction to Multimedia Systems
Spring 2021

Assignment 01
Name: TANG Zizhou
SID: 1155124275
ascii.cpp

*/

#include <stdlib.h>

#include "stdio.h"
#include "malloc.h"
#include "memory.h"
#include "math.h"
#include "bmp.h"		//	Simple .bmp library

#define MAX_SHADES 8

#define MAX_PIXEL 2000

char shades[MAX_SHADES] = {'@','#','%','*','|','-','.',' '};

#define SAFE_FREE(p)  { if(p){ free(p);  (p)=NULL;} }

double pixel_location[MAX_PIXEL][MAX_PIXEL] = {};
double resize_image[MAX_PIXEL][MAX_PIXEL] = {};
int quant_image[MAX_PIXEL][MAX_PIXEL] = {};
int flip_image[MAX_PIXEL][MAX_PIXEL] = {};
int sort_image[MAX_PIXEL][MAX_PIXEL] = {};
char final_image[MAX_PIXEL][MAX_PIXEL] = {};



int main(int argc, char** argv)
{
	//
	//	1. Open BMP file
	//
	// argv[2] is the name of the file that is passed into the main function
	Bitmap image_data(argv[2]);
	if(image_data.getData() == NULL)
	{
		printf("unable to load bmp image!\n");
		return -1;
	}
	

	//	2. Obtain Luminance
	//
	char* artisitc_option;
	artisitc_option = (char*)malloc(sizeof(char)*20);
	artisitc_option = argv[1];
	int size = image_data._height * image_data._width;
	double* process_data;
	process_data = (double*)malloc(sizeof(double)*size);
	int i = 0;
	for (i = 0; i <= size - 1; i++)
	{
		process_data[i] = image_data._data[i].R * 0.299 + image_data._data[i].G * 0.587 + image_data._data[i].B * 0.114;
		//printf("%f ", process_data[i]);
	}
	// store the bmp file in a 2d array
	int image_width = image_data._width;
	int image_height = image_data._height;

	
	int j;	

	for (i=0;i<=image_height-1;i++)
	{
		for (j=0;j<=image_width-1;j++)
		{
			pixel_location[i][j] = process_data[j+i*image_data._width];
		}
	}
	//
	//  3. Resize image
	//
	// calculate the block size
	int block_width;
	int block_height;
	int target_width;
	int target_height;
	double d_block_width;
	double d_block_height;
	double d_target_column;
	double d_target_row;
	int input_width;
	int input_height;

	//d_block_width = image_data._width / 

	// calculate the resolution
	if (argc > 2)
	{
		printf(argv[3]);
		char* limit = argv[3];
		int length = 0;
		int comma_pos;
		while (limit[length]!='\0')
		{
			printf("%c", limit[length]);
			if (limit[length] == ',')
			{
				comma_pos = length;
			}
			length++;
		}
		comma_pos = comma_pos + 1;
		printf("end\n");
		printf("%d ", length);
		printf("%d ", comma_pos);
		
		char* input_width_temp;
		input_width_temp = (char*)malloc(sizeof(char) * 5);
		char* input_height_temp;
		input_height_temp = (char*)malloc(sizeof(char) * 5);
		int j;
		for (j = 0; j <= comma_pos - 2; j++)
		{
			input_width_temp[j] = limit[j];
		}
		for (j = comma_pos; j <= length-1; j++)
		{
			input_height_temp[j-comma_pos] = limit[j];
		}
		input_height = atoi(input_height_temp);
		input_width = atoi(input_width_temp);
		
		if (input_height == 0)
		{
			input_height = input_width;
		}
		printf("\n%d %d", input_width, input_height);
		d_block_width = image_data._width/(double)input_width;
		d_block_height = image_data._height/(double)input_height;
		printf("\n%f", d_block_width);
		printf("\n%f", d_block_height);
		block_width = ceil(d_block_width);
		block_height = ceil(d_block_height);
		printf("\n%d %d", block_width, block_height);
		target_width = (image_data._width-(image_data._width%block_width))/block_width;
		target_height = (image_data._height-(image_data._height%block_height))/block_height;
		if (target_height>input_height)
		{
			target_height = input_height;
		}
		if (target_width>input_width)
		{
			target_width = input_width;
		}
		printf("\ntarget: %d %d\n", target_width, target_height);
		for (i=image_height-1;i>=0;i--)
		{
			for (j=0;j<=image_width-1;j++)
			{
				sort_image[image_height-1-i][j] = pixel_location[i][j];
			}
		}
		int a;
		int b;
		int sum;
		for (i=0;i<=target_height-1;i++)
		{
			for (j=0;j<=target_width-1;j++)
			{
				sum = 0;
				for (a=i*block_height;a<=(i+1)*block_height-1;a++)
				{
					for (b=j*block_width;b<=(j+1)*block_width-1;b++)
					{
						sum += sort_image[a][b];
					}

				}
				resize_image[i][j] = sum/(block_height*block_width);
				// printf("%f", resize_image[i][j]);
			}
		}
	//	4. Quantization
	//
		for (i=0;i<=target_height-1;i++)
		{
			for (j=0;j<=target_width-1;j++)
			{
				quant_image[i][j] = (int)(resize_image[i][j]/32);
			}
		}
		for (i=0;i<=target_height-1;i++)
		{
			for (j=0;j<=target_width-1;j++)
			{
				if (0<=quant_image[i][j] && quant_image[i][j]<=7)
					if (artisitc_option[0] == 'p')
						flip_image[i][j] = shades[7-quant_image[i][j]];
					else if (artisitc_option[0] == 's' )
					{
						flip_image[i][j] = shades[quant_image[i][j]];
					}
					else
					{
						printf("Incorrect artistic option\n");
						return 0;
					}
					
					
			}
		}
	//      5. ASCII Mapping and printout

		for (i=0;i<=target_height-1;i++)
		{
			for (j=0;j<=target_width-1;j++)
			{
				printf("%c", flip_image[i][j]);
			}
			printf("\n");
		}
	// free malloc data
		free(input_height_temp);
		free(input_width_temp);
		
	}
	//      6. ASCII art txt file
	//
	
	if (argc > 3)
	{
		FILE *file = fopen(argv[4], "w");
		printf("%s", argv[4]);
		for (i=0;i<=target_height-1;i++)
		{
			for (j=0;j<=target_width-1;j++)
			{
				fprintf(file, "%c", flip_image[i][j]);
			}
			fprintf(file, "\n");
		}
		// fprintf(file, "asdasdadasdasdas");
		fclose(file);
	}
	//  free memory
	free(artisitc_option);
	
	free(process_data);

	
	return 0;
} 
