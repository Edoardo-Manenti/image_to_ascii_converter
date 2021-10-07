#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
/*
	DOCUMENTATION:
	
	the execution syntax is of the type:
		./[executable_file_name] [image_file_name] [output_line_length]
	
	the output_line_length is expressed in number of characters 	
*/

int WIDTH_L;

// calculate the brightess value of a pixel based on the RGB values
void calculate_brightness(unsigned char *brigh_arr, unsigned char *original, int width, int height, int channels){
    for(int i=0; i < channels*width*height; i+=4){
        brigh_arr[i/channels] = 0.2126*original[i] +
		 0.7152*original[i+1] + 0.0722*original[i+2]; // values in range [0..255]
    }
}

// prints the output
void print_image(char *out, int length){
    for(int i=0; i< length; i++){
	if(i%WIDTH_L == 0)printf("\n");
        printf("%c ", out[i]);
    }
    printf("\n");
}

int main(int argc, char const *argv[])
{
    WIDTH_L = atoi(argv[2]);
    int height, width, channels;
    // load the image
    unsigned char *dirty_img = stbi_load(argv[1], &width, &height, &channels, 0);
    // check if the image is loaded correctly
    if(dirty_img == NULL || channels != 4){
        printf("ERROR loading image\n");
        exit(1);
    }
    printf("Loaded image: width %d px, height %d px, number of channels: %d\n", width, height, channels);
    unsigned char brigh_arr[width*height];
    // populate brightness array.
    calculate_brightness(brigh_arr, dirty_img, width, height, channels);

    // divide the image in blocks of pixel, each output character will represent a block 
    // block dimension is based on the given parameter WIDTH_L and the width of the image
    int block = width/WIDTH_L; // in pixels
    int HEIGHT_L = height/block; // number of lines of the output
    int b_s, index;
    // declare output array
    char b[WIDTH_L*HEIGHT_L];
    // Starting from top left corner
    for(int x=0; x<WIDTH_L; x++){
        for(int y=0; y<HEIGHT_L; y++){
	    // calculate the value of the first index of the block of pixel
	    int s_i = block*(y*width + x);
	    b_s = 0; // total sum of the brightness of the pixels in the block
            for(int i=0; i< block*block; i++){
                    index = s_i + (i/block)*width + i%block;
                b_s += brigh_arr[index];
            }
	    float avr_br = b_s/(block*block); // average brightess level of the block
	    // string of luminance values from darkest to brightest 
	    // the average brightness is first trasposed in the range [0..1].
	    // the value is then multiplied for the number of character in the luminance string to get the approrpiate character
            b[y*WIDTH_L + x] = " .'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$"[(int)(70*(avr_br/255))]; 
        }
    }
    //print to the command line
    print_image(b, HEIGHT_L*WIDTH_L); 
    return 0;
}
