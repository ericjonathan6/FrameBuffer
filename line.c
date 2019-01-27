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
    while((y<949)) {
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
    struct Point A, B, C, D, E, F, G, H;
    struct Point f1,f2,f3,f4,f5,f6,f7;

    if (direction == 1) {
        A.x = xoffset;
        B.x = xoffset + 30;
        C.x = xoffset + 30;
        D.x = xoffset + 60;
        E.x = xoffset + 70;    
        F.x = xoffset + 110;
        G.x = xoffset + 90;
        H.x = xoffset + 10;

        f1.x = xoffset + 15;
        f2.x = xoffset + 15;
        f3.x = xoffset + 15;
        f4.x = xoffset + 15;
        f5.x = xoffset - 15;
        f6.x = xoffset - 15;
        f7.x = xoffset - 15;

    } else {
        A.x = xoffset;        
        B.x = xoffset + 40;
        C.x = xoffset + 50;
        D.x = xoffset + 80;
        E.x = xoffset + 80;
        F.x = xoffset + 110;
        G.x = xoffset + 100;
        H.x = xoffset + 20;
    
        f1.x = xoffset + 95;
        f2.x = xoffset + 95;
        f3.x = xoffset + 95;
        f4.x = xoffset + 95;
        f5.x = xoffset + 125;
        f6.x = xoffset + 125;
        f7.x = xoffset + 125;
    }
    A.y = 200;
    B.y = 200;
    C.y = 165;
    D.y = 165;
    E.y = 200;
    F.y = 200;
    G.y = 250;
    H.y = 250;

    f1.y = 200;
    f2.y = 180;
    f3.y = 165;
    f4.y = 150;
    f5.y = 150;
    f6.y = 165;
    f7.y = 180;

    struct Point awal, akhir;
    awal.x = 960;
    awal.y = 870;

    akhir.x = 960;
    akhir.y = 251;

    drawLines(A, F, vinfo, finfo, fbp);
    drawLines(B, C, vinfo, finfo, fbp);
    drawLines(C, D, vinfo, finfo, fbp);
    drawLines(D, E, vinfo, finfo, fbp);
    drawLines(F, G, vinfo, finfo, fbp);
    drawLines(G, H, vinfo, finfo, fbp);
    drawLines(H, A, vinfo, finfo, fbp);

    drawLines(f1, f4, vinfo, finfo, fbp);
    drawLines(f4, f5, vinfo, finfo, fbp);
    drawLines(f5, f7, vinfo, finfo, fbp);
    drawLines(f2, f7, vinfo, finfo, fbp);
    drawLines(f3, f6, vinfo, finfo, fbp);



}

void clearShip(struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, char *fbp) {
    int y = 0;
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

void drawBullet(struct Point offset, struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, char *fbp) {
    struct Point A, B, C, D;
    A.x = offset.x - 14;
    A.y = offset.y + 60;

    B.x = offset.x + 14;
    B.y = offset.y + 60;

    C.x = offset.x + 14;
    C.y = offset.y + 20;

    D.x = offset.x - 14;
    D.y = offset.y + 20;

    drawLines(A, B, vinfo, finfo, fbp);
    drawLines(B, C, vinfo, finfo, fbp);
    drawLines(C, D, vinfo, finfo, fbp);
    drawLines(D, A, vinfo, finfo, fbp);

    drawLines(offset, C, vinfo, finfo, fbp);
    drawLines(offset, D, vinfo, finfo, fbp);

}

void drawCannon(struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, char *fbp) {
    struct Point A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T;
    A.x = 900;
    A.y = 1050;

    B.x = 1020;
    B.y = 1050;

    C.x = 1020;
    C.y = 1020;

    D.x = 985;
    D.y = 995;

    E.x = 985;
    E.y = 965;

    F.x = 990;
    F.y = 965;

    G.x = 990;
    G.y = 960;

    H.x = 930;
    H.y = 960;

    I.x = 930;
    I.y = 965;

    J.x = 935;
    J.y = 965;

    K.x = 935;
    K.y = 995;

    L.x = 900;
    L.y = 1020;

    M.x = 935;
    M.y = 1050;

    N.x = 985;
    N.y = 1050;

    O.x = 935;
    O.y = 1005;

    P.x = 985;
    P.y = 1005;

    Q.x = 945;
    Q.y = 960;

    R.x = 975;
    R.y = 960;

    S.x = 975;
    S.y = 950;

    T.x = 945;
    T.y = 950;

    drawLines(A, B, vinfo, finfo, fbp);
    drawLines(B, C, vinfo, finfo, fbp);
    drawLines(C, D, vinfo, finfo, fbp);
    drawLines(D, E, vinfo, finfo, fbp);
    drawLines(E, F, vinfo, finfo, fbp);
    drawLines(F, G, vinfo, finfo, fbp);
    drawLines(G, H, vinfo, finfo, fbp);
    drawLines(H, I, vinfo, finfo, fbp);
    drawLines(I, J, vinfo, finfo, fbp);
    drawLines(J, K, vinfo, finfo, fbp);
    drawLines(K, L, vinfo, finfo, fbp);
    drawLines(L, A, vinfo, finfo, fbp);

    drawLines(F, I, vinfo, finfo, fbp);
    drawLines(M, K, vinfo, finfo, fbp);
    drawLines(N, D, vinfo, finfo, fbp);
    drawLines(E, J, vinfo, finfo, fbp);
    drawLines(K, D, vinfo, finfo, fbp);

    drawLines(O, P, vinfo, finfo, fbp);
    drawLines(R, S, vinfo, finfo, fbp);
    drawLines(S, T, vinfo, finfo, fbp);
    drawLines(T, Q, vinfo, finfo, fbp);

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
    struct Point offset;
    offset.x = 960;
    offset.y = 890;

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

    int destroyed = 0;
    int xoffset = 30;
    drawCannon(vinfo, finfo, fbp);
    while(1) {            
        if(offset.y <= 251 && (xoffset+20<=960 && xoffset+100>=960)) {
            drawBullet(offset, vinfo, finfo, fbp);
            clearShot(vinfo, finfo, fbp);
            // break;
        } else {
            drawShip(xoffset, vinfo, finfo, fbp, direction);
        }       
        drawBullet(offset, vinfo, finfo, fbp);
        usleep(50000);
        clearShip(vinfo, finfo, fbp);
        clearShot(vinfo, finfo, fbp);
        if(direction) {
            xoffset += 5;
        } else {
            xoffset -=5;
        }

        if(xoffset < 30) {
            direction = 1;
        }

        if(xoffset > 1760) {
            direction = 0;
        }

        offset.y -= 10;

        if(offset.y < 1) {
            offset.y = 890;
        }
                
        
    }

    munmap(fbp, screensize);
    close(fbfd);
    return 0;
} 