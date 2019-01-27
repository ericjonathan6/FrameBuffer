/*
To test that the Linux framebuffer is set up correctly, and that the device permissions
are correct, use the program below which opens the frame buffer and draws a gradient-
filled red square:
retrieved from:
Testing the Linux Framebuffer for Qtopia Core (qt4-x11-4.2.2)
http://cep.xor.aps.anl.gov/software/qt4-x11-4.2.2/qtopiacore-testingframebuffer.html
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
struct Point { 
   int x, y; 
};  

void clearShot(struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, char *fbp) {
    int y = 251;
    long int location = 0;
    while((y<870)) {
        int x = 0;
        while(x < 1900) {
            location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                    (y+vinfo.yoffset) * finfo.line_length;

            if (vinfo.bits_per_pixel == 32) {
                *(fbp + location) = 0;        // Some blue
                *(fbp + location + 1) = 0;     // A little green
                *(fbp + location + 2) = 0;    // A lot of red
                *(fbp + location + 3) = 0;      // No transparency
            } 
            x++;
        }
        y++;
    }
}
  
void drawLines(struct Point A, struct Point B, struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, char *fbp) { 
    int dx = B.x - A.x;
    int dy = B.y - A.y;
    int x0 = A.x, y0 = A.y;
    int x1 = B.x, y1 = B.y;
    int stepx,stepy;
    int iterator = 0;
    long int location = 0;
    if (dy < 0) { dy = -dy;  stepy = -1; } else { stepy = 1; }
    if (dx < 0) { dx = -dx;  stepx = -1; } else { stepx = 1; }
    dy <<= 1;                        
    dx <<= 1;                        
    
    if (dx > dy) {
        int fraction = dy - (dx >> 1);
        while (x0 != x1) {
            x0 += stepx;
            if (fraction >= 0) {
            y0 += stepy;
            fraction -= dx;
            }
            fraction += dy;
            location = (x0+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                        (y0+vinfo.yoffset) * finfo.line_length;

            if (vinfo.bits_per_pixel == 32) {
                *(fbp + location) = 255;        // Some blue
                *(fbp + location + 1) = 255;     // A little green
                *(fbp + location + 2) = 255;    // A lot of red
                *(fbp + location + 3) = 0;      // No transparency
            }
        }
    } else {
        int fraction = dx - (dy >> 1);
        while (y0 != y1) {
            y0 += stepy;
            
            if (fraction >= 0) {
            x0 += stepx;
            fraction -= dy;
            }
            
            fraction += dx;
            location = (x0+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                        (y0+vinfo.yoffset) * finfo.line_length;

            if (vinfo.bits_per_pixel == 32) {
                *(fbp + location) = 255;        // Some blue
                *(fbp + location + 1) = 255;     // A little green
                *(fbp + location + 2) = 255;    // A lot of red
                *(fbp + location + 3) = 0;      // No transparency
            }
        }
    }
}

void drawShip(int xoffset, struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, char *fbp,  int direction) {
    struct Point A, B, C, D, E, F, G;
    A.x = xoffset;
    A.y = 200;

    B.x = xoffset + 40;
    B.y = 200;

    if (direction == 0) {
        C.x = xoffset + 80;
    } else {
        C.x = xoffset + 40;
    }
    C.y = 150;

    D.x = xoffset + 80;
    D.y = 200;

    E.x = xoffset + 120;
    E.y = 200;

    F.x = xoffset + 100;
    F.y = 250;

    G.x = xoffset + 20;
    G.y = 250;

    struct Point awal, akhir;
    awal.x = 960;
    awal.y = 870;

    akhir.x = 960;
    akhir.y = 251;

    drawLines(A, B, vinfo, finfo, fbp);
    drawLines(B, C, vinfo, finfo, fbp);
    drawLines(C, D, vinfo, finfo, fbp);
    drawLines(D, E, vinfo, finfo, fbp);
    drawLines(E, F, vinfo, finfo, fbp);
    drawLines(F, G, vinfo, finfo, fbp);
    drawLines(G, A, vinfo, finfo, fbp);

}

void clearShip(struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, char *fbp) {
    int y = 150;
    long int location = 0;
    while((y<251)) {
        int x = 0;
        while(x < 1900) {
            location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                    (y+vinfo.yoffset) * finfo.line_length;

            if (vinfo.bits_per_pixel == 32) {
                *(fbp + location) = 0;        // Some blue
                *(fbp + location + 1) = 0;     // A little green
                *(fbp + location + 2) = 0;    // A lot of red
                *(fbp + location + 3) = 0;      // No transparency
            } 
            x++;
        }
        y++;
    }
}

void drawCannon(struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, char *fbp) {
    struct Point A, B, C, D, E, F, G, H;
    A.x = 900;
    A.y = 1000;

    B.x = 1020;
    B.y = 1000;

    C.x = 920;
    C.y = 900;

    D.x = 1000;
    D.y = 900;

    E.x = 950;
    E.y = 900;

    F.x = 970;
    F.y = 900;

    G.x = 950;
    G.y = 870;

    H.x = 970;
    H.y = 870;


    drawLines(A, B, vinfo, finfo, fbp);
    drawLines(B, D, vinfo, finfo, fbp);
    drawLines(A, C, vinfo, finfo, fbp);
    drawLines(C, D, vinfo, finfo, fbp);
    drawLines(E, G, vinfo, finfo, fbp);
    drawLines(G, H, vinfo, finfo, fbp);
    drawLines(F, H, vinfo, finfo, fbp);


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

    printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

    // Figure out the size of the screen in bytes
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    // Map the device to memory
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if ((int)fbp == -1) {
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");

    int direction = 1;
    struct Point A, B;
    A.x = 960;
    A.y = 870;

    B.x = 960;
    B.y = 845;
    int xoffset = 20;

    x = 0; y = 0;       // Where we are going to put the pixel

    while( y < 1079 ) {
        x = 0;
        while( x < 1919 ) {
            location = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) +
                    (y+vinfo.yoffset) * finfo.line_length;

            if (vinfo.bits_per_pixel == 32) {
                *(fbp + location) = 0;        // Some blue
                *(fbp + location + 1) = 0;     // A little green
                *(fbp + location + 2) = 0;    // A lot of red
                *(fbp + location + 3) = 0;      // No transparency
            }
            x++;
        }
        y++;
    }


    drawCannon(vinfo, finfo, fbp);
    while(1) {                            
        drawShip(xoffset, vinfo, finfo, fbp, direction);
        drawLines(A, B, vinfo, finfo, fbp);
        usleep(50000);
        clearShip(vinfo, finfo, fbp);
        clearShot(vinfo, finfo, fbp);
        if(direction) {
            xoffset += 5;
        } else {
            xoffset -=5;
        }

        if(xoffset < 20) {
            direction = 1;
        }

        if(xoffset > 1770) {
            direction = 0;
        }

        A.y = B.y;
        B.y -= 25;

        if(B.y < 251) {
            B.y = 251;
        }

        if(A.y == 251) {
            A.y = 870;
            B.y = 845;
        }
                
        
    }

    munmap(fbp, screensize);
    close(fbfd);
    return 0;
} 