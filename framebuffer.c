#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <time.h>

void writeCharacter(char *fbp, struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, int i, int xoffset, int yoffset, int letter[10][10]);
void writeCharacter(char *fbp, struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, int i, int xoffset, int yoffset, int letter[10][10]) {
    long int location = 0;
    for(int j=9;j>=0;j--) {
        int y = i - (9-j)*10 + yoffset;
        int tempY = y;
        for(int k=0;k<10;k++) {
            int r,g,b;
            if(letter[j][k] == 1) {
                r = 153;
                g = 255;
                b = 51;
            } else {
                r = 0;
                g = 0;
                b = 0;
            }
            int x = xoffset + k*10;
            int tempX = x;
            y = tempY;
            // printf("%d %d : %d\n",j,k,color);
            while((y > i-10-(9-j)*10 + yoffset) && (y>0) && (y<=1079)) {
                while(x < (xoffset+(k+1)*10)) {
                    location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                            (y+vinfo.yoffset) * finfo.line_length;

                    if (vinfo.bits_per_pixel == 32) {
                        *(fbp + location) = b;        // Some blue
                        *(fbp + location + 1) = g;     // A little green
                        *(fbp + location + 2) = r;    // A lot of red
                        *(fbp + location + 3) = 0;      // No transparency
                    } 
                    x++;
                }
                y--;
                x = tempX;
            }
        }
    }
}

int main()
{
    int fbfd = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long int screensize = 0;
    char *fbp = 0;
    int x = 0, y = 0;
    long int location = 0;
              
    int Q[10][10]={{0,0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0,0}};

    int C[10][10]={{0,0,1,1,1,1,1,1,1,0},
                   {0,1,1,1,1,1,1,1,1,0},
                   {1,1,1,0,0,0,0,0,0,0},
                   {1,1,0,0,0,0,0,0,0,0},
                   {1,1,0,0,0,0,0,0,0,0},
                   {1,1,0,0,0,0,0,0,0,0},
                   {1,1,0,0,0,0,0,0,0,0},
                   {1,1,1,0,0,0,0,0,0,0},
                   {0,1,1,1,1,1,1,1,1,0},
                   {0,0,1,1,1,1,1,1,1,0}};
    
    int E[10][10]={{1,1,1,1,1,1,1,1,1,0},
                   {1,1,1,1,1,1,1,1,1,0},
                   {1,1,0,0,0,0,0,0,0,0},
                   {1,1,0,0,0,0,0,0,0,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {1,1,0,0,0,0,0,0,0,0},
                   {1,1,0,0,0,0,0,0,0,0},
                   {1,1,1,1,1,1,1,1,1,0},
                   {1,1,1,1,1,1,1,1,1,0}};

    int H[10][10]={{1,1,0,0,0,0,0,1,1,0},
                   {1,1,0,0,0,0,0,1,1,0},
                   {1,1,0,0,0,0,0,1,1,0},
                   {1,1,0,0,0,0,0,1,1,0},
                   {1,1,1,1,1,1,1,1,1,0},
                   {1,1,1,1,1,1,1,1,1,0},
                   {1,1,0,0,0,0,0,1,1,0},
                   {1,1,0,0,0,0,0,1,1,0},
                   {1,1,0,0,0,0,0,1,1,0},
                   {1,1,0,0,0,0,0,1,1,0}};
    
    int D[10][10]={{1,1,1,1,1,1,1,0,0,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {1,1,0,0,0,0,1,1,1,0},
                   {1,1,0,0,0,0,0,1,1,0},
                   {1,1,0,0,0,0,0,1,1,0},
                   {1,1,0,0,0,0,0,1,1,0},
                   {1,1,0,0,0,0,0,1,1,0},
                   {1,1,0,0,0,0,1,1,1,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {1,1,1,1,1,1,1,0,0,0}};

    int P[10][10]={{1,1,1,1,1,1,1,0,0,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {1,1,0,0,0,0,0,1,1,0},
                   {1,1,0,0,0,0,0,1,1,0},
                   {1,1,0,0,0,0,0,1,1,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {1,1,1,1,1,1,1,0,0,0},
                   {1,1,0,0,0,0,0,0,0,0},
                   {1,1,0,0,0,0,0,0,0,0},
                   {1,1,0,0,0,0,0,0,0,0}};


    // Open the file for reading and writing
    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        perror("Error: cannot open framebuffer device"); 
    }

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
        writeCharacter(fbp, vinfo, finfo, i, 100, 0, C);
        writeCharacter(fbp, vinfo, finfo, i, 200, 0, H);
        writeCharacter(fbp, vinfo, finfo, i, 300, 0, E);
        writeCharacter(fbp, vinfo, finfo, i, 400, 0, E);
        writeCharacter(fbp, vinfo, finfo, i, 500, 0, P);
        writeCharacter(fbp, vinfo, finfo, i, 600, 0, E);
        writeCharacter(fbp, vinfo, finfo, i, 700, 0, E);
        writeCharacter(fbp, vinfo, finfo, i, 800, 0, D);
        usleep(90000);
        y = i-100;
        if(y<0) {
            y = 1;
        }
        while((y<=i)) {
            x = 100;
            while(x < 1900) {

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
            y++;
        }
        i-=8;
    }
    munmap(fbp, screensize);
    close(fbfd);
    return 0;
}
