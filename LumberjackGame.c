#include <stdio.h>
#include "raylib.h"
#include <stdlib.h>
#include <time.h>

#define TREE_HEIHGT 9
#define TREE_WIDTH 2
#define BRANCH_SIZE 42
#define BRANCH_TYPES 2

const char branch_chars[BRANCH_TYPES] = {'#', ' '};

char tree[TREE_HEIHGT][TREE_WIDTH];

Vector2 grid_origin;
int score = 0;
int best_score = 0;

// funkcja losujaca galaz
char random_branch() {
    return branch_chars[rand() % BRANCH_TYPES];
}

// stworzenie drzewa 
void init_tree() {
    for (int i = 0; i < TREE_HEIHGT; i++){
        tree[i][0] = random_branch();               // losowe galezie
        if (tree[i][0] == '#') tree[i][1] = ' ';    // jezeli w ktoryms z wierszy jest galaz to po drugiej stronie nie ma galezi
        if (tree[i][1] == '#') tree[i][0] = ' ';    //
    }

    tree[TREE_HEIHGT-1][0] = 'P';                   // inicjalizacja gracza
    tree[TREE_HEIHGT-1][1] = ' ';

    int grid_width = TREE_WIDTH * BRANCH_SIZE;      // szerokosc calego grid drzewa
    int grid_height = TREE_HEIHGT * BRANCH_SIZE;    // wysokosc calego grid drzewa

    grid_origin = (Vector2){                        // 
        (GetScreenWidth() - grid_width) / 2,        // wysrodkowanie drzewa
        (GetScreenHeight() - grid_height) / 2       // 
    };
}

bool isSplittable(int dir){                         // sprawdzenie czy drzewo mozna przeciac
    if (dir == 0 && tree[TREE_HEIHGT-2][0] == '#') return false;
    if (dir == 1 && tree[TREE_HEIHGT-2][1] == '#') return false;
    else return true;
}

void move_tree() {                                  // funkcja 'przesuwajaca' wszystkie galezie drzewa do dolu
    for (int i = TREE_HEIHGT - 1; i > 0; i--){          // przesuniecie wszystkiego w dol
        int tmp = tree[i-1][0];
        tree[i][0] = tmp;
        tmp = tree[i-1][1];
        tree[i][1] = tmp;
    }
    tree[0][0] = random_branch();                       // losowe galezie na samej gorze
    tree[0][1] = random_branch();
    if (tree[0][0] == '#') tree[0][1] = ' ';
    if (tree[0][1] == '#') tree[0][0] = ' ';
}

void reset() {                                      // funkcja resetujaca drzewo i sprawdzajaca best score
    if (score > best_score) best_score = score;
    score = 0;
    init_tree();
}

int main(void) {
    const int screen_width = 800;
    const int screen_height = 450;

    InitWindow(screen_width, screen_height, "Lumberjack Game");
    SetTargetFPS(60);
    srand(time(NULL));

    init_tree();

    while (!WindowShouldClose()) {
        if (IsKeyReleased(KEY_A)) {
            int dir = 0;
            if (!isSplittable(dir)) reset();
            else {
                move_tree();
                tree[TREE_HEIHGT-1][0] = 'P';
                score += 1;
            }
        }
        if (IsKeyReleased(KEY_D)) {
            int dir = 1;
            if (!isSplittable(dir)) reset();
            else {
                move_tree();
                tree[TREE_HEIHGT-1][1] = 'P';
                score += 1;
            } 
        }
        BeginDrawing();
        ClearBackground(BLACK);

        for (int i = 0; i < TREE_HEIHGT; i++) {
            for (int j = 0; j < TREE_WIDTH; j++) {
                Rectangle rect = {
                    grid_origin.x + (j * BRANCH_SIZE),
                    grid_origin.y + (i * BRANCH_SIZE),
                    BRANCH_SIZE,
                    BRANCH_SIZE
                };

                DrawRectangleLinesEx(rect, 1, DARKGRAY);

                DrawTextEx(
                    GetFontDefault(),
                    TextFormat("%c", tree[i][j]),
                    (Vector2) {rect.x + 12, rect.y + 8}, 20, 1, WHITE);
            }
        }

        DrawText(TextFormat("SCORE: %d", score), 20, 20, 24, YELLOW);
        DrawText(TextFormat("BEST SCORE: %d", best_score), screen_width-220, 20, 24, YELLOW);

        EndDrawing();

    }
    CloseWindow();
    return 0;
}