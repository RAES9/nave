#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

using namespace std;

void dibujarNave(WINDOW *ventana, int y, int x);
void lanzarProyectil(WINDOW *ventana, int y, int x, int altura, int ancho);
void dibujarExplosion(WINDOW *ventana, int y, int x, int altura, int ancho);
void dibujarBordes(WINDOW *ventana);
void dibujarCuadros(WINDOW *ventana, int altura, int ancho);
bool colisionaCuadro(int proyectilY, int proyectilX, int cuadroY, int cuadroX);
void lanzarProyectilLateral(WINDOW *ventana, int y, int x, int altura, int ancho, char direccion);

int main() {
    initscr();
    refresh();
    
    int xMax, yMax;
    getmaxyx(stdscr, yMax, xMax);
    
    int cuadroAltura = yMax - 2;
    int cuadroAnchura = xMax - 2;

    int cuadroY = (yMax - cuadroAltura) / 2;
    int cuadroX = (xMax - cuadroAnchura) / 2;
    
    WINDOW *ventana = newwin(cuadroAltura, cuadroAnchura, cuadroY, cuadroX);
    
    dibujarBordes(ventana);

    int naveY = cuadroAltura - 2;
    int naveX = (cuadroAnchura - 3) / 2;

    dibujarNave(ventana, naveY, naveX);
    dibujarCuadros(ventana, cuadroAltura, cuadroAnchura);
    
    wrefresh(ventana);

    keypad(ventana, TRUE);
    noecho();

    int ch;
    while ((ch = getch()) != 'q') {
        switch (ch) {
            case 'a':
                if (naveX > 2) {
                    mvwprintw(ventana, naveY, naveX + 2, "     ");
                    naveX--;
                    dibujarNave(ventana, naveY, naveX);
                }
                break;
            case 'd':
                if (naveX < cuadroAnchura - 5) {
                    mvwprintw(ventana, naveY, naveX - 2, "     ");
                    naveX++;
                    dibujarNave(ventana, naveY, naveX);
                }
                break;
            case ' ':
                lanzarProyectil(ventana, naveY, naveX, cuadroAltura, cuadroAnchura);
                break;
            case 'n':
                lanzarProyectilLateral(ventana, naveY, naveX, cuadroAltura, cuadroAnchura, 'l');
                break;
            case 'm':
                lanzarProyectilLateral(ventana, naveY, naveX, cuadroAltura, cuadroAnchura, 'r');
                break;
        }
    }
    
    endwin();
    
    return 0;
}

void dibujarBordes(WINDOW *ventana) {
    box(ventana, 0, 0);
    
    srand(static_cast<unsigned int>(time(0)));
}

void lanzarProyectil(WINDOW *ventana, int y, int x, int altura, int ancho) {
    beep();
    int proyectilY = y - 2;
    int proyectilX = x + 2;
    mvwprintw(ventana, proyectilY, proyectilX, "|");
    wrefresh(ventana);
    
    while (proyectilY > 3) {
        usleep(20000);
        mvwprintw(ventana, proyectilY, proyectilX, " ");
        proyectilY--;
        wrefresh(ventana);
        
        for (int i = 0; i <= 3; i++) {
            for (int j = 1; j < ancho - 1; j += 6) {
                if (colisionaCuadro(proyectilY, proyectilX, i, j)) {
                    mvwprintw(ventana, i, j, "      ");
                    mvwprintw(ventana, i - 1, j - 2, "      ");
                    mvwprintw(ventana, i - 2, j - 2, "      ");
                    beep();
                    break;
                } else {
                    mvwprintw(ventana, proyectilY, proyectilX, "|");
                }
            }
        }
    }
    
    dibujarBordes(ventana);
    wrefresh(ventana);
}

void dibujarNave(WINDOW *ventana, int y, int x) {
    mvwprintw(ventana, y, x, "[-0-]");
    dibujarBordes(ventana);
    wrefresh(ventana);
}

void dibujarCuadros(WINDOW *ventana, int altura, int ancho) {
    // Dibuja cuadros arriba
    for (int i = 1; i <= 3; i++) {
        for (int j = 1; j < ancho - 1; j += 6) {
            mvwprintw(ventana, i, j, "+--+");
        }
    }
}

bool colisionaCuadro(int proyectilY, int proyectilX, int cuadroY, int cuadroX) {
    return (proyectilY == cuadroY) &&
           (proyectilX >= cuadroX && proyectilX <= cuadroX + 4);
}

void lanzarProyectilLateral(WINDOW *ventana, int y, int x, int altura, int ancho, char direccion) {
    beep();
    int proyectilY = y;

    if (direccion == 'l') {
        for (int proyectilX = x - 2; proyectilX >= 1; proyectilX--) {
            mvwprintw(ventana, proyectilY, proyectilX + 1, " ");
            mvwprintw(ventana, proyectilY, proyectilX, "-");
            wrefresh(ventana);
            usleep(20000);
        }
        mvwprintw(ventana, proyectilY, 1, " ");
        wrefresh(ventana);
    } else if (direccion == 'r') {
        for (int proyectilX = x + 6; proyectilX < ancho - 1; proyectilX++) {
            mvwprintw(ventana, proyectilY, proyectilX - 1, " ");
            mvwprintw(ventana, proyectilY, proyectilX, "-");
            wrefresh(ventana);
            usleep(20000);
        }
        mvwprintw(ventana, proyectilY, ancho - 2, " ");
        wrefresh(ventana);
    }
    beep();
}
