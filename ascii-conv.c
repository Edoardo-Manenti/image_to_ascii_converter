#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int WIDTH_L;
void calculate_brightness(unsigned char *brigh_arr, unsigned char *original, int width, int height, int channels){
    for(int i=0; i < channels*width*height; i+=4){
        brigh_arr[i/channels] = 0.2126*original[i] +
		 0.7152*original[i+1] + 0.0722*original[i+2];
    }
}

void print_image(char *out, int length){
    for(int i=0; i< length; i++){
	if(i%WIDTH_L == 0)printf("\n");
        printf("%c ", out[i]);
    }
    printf("\n");
}

int main(int argc, char const *argv[])
{
    /* code */
    WIDTH_L = atoi(argv[2]);
    int height, width, channels;
    unsigned char *dirty_img = stbi_load(argv[1], &width, &height, &channels, 0);
    if(dirty_img == NULL || channels != 4){
        printf("ERROR loading image\n");
        exit(1);
    }
    printf("Loaded image: width %d px, height %d px\nNumber of channels: %d\n", width, height, channels);
    unsigned char brigh_arr[width*height];
    calculate_brightness(brigh_arr, dirty_img, width, height, channels);
    int block = width/WIDTH_L;
    int HEIGHT_L = height/block;
    int b_s, index;
    printf("WIDTH: %d, HEIGHT_L: %d", WIDTH_L, HEIGHT_L);
    char b[WIDTH_L*HEIGHT_L];
    // Starting from top left corner
    for(int x=0; x<WIDTH_L; x++){
        for(int y=0; y<HEIGHT_L; y++){
	    b_s = 0;
	    int s_i = block*(y*width + x);
            for(int i=0; i< block*block; i++){
                    index = s_i + (i/block)*width + i%block;
                b_s += brigh_arr[index];
            }
	    float avr_br = b_s/(block*block);
            b[y*WIDTH_L + x] = " .:-=+*#%@"[(int)(10*(avr_br/255))];
        }
    }
    print_image(b, HEIGHT_L*WIDTH_L); 
    return 0;
}
