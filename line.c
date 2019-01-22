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
// #include <conio.h>
// #include <graphics.h>

struct Point { 
   int x, y; 
};  
  
// function for line generation 
struct Point* bresenham(struct Point A, struct Point B){ 
   int m_new = 2 * (B.y - A.y);  //multiple by 2 to avoid comparison with 0.5
   // prove : https://csustan.csustan.edu/~tom/Lecture-Notes/Graphics/Bresenham-Line/Bresenham-Line.pdf
   int slope_error_new = m_new - (B.x - A.x);
   struct Point result_points[5000], new_point;
   int iterator = 0;
   for (int x = A.x, y = A.y; x <= B.x; x++){ 
      
      new_point.x = x;
      new_point.y = y;

      result_points[iterator] = new_point;
      slope_error_new += m_new;  
      if (slope_error_new >= 0){ 
         y++; 
         slope_error_new  -= 2 * (B.x - A.x); 
      } 
      iterator++;
   } 
   // for(int i=0;i<iterator;i++){
   //    printf("%d, %d\n", result_points[i].x,result_points[i].y);
   // }
   return result_points;
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

    x = 100; y = 100;       // Where we are going to put the pixel

    // Figure out where in memory to put the pixel
    for (y = 100; y < 300; y++)
        for (x = 100; x < 300; x++) {

            location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                       (y+vinfo.yoffset) * finfo.line_length;

            if (vinfo.bits_per_pixel == 32) {
                *(fbp + location) = 100;        // Some blue
                *(fbp + location + 1) = 15+(x-100)/2;     // A little green
                *(fbp + location + 2) = 200-(y-100)/5;    // A lot of red
                *(fbp + location + 3) = 0;      // No transparency
            } else  { //assume 16bpp
                int b = 10;
                int g = (x-100)/6;     // A little green
                int r = 31-(y-100)/16;    // A lot of red
                unsigned short int t = r<<11 | g << 5 | b;
                *((unsigned short int*)(fbp + location)) = t;
            }

        }
    munmap(fbp, screensize);
    close(fbfd);
    return 0;
} 