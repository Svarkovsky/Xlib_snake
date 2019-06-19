/*
    # Author: Ivan Svarkovsky (ivansvarkovsky@gmail.com)
    # Date: 14/06/2019
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>   // srand(clock());
#include <unistd.h> // usleep
#include <math.h>
#include "gfx.h"
#include "m_deq.h" // cc snake.c gfx.c m_deq.c -o snake -I/usr/X11R6/include -L/usr/X11R6/lib -lX11 -lm
//
#define xsize 600 // X — горизонталь, Y — вертикаль.
#define ysize 600
#define Xsize xsize / 2        // условный центр
#define Ysize ysize / 2        // 300
#define A ysize / 60           // 10
#define B ysize - (ysize / 60) // 590
#define D ysize / 60           // 10
#define C ysize - (ysize / 60) // 590
#define N xsize / 10           // 60
// Dec and Hex
#define UP 65362             // ff52
#define DOWN 65364           // ff54
#define LEFT 65361           // ff51
#define RIGHT 65363          // ff53
#define ESCAPE 65307         // ff1b
#define q 113                // 71
#define g 103                // 67
#define snake_color 16711680 // RED (Red = 255 Green =   0 Blue =   0 -- RGB int = 16711680)
#define wall_color 255       // BLUE (Red =   0 Green =   0 Blue = 255 -- RGB int = 255)
#define food_color 16766720  // Red = 255 Green = 215 Blue =   0 -- RGB int = 16766720
//
void info(void)
{
#define couleur(param) printf("\033[%sm", param)
    couleur("32");
    printf("Ivan Svarkovsky | 2019 | Ukraine \n");
    couleur("0"); }
void m_fill(int n, int m, int r, int a[n][m][r], int step)
{   // последовательное заполнение массива a[n][0][0] и  a[n][0][1]
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            for (int k = 0; k < r; k++)
            {   a[i][j][k] = step * i + (step / 2); // попасть в середину десятки (A)
            } }
void m_print(int n, int m, int r, int a[][m][r]) // а старшую (первую) размерность можно опустить !!!
{   // вывод массива
    for (int i = 0; i < n; i++)
    {   for (int j = 0; j < m; j++)
        {   for (int k = 0; k < r; k++)
            {   printf("%6d -> [%d][%d][%d] ", a[i][j][k], i, j, k); }
            printf("\n"); } } }
void m_delmem(int n, int m, int r, int a[][m][r])
{   // отказ от использования, предварительно заполнили 0
    if (a != NULL)
    {   for (int i = 0; i < n; i++)
        {   for (int j = 0; j < m; j++)
            {   for (int k = 0; k < r; k++)
                {   a[i][j][k] = 0; } } }
        free(a);
        a = NULL; } }
void draw_borders(int n) // очертим границы
{   //  gfx_point(Xsize, Ysize);              // Нарисовать одну точку в центре
    gfx_point(A - n, D - n);              // верхняя левая
    gfx_point(B + n, D - n);              // верхняя правая
    gfx_point(A - n, C + n);              // нижняя левая
    gfx_point(B + n, C + n);              // нижняя правая
    gfx_line(A - n, D - n, B + n, D - n); // горизонтальная верхняя
    gfx_line(A - n, C + n, B + n, C + n); // горизонтальная нижняя
    gfx_line(A - n, D - n, A - n, C + n); // вертикальная левая
    gfx_line(B + n, C + n, B + n, D - n); // вертикальная правая
}
void diagonal()
{   gfx_line(B, C, A, D); // диагональ
    gfx_line(A, C, B, D); // диагональ
}
void cell_division(int n) // деление на клетки
{   for (int i = 0; i <= C; i = (i + n)) //
    {   gfx_line(A + i, D, A + i, C); // вертикальная левая
        gfx_line(A, D + i, B, D + i); // горизонтальная верхняя
    } }
void m_filling(int x, int y, int step) // заливка одной ячейки
{   step = step - 4;
    x = x - (A / 2) + 2; //
    y = y - (A / 2) + 2;
    int t_A = x;
    int t_D = y; //
    for (;;)
    {   // printf("%d t_A %d t_D \n", t_A, t_D);
        gfx_point(t_A, t_D); // Нарисовать одну точку в (х, у)
        if (t_A < x + step)  // выводим точки пока не достигнем
        {   //  определенного края
            ++t_A;           // ход вправо
        }
        else if (t_D < y + step) //
        {   t_A = x;
            ++t_D; // ход вниз
        }
        else
        {   break; } } }
void m_pampering_fill(int n, int m, int r, int a[][m][r]) //
{   // баловство, закрашиваем ячейки
    int i = 2;
    for (; i <= n - 3 && r <= n - 3; i++)
    {   m_filling(a[r][m - 1][m], a[i][m - 1][m], A);
        if (i == n - 3)
        {   r++;
            i = 1; } } }
void wall(int n, int m, int r, int a[][m][r], int step)
{   int i, j, k;
    j = 1;
    k = 0;
    for (i = 0; i < n - 2; i++)
    {   m_filling(a[j][k][k], a[i][j][k], step);
        m_filling(a[i][j][k], a[j][k][k], step); }
    j = N - 2;
    k = 0;
    for (i = 1; i < n - 1; i++)
    {   m_filling(a[j][k][k], a[i][k][k], step);
        m_filling(a[i][k][k], a[j][k][k], step); } }
void random_wall(int n, int m, int r, int a[][m][r], int step, int quantity)
{   //    srand(time(NULL)); // используем текущее время, как начальное значение для генератора случайных чисел
    srand(clock()); // clock() возвращает время, пройденное с момента запуска программы
    for (int r = 0; r < n - quantity; r++)
    {   int i = 3 + rand() % (A * 5);
        int j = 3 + rand() % (A * 5); // начальное значение + rand() % конечное значение
        m_filling(a[i][m][m], a[j][m][m], step); } }
int random_position(int n, int m, int r, int a[][m][r])
{   //    srand(time(NULL)); // используем текущее время, как начальное значение для генератора случайных чисел
    srand(clock()); // clock() возвращает время, пройденное с момента запуска программы
    return a[3 + rand() % (A * 5)][m][r]; }
void splash(int n, int m, int r, int a[][m][r])
{   gfx_color(30, 30, 30); // Grey   Red =  30 Green =  30 Blue =  30 -- RGB int = 1973790
    cell_division(A);
    gfx_color(0, 0, 255); // Blue     // gfx_color(255, 0, 0); // Red
    for (int i = 0; i > -9; --i)
    {   draw_borders(i); }
    gfx_color(255, 255, 255);
    gfx_rectangle(A * 3, A * 3, B - N + A, N * 2 + A);
    char *creator = "Ivan Svarkovsky | 2019 | Ukraine                            ivansvarkovsky@gmail.com";
    gfx_string(A * 4, N, creator);
    char *target = "The objective of the snake is to eat the target fruit, which makes it grow in length.";
    gfx_string(A * 5, N + (A * 4), target);
    char *control = "Use arrows to control the snake";
    gfx_string(A * 5, N + (A * 6), control);
    char *start_game = "Press any key to start the game";
    gfx_string(A * 5, N + (A * 8), start_game);
    gfx_rectangle(A * 3, B - (N + A), B - N + A, N - A);
    char *very_cruel_game = "This is a very cruel game. Not recommended for children and impressionable people.";
    gfx_string(A * 5, B - A * 4, very_cruel_game);
    gfx_color(255, 0, 0); // Red
    // S start
    m_filling(a[4][m][r], a[20][m][r], A);
    m_filling(a[5][m][r], a[20][m][r], A);
    m_filling(a[6][m][r], a[20][m][r], A);
    m_filling(a[7][m][r], a[20][m][r], A);
    m_filling(a[8][m][r], a[20][m][r], A);
    m_filling(a[4][m][r], a[21][m][r], A);
    m_filling(a[4][m][r], a[22][m][r], A);
    m_filling(a[4][m][r], a[23][m][r], A);
    m_filling(a[4][m][r], a[24][m][r], A);
    m_filling(a[4][m][r], a[25][m][r], A);
    m_filling(a[5][m][r], a[25][m][r], A);
    m_filling(a[6][m][r], a[25][m][r], A);
    m_filling(a[7][m][r], a[25][m][r], A);
    m_filling(a[8][m][r], a[25][m][r], A);
    m_filling(a[9][m][r], a[25][m][r], A);
    m_filling(a[9][m][r], a[26][m][r], A);
    m_filling(a[9][m][r], a[27][m][r], A);
    m_filling(a[9][m][r], a[28][m][r], A);
    m_filling(a[9][m][r], a[29][m][r], A);
    m_filling(a[5][m][r], a[30][m][r], A);
    m_filling(a[6][m][r], a[30][m][r], A);
    m_filling(a[7][m][r], a[30][m][r], A);
    m_filling(a[8][m][r], a[30][m][r], A);
    m_filling(a[9][m][r], a[30][m][r], A);
    // S end
    // N start
    m_filling(a[12][m][r], a[20][m][r], A);
    m_filling(a[13][m][r], a[20][m][r], A);
    m_filling(a[14][m][r], a[20][m][r], A);
    m_filling(a[15][m][r], a[20][m][r], A);
    m_filling(a[16][m][r], a[20][m][r], A);
    m_filling(a[17][m][r], a[20][m][r], A);
    m_filling(a[18][m][r], a[20][m][r], A);
    m_filling(a[19][m][r], a[20][m][r], A);
    m_filling(a[20][m][r], a[20][m][r], A);
    m_filling(a[21][m][r], a[20][m][r], A);
    m_filling(a[22][m][r], a[20][m][r], A);
    m_filling(a[23][m][r], a[20][m][r], A);
    m_filling(a[12][m][r], a[21][m][r], A);
    m_filling(a[12][m][r], a[22][m][r], A);
    m_filling(a[12][m][r], a[23][m][r], A);
    m_filling(a[12][m][r], a[24][m][r], A);
    m_filling(a[12][m][r], a[25][m][r], A);
    m_filling(a[12][m][r], a[26][m][r], A);
    m_filling(a[12][m][r], a[27][m][r], A);
    m_filling(a[12][m][r], a[28][m][r], A);
    m_filling(a[12][m][r], a[29][m][r], A);
    m_filling(a[12][m][r], a[30][m][r], A);
    m_filling(a[12][m][r], a[31][m][r], A);
    m_filling(a[12][m][r], a[32][m][r], A);
    m_filling(a[12][m][r], a[33][m][r], A);
    m_filling(a[12][m][r], a[34][m][r], A);
    m_filling(a[12][m][r], a[35][m][r], A);
    m_filling(a[12][m][r], a[36][m][r], A);
    m_filling(a[12][m][r], a[37][m][r], A);
    m_filling(a[12][m][r], a[38][m][r], A);
    m_filling(a[12][m][r], a[39][m][r], A);
    m_filling(a[23][m][r], a[21][m][r], A);
    m_filling(a[23][m][r], a[22][m][r], A);
    m_filling(a[23][m][r], a[23][m][r], A);
    m_filling(a[23][m][r], a[24][m][r], A);
    m_filling(a[23][m][r], a[25][m][r], A);
    m_filling(a[23][m][r], a[26][m][r], A);
    m_filling(a[23][m][r], a[27][m][r], A);
    m_filling(a[23][m][r], a[28][m][r], A);
    m_filling(a[23][m][r], a[29][m][r], A);
    m_filling(a[23][m][r], a[30][m][r], A);
    m_filling(a[23][m][r], a[31][m][r], A);
    m_filling(a[23][m][r], a[32][m][r], A);
    m_filling(a[23][m][r], a[33][m][r], A);
    m_filling(a[23][m][r], a[34][m][r], A);
    m_filling(a[23][m][r], a[35][m][r], A);
    m_filling(a[23][m][r], a[36][m][r], A);
    m_filling(a[23][m][r], a[37][m][r], A);
    m_filling(a[23][m][r], a[38][m][r], A);
    m_filling(a[23][m][r], a[39][m][r], A);
    // N end
    // A start
    m_filling(a[26][m][r], a[20][m][r], A);
    m_filling(a[27][m][r], a[20][m][r], A);
    m_filling(a[28][m][r], a[20][m][r], A);
    m_filling(a[29][m][r], a[20][m][r], A);
    m_filling(a[30][m][r], a[20][m][r], A);
    m_filling(a[31][m][r], a[20][m][r], A);
    m_filling(a[31][m][r], a[21][m][r], A);
    m_filling(a[31][m][r], a[22][m][r], A);
    m_filling(a[31][m][r], a[23][m][r], A);
    m_filling(a[31][m][r], a[24][m][r], A);
    m_filling(a[31][m][r], a[25][m][r], A);
    m_filling(a[31][m][r], a[26][m][r], A);
    m_filling(a[31][m][r], a[27][m][r], A);
    m_filling(a[31][m][r], a[28][m][r], A);
    m_filling(a[31][m][r], a[29][m][r], A);
    m_filling(a[26][m][r], a[21][m][r], A);
    m_filling(a[26][m][r], a[22][m][r], A);
    m_filling(a[26][m][r], a[23][m][r], A);
    m_filling(a[26][m][r], a[24][m][r], A);
    m_filling(a[26][m][r], a[25][m][r], A);
    m_filling(a[26][m][r], a[26][m][r], A);
    m_filling(a[26][m][r], a[27][m][r], A);
    m_filling(a[26][m][r], a[28][m][r], A);
    m_filling(a[26][m][r], a[29][m][r], A);
    m_filling(a[27][m][r], a[25][m][r], A);
    m_filling(a[28][m][r], a[25][m][r], A);
    m_filling(a[29][m][r], a[25][m][r], A);
    m_filling(a[30][m][r], a[25][m][r], A);
    // A end
    // K start
    m_filling(a[34][m][r], a[20][m][r], A);
    m_filling(a[34][m][r], a[21][m][r], A);
    m_filling(a[34][m][r], a[22][m][r], A);
    m_filling(a[34][m][r], a[23][m][r], A);
    m_filling(a[34][m][r], a[24][m][r], A);
    m_filling(a[34][m][r], a[25][m][r], A);
    m_filling(a[34][m][r], a[26][m][r], A);
    m_filling(a[34][m][r], a[27][m][r], A);
    m_filling(a[34][m][r], a[28][m][r], A);
    m_filling(a[34][m][r], a[29][m][r], A);
    m_filling(a[35][m][r], a[24][m][r], A);
    m_filling(a[36][m][r], a[24][m][r], A);
    m_filling(a[37][m][r], a[24][m][r], A);
    m_filling(a[38][m][r], a[24][m][r], A);
    m_filling(a[39][m][r], a[24][m][r], A);
    m_filling(a[35][m][r], a[26][m][r], A);
    m_filling(a[36][m][r], a[26][m][r], A);
    m_filling(a[37][m][r], a[26][m][r], A);
    m_filling(a[38][m][r], a[26][m][r], A);
    m_filling(a[39][m][r], a[26][m][r], A);
    m_filling(a[39][m][r], a[27][m][r], A);
    m_filling(a[39][m][r], a[28][m][r], A);
    m_filling(a[39][m][r], a[29][m][r], A);
    m_filling(a[39][m][r], a[23][m][r], A);
    m_filling(a[39][m][r], a[22][m][r], A);
    m_filling(a[39][m][r], a[21][m][r], A);
    // K end
    // E start
    m_filling(a[42][m][r], a[20][m][r], A);
    m_filling(a[43][m][r], a[20][m][r], A);
    m_filling(a[44][m][r], a[20][m][r], A);
    m_filling(a[45][m][r], a[20][m][r], A);
    m_filling(a[46][m][r], a[20][m][r], A);
    m_filling(a[47][m][r], a[20][m][r], A);
    m_filling(a[48][m][r], a[20][m][r], A);
    m_filling(a[49][m][r], a[20][m][r], A);
    m_filling(a[50][m][r], a[20][m][r], A);
    m_filling(a[42][m][r], a[21][m][r], A);
    m_filling(a[42][m][r], a[22][m][r], A);
    m_filling(a[42][m][r], a[23][m][r], A);
    m_filling(a[42][m][r], a[24][m][r], A);
    m_filling(a[42][m][r], a[25][m][r], A);
    m_filling(a[42][m][r], a[26][m][r], A);
    m_filling(a[42][m][r], a[27][m][r], A);
    m_filling(a[42][m][r], a[28][m][r], A);
    m_filling(a[42][m][r], a[29][m][r], A);
    m_filling(a[42][m][r], a[30][m][r], A);
    m_filling(a[43][m][r], a[25][m][r], A);
    m_filling(a[44][m][r], a[25][m][r], A);
    m_filling(a[45][m][r], a[25][m][r], A);
    m_filling(a[46][m][r], a[25][m][r], A);
    m_filling(a[43][m][r], a[30][m][r], A);
    m_filling(a[44][m][r], a[30][m][r], A);
    m_filling(a[45][m][r], a[30][m][r], A);
    m_filling(a[46][m][r], a[30][m][r], A);
    m_filling(a[47][m][r], a[30][m][r], A);
    m_filling(a[48][m][r], a[30][m][r], A);
    m_filling(a[49][m][r], a[30][m][r], A);
    m_filling(a[50][m][r], a[30][m][r], A);
    // E end
    //
    while (gfx_m_xreadkeys() == -1)
    {   usleep(500000);
        if (rand() % 2)
        {   gfx_color(255, 215, 0); // золотой
        }
        else
        {   gfx_color(124, 252, 0); // Зелёный газон
        }
        m_filling(a[9][m][r], a[20][m][r], A);
        m_filling(a[4][m][r], a[30][m][r], A);
        m_filling(a[12][m][r], a[40][m][r], A);
        m_filling(a[23][m][r], a[40][m][r], A);
        m_filling(a[31][m][r], a[30][m][r], A);
        m_filling(a[26][m][r], a[30][m][r], A);
        m_filling(a[34][m][r], a[30][m][r], A);
        m_filling(a[39][m][r], a[30][m][r], A);
        m_filling(a[39][m][r], a[20][m][r], A);
        m_filling(a[51][m][r], a[20][m][r], A);
        m_filling(a[47][m][r], a[25][m][r], A);
        m_filling(a[51][m][r], a[30][m][r], A); } }
int game_over(int n, int m, int r, int a[][m][r], int score)
{   gfx_clear();
    gfx_color(0, 0, 255); // Blue
    for (int i = 0; i > -9; --i)
    {   draw_borders(i); }
    gfx_color(255, 255, 255);
    char *exit_game = "Press any key except [ESCAPE] to play.";
    gfx_string(a[A + rand() % (A * 2)][m][r], a[A + rand() % (A * 2)][m][r], exit_game);
    gfx_color(255, 0, 0); // Red
    char *game_over = "Game over";
    gfx_string(a[A + rand() % (A * 3)][m][r], a[A + rand() % (A * 3)][m][r], game_over);
    char buffer[900];
    snprintf(buffer, sizeof(buffer), "%d", score - 3);
    gfx_string(N, N, buffer);
    gfx_color(255, 255, 255);
    char *s_score = "score";
    gfx_string(N - A, N + A * 2, s_score);
    while (gfx_m_xreadkeys() == -1)
    {   if (gfx_m_xreadkeys() == ESCAPE)
        {   return -100;
            break; } } }
void food_gen(int n, int m, int r, int a[][m][r])
{   // генерация еды или преград
    for (int i = 0 + rand() % 2; i < r; i++)
    {   gfx_color(255, 215, 0); // золотой
        m_filling(random_position(n, m, r, (int(*)[m][r])a), random_position(n, m, r, (int(*)[m][r])a), A);
        if (i == 1)
        {   if (rand() % 2)
            {   gfx_color(0, 0, 255); // Blue
                m_filling(random_position(n, m, r, (int(*)[m][r])a), random_position(n, m, r, (int(*)[m][r])a), A); } } } }
int dir_snake_move(int n, int m, int r, int a[][m][r]) //
{   int current_color; // current color
    int key;           // переменная для хранения кода нажатой клавиши
    int old_key;
    int S_size = 3;           // размер змея
    unsigned int ms = 100000; //  	%u
    int x = Xsize - 5;        // начальные координаты змея
    int y = random_position(n, m, r, (int(*)[m][r])a);
    food_gen(n, m, r, (int(*)[m][r])a); // генерация еды для затравки
    //
    deq *deq_x = create_deq(N * N / 4); // 60 * 15 = 900
    deq *deq_y = create_deq(N * N / 4);
    do
    {   key = gfx_m_xreadkeys();
        if (key == UP && old_key != DOWN)
        {   y -= A;
            current_color = GetPix(x, y);
            if (current_color != wall_color && current_color != snake_color)
            {   push_front(deq_x, x); // зашли данные X, Y
                push_front(deq_y, y); // занесли в начало
                old_key = key;
                if (current_color == food_color)
                {   S_size++;
                    food_gen(n, m, r, (int(*)[m][r])a); } }
            else
            {   break; } }
        if (key == DOWN && old_key != UP)
        {   y += A;
            current_color = GetPix(x, y);
            if (current_color != wall_color && current_color != snake_color)
            {   push_front(deq_x, x); // зашли данные X, Y
                push_front(deq_y, y); // занесли в начало
                old_key = key;
                if (current_color == food_color)
                {   S_size++;
                    food_gen(n, m, r, (int(*)[m][r])a); } }
            else
            {   break; } }
        if (key == LEFT && old_key != RIGHT)
        {   x -= A;
            current_color = GetPix(x, y);
            if (current_color != wall_color && current_color != snake_color)
            {   push_front(deq_x, x); // зашли данные X, Y
                push_front(deq_y, y); // занесли в начало
                old_key = key;
                if (current_color == food_color)
                {   S_size++;
                    food_gen(n, m, r, (int(*)[m][r])a); } }
            else
            {   break; } }
        if (key == RIGHT && old_key != LEFT)
        {   x += A;
            current_color = GetPix(x, y);
            if (current_color != wall_color && current_color != snake_color)
            {   push_front(deq_x, x); // зашли данные X, Y
                push_front(deq_y, y); // занесли в начало
                old_key = key;
                if (current_color == food_color)
                {   S_size++;
                    food_gen(n, m, r, (int(*)[m][r])a); } }
            else
            {   break; } }
        if (key == -1)
        {   if (old_key == UP)
            {   y -= A;
                current_color = GetPix(x, y);
                if (current_color != wall_color && current_color != snake_color)
                {   push_front(deq_x, x); // зашли данные X, Y
                    push_front(deq_y, y); // занесли в начало
                    if (current_color == food_color)
                    {   S_size++;
                        food_gen(n, m, r, (int(*)[m][r])a); } }
                else
                {   break; } }
            if (old_key == DOWN)
            {   y += A;
                current_color = GetPix(x, y);
                if (current_color != wall_color && current_color != snake_color)
                {   push_front(deq_x, x); // зашли данные X, Y
                    push_front(deq_y, y); // занесли в начало
                    if (current_color == food_color)
                    {   S_size++;
                        food_gen(n, m, r, (int(*)[m][r])a); } }
                else
                {   break; } }
            if (old_key == LEFT)
            {   x -= A;
                current_color = GetPix(x, y);
                if (current_color != wall_color && current_color != snake_color)
                {   push_front(deq_x, x); // зашли данные X, Y
                    push_front(deq_y, y); // занесли в начало
                    if (current_color == food_color)
                    {   S_size++;
                        food_gen(n, m, r, (int(*)[m][r])a); } }
                else
                {   break; } }
            if (old_key == RIGHT)
            {   x += A;
                current_color = GetPix(x, y);
                if (current_color != wall_color && current_color != snake_color)
                {   push_front(deq_x, x); // зашли данные X, Y
                    push_front(deq_y, y); // занесли в начало
                    if (current_color == food_color)
                    {   S_size++;
                        food_gen(n, m, r, (int(*)[m][r])a); } }
                else
                {   break; } } }
        if (!is_empty(deq_x)) // если есть x значит есть и y, две проверки излишни
        {   gfx_color(15, 15, 15);
            m_filling(peek_back(deq_x), peek_back(deq_y), A); // смотрим координаты в конце и рисуем
            if (in_count_deq(deq_x) >= S_size)                // если есть x значит есть и y
            {   gfx_color(255, 0, 0);                               // Red
                m_filling(peek_front(deq_x), peek_front(deq_y), A); // смотрим координаты в начале и рисуем
                pop_back(deq_x);                                    // удаляем ячейку в хвосте (конце)
                pop_back(deq_y); } }
        if (ms > 0)
        {   usleep(ms--); }         //   printf("%d key %u ms %d \n", key, ms, in_count_deq(deq_x));
    } while (1);
    dell_deq(deq_x);
    dell_deq(deq_y);
    return S_size; }
int main()
{   info();
    gfx_open(xsize, ysize, "Snake - Graphics Program"); // 
    int n = N;
    int m = 1;
    int r = 2;
    int nn = n * m * r;
    int *a = malloc(sizeof(*a) * nn);
    m_fill(n, m, r, (int(*)[m][r])a, A);
    // m_print(n, m, r, (int(*)[m][r])a);
    splash(n, m, r, (int(*)[m][r])a);
    int score;
    do
    {   gfx_clear();
        gfx_color(30, 30, 30); // Grey   Red =  30 Green =  30 Blue =  30 -- RGB int = 1973790
        cell_division(A);
        gfx_color(0, 0, 255); // Blue     // gfx_color(255, 0, 0); // Red
        for (int i = 0; i > -9; --i)
        {   draw_borders(i); }
        gfx_color(15, 15, 15); // Red =  15 Green =  15 Blue =  15 -- RGB int = 986895
        m_pampering_fill(n, m, r, (int(*)[m][r])a);
        score = dir_snake_move(n, m, r, (int(*)[m][r])a); // вернет количество очков
    } while (game_over(n, m, r, (int(*)[m][r])a, score) != -100);
    m_delmem(n, m, r, (int(*)[m][r])a);
    return 0; }

// UP x, y - A; DOWN x, y + A; LEFT x - A, y; RIGHT x + A, y;

//m_filling(A, D, C);
//gfx_color(150, 50, 200); // симпатичный цвет

//  gfx_color(15, 15, 15);
//  m_pampering_fill(n, m, r, (int(*)[m][r])a);
