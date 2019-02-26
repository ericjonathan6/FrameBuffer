/*
To test that the Linux framebuffer is set up correctly, and that the device permissions
are correct, use the program below which opens the frame buffer and draws a gradient-
filled red square:
retrieved from:
Testing the Linux Framebuffer for Qtopia Core (qt4-x11-4.2.2)
http://cep.xor.aps.anl.gov/software/qt4-x11-4.2.2/qtopiacore-testingframebuffer.html
*/

#include <ncurses.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <bits/stdc++.h>
using namespace std;

const int WIDTH = 1300;
const int HEIGHT = 755;
const int WIDTH_MARGIN = 20;

#define PI 3.14159265

struct Point { 
   int x, y; 
};

struct Color {
    unsigned char red, green, blue, opacity;
};

int kbhit(void)
{
    int ch = getch();
    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } else {
        return 0;
    }
}

void drawColor(int x, int y, struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, char *fbp, struct Color color) {
    // *fbp = color.blue;
    long int location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                        (y+vinfo.yoffset) * finfo.line_length;
    // if (vinfo.bits_per_pixel == 32) {
    //     *(fbp + location) = color.blue;        // Some blue
    //     *(fbp + location + 1) = color.green;     // A little green
    //     *(fbp + location + 2) = color.red;    // A lot of red
    //     *(fbp + location + 3) = color.opacity;      // No transparency
    // }
    if (vinfo.bits_per_pixel == 32) {
        if  ( 
            ( *(fbp + location) == 0) &&        // Some blue
            ( *(fbp + location + 1) == 0 ) &&     // A little green
            ( *(fbp + location + 2) == 0 ) &&   // A lot of red
            ( *(fbp + location + 3) == 0 )
            ) {
               if (vinfo.bits_per_pixel == 32) {
                    *(fbp + location) = color.blue;        // Some blue
                    *(fbp + location + 1) = color.green;     // A little green
                    *(fbp + location + 2) = color.red;    // A lot of red
                    *(fbp + location + 3) = color.opacity;      // No transparency
                }
                drawColor(x+1,y,vinfo,finfo,fbp,color);
                drawColor(x,y+1,vinfo,finfo,fbp,color);
                drawColor(x-1,y,vinfo,finfo,fbp,color);
                drawColor(x,y-1,vinfo,finfo,fbp,color);   
            }      // No transparency
    }
}

void drawLines(struct Point A, struct Point B, struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, char *fbp, struct Color color) { 
    int dx = B.x - A.x;
    int dy = B.y - A.y;
    int x0 = A.x, y0 = A.y;
    int x1 = B.x, y1 = B.y;
    int stepx,stepy;
    int iterator = 0;
    long int location = 0;

    if (dy < 0) {
        dy = -dy;  stepy = -1;
    } else {
        stepy = 1;
    }
    
    if (dx < 0) {
        dx = -dx;
        stepx = -1;
    } else {
        stepx = 1;
    }

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
                *(fbp + location) = color.blue;        // Some blue
                *(fbp + location + 1) = color.green;     // A little green
                *(fbp + location + 2) = color.red;    // A lot of red
                *(fbp + location + 3) = color.opacity;      // No transparency
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
                *(fbp + location) = color.blue;        // Some blue
                *(fbp + location + 1) = color.green;     // A little green
                *(fbp + location + 2) = color.red;    // A lot of red
                *(fbp + location + 3) = color.opacity;      // No transparency
            }
        }
    }
}

void create_building(int n_of_point, Point p[], Color color, struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, char *fbp){
    for(int i=1;i<n_of_point;i++){
        drawLines(p[i], p[i-1], vinfo, finfo, fbp, color);
    }
    drawLines(p[n_of_point-1], p[0], vinfo, finfo, fbp, color);
}


