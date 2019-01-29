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
int width = 800;
int height = 600;
int width_margin = 20;

struct Point rotate_point(int cx,int cy,float angle,struct Point p)
{
  float s = sin(angle);
  float c = cos(angle);

  // translate point back to origin:
  p.x -= cx;
  p.y -= cy;

  // rotate point
  float xnew = p.x * c - p.y * s;
  float ynew = p.x * s + p.y * c;

  // translate point back:
  p.x = ((int) xnew) + cx;
  p.y = ((int) ynew) + cy;
  return p;
}

struct Point find_center(struct Point A, struct Point B, struct Point C, struct Point D)
{
  struct Point E;
  E.x = (int) (A.x+B.x)/2;
  E.y = (int) (A.y+D.y)/2;
  return E;
}

void clearShot(struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, char *fbp) {
    int y = 251*height/1080;
    long int location = 0;
    while((y<949*height/1080)) {
        int x = 0;
        while(x < width - width_margin*width/1080) {
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
  
void drawLines(struct Point A, struct Point B, struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, char *fbp, struct Color color) { 
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

void destroyShip(int xoffset, struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, char *fbp,  int direction) {
    struct Point A, B, C, D, E, F, G, H, X, Z;
    struct Point f1,f2,f3,f4,f5,f6,f7;

    if (direction == 1) {
        A.x = xoffset;
        B.x = xoffset + 30*width/1920;
        C.x = xoffset + 30*width/1920;
        D.x = xoffset + 60*width/1920;
        E.x = xoffset + 70*width/1920;    
        F.x = xoffset + 110*width/1920;
        G.x = xoffset + 90*width/1920;
        H.x = xoffset + 10*width/1920;
        X.x = xoffset + 30*width/1920;
        Z.x = xoffset + 70*width/1920;

        f1.x = xoffset + 15*width/1920;
        f2.x = xoffset + 15*width/1920;
        f3.x = xoffset + 15*width/1920;
        f4.x = xoffset + 15*width/1920;
        f5.x = xoffset - 15*width/1920;
        f6.x = xoffset - 15*width/1920;
        f7.x = xoffset - 15*width/1920;

    } else {
        A.x = xoffset;        
        B.x = xoffset + 40*width/1920;
        C.x = xoffset + 50*width/1920;
        D.x = xoffset + 80*width/1920;
        E.x = xoffset + 80*width/1920;
        F.x = xoffset + 110*width/1920;
        G.x = xoffset + 100*width/1920;
        H.x = xoffset + 20*width/1920;
        X.x = xoffset + 40*width/1920;
        Z.x = xoffset + 80*width/1920;
    
        f1.x = xoffset + 95*width/1920;
        f2.x = xoffset + 95*width/1920;
        f3.x = xoffset + 95*width/1920;
        f4.x = xoffset + 95*width/1920;
        f5.x = xoffset + 125*width/1920;
        f6.x = xoffset + 125*width/1920;
        f7.x = xoffset + 125*width/1920;
    }
    A.y = 200*height/1080;
    B.y = 200*height/1080;
    C.y = 165*height/1080;
    D.y = 165*height/1080;
    E.y = 200*height/1080;
    F.y = 200*height/1080;
    G.y = 250*height/1080;
    H.y = 250*height/1080;
    X.y = 220*height/1080;
    Z.y = 220*height/1080;

    f1.y = 200*height/1080;
    f2.y = 180*height/1080;
    f3.y = 165*height/1080;
    f4.y = 150*height/1080;
    f5.y = 150*height/1080;
    f6.y = 165*height/1080;
    f7.y = 180*height/1080;

    struct Point awal, akhir;
    awal.x = 960*width/1920;
    awal.y = 870*height/1080;

    akhir.x = 960*width/1920;
    akhir.y = 251*height/1080;

    clearShip(vinfo, finfo, fbp);

    struct Color color;

    color.red = 255;
    color.green = 255;
    color.blue = 255;
    color.opacity = 0;

    drawLines(A, B, vinfo, finfo, fbp, color);
    drawLines(B, B, vinfo, finfo, fbp, color);
    drawLines(C, B, vinfo, finfo, fbp, color);
    drawLines(D, B, vinfo, finfo, fbp, color);
    drawLines(E, B, vinfo, finfo, fbp, color);
    drawLines(F, B, vinfo, finfo, fbp, color);
    drawLines(G, B, vinfo, finfo, fbp, color);
    drawLines(H, B, vinfo, finfo, fbp, color);
    drawLines(f1, B, vinfo, finfo, fbp, color);
    drawLines(f2, B, vinfo, finfo, fbp, color);
 
    drawLines(f4, B, vinfo, finfo, fbp, color);
    drawLines(f5, B, vinfo, finfo, fbp, color);
 

    usleep(70000);
    clearShip(vinfo, finfo, fbp);

    drawLines(A, X, vinfo, finfo, fbp, color);
    drawLines(B, X, vinfo, finfo, fbp, color);
    drawLines(C, X, vinfo, finfo, fbp, color);
    drawLines(D, X, vinfo, finfo, fbp, color);
    drawLines(E, X, vinfo, finfo, fbp, color);
    drawLines(F, X, vinfo, finfo, fbp, color);
    drawLines(G, X, vinfo, finfo, fbp, color);
    drawLines(H, X, vinfo, finfo, fbp, color);
    drawLines(f1, X, vinfo, finfo, fbp, color);
    drawLines(f2, X, vinfo, finfo, fbp, color);
 
    drawLines(f4, X, vinfo, finfo, fbp, color);
    drawLines(f5, X, vinfo, finfo, fbp, color);
 

    usleep(70000);
    clearShip(vinfo, finfo, fbp);

    drawLines(A, E, vinfo, finfo, fbp, color);
    drawLines(B, E, vinfo, finfo, fbp, color);
    drawLines(C, E, vinfo, finfo, fbp, color);
    drawLines(D, E, vinfo, finfo, fbp, color);
    drawLines(E, E, vinfo, finfo, fbp, color);
    drawLines(F, E, vinfo, finfo, fbp, color);
    drawLines(G, E, vinfo, finfo, fbp, color);
    drawLines(H, E, vinfo, finfo, fbp, color);
    drawLines(f1, E, vinfo, finfo, fbp, color);
    drawLines(f2, E, vinfo, finfo, fbp, color);
 
    drawLines(f4, E, vinfo, finfo, fbp, color);
    drawLines(f5, E, vinfo, finfo, fbp, color);
 
    
    usleep(70000);

    clearShip(vinfo, finfo, fbp);

    drawLines(A, Z, vinfo, finfo, fbp, color);
    drawLines(B, Z, vinfo, finfo, fbp, color);
    drawLines(C, Z, vinfo, finfo, fbp, color);
    drawLines(D, Z, vinfo, finfo, fbp, color);
    drawLines(E, Z, vinfo, finfo, fbp, color);
    drawLines(F, Z, vinfo, finfo, fbp, color);
    drawLines(G, Z, vinfo, finfo, fbp, color);
    drawLines(H, Z, vinfo, finfo, fbp, color);
    drawLines(f1, Z, vinfo, finfo, fbp, color);
    drawLines(f2, Z, vinfo, finfo, fbp, color);
 
    drawLines(f4, Z, vinfo, finfo, fbp, color);
    drawLines(f5, Z, vinfo, finfo, fbp, color);
 

    usleep(70000);
    clearShip(vinfo, finfo, fbp);
}

void drawShip(int xoffset, struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, char *fbp,  int direction) {
    struct Point A, B, C, D, E, F, G, H;
    struct Point f1,f2,f3,f4,f5,f6,f7;

    if (direction == 1) {
        A.x = xoffset;
        B.x = xoffset + 30*width/1920;
        C.x = xoffset + 30*width/1920;
        D.x = xoffset + 60*width/1920;
        E.x = xoffset + 70*width/1920;    
        F.x = xoffset + 110*width/1920;
        G.x = xoffset + 90*width/1920;
        H.x = xoffset + 10*width/1920;

        f1.x = xoffset + 15*width/1920;
        f2.x = xoffset + 15*width/1920;
        f3.x = xoffset + 15*width/1920;
        f4.x = xoffset + 15*width/1920;
        f5.x = xoffset - 15*width/1920;
        f6.x = xoffset - 15*width/1920;
        f7.x = xoffset - 15*width/1920;

    } else {
        A.x = xoffset;        
        B.x = xoffset + 40*width/1920;
        C.x = xoffset + 50*width/1920;
        D.x = xoffset + 80*width/1920;
        E.x = xoffset + 80*width/1920;
        F.x = xoffset + 110*width/1920;
        G.x = xoffset + 100*width/1920;
        H.x = xoffset + 20*width/1920;
    
        f1.x = xoffset + 95*width/1920;
        f2.x = xoffset + 95*width/1920;
        f3.x = xoffset + 95*width/1920;
        f4.x = xoffset + 95*width/1920;
        f5.x = xoffset + 125*width/1920;
        f6.x = xoffset + 125*width/1920;
        f7.x = xoffset + 125*width/1920;
    }
    A.y = 200*height/1080;
    B.y = 200*height/1080;
    C.y = 165*height/1080;
    D.y = 165*height/1080;
    E.y = 200*height/1080;
    F.y = 200*height/1080;
    G.y = 250*height/1080;
    H.y = 250*height/1080;

    f1.y = 200*height/1080;
    f2.y = 180*height/1080;
    f3.y = 165*height/1080;
    f4.y = 150*height/1080;
    f5.y = 150*height/1080;
    f6.y = 165*height/1080;
    f7.y = 180*height/1080;

    struct Point awal, akhir;
    awal.x = 960*width/1920;
    awal.y = 870*height/1080;

    akhir.x = 960*width/1920;
    akhir.y = 251*height/1080;

    struct Color color;
    color.red = 0;
    color.green = 255;
    color.blue = 0;
    color.opacity = 0;

    drawLines(A, F, vinfo, finfo, fbp, color);
    drawLines(B, C, vinfo, finfo, fbp, color);
    drawLines(C, D, vinfo, finfo, fbp, color);
    drawLines(D, E, vinfo, finfo, fbp, color);
    drawLines(F, G, vinfo, finfo, fbp, color);
    drawLines(G, H, vinfo, finfo, fbp, color);
    drawLines(H, A, vinfo, finfo, fbp, color);

    drawLines(f1, f4, vinfo, finfo, fbp, color);
    drawLines(f4, f5, vinfo, finfo, fbp, color);
    drawLines(f5, f7, vinfo, finfo, fbp, color);
    drawLines(f2, f7, vinfo, finfo, fbp, color);
    drawLines(f3, f6, vinfo, finfo, fbp, color);



}

void clearShip(struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, char *fbp) {
    int y = 0;
    long int location = 0;
    while((y<251*height/1080)) {
        int x = 0;
        while(x < width) {
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

void drawBullet(struct Point offset, struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, char *fbp, int rotate) {
    struct Point A, B, C, D, E;
    A.x = offset.x - 14*width/1920;
    A.y = offset.y + 60*height/1080;

    B.x = offset.x + 14*width/1920;
    B.y = offset.y + 60*height/1080;

    C.x = offset.x + 14*width/1920;
    C.y = offset.y + 20*height/1080;

    D.x = offset.x - 14*width/1920;
    D.y = offset.y + 20*height/1080;
    
    if (rotate == 1) {
        E = find_center(D, C, B, A);
        A = rotate_point(E.x, E.y, -0.523, A);
        B = rotate_point(E.x, E.y, -0.523, B);
        C = rotate_point(E.x, E.y, -0.523, C);
        D = rotate_point(E.x, E.y, -0.523, D);
        offset = rotate_point(E.x, E.y, -0.523, offset);
    }

    if (rotate == 2) {
        E = find_center(D, C, B, A);
        A = rotate_point(E.x, E.y, 0.523, A);
        B = rotate_point(E.x, E.y, 0.523, B);
        C = rotate_point(E.x, E.y, 0.523, C);
        D = rotate_point(E.x, E.y, 0.523, D);
        offset = rotate_point(E.x, E.y, 0.523, offset);
    }

    struct Color color;
    color.red = 255;
    color.green = 0;
    color.blue = 0;
    color.opacity = 0;

    drawLines(A, B, vinfo, finfo, fbp, color);
    drawLines(B, C, vinfo, finfo, fbp, color);
    drawLines(C, D, vinfo, finfo, fbp, color);
    drawLines(D, A, vinfo, finfo, fbp, color);

    drawLines(offset, C, vinfo, finfo, fbp, color);
    drawLines(offset, D, vinfo, finfo, fbp, color);

}

void drawCannon(struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, char *fbp) {
    struct Point A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T;
    A.x = 900*width/1920;
    A.y = 1050*height/1080;

    B.x = 1020*width/1920;
    B.y = 1050*height/1080;

    C.x = 1020*width/1920;
    C.y = 1020*height/1080;

    D.x = 985*width/1920;
    D.y = 995*height/1080;

    E.x = 985*width/1920;
    E.y = 965*height/1080;

    F.x = 990*width/1920;
    F.y = 965*height/1080;

    G.x = 990*width/1920;
    G.y = 960*height/1080;

    H.x = 930*width/1920;
    H.y = 960*height/1080;

    I.x = 930*width/1920;
    I.y = 965*height/1080;

    J.x = 935*width/1920;
    J.y = 965*height/1080;

    K.x = 935*width/1920;
    K.y = 995*height/1080;

    L.x = 900*width/1920;
    L.y = 1020*height/1080;

    M.x = 935*width/1920;
    M.y = 1050*height/1080;

    N.x = 985*width/1920;
    N.y = 1050*height/1080;

    O.x = 935*width/1920;
    O.y = 1005*height/1080;

    P.x = 985*width/1920;
    P.y = 1005*height/1080;

    Q.x = 945*width/1920;
    Q.y = 960*height/1080;

    R.x = 975*width/1920;
    R.y = 960*height/1080;

    S.x = 975*width/1920;
    S.y = 950*height/1080;

    T.x = 945*width/1920;
    T.y = 950*height/1080;

    struct Color color;
    color.red = 0;
    color.blue = 255;
    color.green = 0;
    color.opacity = 0;

    drawLines(A, B, vinfo, finfo, fbp, color);
    drawLines(B, C, vinfo, finfo, fbp, color);
    drawLines(C, D, vinfo, finfo, fbp, color);
    drawLines(D, E, vinfo, finfo, fbp, color);
    drawLines(E, F, vinfo, finfo, fbp, color);
    drawLines(F, G, vinfo, finfo, fbp, color);
    drawLines(G, H, vinfo, finfo, fbp, color);
    drawLines(H, I, vinfo, finfo, fbp, color);
    drawLines(I, J, vinfo, finfo, fbp, color);
    drawLines(J, K, vinfo, finfo, fbp, color);
    drawLines(K, L, vinfo, finfo, fbp, color);
    drawLines(L, A, vinfo, finfo, fbp, color);

    drawLines(F, I, vinfo, finfo, fbp, color);
    drawLines(M, K, vinfo, finfo, fbp, color);
    drawLines(N, D, vinfo, finfo, fbp, color);
    drawLines(E, J, vinfo, finfo, fbp, color);
    drawLines(K, D, vinfo, finfo, fbp, color);

    drawLines(O, P, vinfo, finfo, fbp, color);
    drawLines(R, S, vinfo, finfo, fbp, color);
    drawLines(S, T, vinfo, finfo, fbp, color);
    drawLines(T, Q, vinfo, finfo, fbp, color);

}



int main(void)
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

    struct Point offset;
    offset.x = 960*width/1920;
    offset.y = 890*height/1080;

    x = 0; y = 0;       // Where we are going to put the pixel

    while( y < height-1 ) {
        x = 0;
        while( x < width-1 ) {
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

    int destroyed = 0;
    int shot = 0;

    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    scrollok(stdscr, TRUE);
    int xoffset = 30*width/1920;
    while(1) {        
        drawCannon(vinfo, finfo, fbp);    
        if(offset.y <= 251*height/1080 && (xoffset+20*width/1920<=960*width/1920 && xoffset+100*width/1920>=960*width/1920)) {
            drawBullet(offset, vinfo, finfo, fbp, 0);
            clearShot(vinfo, finfo, fbp);
            while(1)
                destroyShip(xoffset, vinfo, finfo, fbp, direction);
            sleep(1);
            endwin();
            return 0;
        } else {
            drawShip(xoffset, vinfo, finfo, fbp, direction);
        }
         if (kbhit()) {
            int n = getch();
            refresh();
            if (n == 119) {
                shot = 1;
            } else if (n == 97) {
                shot = 2;
            } else if (n == 100) {
                shot = 3;
            }
        }
        if(shot == 1) {
            drawBullet(offset, vinfo, finfo, fbp, 0);
            offset.y -= 10*height/1080;
        }
        else if(shot == 2) {
            drawBullet(offset, vinfo, finfo, fbp, 1);
            offset.y -= 10*height/1080;
            offset.x -= 10*height/1080;
        }
        else if(shot == 3) {
            drawBullet(offset, vinfo, finfo, fbp, 2);
            offset.y -= 10*height/1080;
            offset.x += 10*height/1080;
        }
        usleep(50000);
        clearShip(vinfo, finfo, fbp);
        clearShot(vinfo, finfo, fbp);
        if(direction) {
            xoffset += 5*width/1920;
        } else {
            xoffset -= 5*width/1920;
        }

        if(xoffset < 30*width/1920) {
            direction = 1;
        }

        if(xoffset > 1760*width/1920) {
            direction = 0;
        }

        if((offset.y < 1) || (offset.x < 1)) {
            offset.y = 890*height/1080;
            offset.x = 960*width/1920;
            shot = 0;
        }
    }

    munmap(fbp, screensize);
    close(fbfd);
    return 0;
}