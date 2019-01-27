#include <ncurses.h>
#include <unistd.h>

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

int main(void)
{
    initscr();

    cbreak();
    noecho();
    nodelay(stdscr, TRUE);

    scrollok(stdscr, TRUE);
    while (1) {
        if (kbhit()) {
            int n = getch();
            printw("Key pressed! It was: %d\n", n);
            refresh();
            sleep(1);
            if (n == 97) {
                printw("EUE\n");
                endwin();
                return 0;
            }
        } else {
            printw("No key pressed yet...\n");
            refresh();
            sleep(1);
        }
    }
}