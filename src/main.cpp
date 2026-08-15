#include "raylib.h"
#include <vector>
#define BLOCK_SIZE 55
#define COLUMNS 16
#define ROWS 16

constexpr int screen_width = COLUMNS * BLOCK_SIZE;
constexpr int screen_height = ROWS * BLOCK_SIZE;
constexpr float blockGap = BLOCK_SIZE * 0.12f;

void resetSnake(std::vector<Vector2> &snakeBody)
{
    snakeBody.clear();
    snakeBody.push_back({3, 1});
    snakeBody.push_back({2, 1});
    snakeBody.push_back({1, 1});
}

Vector2 spawnFood(const std::vector<Vector2> &snakeBody)
{
    Vector2 food;
    bool isOnSnake;

    do
    {
        food = {(float)GetRandomValue(0, COLUMNS - 1), (float)GetRandomValue(0, ROWS - 1)};
        isOnSnake = false;

        for (int i = 0; i < snakeBody.size(); i++)
        {
            if (snakeBody[i].x == food.x && snakeBody[i].y == food.y)
            {
                isOnSnake = true;
                break;
            }
        }

    } while (isOnSnake);

    return food;
}

int main()
{
    InitWindow(screen_width, screen_height, "Snake");
    SetTargetFPS(60);

    float moveTimer = 0.0f;
    const float moveInterval = 0.16f; // Intervall für jede Bewegung

    int moveX = 1; // 1->Rechts  -1->Links
    int moveY = 0; // 1->Unten   -1->Oben

    int nextMoveX = 1;
    int nextMoveY = 0;

    std::vector<Vector2> snakeBody;

    snakeBody.push_back({3, 1});
    snakeBody.push_back({2, 1});
    snakeBody.push_back({1, 1});

    // Vergibt für das Essen eine zufällige position
    Vector2 food = spawnFood(snakeBody);

    int foodX, foodY;

    bool gameOver = false;
    bool gameFinished = false;

    unsigned int score = 0;
    unsigned int highscore = 0;

    while (!WindowShouldClose())
    {
        //-------------------------------------UPDATE---------------------------------------------
        if (!gameFinished && !gameOver)
        {
            if ((IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) && moveX == 0)
            {
                nextMoveX = 1;
                nextMoveY = 0;
            }
            else if ((IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) && moveX == 0)
            {
                nextMoveX = -1;
                nextMoveY = 0;
            }
            else if ((IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) && moveY == 0)
            {
                nextMoveY = 1;
                nextMoveX = 0;
            }
            else if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) && moveY == 0)
            {
                nextMoveY = -1;
                nextMoveX = 0;
            }

            moveTimer += GetFrameTime();

            if (moveTimer >= moveInterval)
            {
                // Erst hier gewünschte richtung übernehmen, damit man nicht durch schnelle tastendrücke 180 grad
                // wende machen kann.
                moveX = nextMoveX;
                moveY = nextMoveY;

                for (int i = snakeBody.size() - 1; i > 0; i--)
                {
                    snakeBody[i] = snakeBody[i - 1];
                }

                snakeBody[0].x += moveX;
                snakeBody[0].y += moveY;

                moveTimer = 0;

                // Wenn Snake Kopf auf Food bzw. wenn snake frisst
                if (snakeBody[0].x == food.x && snakeBody[0].y == food.y)
                {
                    food = spawnFood(snakeBody);
                    snakeBody.push_back(snakeBody.back());
                    score++;
                }

                foodX = food.x * BLOCK_SIZE;
                foodY = food.y * BLOCK_SIZE;

                // Wenn snake das fenster verlässt, dann game over
                if (snakeBody[0].x > COLUMNS - 1 || snakeBody[0].x < 0 || snakeBody[0].y > ROWS - 1 || snakeBody[0].y < 0)
                {
                    gameOver = true;
                }

                for (int i = 1; i < snakeBody.size(); i++)
                {
                    if (snakeBody[0].x == snakeBody[i].x && snakeBody[0].y == snakeBody[i].y)
                    {
                        gameOver = true;
                        break;
                    }
                }
            }

            if (snakeBody.size() >= ROWS * COLUMNS)
            {
                gameFinished = true;
            }
        }

        if (gameOver)
        {
            if (score > highscore)
                highscore = score;

            if (IsKeyDown(KEY_ENTER))
            {
                gameOver = false;
                resetSnake(snakeBody);
                nextMoveX = 1;
                nextMoveY = 0;
                score = 0;
            }
        }

        else if (gameFinished)
        {
            if (score > highscore)
                highscore = score;

            if (IsKeyDown(KEY_ENTER))
            {
                gameFinished = false;
                resetSnake(snakeBody);
                nextMoveX = 1;
                nextMoveY = 0;
                score = 0;
            }
        }

        //-------------------------------------DRAW---------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);

        const char *highscoreText = "HIGHSCORE";
        const char *hscoreText = TextFormat("%d", highscore);
        const char *tryAgain = "PRESS ENTER TO PLAY AGAIN";

        if (gameFinished)
        {
            const char *gameFinishedText = "END OF THE LINE!";
            DrawText(gameFinishedText, (screen_width - MeasureText(gameFinishedText, 50)) / 2, 150, 50, RAYWHITE);
            DrawText(highscoreText, (screen_width - MeasureText(highscoreText, 30)) / 2, 220, 30, RAYWHITE);
            DrawText(hscoreText, (screen_width - MeasureText(hscoreText, 30)) / 2, 260, 30, RAYWHITE);
            DrawText(tryAgain, (screen_width - MeasureText(tryAgain, 30)) / 2, 500, 30, RAYWHITE);
        }
        else if (gameOver)
        {
            const char *gameOverText = "GAME OVER!";
            DrawText(gameOverText, (screen_width - MeasureText(gameOverText, 50)) / 2, 150, 50, RAYWHITE);
            DrawText(highscoreText, (screen_width - MeasureText(highscoreText, 30)) / 2, 220, 30, RAYWHITE);
            DrawText(hscoreText, (screen_width - MeasureText(hscoreText, 30)) / 2, 260, 30, RAYWHITE);
            DrawText(tryAgain, (screen_width - MeasureText(tryAgain, 30)) / 2, 500, 30, RAYWHITE);
        }
        else
        {
            // Snake körper zeichnen
            for (int i = 0; i < snakeBody.size(); i++)
            {
                int px = snakeBody[i].x * BLOCK_SIZE;
                int py = snakeBody[i].y * BLOCK_SIZE;
                DrawRectangle(px + (blockGap / 2), py + (blockGap / 2), BLOCK_SIZE - blockGap, BLOCK_SIZE - blockGap, RAYWHITE);
            }

            // Food zeichnen
            DrawRectangle(foodX + (blockGap / 2), foodY + (blockGap / 2), BLOCK_SIZE - blockGap, BLOCK_SIZE - blockGap, RED);
        }

        const char *scoreText = TextFormat("score: %d", score);
        DrawText(scoreText, 10, 10, 20, GREEN);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}