int main(void)
{
    int fbfd = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long int screensize = 0;
    char *fbp = 0;
    int x, y;
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
    if (atoi(fbp) == -1) {
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");

    int direction = 1;

    struct Point offset;
    offset.x = 960 * WIDTH / 1920;
    offset.y = 885 * HEIGHT / 1080;
    int degree1 = 0;
    int degree2 = 90;

    x = 0; y = 0;       // Where we are going to put the pixel

    while( y < HEIGHT - 1 ) {
        x = 0;
        while( x < WIDTH - 1 ) {
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

    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    scrollok(stdscr, TRUE);
   
    int xoffset = WIDTH - 200 * WIDTH / 1920;
    int yoffset = 200 * HEIGHT / 1080;

    struct Color color,color_window;

    struct Point A,B,C,D,a,b,c,d;
    struct Point A1,B1,C1,D1;
    a.x = 187;
    a.y = 109;

    b.x = 287;
    b.y = 109;

    c.x = 287;
    c.y = 209;

    d.x = 187;
    d.y = 209;

    color_window.red = 255;
    color_window.blue = 55;
    color_window.green = 0;
    color_window.opacity = 10;

    drawLines(a, b, vinfo, finfo, fbp, color_window);
    drawLines(b, c, vinfo, finfo, fbp, color_window);
    drawLines(c, d, vinfo, finfo, fbp, color_window);
    drawLines(d, a, vinfo, finfo, fbp, color_window);

    color.red = 255;
    color.blue = 255;
    color.green = 255;
    color.opacity = 0;


    A.x = 742;
    A.y = 107;

    B.x = 1115;
    B.y = 107;

    C.x = 742;
    C.y = 643;

    D.x = 1115;
    D.y = 643;

    drawLines(A, B, vinfo, finfo, fbp, color);
    drawLines(B, D, vinfo, finfo, fbp, color);
    drawLines(C, D, vinfo, finfo, fbp, color);
    drawLines(C, A, vinfo, finfo, fbp, color);    

    A1.x = 185;
    A1.y = 107;

    B1.x = 557;
    B1.y = 107;

    C1.x = 185;
    C1.y = 643;

    D1.x = 557;
    D1.y = 643;

    drawLines(A1, B1, vinfo, finfo, fbp, color);
    drawLines(B1, D1, vinfo, finfo, fbp, color);
    drawLines(C1, D1, vinfo, finfo, fbp, color);
    drawLines(C1, A1, vinfo, finfo, fbp, color);    


    struct Point point[50][20];

    int n_of_object;
    int count = 0;
    int n_of_point, idx, idy;
    ifstream file("building.txt");

    if (file.is_open()){
        int index_counter=0;
        file >> n_of_object;
        while (count < n_of_object && file >> n_of_point){
          for(int i=0;i<n_of_point;i++){
            file >> idx;
            file >> idy;
            point[index_counter][i].x = idx + A1.x;
            point[index_counter][i].y = idy + A1.y;
          }
          // create_building(n_of_point,point[index_counter],color, vinfo, finfo, fbp);
          count++;
          index_counter++;
        } 
        file.close();
    }

    else cout << "Unable to open file"; 
      
    int key;
    while(1){
        for(int i=0;i<n_of_object;i++){
            create_building(n_of_point,point[i],color, vinfo, finfo, fbp);
        }
        drawLines(A, B, vinfo, finfo, fbp, color);
        drawLines(B, D, vinfo, finfo, fbp, color);
        drawLines(C, D, vinfo, finfo, fbp, color);
        drawLines(C, A, vinfo, finfo, fbp, color);

        drawLines(A1, B1, vinfo, finfo, fbp, color);
        drawLines(B1, D1, vinfo, finfo, fbp, color);
        drawLines(C1, D1, vinfo, finfo, fbp, color);
        drawLines(C1, A1, vinfo, finfo, fbp, color);

        drawLines(a, b, vinfo, finfo, fbp, color_window);
        drawLines(b, c, vinfo, finfo, fbp, color_window);
        drawLines(c, d, vinfo, finfo, fbp, color_window);
        drawLines(d, a, vinfo, finfo, fbp, color_window);
        if (kbhit()) {
            int n = getch();
            refresh();
            if (n == 72) { // Up
                // shot = 1;
            } else if (n == 75) { //Left
                // shot = 2;
            } else if (n == 77) { //Right
                // shot = 3;
            } else if (n == 80) { //Down

            }
        }
    }
    // while( (key=getch()) != 27 ){
    //         int n = getch();
    //         refresh();
    //         if (n == 72) { //up
    //             a.y-=25;
    //             b.y-=25;
    //             c.y-=25;
    //             d.y-=25;
    //         } else if (n == 75) { //left
    //             a.x-=25;
    //             b.x-=25;
    //             c.x-=25;
    //             d.x-=25;
    //         } else if (n == 77) { //right
    //             a.x+=25;
    //             b.x+=25;
    //             c.x+=25;
    //             d.x+=25;
    //         } else if (n == 80) { //down
    //             a.y+=5;
    //             b.y+=5;
    //             c.y+=5;
    //             d.y+=5;
    //         }
    //     drawLines(a, b, vinfo, finfo, fbp, color_window);
    //     drawLines(b, c, vinfo, finfo, fbp, color_window);
    //     drawLines(c, d, vinfo, finfo, fbp, color_window);
    //     drawLines(d, a, vinfo, finfo, fbp, color_window);
 
    // }

    munmap(fbp, screensize);
    close(fbfd);
    return 0;
}