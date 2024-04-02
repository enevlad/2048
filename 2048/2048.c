/* Programarea Calculatoarelor, seria CC
 * Tema2 - 2048
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curses.h>
#include <ctype.h>
#include <ncurses.h>
#include <time.h>

#define FOREVER 	1
#define INIT_ROW 	20
#define INIT_COL 	40

//defineste nume de culori noi adaugate
#define ORANGE 172
#define HOT_PINK 132
#define DARK_TURQUOISE 44
#define GOLD 178

#define RSIZE 4
#define BSIZE RSIZE * RSIZE


//intializeaza culori si perechi de culori
void initcolor(){
    init_color(ORANGE, 843, 529, 0);
    init_color(HOT_PINK, 686, 372, 529);
    init_color(DARK_TURQUOISE, 0, 843, 843);
    init_color(GOLD, 843, 686, 0);

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, ORANGE, COLOR_BLACK);
    init_pair(4, COLOR_RED, COLOR_BLACK);
    init_pair(5, COLOR_GREEN, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_BLUE, COLOR_BLACK);
    init_pair(8, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(9, HOT_PINK, COLOR_BLACK);
    init_pair(10, DARK_TURQUOISE, COLOR_BLACK);
    init_pair(11, GOLD, COLOR_BLACK);
}

//porneste culoarea petntru o anumita valoare
void color_on(int a, WINDOW *win){
    switch(a){
        case 0: case 2:
            wattron(win, COLOR_PAIR(1));
            break;
        case 4:
            wattron(win, COLOR_PAIR(2));
            break;
        case 8:
            wattron(win, COLOR_PAIR(3));
            break;
        case 16:
            wattron(win, COLOR_PAIR(4));
            break;
        case 32:
            wattron(win, COLOR_PAIR(5));
            break;
        case 64:
            wattron(win, COLOR_PAIR(6));
            break;
        case 128:
            wattron(win, COLOR_PAIR(7));
            break;
        case 256:
            wattron(win, COLOR_PAIR(8));
            break;
        case 512:
            wattron(win, COLOR_PAIR(9));
            break;
        case 1024:
            wattron(win, COLOR_PAIR(10));
            break;
        case 2048:
            wattron(win, COLOR_PAIR(11));
            break;
        default:
            break;
    }
}

//opreste culoarea pentru valoarea respectiva
void color_off(int a, WINDOW *win){
    switch(a){
        case 0: case 2:
            wattroff(win, COLOR_PAIR(1));
            break;
        case 4:
            wattroff(win, COLOR_PAIR(2));
            break;
        case 8:
            wattroff(win, COLOR_PAIR(3));
            break;
        case 16:
            wattroff(win, COLOR_PAIR(4));
            break;
        case 32:
            wattroff(win, COLOR_PAIR(5));
            break;
        case 64:
            wattroff(win, COLOR_PAIR(6));
            break;
        case 128:
            wattroff(win, COLOR_PAIR(7));
            break;
        case 256:
            wattroff(win, COLOR_PAIR(8));
            break;
        case 512:
            wattroff(win, COLOR_PAIR(9));
            break;
        case 1024:
            wattroff(win, COLOR_PAIR(10));
            break;
        case 2048:
            wattroff(win, COLOR_PAIR(11));
            break;
        default:
            break;
    }
}

//sterge un window
void delete_win(WINDOW *win){
    wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wclear(win);
    clear();
    refresh();
    wrefresh(win);
    delwin(win);
}

//adauga o valoare 
void add_tile(int board[][RSIZE]){
    srand(time(NULL));
    int i, free[BSIZE], cnt = 0, pos;
    for(i = 0; i < BSIZE; i++)
        free[i] = 0;
    for(i = 0; i < BSIZE; i++){
        if(board[i / RSIZE][i % RSIZE] == 0)
            free[cnt++] = i;
    }
    if(cnt > 0){
        pos = rand() % cnt;
        board[free[pos] / RSIZE][free[pos] % RSIZE] = ((rand() % 2) + 1) * 2;
    }
}

//initializeaza board cu 0 si doua valori aleatori intre 2 si 4
void init_board(int board[][RSIZE]){
    int i, j; 
    for(i = 0; i < RSIZE; i++){
        for(j = 0; j < RSIZE; j++) {
            board[i][j] = 0;
        }
    }
    add_tile(board);
    add_tile(board);
}

//deseneaza pe ecran tabla de joc
void print_board(WINDOW *win){
    wattron(win, COLOR_PAIR(1));
    box(win, 0, 0);
    mvwvline(win, 1, 10, '|', INIT_ROW - 1);
    mvwvline(win, 1, 20, '|', INIT_ROW - 1);
    mvwvline(win, 1, 30, '|', INIT_ROW - 1);
    mvwhline(win, 5, 1, '-', INIT_COL - 1);
    mvwhline(win, 10, 1, '-', INIT_COL - 1);
    mvwhline(win, 15, 1, '-', INIT_COL - 1);
    wattroff(win, COLOR_PAIR(1));
    wrefresh(win);
}

//actualizeaza afisarea numerelor pe tabla si a scorului
void update_board(int board[][RSIZE], WINDOW *win, int score, int startx, int starty, int scorem){
    int i, j, x, y, k;
    char scr[10], scrm[10];
    sprintf(scr, "%d", score);
    sprintf(scrm, "%d", scorem);
    //afisez scoreul si highscoreul deasupra tablei
    mvaddstr(starty - 2, startx, "score: ");
    mvaddstr(starty - 2, startx + 7, scr);
    mvaddstr(starty - 2, startx + 3 * INIT_COL / 4, "Highscore: ");
    mvaddstr(starty - 2, startx + 3 * INIT_COL / 4 + 13, scrm);
    refresh();
    for(i = 0; i < RSIZE; i++){
        for(j = 0; j < RSIZE; j++){
            x = 1 + (i *(INIT_ROW / 4)) + ((INIT_ROW / 4) / 2);
            y = (j * (INIT_COL / 4)) + ((INIT_COL / 4) / 2);
            char buffer[INIT_COL / 4] = {0};
            //initializez cu spatii buffer
            for(k = 0; k < INIT_COL / 4 - 1; k++){
                buffer[k] = ' ';
            }
            mvwaddstr(win, x, y - (strlen(buffer) / 2), buffer);
            if(board[i][j] != 0){
                //adaug in buffer numaul ca sir de caractere
                sprintf(buffer, "%d", board[i][j]);
            }
            //afisez numarul pe tabla cu culoare corespunzatoare
            color_on(board[i][j], win);
            mvwaddstr(win, x, y - ((strlen(buffer) + 1) / 2) + 1, buffer);
            color_off(board[i][j], win);
        }
    }
    wrefresh(win);
}

//afiseaza meniul cu highlight
void print_menu(WINDOW *win, int highlight, char *choices[], int nch){ 
    int x, y, i;
    x = INIT_COL / 4 + INIT_COL / 8; y = INIT_ROW / 4;
    box(win, 0, 0);
    for(i = 0; i < nch; i++){ 
        if(highlight == i + 1){
            wattron(win, A_REVERSE);
            mvwprintw(win, y, x, "%s", choices[i]);
            wattroff(win, A_REVERSE );
        }
        else
            mvwprintw(win, y, x, "%s", choices[i]);
        y += 2;
    }
    wrefresh(win);
}

//schimba doua valori intre ele
void swap(int *a, int *b){
    int aux;
    aux = (*a);
    (*a) = (*b);
    (*b) = aux;
}

//verifica conditia de pierdere
int check_lose(int board[][RSIZE]){
    int i, j;
    for(i = 0; i < RSIZE; i++){
        for(j = 0; j < RSIZE; j++){
            if(board[i][j] == 0)
                return 0;
        }
    }
    for(i = 0; i < RSIZE - 1; i++){
        for(j = 0; j < RSIZE - 1; j++){
            if(board[i][j] == board[i][j + 1] || board[i][j] == board[i + 1][j])
                return 0;
        }
    }
    for(i = 0; i < RSIZE - 1; i++)
        if(board[i][RSIZE - 1] == board[i + 1][RSIZE - 1])
            return 0;
    for(i = 0; i < RSIZE - 1; i++)
        if(board[RSIZE - 1][i] == board[RSIZE - 1][i + 1])
            return 0;
    return 1;        
}

//verifica conditia de castig
int check_win(int board[][RSIZE]){
    int i, j;
    for(i = 0; i < RSIZE; i++){
        for(j = 0; j < RSIZE; j++){
            if(board[i][j] == 2048)
                return 1;
        }
    }
    return 0;
}

//afiseaza data si ora in panoul de control
void print_time(int startx, int starty){
    time_t t;
    struct tm *area;
    time(&t);
    area = localtime(&t);
    mvaddstr(starty + 12, startx + INIT_COL + 2, asctime(area));
    refresh();
}

//afiseaza tastele care pot fi apasata in panoul de control
void print_info(int startx, int starty){
    mvaddstr(starty + 2, startx + INIT_COL + 2, "UP ARROW KEY - move upwards");
    mvaddstr(starty + 4, startx + INIT_COL + 2, "DOWN ARROW KEY - move downwards");
    mvaddstr(starty + 6, startx + INIT_COL + 2, "LEFT ARROW KEY - move leftwards");
    mvaddstr(starty + 8, startx + INIT_COL + 2, "RIGHT ARROW KEY - move rightwards");
    mvaddstr(starty + 10, startx + INIT_COL + 2, "Q / q - return to menu");
}

//uneste doua celule din board si incrementez scorul
void merge(int *score, int *valid, int *a, int *b){
    (*a) *= 2;
    (*score) += (*a);
    (*b) = 0;
    (*valid) = 1;
}

//are loc imbinarea si apoi mutarea in sus a valorilor matricei
void move_up(int board[][RSIZE], int *score, int *valid){
    int i, j, k;
    //imbin celulele
    for(i = 0; i < RSIZE; i++){
        for(j = 0; j < RSIZE - 1; j++){
            if(board[j][i] != 0){
                for(k = j + 1; k < RSIZE; k++){
                    if(board[k][i] != 0 && board[k][i] == board[j][i]){
                        merge(score, valid, &board[j][i], &board[k][i]);
                       break;
                    }
                    else if(board[k][i] != 0)
                        break;
                }
            }
        }
    }
    //mut celulele
    for(i = 0; i < RSIZE; i++){
        for(j = 0; j < RSIZE - 1; j++){
            if(board[j][i] == 0){
                for(k = j + 1; k < RSIZE; k++){
                    if(board[k][i] != 0){
                        swap(&board[j][i], &board[k][i]);
                        (*valid) = 1;
                        break;
                    }
                }
            }
        }
    }
}

//are loc imbinarea si apoi mutarea in jos a valorilor matricei
void move_down(int board[][RSIZE], int *score, int *valid){
    int i, j, k;
    //imbin celulele
    for(i = 0; i < RSIZE; i++){
        for(j = RSIZE - 1; j > 0; j--){
            if(board[j][i] != 0){
                for(k = j - 1; k >= 0; k--){
                    if(board[k][i] != 0 && board[k][i] == board[j][i]){
                        merge(score, valid, &board[j][i], &board[k][i]);
                        break;
                    }
                    else if(board[k][i] != 0)
                        break;
                }
            }
        }
    }
    //misc celulele 
    for(i = 0; i < RSIZE; i++){
        for(j = RSIZE - 1; j > 0; j--){
            if(board[j][i] == 0){
                for(k = j - 1; k >= 0; k--){
                    if(board[k][i] != 0){
                        swap(&board[j][i], &board[k][i]);
                        (*valid) = 1;
                        break;
                    }
                }
            }
        }
    }
}

//are loc imbinarea si apoi mutarea in stanga a valorilor matricei
void move_left(int board[][RSIZE], int *score, int *valid){
    int i, j, k;
    //imbin celulele
    for(i = 0; i < RSIZE; i++){
        for(j = 0; j < RSIZE - 1; j++){
            if(board[i][j] != 0){
                for(k = j + 1; k < RSIZE; k++){
                    if(board[i][k] != 0 && board[i][k] == board[i][j]){
                        merge(score, valid, &board[i][j], &board[i][k]);
                        break;
                    }
                    else if(board[i][k] != 0)
                        break;
                }
            }
        }
    }
    //misc celulele in board
    for(i = 0; i < RSIZE; i++){
        for(j = 0; j < RSIZE - 1; j++){
            if(board[i][j] == 0){
                for(k = j + 1; k < RSIZE; k++){
                    if(board[i][k] != 0){
                        swap(&board[i][j], &board[i][k]);
                        (*valid) = 1;
                        break;
                    }                                            
                }
            }
        }
    }
}

//are loc imbinarea si apoi mutarea in dreapta a valorilor matricei
void move_right(int board[][RSIZE], int *score, int *valid){
    int i, j, k;
    //imbin celulele
    for(i = 0; i < RSIZE; i++){
        for(j = RSIZE - 1; j > 0; j--){
            if(board[i][j] != 0){
                for(k = j - 1; k >= 0; k--){
                    if(board[i][k] != 0 && board[i][k] == board[i][j]){
                        merge(score, valid, &board[i][j], &board[i][k]);
                        break;
                    }
                    else if(board[i][k] != 0)
                        break;
                }
            }
        }
    }
    //misc celulele in board
    for(i = 0; i < RSIZE; i++){
        for(j = RSIZE - 1; j > 0; j--){
            if(board[i][j] == 0){
                for(k = j - 1; k >= 0; k--){
                    if(board[i][k] != 0){
                        swap(&board[i][j], &board[i][k]);
                        (*valid) = 1;
                        break;
                    }
                }
            }
        }
    }
}

//apeleaza miscarea celulelor in functie de tasta apasata
void play_game(int board[][RSIZE], WINDOW *win, int *score, int *valid, int key){
    switch(key){
        case KEY_UP:
            move_up(board, score, valid);
            break;
        case KEY_DOWN:
            move_down(board, score, valid);
            break;
        case KEY_LEFT:
            move_left(board, score, valid);
            break;
        case KEY_RIGHT:
            move_right(board, score, valid);
            break;
        default:
            break;
    }
}

//determina urmatoarea mutare automata
int automove(int board[][RSIZE], int *score, int *valid, int startx, int starty){
    int zero_max = 0, move, cnt = 0;
    int i, t, cvalid, k;
    int cpboard[RSIZE][RSIZE], cscore;
    cscore = (*score);
    for(i = 0; i < RSIZE; i++){
        //copiez score valid si board
        cnt = 0;
        cscore = (*score);
        cvalid = (*valid);
        for(k = 0; k < RSIZE; k++){
            for(t = 0; t < RSIZE; t++){
                cpboard[k][t] = board[k][t];
            }
        }
        //verific fiecare mutare la stanga dreapta sus sau jos
        //daca are loc inseamna ca e valida si verific cate zerouri creeza mutarea
        //daca e mai mare retin mutarea
        if(i == 0){
            move_left(cpboard, &cscore, &cvalid);
            if(cvalid == 1){
                for(k = 0; k < RSIZE; k++){
                    for(t = 0; t < RSIZE; t++){
                        if(cpboard[k][t] == 0){
                            cnt++;
                        }
                    }
                }
                if(cnt > zero_max){
                    zero_max = cnt;
                    move = i;
                }
            }
        }
        else if(i == 1){
            move_right(cpboard, &cscore, &cvalid);
            if(cvalid == 1){
                for(k = 0; k < RSIZE; k++){
                    for(t = 0; t < RSIZE; t++){
                        if(cpboard[k][t] == 0){
                            cnt++;
                        }
                    }
                }
                if(cnt > zero_max){
                    zero_max = cnt;
                    move = i;
                }
            }
        }
        else if(i == 2){
            move_up(cpboard, &cscore, &cvalid);
            if(cvalid == 1){
                for(k = 0; k < RSIZE; k++){
                    for(t = 0; t < RSIZE; t++){
                        if(cpboard[k][t] == 0){
                            cnt++;
                        }
                    }
                }
                if(cnt > zero_max){
                    zero_max = cnt;
                    move = i;
                }
            }
        }
        else if(i == 3){
            move_down(cpboard, &cscore, &cvalid);
            if(cvalid == 1){
                for(k = 0; k < RSIZE; k++){
                    for(t = 0; t < RSIZE; t++){
                        if(cpboard[k][t] == 0){
                            cnt++;
                        }
                    }
                }
                if(cnt > zero_max){
                    zero_max = cnt;
                    move = i;
                }
                break;
            }
        }
    }
    //afiseaza sub tabla ce mutare va avea loc
    if(move == 0){
        mvaddstr(starty + INIT_ROW + 2, startx + INIT_COL / 4, "AUTOMOVE: LEFT");
        refresh();
    }
    else if(move == 1){
        mvaddstr(starty + INIT_ROW + 2, startx + INIT_COL / 4, "AUTOMOVE: RIGHT");
        refresh();
    }
    else if(move == 2){
        mvaddstr(starty + INIT_ROW + 2, startx + INIT_COL / 4, "AUTOMOVE: UP");
        refresh();
    }
    else if(move == 3){
        mvaddstr(starty + INIT_ROW + 2, startx + INIT_COL / 4, "AUTOMOVE: DOWN");
        refresh();
    }
    //returneaza directia muatrii
    return move;
}

//afiseaza ecranul de winner
void winner_window(WINDOW *win, char *winner, int nrows, int ncols, int score, int score_max){
    delete_win(win);
    nocbreak();
    cbreak();
    refresh();
    mvaddstr(nrows / 2, ncols / 2 - strlen(winner) / 2 + 1, winner);
    char scr[10], scrm[10];
    sprintf(scr, "%d", score);
    sprintf(scrm, "%d", score_max);
    mvaddstr(nrows / 2 + 2, (ncols - 7 / 2) / 2 - 6, "Score: ");
    mvaddstr(nrows / 2 + 2, (ncols - 7 / 2) / 2 + 1, scr);
    mvaddstr(nrows / 2 + 2, (ncols - 7 / 2) / 2 + 8, "Highscore: ");
    mvaddstr(nrows / 2 + 2, (ncols - 7 / 2) / 2 + 20, scrm);
    mvaddstr(nrows / 2 + 4, (ncols - 31 / 2) / 2 - 7, "Press any key to go back to main menu");
    getch();
    clear();
    refresh();
}

//afiseaza ecranul de loser
void loser_window(WINDOW *win, char *lose, int nrows, int ncols, int score, int score_max){
    nocbreak();
    cbreak();
    delete_win(win);
    refresh();
    mvaddstr(nrows / 2, (ncols - (strlen(lose) / 2)) / 2, lose);
    char scr[10], scrm[10];
    sprintf(scr, "%d", score);
    sprintf(scrm, "%d", score_max);
    mvaddstr(nrows / 2 + 2, (ncols - 7 / 2) / 2 - 6, "Score: ");
    mvaddstr(nrows / 2 + 2, (ncols - 7 / 2) / 2 + 1, scr);
    mvaddstr(nrows / 2 + 2, (ncols - 7 / 2) / 2 + 8, "Highscore: ");
    mvaddstr(nrows / 2 + 2, (ncols - 7 / 2) / 2 + 20, scrm);
    mvaddstr(nrows / 2 + 4, (ncols - 31 / 2) / 2 - 7, "Press any key to go back to main menu");
    getch();
    clear();
    refresh();
}

int main(void){
	int board[RSIZE][RSIZE], score = 0, score_max = 0;
	int nrows, ncols;
    char lose[10] = "GAME OVER";
    char winner[10] = "YOU WON!!";
	char *choices[] = {"New Game", "Resume", "Quit"};
    int nch = sizeof(choices) / sizeof(char *);
    WINDOW *menu;
    int key, move;
    int highlight = 1;
    int choice = 0;
    int startx = 0, starty = 0;
    int game = 0, enter = 0, castig = 0, pierdut = 0;
    int valid = 0;

    srand(time(NULL));
	/* Se inițializează ecranul; initscr se va folosi în mod obligatoriu */
	initscr();
	/* Se va reține în nrows și ncols numărul maxim de linii și coloane */
	getmaxyx(stdscr, nrows, ncols);
	/* Se șterge ecranul */
	clear();
	/* Se inhibă afișarea caracterelor introduse de la tastatură */
	noecho();
	/* Caracterele introduse sunt citite imediat - fără 'buffering' */
	cbreak();
	/* Se ascunde cursorul */	
	curs_set(0);
    //pornesc culorile
    start_color();
    initcolor();
    //colorez fundalul in negru
    bkgd(COLOR_BLACK);
    refresh();
    //obtin valorile de unde incepe desenarea meniului si a tablei
    startx = (ncols - INIT_COL) / 2;
    starty = (nrows - INIT_ROW) / 2;
    //creez un window pentru meniu
    menu = newwin(INIT_ROW, INIT_COL, starty, startx);
    //pornesc inputul de la tastatura pentru menu
    keypad(menu, TRUE);
    //pornesc inputul de la tastatura pentru ecranul standard
    keypad(stdscr, TRUE);
    refresh();
    print_menu(menu, highlight, choices, nch);
    //incepe programu
	while (FOREVER) {
		enter = 0;
        key = wgetch(menu);
        switch(key){
            case KEY_UP:
                if(highlight == 1)  
                    highlight = nch;
                else
                    highlight--;
                break;
            case KEY_DOWN:
                if(highlight == nch)
                    highlight = 1;
                else
                    highlight++;
                break;
            case 10:
                enter = 1;
                choice = highlight;
                break;
            default:
                refresh();
                break;
        }
		print_menu(menu, highlight, choices, nch);
        //obtiunea de new game
		if(choice == 1  && enter == 1){
            //sterg meniul pentru a creea tabla
            delete_win(menu);
            game = 1;
            score = 0;
            //creez tabla
            init_board(board);
            WINDOW *win = newwin(INIT_ROW + 1,  INIT_COL + 1, starty, startx);
            refresh();
            print_board(win);
            print_info(startx, starty);
            update_board(board, win, score, startx, starty, score_max);
            refresh();
            //incepe jocul
            while(game){
                print_time(startx, starty);
                refresh();
                valid = 0;
                //se decide mutarea automata
                move = automove(board, &score, &valid, startx, starty);
                //se trece pe modul halfdelay
                halfdelay(100);
                key = getch();
                if(key == ERR){
                    //miscarile automate
                    switch(move){
                        case 0:
                            move_left(board, &score, &valid);
                            break;
                        case 1:
                            move_right(board, &score, &valid);
                            break;
                        case 2:
                            move_up(board, &score, &valid);
                            break;
                        case 3:
                            move_down(board, &score, &valid);
                            break;
                        default:
                            break;
                    }
                    move(starty + INIT_ROW + 2, startx + INIT_COL / 4 - 1);
                    clrtobot();
                    refresh();
                }
                else if(key == 81 || key == 113){
                    //intoarcerea la meniu la apasarea tastei q / Q
                    delete_win(win);
                    break;
                }
                else{
                    //jocul cu input uman
                    play_game(board, win, &score, &valid, key);
                    move(starty + INIT_ROW + 2, startx + INIT_COL / 4 - 1);
                    clrtobot();
                    refresh();
                }
                //daca a avut loc mutarea se aduga valoare de 2 sau 4     
                if(valid)
                    add_tile(board);
                update_board(board, win, score, startx, starty, score_max);
                refresh();
                //verificam conditiile de terminare a jocului
                castig = check_win(board);
                pierdut = check_lose(board);
                if(castig == 1){
                    game = 0;
                    //se verifica daca e highscore
                    if(score > score_max)
                        score_max = score;
                    winner_window(win, winner, nrows, ncols, score, score_max);
                }
                if(pierdut == 1){
                    game = 0;
                    if(score > score_max)
                        score_max = score;
                    loser_window(win, lose, nrows, ncols, score, score_max);
                }
                //iesirea dub modul halfdelay
                nocbreak();
                cbreak();
            }      
            //creerea meniului dupa iesirea din joc
            menu = newwin(INIT_ROW, INIT_COL, starty, startx);
            keypad(menu, TRUE);
            wrefresh(menu);
        }
        else if(choice == 2 && enter == 1){
            delete_win(menu);
            if(game == 0){
                //in caz ca se selecteaza resum inainte de inceperea unui nou joc
                while(FOREVER){
                    mvaddstr(nrows / 2, (ncols - 21 / 2) / 2, "Start a new game first");
                    mvaddstr(nrows / 2 + 2, (ncols - 31 / 2) / 2 - 2, "Press q to go back to main menu");
                    refresh();
                    char c;
                    if(tolower(c = getch()) == 'q')
                        break;
                }
                clear();
                refresh();
            }
            else if(game){
                WINDOW *win = newwin(INIT_ROW + 1,  INIT_COL + 1, starty, startx);
                refresh();
                print_board(win);
                print_info(startx, starty);
                update_board(board, win, score, startx, starty, score_max);
                refresh();
                while(game){
                    print_time(startx, starty);
                    refresh();
                    valid = 0;
                    move = automove(board, &score, &valid, startx, starty);
                    halfdelay(100);
                    key = getch();
                    if(key == ERR){
                        switch(move){
                            case 0:
                                move_left(board, &score, &valid);
                                break;
                            case 1:
                                move_right(board, &score, &valid);
                                break;
                            case 2:
                                move_up(board, &score, &valid);
                                break;
                            case 3:
                                move_down(board, &score, &valid);
                                break;
                            default:
                                break;
                        }
                        move(starty + INIT_ROW + 2, startx + INIT_COL / 4 - 1);
                        clrtobot();
                        refresh();
                    }
                    else if(key == 81 || key == 113){
                        delete_win(win);
                        break;
                    }
                    else{
                        play_game(board, win, &score, &valid, key);
                        move(starty + INIT_ROW + 2, startx + INIT_COL / 4 - 1);
                        clrtobot();
                        refresh();
                    }      
                    if(valid)
                        add_tile(board);
                    update_board(board, win, score, startx, starty, score_max);
                    refresh();
                    castig = check_win(board);
                    pierdut = check_lose(board);
                    if(castig == 1){
                        game = 0;
                        if(score > score_max)
                            score_max = score;
                        winner_window(win, winner, nrows, ncols, score, score_max);
                    }
                    if(pierdut == 1){
                        game = 0;
                        if(score > score_max)
                            score_max = score;
                        loser_window(win, lose, nrows, ncols, score, score_max);
                    }
                    nocbreak();
                    cbreak();  
                }
            }
            menu = newwin(INIT_ROW, INIT_COL, starty, startx);
            keypad(menu, TRUE);
            wrefresh(menu);     
        }
        else if(choice == 3 && enter == 1){
            //optiunea de quit care inchide programul
            break;
        } 
        nocbreak();
        cbreak();
        print_menu(menu, highlight, choices, nch);    
    }	
	clrtoeol();
	refresh();
	/* Se închide fereastra ncurses */	
	endwin();
	return 0;
}
