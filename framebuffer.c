#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <time.h>
#include <string.h>

void writeCharacter(char *fbp, struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, int i, int xoffset, int yoffset, int letter[10][10]);
void writeCharacter(char *fbp, struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, int i, int xoffset, int yoffset, int letter[10][10]) {
    long int location = 0;
    for(int j=9;j>=0;j--) {
        int y = i - (9-j)*10 + yoffset;
        int tempY = y;
        for(int k=0;k<10;k++) {
            int r,g,b;
            if(letter[j][k] == 1) {
                r = 127;
                g = 0;
                b = 255;
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

    int A[10][10]={{0,0,0,1,1,0,0,0,0,0},
                   {0,0,0,1,1,0,0,0,0,0},
                   {0,1,1,1,1,1,1,0,0,0},
                   {0,1,1,1,0,1,1,0,0,0},
                   {0,1,1,0,0,0,1,0,0,0},
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
                   {1,1,0,0,0,1,1,0,0,0},
                   {1,1,0,0,1,1,0,0,0,0},
                   {1,1,0,1,1,0,0,0,0,0},
                   {1,1,1,1,0,0,0,0,0,0},
                   {1,1,1,1,0,0,0,0,0,0},
                   {1,1,0,1,1,0,0,0,0,0},
                   {1,1,0,0,1,1,0,0,0,0},
                   {1,1,0,0,0,1,1,0,0,0},
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
          
    int M[10][10]={{0,0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0,0}};
          
    int N[10][10]={{0,0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0,0}};
          
    int O[10][10]={{1,1,1,1,1,1,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
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
          
    int R[10][10]={{1,1,1,1,1,1,1,1,0,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,0,0,0,0,1,1,0,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {1,1,1,1,1,1,1,1,0,0},
                   {1,1,0,1,1,0,0,0,0,0},
                   {1,1,0,0,1,1,0,0,0,0},
                   {1,1,0,0,0,1,1,0,0,0},
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
                   {0,1,1,0,0,1,1,0,0,0},
                   {0,0,1,1,1,1,0,0,0,0},
                   {0,0,0,1,1,0,0,0,0,0},
                   {0,0,0,1,1,0,0,0,0,0},
                   {0,0,0,1,1,0,0,0,0,0},
                   {0,0,0,1,1,0,0,0,0,0},
                   {0,0,0,1,1,0,0,0,0,0},
                   {0,0,0,1,1,0,0,0,0,0},
                   {0,0,0,1,1,0,0,0,0,0}};
          
    int Z[10][10]={{0,0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0,0},
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

    // Figure out where in memory to put the pixel
    // printf("%d\n",C[0][0]);
    int i = 1079;
    int numberLine = 7;
    int spaceOfLine = 150;
    char  *listNama[] = {"cheepeed",
                         "jason",
                         "ensof",
                         "steven",
                         "ricky",
                         "ejo",
                         "cis"
                        };

    while(i+(numberLine*spaceOfLine) > 0) {
        
        int len = 0;
        int titikAwal = 0 ;
        for(int iterator = 0; iterator < 7; iterator++){
            len = strlen(listNama[iterator]);
            // printf("%d %s",len, listNama[iterator]);
            titikAwal = 100;
            if (iterator == 0) {
                titikAwal += 500;
            }
            for (int it = 0;it < len ; it++) {
                if (listNama[iterator][it] == 'a') {
                    writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), titikAwal, 0, A);
                }
                if (listNama[iterator][it] == 'b') {
                    writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), titikAwal, 0, B);
                }
                if (listNama[iterator][it] == 'c') {
                    writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), titikAwal, 0, C);
                }
                if (listNama[iterator][it] == 'd') {
                    writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), titikAwal, 0, D);
                }
                if (listNama[iterator][it] == 'e') {
                    writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), titikAwal, 0, E);
                }
                if (listNama[iterator][it] == 'f') {
                    writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), titikAwal, 0, F);
                }
                if (listNama[iterator][it] == 'g') {
                    writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), titikAwal, 0, G);
                }
                if (listNama[iterator][it] == 'h') {
                    writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), titikAwal, 0, H);
                }
                if (listNama[iterator][it] == 'i') {
                    writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), titikAwal, 0, I);
                }
                if (listNama[iterator][it] == 'j') {
                    writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), titikAwal, 0, J);
                }
                if (listNama[iterator][it] == 'k') {
                    writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), titikAwal, 0, K);
                }
                if (listNama[iterator][it] == 'l') {
                    writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), titikAwal, 0, L);
                }
                if (listNama[iterator][it] == 'm') {
                    writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), titikAwal, 0, M);
                }
                if (listNama[iterator][it] == 'n') {
                    writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), titikAwal, 0, N);
                }
                if (listNama[iterator][it] == 'o') {
                    writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), titikAwal, 0, O);
                }
                if (listNama[iterator][it] == 'p') {
                    writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), titikAwal, 0, P);
                }
                if (listNama[iterator][it] == 'q') {
                    writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), titikAwal, 0, Q);
                }
                if (listNama[iterator][it] == 'r') {
                    writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), titikAwal, 0, R);
                }
                if (listNama[iterator][it] == 's') {
                    writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), titikAwal, 0, S);
                }
                if (listNama[iterator][it] == 't') {
                    writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), titikAwal, 0, T);
                }
                if (listNama[iterator][it] == 'u') {
                    writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), titikAwal, 0, U);
                }
                if (listNama[iterator][it] == 'v') {
                    writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), titikAwal, 0, V);
                }
                if (listNama[iterator][it] == 'w') {
                    writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), titikAwal, 0, W);
                }
                if (listNama[iterator][it] == 'x') {
                    writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), titikAwal, 0, X);
                }
                if (listNama[iterator][it] == 'y') {
                    writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), titikAwal, 0, Y);
                }
                if (listNama[iterator][it] == 'z') {
                    writeCharacter(fbp, vinfo, finfo, i+(iterator*spaceOfLine), titikAwal, 0, Z);
                }
                if (listNama[iterator][it] == ' ') {
                    titikAwal+=100;
                }

                titikAwal+=100;       
            }
        
        }
        // writeCharacter(fbp, vinfo, finfo, i, 100, 0, C);
        // writeCharacter(fbp, vinfo, finfo, i, 200, 0, H);
        // writeCharacter(fbp, vinfo, finfo, i, 300, 0, E);
        // writeCharacter(fbp, vinfo, finfo, i, 400, 0, E);
        // writeCharacter(fbp, vinfo, finfo, i, 500, 0, P);
        // writeCharacter(fbp, vinfo, finfo, i, 600, 0, E);
        // writeCharacter(fbp, vinfo, finfo, i, 700, 0, E);
        // writeCharacter(fbp, vinfo, finfo, i, 800, 0, D);
        // writeCharacter(fbp, vinfo, finfo, i + spaceOfLine , 100,0,F);
        // writeCharacter(fbp, vinfo, finfo, i + spaceOfLine , 200,0,G);
        // writeCharacter(fbp, vinfo, finfo, i + spaceOfLine , 300,0,I);
        // writeCharacter(fbp, vinfo, finfo, i + spaceOfLine , 400,0,J);


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
    munmap(fbp, screensize);
    close(fbfd);
    return 0;
}
