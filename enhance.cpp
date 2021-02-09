/*

CSCI 3280, Introduction to Multimedia Systems
Spring 2021

Assignment 01
Name: TANG Zizhou
SID: 1155124275
enhance_1: this enhancement is for resolution resizing

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



struct AsciiArt {
	double pixel_location[MAX_PIXEL][MAX_PIXEL] = {};
	double resize_image[MAX_PIXEL][MAX_PIXEL] = {};
	int quant_image[MAX_PIXEL][MAX_PIXEL] = {};
	int final_image[MAX_PIXEL][MAX_PIXEL] = {};
	int sort_image[MAX_PIXEL][MAX_PIXEL] = {};
};

struct AsciiArt black;
struct AsciiArt red;
struct AsciiArt green;
struct AsciiArt blue;

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
	
	//
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
			black.pixel_location[i][j] = process_data[j+i*image_data._width];
			red.pixel_location[i][j] = image_data._data[j+i*image_data._width].R;
			green.pixel_location[i][j] = image_data._data[j+i*image_data._width].G;
			blue.pixel_location[i][j] = image_data._data[j+i*image_data._width].B;
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
		printf("\n%f\n", d_block_height);
		for (i=image_height-1;i>=0;i--)
		{
			for (j=0;j<=image_width-1;j++)
			{
				black.sort_image[image_height-1-i][j] = black.pixel_location[i][j];
				red.sort_image[image_height-1-i][j] = red.pixel_location[i][j];
				green.sort_image[image_height-1-i][j] = green.pixel_location[i][j];
				blue.sort_image[image_height-1-i][j] = blue.pixel_location[i][j];
			}
		}
		int a;
		int b;
		int sum;
		int sum_r;
		int sum_g;
		int sum_b;
		for (i=0;i<=input_height-1;i++)
		{
			for (j=0;j<=input_width-1;j++)
			{
				sum = 0;
				sum_r = 0;
				sum_g = 0;
				sum_b = 0;
                block_height = round((i+1)*d_block_width)-round(i*d_block_width);
                block_width = round((j+1)*d_block_height)-round(j*d_block_height);
				for (a=round(i*d_block_height);a<=round((i+1)*d_block_height)-1;a++)
				{
					for (b=round(j*d_block_width);b<=round((j+1)*d_block_width)-1;b++)
					{
						sum += black.sort_image[a][b];
						sum_r += red.sort_image[a][b];
						sum_g += green.sort_image[a][b];
						sum_b += blue.sort_image[a][b];
					
					}
				}
				black.resize_image[i][j] = sum/(block_height*block_width);
				red.resize_image[i][j] = sum_r/(block_height*block_width);
				green.resize_image[i][j] = sum_g/(block_height*block_width);
				blue.resize_image[i][j] = sum_b/(block_height*block_width);
				// printf("%f", black.resize_image[i][j]);
			}
		}
        target_height = input_height;
        target_width = input_width;
		//	4. Quantization
		for (i=0;i<=target_height-1;i++)
		{
			for (j=0;j<=target_width-1;j++)
			{
				black.quant_image[i][j] = (int)(black.resize_image[i][j]/32);
				red.quant_image[i][j] = (ceil)(red.resize_image[i][j]);
				green.quant_image[i][j] = (ceil)(green.resize_image[i][j]);
				blue.quant_image[i][j] = (ceil)(blue.resize_image[i][j]);
				
			}
		}
		for (i=0;i<=target_height-1;i++)
		{
			for (j=0;j<=target_width-1;j++)
			{
				if (0<=black.quant_image[i][j] && black.quant_image[i][j]<=7)
				{
					if (artisitc_option[0] == 'p')
						black.final_image[i][j] = shades[7-black.quant_image[i][j]];
					else if (artisitc_option[0] == 's' )
					{
						black.final_image[i][j] = shades[black.quant_image[i][j]];
					}
					else
					{
						printf("Incorrect artistic option\n");
						return 0;
					}
				}
					
			}
		}
	//      5. ASCII Mapping and printout
		for (i=0;i<=target_height-1;i++)
		{
			for (j=0;j<=target_width-1;j++)
			{
				printf("%c", black.final_image[i][j]);
			}
			printf("\n");
		}
		// free malloc data
		free(input_height_temp);
		free(input_width_temp);
	}
	// generate the HTML file
	FILE *html_file = fopen("result.htm", "w");
	fprintf(html_file,"<html>\n");
	fprintf(html_file,"<body bgcolor=#FFFFFF>\n");
	fprintf(html_file,"<font face=\"Courier New\">\n");
	for (i=0;i<=target_height-1;i++)
		{
			for (j=0;j<=target_width-1;j++)
			{
				// red_hex = intHex(red.quant_image[i][j]);
				// green_hex = intHex(green.quant_image[i][j]);
				// blue_hex = intHex(blue.quant_image[i][j]);
				fprintf(html_file, "<font color=#");
				fprintf(html_file, "%x%x%x", 
				red.quant_image[i][j],
				green.quant_image[i][j],
				blue.quant_image[i][j]);
				// fprintf(html_file, "%c", red_hex[0]);
				// fprintf(html_file, "%c", red_hex[1]);
				// fprintf(html_file, "%c", green_hex[0]);
				// fprintf(html_file, "%c", green_hex[1]);
				// fprintf(html_file, "%c", blue_hex[0]);
				// fprintf(html_file, "%c", blue_hex[1]);
				fprintf(html_file, ">%c</font>", black.final_image[i][j]);
			}
			fprintf(html_file,"<br>\n");
		}
	fprintf(html_file,"</body>\n</html>");
	fclose(html_file);

	//
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
				fprintf(file, "%c", black.final_image[i][j]);
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
