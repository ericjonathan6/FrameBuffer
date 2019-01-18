#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <time.h>

int main()
{
    int fbfd = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long int screensize = 0;
    char *fbp = 0;
    int x = 0, y = 0;
    long int location = 0;

    int C[10][10]={{0,0,0,1,1,1,1,0,0,0},
                   {0,0,1,1,1,1,1,1,0,0},
                   {0,1,1,1,0,0,1,1,1,0},
                   {1,1,1,0,0,0,0,1,1,1},
                   {1,1,0,0,0,0,0,0,1,1},
                   {1,1,0,0,0,0,0,0,1,1},
                   {1,1,1,1,1,1,1,1,1,1},
                   {1,1,1,1,1,1,1,1,1,1},
                   {1,1,0,0,0,0,0,0,1,1},
                   {1,1,0,0,0,0,0,0,1,1}};

    // Open the file for reading and writing
    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        perror("Error: cannot open framebuffer device");
        exit(1);
    }
    printf("The framebuffer device was opened successfully.\n");

    // Get fixed screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed information");
        exit(2);
    }

    // Get variable screen information
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        exit(3);
    }

    printf("%dx%d, %dbpp, %dx%d\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel, finfo.line_length, vinfo.xoffset);

    // Figure out the size of the screen in bytes
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    // Map the device to memory
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if ((int)fbp == -1) {
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");

    x = 0; y = 0;       // Where we are going to put the pixel

    // Figure out where in memory to put the pixel
    // printf("%d\n",C[0][0]);
    int i = 1079;
    while(i > 0) {
        for(int j=9;j>=0;j--) {
            x = 100;
            y = i - (9-j)*10;
            int tempY = y;
            for(int k=0;k<10;k++) {
                int color;
                if(C[j][k] == 1) {
                    color = 255;
                } else {
                    color = 0;
                }
                x = 100 + k*10;
                int tempX = x;
                y = tempY;
                // printf("%d %d : %d\n",j,k,color);
                while((y > i-10-(9-j)*10) && (y>0)) {
                    while(x < (100+(k+1)*10)) {
                        location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                                (y+vinfo.yoffset) * finfo.line_length;

                        if (vinfo.bits_per_pixel == 32) {
                            *(fbp + location) = color;        // Some blue
                            *(fbp + location + 1) = color;     // A little green
                            *(fbp + location + 2) = color;    // A lot of red
                            *(fbp + location + 3) = 0;      // No transparency
                        } 
                        x++;
                    }
                    y--;
                    x = tempX;
                }
            }
        }
        usleep(80000);
        y = i;
        while((y>i-100) && (y>0)) {
            x = 100;
            while(x < 200) {

                location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                        (y+vinfo.yoffset) * finfo.line_length;

                if (vinfo.bits_per_pixel == 32) {
                    *(fbp + location) = 0;        // Some blue
                    *(fbp + location + 1) = 0;     // A little green
                    *(fbp + location + 2) = 0;    // A lot of red
                    *(fbp + location + 3) = 0;      // No transparency
            //location += 4;
                } else  { //assume 16bpp
                    int b = 10;
                    int g = (x-100)/6;     // A little green
                    int r = 31-(y-100)/16;    // A lot of red
                    unsigned short int t = r<<11 | g << 5 | b;
                    *((unsigned short int*)(fbp + location)) = t;
                }
                x++;
            }
            y--;
        }
        i-=8;
    }
    munmap(fbp, screensize);
    close(fbfd);
    return 0;
}
