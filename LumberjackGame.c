#include <stdio.h>
#include "raylib.h"
#include <stdlib.h>
#include <time.h>

#define TREE_HEIHGT 8
#define TREE_WIDTH 3
#define CELL_SIZE 125
#define BRANCH_TYPES 2

const char branch_chars[BRANCH_TYPES] = {'X', ' '};

char tree[TREE_HEIHGT][TREE_WIDTH];

Vector2 grid_origin;
Vector2 hp_bar_size;
Vector2 hp_origin;
int score = 0;
int best_score = 0;
Texture2D player_sprite, branch_sprite, tree_sprite;
bool gameover = false;
float time_limit = 5.0f;
float timer;

// funkcja losujaca galaz
char random_branch() {
    return branch_chars[rand() % BRANCH_TYPES];
}

// stworzenie drzewa 
void init_tree() {
    for (int i = 0; i < TREE_HEIHGT; i++){
        tree[i][0] = random_branch();               // losowe galezie
        if (tree[i][0] == 'X') tree[i][2] = ' ';    // jezeli w ktoryms z wierszy jest galaz to po drugiej stronie nie ma galezi
        if (tree[i][1] == 'X') tree[i][0] = ' ';    //
    }

    tree[TREE_HEIHGT-1][0] = 'P';                   // inicjalizacja gracza
    tree[TREE_HEIHGT-1][2] = ' ';

    int grid_width = TREE_WIDTH * CELL_SIZE;      // szerokosc calego grid drzewa
    int grid_height = TREE_HEIHGT * CELL_SIZE;   // wysokosc calego grid drzewa

    grid_origin = (Vector2){                        // 
        (GetScreenWidth() - grid_width) / 2,        // wysrodkowanie drzewa
        (GetScreenHeight() - grid_height) / 2       // 
    };
}

bool isSplittable(int dir){                         // sprawdzenie czy drzewo mozna przeciac
    if (dir == 0 && tree[TREE_HEIHGT-2][0] == 'X') return false;
    if (dir == 1 && tree[TREE_HEIHGT-2][2] == 'X') return false;
    else return true;
}

void move_tree(char tree_to_move[TREE_HEIHGT][TREE_WIDTH]) {    // funkcja 'przesuwajaca' wszystkie galezie drzewa do dolu
    for (int i = TREE_HEIHGT - 1; i > 0; i--){                  // przesuniecie wszystkiego w dol
        int tmp = tree_to_move[i-1][0];
        tree_to_move[i][0] = tmp;
        tmp = tree_to_move[i-1][2];
        tree_to_move[i][2] = tmp;
    }
    tree[0][0] = random_branch();                       // losowe galezie na samej gorze
    tree[0][2] = random_branch();
    if (tree[0][2] == 'X') tree[0][0] = ' ';
    if (tree[0][0] == 'X') tree[0][2] = ' ';
}

void reset() {                                      // funkcja resetujaca drzewo i sprawdzajaca best score
    if (score > best_score) best_score = score;
    score = 0;
    init_tree();
}

void reset_timer(){
    timer = time_limit;
}

