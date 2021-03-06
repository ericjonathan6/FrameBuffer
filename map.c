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

void draw_color(int x, int y, struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, char *fbp, struct Color color) {
    // *fbp = color.blue;
    long int location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                        (y+vinfo.yoffset) * finfo.line_length;

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
                draw_color(x+1,y,vinfo,finfo,fbp,color);
                draw_color(x,y+1,vinfo,finfo,fbp,color);
                draw_color(x-1,y,vinfo,finfo,fbp,color);
                draw_color(x,y-1,vinfo,finfo,fbp,color);   
            }      // No transparency
    }
}

void fill_pixel(int x, int y, struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, char *fbp, struct Color color) {
    // *fbp = color.blue;
    long int location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                        (y+vinfo.yoffset) * finfo.line_length;

    if (vinfo.bits_per_pixel == 32) {
        *(fbp + location) = color.blue;        // Some blue
        *(fbp + location + 1) = color.green;     // A little green
        *(fbp + location + 2) = color.red;    // A lot of red
        *(fbp + location + 3) = color.opacity;      // No transparency
    }
}

bool check_pixel(int x, int y, struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, char *fbp, struct Color color) {
    
    long int location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                        (y+vinfo.yoffset) * finfo.line_length;

    if (vinfo.bits_per_pixel == 32) {
        if  ( 
            ( *(fbp + location) == color.blue) &&        // Some blue
            ( *(fbp + location + 1) == color.green ) &&     // A little green
            ( *(fbp + location + 2) == color.red ) &&   // A lot of red
            ( *(fbp + location + 3) == color.opacity )
            ) {
               return true;
            }      // No transparency
        return false;
        
    }
    return false;
}

