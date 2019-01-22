#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <time.h>
#include <string.h>

void writeCharacter(char *fbp, struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, int i, int xoffset, int yoffset, int letter[10][10], int rgb[3], int size);
void writeCharacter(char *fbp, struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, int i, int xoffset, int yoffset, int letter[10][10], int rgb[3], int size) {
    long int location = 0;
    for(int j=9; j>=0 ; j--) {
        int y = i - (9 - j) * size + yoffset;
        int tempY = y;
        for(int k=0;k<10;k++) {
            int r,g,b;
            if(letter[j][k] == 1) {
                r = rgb[0];
                g = rgb[1];
                b = rgb[2];
            } else {
                r = 0;
                g = 0;
                b = 0;
            }
            int x = xoffset + k*size;
            int tempX = x;
            y = tempY;
            while((y > i-10-(9-j)*size + yoffset) && (y>0) && (y<=1079)) {
                while(x < (xoffset+(k+1)*size)) {
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


int main() {
    int fbfd = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long int screensize = 0;
    char *fbp = 0;
    int x = 0, y = 0;
    long int location = 0;

    int A[10][10]={{1,1,1,1,1,1,1,1,0,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0}};

    int B[10][10]={{1,1,1,1,1,1,1,0,0,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,1,1,1,1,1,0,0,0},
                   {1,1,1,1,1,1,1,0,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {1,1,1,1,1,1,1,0,0,0}};

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

    int F[10][10]={{1,1,1,1,1,1,1,1,0,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {1,1,0,0,0,0,0,0,0,0},
                   {1,1,0,0,0,0,0,0,0,0},
                   {1,1,1,1,1,1,0,0,0,0},
                   {1,1,1,1,1,1,0,0,0,0},
                   {1,1,0,0,0,0,0,0,0,0},
                   {1,1,0,0,0,0,0,0,0,0},
                   {1,1,0,0,0,0,0,0,0,0},
                   {1,1,0,0,0,0,0,0,0,0}};

    int G[10][10]={{1,1,1,1,1,1,1,1,0,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {1,1,0,0,0,0,0,0,0,0},
                   {1,1,0,0,0,0,0,0,0,0},
                   {1,1,0,0,1,1,1,1,0,0},
                   {1,1,0,0,1,1,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {1,1,1,1,1,1,1,1,0,0}};


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

    int I[10][10]={{1,1,1,1,1,1,1,1,0,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {0,0,0,1,1,0,0,0,0,0},
                   {0,0,0,1,1,0,0,0,0,0},
                   {0,0,0,1,1,0,0,0,0,0},
                   {0,0,0,1,1,0,0,0,0,0},
                   {0,0,0,1,1,0,0,0,0,0},
                   {0,0,0,1,1,0,0,0,0,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {1,1,1,1,1,1,1,1,0,0}};

    int J[10][10]={{1,1,1,1,1,1,1,1,0,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {0,0,0,0,0,1,1,0,0,0},
                   {0,0,0,0,0,1,1,0,0,0},
                   {0,0,0,0,0,1,1,0,0,0},
                   {0,0,0,0,0,1,1,0,0,0},
                   {1,1,0,0,0,1,1,0,0,0},
                   {1,1,0,0,0,1,1,0,0,0},
                   {1,1,1,1,1,1,1,0,0,0},
                   {1,1,1,1,1,1,1,0,0,0}};

    int K[10][10]={{1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,1,1,1,1,1,0,0,0},
                   {1,1,1,1,1,1,1,0,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0}};

    int L[10][10]={{1,1,0,0,0,0,0,0,0,0},
                   {1,1,0,0,0,0,0,0,0,0},
                   {1,1,0,0,0,0,0,0,0,0},
                   {1,1,0,0,0,0,0,0,0,0},
                   {1,1,0,0,0,0,0,0,0,0},
                   {1,1,0,0,0,0,0,0,0,0},
                   {1,1,0,0,0,0,0,0,0,0},
                   {1,1,0,0,0,0,0,0,0,0},
                   {1,1,1,1,1,1,1,1,1,0},
                   {1,1,1,1,1,1,1,1,1,0}};

    int M[10][10]={{1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {1,1,0,1,1,0,1,1,0,0},
                   {1,1,0,1,1,0,1,1,0,0},
                   {1,1,0,1,1,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0}};

    int N[10][10]={{1,1,1,0,0,0,1,1,0,0},
                   {1,1,1,0,0,0,1,1,0,0},
                   {1,1,1,1,0,0,1,1,0,0},
                   {1,1,1,1,0,0,1,1,0,0},
                   {1,1,0,1,1,0,1,1,0,0},
                   {1,1,0,1,1,0,1,1,0,0},
                   {1,1,0,0,1,1,1,1,0,0},
                   {1,1,0,0,1,1,1,1,0,0},
                   {1,1,0,0,0,1,1,1,0,0},
                   {1,1,0,0,0,1,1,1,0,0}};

    int O[10][10]={{1,1,1,1,1,1,1,1,0,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {1,1,1,1,1,1,1,1,0,0}};


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

    int Q[10][10]={{0,0,1,1,1,1,0,0,0,0},
                   {0,1,1,1,1,1,1,0,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,1,0,0,1,1,0,0},
                   {1,1,0,0,1,0,1,1,0,0},
                   {1,1,0,0,0,1,1,1,0,0},
                   {0,1,1,1,1,1,1,0,0,0},
                   {0,0,1,1,1,1,0,1,0,0}};

    int R[10][10]={{1,1,1,1,1,1,1,0,0,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,1,1,1,1,1,0,0,0},
                   {1,1,1,1,1,1,1,0,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0}};


    int S[10][10]={{1,1,1,1,1,1,1,1,0,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {1,1,0,0,0,0,0,0,0,0},
                   {1,1,0,0,0,0,0,0,0,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {0,0,0,0,0,0,1,1,0,0},
                   {0,0,0,0,0,0,1,1,0,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {1,1,1,1,1,1,1,1,0,0}};

    int T[10][10]={{1,1,1,1,1,1,1,1,0,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {0,0,0,1,1,0,0,0,0,0},
                   {0,0,0,1,1,0,0,0,0,0},
                   {0,0,0,1,1,0,0,0,0,0},
                   {0,0,0,1,1,0,0,0,0,0},
                   {0,0,0,1,1,0,0,0,0,0},
                   {0,0,0,1,1,0,0,0,0,0},
                   {0,0,0,1,1,0,0,0,0,0},
                   {0,0,0,1,1,0,0,0,0,0}};

    int U[10][10]={{1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {1,1,1,1,1,1,1,1,0,0}};

    int V[10][10]={{1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {0,1,1,0,0,1,1,0,0,0},
                   {0,1,1,0,0,1,1,0,0,0},
                   {0,1,1,0,0,1,1,0,0,0},
                   {0,0,1,1,1,1,0,0,0,0},
                   {0,0,1,1,1,1,0,0,0,0},
                   {0,0,1,1,1,1,0,0,0,0},
                   {0,0,0,1,1,0,0,0,0,0}};

    int W[10][10]={{1,1,0,0,0,0,0,0,1,1},
                   {1,1,0,0,0,0,0,0,1,1},
                   {1,1,0,0,0,0,0,0,1,1},
                   {1,1,0,0,0,0,0,0,1,1},
                   {0,1,1,0,1,1,0,1,1,0},
                   {0,1,1,0,1,1,0,1,1,0},
                   {0,1,1,1,1,1,1,1,1,0},
                   {0,1,1,1,1,1,1,1,1,0},
                   {0,0,1,1,0,0,1,1,0,0},
                   {0,0,1,1,0,0,1,1,0,0}};

    int X[10][10]={{0,1,1,0,0,0,1,1,0,0},
                   {0,1,1,0,0,0,1,1,0,0},
                   {0,1,1,0,0,0,1,1,0,0},
                   {0,0,1,1,0,1,1,0,0,0},
                   {0,0,0,1,1,1,0,0,0,0},
                   {0,0,0,1,1,1,0,0,0,0},
                   {0,0,1,1,0,1,1,0,0,0},
                   {0,1,1,0,0,0,1,1,0,0},
                   {0,1,1,0,0,0,1,1,0,0},
                   {0,1,1,0,0,0,1,1,0,0}};

    int Y[10][10]={{1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {0,1,1,1,1,1,1,0,0,0},
                   {0,0,0,1,1,0,0,0,0,0},
                   {0,0,0,1,1,0,0,0,0,0},
                   {0,0,0,1,1,0,0,0,0,0},
                   {0,0,0,1,1,0,0,0,0,0}};

    int Z[10][10]={{1,1,1,1,1,1,1,1,0,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {0,0,0,0,0,0,1,1,0,0},
                   {0,0,0,0,0,1,1,0,0,0},
                   {0,0,0,0,1,1,0,0,0,0},
                   {0,0,0,1,1,0,0,0,0,0},
                   {0,0,1,1,0,0,0,0,0,0},
                   {0,1,1,0,0,0,0,0,0,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {1,1,1,1,1,1,1,1,0,0}};

    int zero[10][10]={{1,1,1,1,1,1,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0},
                        {1,1,1,0,0,0,1,1,0,0},
                        {1,1,0,1,0,0,1,1,0,0},
                        {1,1,0,0,1,0,1,1,0,0},
                        {1,1,0,0,1,0,1,1,0,0},
                        {1,1,0,0,0,1,1,1,0,0},
                        {1,1,0,0,0,0,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0}};

    int one[10][10]={{0,0,1,1,1,0,0,0,0,0},
                        {0,1,1,1,1,1,0,0,0,0},
                        {1,1,1,1,1,1,0,0,0,0},
                        {1,1,0,1,1,1,0,0,0,0},
                        {0,0,0,1,1,1,0,0,0,0},
                        {0,0,0,1,1,1,0,0,0,0},
                        {0,0,0,1,1,1,0,0,0,0},
                        {0,0,0,1,1,1,0,0,0,0},
                        {1,1,1,1,1,1,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0}};

    int two[10][10]={{1,1,1,1,1,1,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0},
                        {0,0,0,0,0,0,1,1,0,0},
                        {0,0,0,0,0,0,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0},
                        {1,1,1,0,0,0,0,0,0,0},
                        {1,1,1,0,0,0,0,0,0,0},
                        {1,1,1,0,0,0,0,0,0,0},
                        {1,1,1,1,1,1,1,1,0,0}};

    int three[10][10]={{1,1,1,1,1,1,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0},
                        {0,0,0,0,0,1,1,1,0,0},
                        {0,0,0,0,0,1,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0},
                        {0,0,0,0,0,1,1,1,0,0},
                        {0,0,0,0,0,1,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0}};

    int four[10][10]={{1,1,0,0,0,0,1,1,0,0},
                        {1,1,0,0,0,0,1,1,0,0},
                        {1,1,0,0,0,0,1,1,0,0},
                        {1,1,0,0,0,0,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0},
                        {0,0,0,0,0,0,1,1,0,0},
                        {0,0,0,0,0,0,1,1,0,0},
                        {0,0,0,0,0,0,1,1,0,0},
                        {0,0,0,0,0,0,1,1,0,0}};

    int five[10][10]={{1,1,1,1,1,1,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0},
                        {1,1,0,0,0,0,0,0,0,0},
                        {1,1,0,0,0,0,0,0,0,0},
                        {1,1,1,1,1,1,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0},
                        {0,0,0,0,0,0,1,1,0,0},
                        {0,0,0,0,0,0,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0}};

    int six[10][10]={{1,1,1,1,1,1,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0},
                        {1,1,1,0,0,0,0,0,0,0},
                        {1,1,1,0,0,0,0,0,0,0},
                        {1,1,1,1,1,1,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0},
                        {1,1,1,0,0,0,1,1,0,0},
                        {1,1,1,0,0,0,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0}};

    int seven[10][10]={{1,1,1,1,1,1,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0},
                        {0,0,0,0,0,1,1,1,0,0},
                        {0,0,0,0,0,1,1,1,0,0},
                        {0,0,0,0,1,1,1,0,0,0},
                        {0,0,0,0,1,1,1,0,0,0},
                        {0,0,0,1,1,1,0,0,0,0},
                        {0,0,0,1,1,1,0,0,0,0},
                        {0,0,1,1,1,0,0,0,0,0},
                        {0,0,1,1,1,0,0,0,0,0}};

    int eight[10][10]={{1,1,1,1,1,1,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0},
                        {1,1,0,0,0,0,1,1,0,0},
                        {1,1,0,0,0,0,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0},
                        {1,1,0,0,0,0,1,1,0,0},
                        {1,1,0,0,0,0,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0}};

    int nine[10][10]={{1,1,1,1,1,1,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0},
                        {1,1,0,0,0,0,1,1,0,0},
                        {1,1,0,0,0,0,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0},
                        {0,0,0,0,0,0,1,1,0,0},
                        {0,0,0,0,0,0,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0}};

    int dash[10][10]=  {{0,0,0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0,0,0},
                        {1,1,1,1,1,1,1,1,0,0},
                        {1,1,1,1,1,1,1,1,0,0},
                        {0,0,0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0,0,0}};

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

     while( y<700 ) {
                x = 0;
                while( x<1000 ) {
                    location = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) +
                               (y+vinfo.yoffset) * finfo.line_length;

                    if (vinfo.bits_per_pixel == 32) {
                        *(fbp + location) = 200;        // Some blue
                        *(fbp + location + 1) = 200;     // A little green
                        *(fbp + location + 2) = 200;    // A lot of red
                        *(fbp + location + 3) = 0;      // No transparency
                    } else { //assume 16bpp
                        int b = 10;
                        int g = (x - 100) / 6;     // A little green
                        int r = 31 - (y-100) / 16;    // A lot of red
                        unsigned short int t = r<<11 | g << 5 | b;
                        *((unsigned short int*)(fbp + location)) = t;
                    }
                    x++;
                }
                y++;
    }

    while(1) {

        int i = 1079;
        int numberLine = 7;
        int spaceOfLine = 150;
        char  *words[] = {"cheepeed",
                            "jason - 13516024 - anglo school",
                            "ensof - 13516096 - sma kharisma",
                            "steven - 13516102 - sma don bosco",
                            "ricky - 13516105 - sma bhinneka",
                            "eric - 13516117 - sma xaverius",
                            "christian - 13516147 - sma n 1 solo"
                            };

        srand(time(NULL));
        while(i+(numberLine*spaceOfLine) > 0) {
            int len = 0;
            int startPx = 0 ;
            int rgb[3] = {255,192,203};
            for(int iterator = 0; iterator < 7; iterator++){
                int size = 5;
                int inc = size*10;
                rgb[0] = rand()%200 + 50;
                rgb[1] = rand()%200 + 50;
                rgb[2] = rand()%200 + 50;
                len = strlen(words[iterator]);
                startPx = 50;
                if (iterator == 0) {
                    size = 10;
                    inc = size*10;
                }
                for (int it = 0;it < len ; it++) {
                    if (words[iterator][it] == 'a') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, A, rgb, size);
                    }
                    if (words[iterator][it] == 'b') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, B, rgb, size);
                    }
                    if (words[iterator][it] == 'c') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, C, rgb, size);
                    }
                    if (words[iterator][it] == 'd') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, D, rgb, size);
                    }
                    if (words[iterator][it] == 'e') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, E, rgb, size);
                    }
                    if (words[iterator][it] == 'f') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, F, rgb, size);
                    }
                    if (words[iterator][it] == 'g') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, G, rgb, size);
                    }
                    if (words[iterator][it] == 'h') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, H, rgb, size);
                    }
                    if (words[iterator][it] == 'i') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, I, rgb, size);
                    }
                    if (words[iterator][it] == 'j') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, J, rgb, size);
                    }
                    if (words[iterator][it] == 'k') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, K, rgb, size);
                    }
                    if (words[iterator][it] == 'l') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, L, rgb, size);
                    }
                    if (words[iterator][it] == 'm') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, M, rgb, size);
                    }
                    if (words[iterator][it] == 'n') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, N, rgb, size);
                    }
                    if (words[iterator][it] == 'o') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, O, rgb, size);
                    }
                    if (words[iterator][it] == 'p') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, P, rgb, size);
                    }
                    if (words[iterator][it] == 'q') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, Q, rgb, size);
                    }
                    if (words[iterator][it] == 'r') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, R, rgb, size);
                    }
                    if (words[iterator][it] == 's') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, S, rgb, size);
                    }
                    if (words[iterator][it] == 't') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, T, rgb, size);
                    }
                    if (words[iterator][it] == 'u') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, U, rgb, size);
                    }
                    if (words[iterator][it] == 'v') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, V, rgb, size);
                    }
                    if (words[iterator][it] == 'w') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, W, rgb, size);
                    }
                    if (words[iterator][it] == 'x') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, X, rgb, size);
                    }
                    if (words[iterator][it] == 'y') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, Y, rgb, size);
                    }
                    if (words[iterator][it] == 'z') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, Z, rgb, size);
                    }
                    if (words[iterator][it] == '1') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, one, rgb, size);
                    }
                    if (words[iterator][it] == '2') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, two, rgb, size);
                    }
                    if (words[iterator][it] == '3') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, three, rgb, size);
                    }
                    if (words[iterator][it] == '4') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, four, rgb, size);
                    }
                    if (words[iterator][it] == '5') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, five, rgb, size);
                    }
                    if (words[iterator][it] == '6') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, six, rgb, size);
                    }
                    if (words[iterator][it] == '7') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, seven, rgb, size);
                    }
                    if (words[iterator][it] == '8') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, eight, rgb, size);
                    }
                    if (words[iterator][it] == '9') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, nine, rgb, size);
                    }
                    if (words[iterator][it] == '0') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, zero, rgb, size);
                    }
                    if (words[iterator][it] == '-') {
                        writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), startPx, 0, dash, rgb, size);
                    }
                    if (words[iterator][it] == ' ') {
                        startPx+= (inc / 4);
                    }

                    startPx += inc;
                }
            }
            usleep(90000);
            y = i-100;
            if(y<0) {
                y = 1;
            }
            while((y<1080)) {
                x = 0;
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
    }
    munmap(fbp, screensize);
    close(fbfd);
    return 0;
}