int main(void) {
    const int screen_width = 1000;
    const int screen_height = 1000;

    hp_bar_size = (Vector2){screen_width / 2, screen_height / 15};
    hp_origin = (Vector2){(screen_width - hp_bar_size.x) / 2, 20};
    
    InitWindow(screen_width, screen_height, "Lumberjack Game");
    SetTargetFPS(60);
    srand(time(NULL));

    player_sprite = LoadTexture("resources/Player_sprite.png");
    branch_sprite = LoadTexture("resources/Branch_sprite.png"); 
    tree_sprite = LoadTexture("resources/Tree_sprite.png");

    int gameoverWidth = MeasureText("GAME OVER", 64);

    reset_timer();

    init_tree();

    while (!WindowShouldClose()) {
        if(!gameover){
            if (score >= 100 && score < 200) time_limit = 4.0f;
            if (score >= 200) time_limit = 3.0f;
            timer -= GetFrameTime();
            if (timer <= 0.0f) gameover = true;
            if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) {
                int dir = 0;
                if (!isSplittable(dir)) gameover = true;
                else {
                    move_tree(tree);
                    tree[TREE_HEIHGT-1][0] = 'P';
                    timer += 0.2f;
                    if (timer > time_limit) timer = time_limit;
                    score += 1;
                }
            }
            if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) {
                int dir = 1;
                if (!isSplittable(dir)) gameover = true;
                else {
                    move_tree(tree);
                    tree[TREE_HEIHGT-1][2] = 'P';
                    timer += 0.2f;
                    if (timer > time_limit) timer = time_limit;
                    score += 1;
                } 
            }
            BeginDrawing();
            ClearBackground(BLACK);
            Rectangle hp_bar_rect = (Rectangle){hp_origin.x, hp_origin.y, hp_bar_size.x, hp_bar_size.y};
            Rectangle timer_rect = (Rectangle){hp_origin.x, hp_origin.y, hp_bar_size.x * (timer/time_limit), hp_bar_size.y};
            for (int i = 0; i < TREE_HEIHGT; i++) {
                for (int j = 0; j < TREE_WIDTH; j++) {
                    Rectangle rect = (Rectangle){
                        grid_origin.x + (j * CELL_SIZE),
                        grid_origin.y + (i * CELL_SIZE),
                        CELL_SIZE, CELL_SIZE};

                    Rectangle source_rect = (Rectangle){0,0,16, 16};
                    Rectangle source_rect_flipped = (Rectangle){0,0,-16, 16};
                    
                    
                    if (j == 1) DrawTexturePro(tree_sprite, source_rect, rect, (Vector2){0,0},0,WHITE);

                    if (tree[i][j] == 'X' && j == 0) {
                        // DrawRectanglePro(rect, (Vector2){0,0}, 0, RED);
                        DrawTexturePro(branch_sprite, source_rect, rect, (Vector2){0,0},0,WHITE);
                    } else if(tree[i][j] == 'X' && j == 2) {
                        DrawTexturePro(branch_sprite, source_rect_flipped, rect, (Vector2){0,0},0,WHITE);
                    }

                    if (tree[i][j] == 'P' && j == 0) {
                        //DrawRectanglePro(rect, (Vector2){0,0}, 0, WHITE);
                        DrawTexturePro(player_sprite, source_rect_flipped, rect, (Vector2){0,0},0,WHITE);
                    } else if(tree[i][j] == 'P' && j == 2) {
                        DrawTexturePro(player_sprite, source_rect, rect, (Vector2){0,0},0,WHITE);
                    }
                    // DrawRectangleLinesEx(rect, 1, DARKGRAY);
                }
            }

            DrawText(TextFormat("SCORE: %d", score), 20, 20, 24, YELLOW);
            DrawText(TextFormat("BEST SCORE: %d", best_score), screen_width-220, 20, 24, YELLOW);
            DrawRectangleRec(timer_rect, RED);
            DrawRectangleLinesEx(hp_bar_rect, 2, RED);
            EndDrawing();
        }
        else {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText(TextFormat("SCORE: %d", score), 20, 20, 24, YELLOW);
            DrawText(TextFormat("BEST SCORE: %d", best_score), screen_width-220, 20, 24, YELLOW);
            DrawText(TextFormat("GAME OVER"), (screen_width-gameoverWidth)/2, (screen_height-40)/2, 64, YELLOW);
            DrawText(TextFormat("Press [R] to restart."), (screen_width-gameoverWidth+40)/2, (screen_height+200)/2, 32, YELLOW);
            EndDrawing();
            if (IsKeyReleased(KEY_R)) {
                gameover = false; 
                reset_timer();
                reset();
            }
        }
    }
    CloseWindow();
    return 0;
}