void draw_lines(struct Point A, struct Point B, struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, char *fbp, struct Color color) { 
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

void create_polygon(int n_of_point, struct Point p[], struct Color color, struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, char *fbp){
    for(int i=1;i<n_of_point;i++){
        draw_lines(p[i], p[i-1], vinfo, finfo, fbp, color);
    }
    draw_lines(p[n_of_point-1], p[0], vinfo, finfo, fbp, color);
}

void delete_polygon(int n_of_point,struct Point p[], struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, char *fbp){
    struct Color color;
    color.red = 0;
    color.green = 0;
    color.blue = 0;
    for(int i=1;i<n_of_point;i++){
        draw_lines(p[i], p[i-1], vinfo, finfo, fbp, color);
    }
    draw_lines(p[n_of_point-1], p[0], vinfo, finfo, fbp, color);
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
    printf("AHAHSDFASDF JANCOKKKKKKKKKK\n");

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
    offset.x = 960 * WIDTH / 1300;
    offset.y = 885 * HEIGHT / 755;
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
   
    int xoffset = WIDTH - 200 * WIDTH / 1300;
    int yoffset = 200 * HEIGHT / 755;

    struct Color color,color_window, white, red;

    struct Point global[5], local[5], window[5];
    
    
    color.red = 255;
    color.blue = 255;
    color.green = 255;
    color.opacity = 0;

    white = color;

    color_window.red = 255;
    color_window.blue = 55;
    color_window.green = 0;
    color_window.opacity = 10;

    red = color_window;

    // Right View
    local[0].x = 742 * WIDTH / 1300;
    local[0].y = 107 * HEIGHT / 755;

    local[1].x = 1115 * WIDTH / 1300;
    local[1].y = 107 * HEIGHT / 755;

    local[3].x = 742 * WIDTH / 1300;
    local[3].y = 643 * HEIGHT / 755;

    local[2].x = 1115 * WIDTH / 1300;
    local[2].y = 643 * HEIGHT / 755;

    // Left View
    global[0].x = 185 * WIDTH / 1300;
    global[0].y = 107 * HEIGHT / 755;

    global[1].x = 557 * WIDTH / 1300;
    global[1].y = 107 * HEIGHT / 755;

    global[3].x = 185 * WIDTH / 1300;
    global[3].y = 643 * HEIGHT / 755;

    global[2].x = 557 * WIDTH / 1300;
    global[2].y = 643 * HEIGHT / 755;

    double scale = 1;

    window[0].x = global[0].x;
    window[0].y = global[0].y;

    window[1].x = global[0].x + (global[1].x-global[0].x)*scale;
    window[1].y = global[0].y;

    window[2].x = global[0].x + (global[1].x-global[0].x)*scale;
    window[2].y = global[0].y + (global[2].y-global[0].y)*scale;

    window[3].x = global[0].x;
    window[3].y = global[0].y + (global[2].y-global[0].y)*scale;



    struct Point building[70][20];

    int n_of_object;
    int count = 0;
    int n_of_point, idx, idy;
    
    char ch, file_name[25];
   FILE *fp;
 
 
   fp = fopen("building.txt", "r"); 
    
    
    // ifstream file("building.txt");

    // fscanf(fp, "%d", &number);
   bool is_show[100];
   for(int i=0;i<100;i++){
    is_show[i] = true;
   }
        int index_counter=0;
        // file >> n_of_object;
        fscanf(fp, "%d", &n_of_object);

        while (count < n_of_object ){
            fscanf(fp, "%d", &n_of_point);
          
          for(int i=0;i<n_of_point;i++){

            fscanf(fp, "%d", &idx);
            fscanf(fp, "%d", &idy);

            building[index_counter][i].x = idx * WIDTH / 1300 ;
            building[index_counter][i].y = idy * HEIGHT / 755 ;
          }
          
          count++;
          index_counter++;
        

        
        
        }
        fclose(fp);

    // else cout << "Unable to open file"; 
      
    int key;
    int rectangle_side = 4;
    struct Color zero;
    zero.red = 0;
    zero.green = 0;
    zero.blue = 0;
    zero.opacity = 0;
    int zoom = 1;

    while(1){
        for(int i=0;i<n_of_object;i++){
            if(is_show[i])
                create_polygon(n_of_point,building[i],color, vinfo, finfo, fbp);
            
                draw_color(building[i][0].x+1   , building[i][0].y+1  ,vinfo,finfo,fbp,red);
        }

        create_polygon(rectangle_side, local,white, vinfo, finfo, fbp);
        create_polygon(rectangle_side, global,white, vinfo, finfo, fbp);
        create_polygon(rectangle_side, window, red, vinfo, finfo, fbp);


        if (kbhit()) {
            for(int i=local[0].y+1 ; i<local[2].y; i++){
                for(int j=local[0].x+1; j<local[2].x;j++){
                    fill_pixel(j , i  ,vinfo,finfo,fbp,zero);
                }
            }
            int n = getch();
            refresh();
            if (n == 119) { // Up --> w
                if(window[0].y > global[0].y)
                    for(int i=0;i<rectangle_side;i++){
                        delete_polygon(rectangle_side, window, vinfo, finfo, fbp);
                        window[i].y-=10;
                    }
            } else if (n == 97) { //Left --> a
                if(window[0].x > global[0].x)
                    for(int i=0;i<rectangle_side;i++){
                        delete_polygon(rectangle_side, window, vinfo, finfo, fbp);
                        window[i].x-=10;
                    }
            } else if (n == 100) { //Right --> d
                if(window[2].x <= global[2].x)
                    for(int i=0;i<rectangle_side;i++){
                        delete_polygon(rectangle_side, window,vinfo, finfo, fbp);
                        window[i].x+=10;
                    }
            } else if (n == 115) { //Down --> s
                if(window[2].y + 10 <= global[2].y)
                for(int i=0;i<rectangle_side;i++){
                    delete_polygon(rectangle_side, window,vinfo, finfo, fbp);
                    window[i].y+=10;
                }
            } else if (n == 98) { // --> b big
                zoom*=2;
                scale/=2;
                window[0].x = global[0].x;
                window[0].y = global[0].y;

                window[1].x = global[0].x + (global[1].x-global[0].x)*scale;
                window[1].y = global[0].y;

                window[2].x = global[0].x + (global[1].x-global[0].x)*scale;
                window[2].y = global[0].y + (global[2].y-global[0].y)*scale;

                window[3].x = global[0].x;
                window[3].y = global[0].y + (global[2].y-global[0].y)*scale;
                for(int i=global[0].y+1 ; i<global[2].y; i++){
                    for(int j=global[0].x+1; j<global[2].x;j++){
                        fill_pixel(j , i  ,vinfo,finfo,fbp,zero);
                    }
                }
                create_polygon(rectangle_side, window, red, vinfo, finfo, fbp);  
            } else if (n == 99) { // --> c cilik
                zoom/=2;
                scale*=2;
                window[0].x = global[0].x;
                window[0].y = global[0].y;

                window[1].x = global[0].x + (global[1].x-global[0].x)*scale;
                window[1].y = global[0].y;

                window[2].x = global[0].x + (global[1].x-global[0].x)*scale;
                window[2].y = global[0].y + (global[2].y-global[0].y)*scale;

                window[3].x = global[0].x;
                window[3].y = global[0].y + (global[2].y-global[0].y)*scale;
                for(int i=global[0].y+1 ; i<global[2].y; i++){
                    for(int j=global[0].x+1; j<global[2].x;j++){
                        fill_pixel(j , i  ,vinfo,finfo,fbp,zero);
                    }
                }
                create_polygon(rectangle_side, window, red, vinfo, finfo, fbp); 
            } else {
                for(int i=global[0].y+1 ; i<global[2].y; i++){
                    for(int j=global[0].x+1; j<global[2].x;j++){
                        fill_pixel(j , i  ,vinfo,finfo,fbp,zero);
                    }
                }
                if (n == 'j') { // rumah doang
                    is_show[0] = false;
                } else if (n == 'k') { // jalan doang
                    for(int i=1;i<n_of_object;i++){
                        is_show[i] = false;
                    }
                    is_show[0] = true;
                } else if (n == 'l') { // dua duanya
                    for(int i=0;i<n_of_object;i++){
                        is_show[i] = true;
                    }
                } else if (n == '0') {
                    for(int i=0;i<n_of_object;i++){
                        is_show[i] = false;
                    }
                }
            }
            int x_start = window[0].x;
            int y_start = window[0].y;
            int x_finish = window[2].x;
            int y_finish = window[2].y;


                        bool not_found = false;
            for(int i=y_start+1;i<y_finish;i++){
                for(int j=x_start+1;j<x_finish;j++){
                    // if(check_pixel(i,j,vinfo,finfo,fbp,white)){
                        location = (j+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                        (i+vinfo.yoffset) * finfo.line_length;
                        if (vinfo.bits_per_pixel == 32) {
                            if  ( 
                                ( *(fbp + location) == (char)255) &&        // Some blue
                                ( *(fbp + location + 1) == (char)255 ) &&     // A little green
                                ( *(fbp + location + 2) == (char)255 ) &&   // A lot of red
                                ( *(fbp + location + 3) == (char)0 )
                                ) {
                                    //Fill zoom
                                    int scale_y = (i - y_start)*zoom;
                                    int scale_x = (j - x_start)*zoom;
                                    for(int yy = 0;yy<zoom;yy++){
                                        for(int xx = 0;xx<zoom;xx++){

                                            fill_pixel((scale_x)+ local[0].x + xx   , scale_y + yy+local[0].y  ,vinfo,finfo,fbp,white);
                            
                                        }
                                    }
                                            // fill_pixel((j)+ 557   , i  ,vinfo,finfo,fbp,white);

                                } 
                        }

                        // create_polygon(n_of_point,building[n_of_object-1],color, vinfo, finfo, fbp);
                    // }
                }
                // if(not_found) {
                //     break;
                // }
            }
        }
        
    }

    munmap(fbp, screensize);
    close(fbfd);
    return 0;
}
