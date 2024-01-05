#include <iostream>
#include <raylib.h>

const int screenWidth = 1280;
const int screenHeight = 720;
bool running = true;
int score = 0;

class Paddle {
    public:
        Vector2 pos = {screenWidth / 2 - 60, screenHeight - 50};
        int width = 120;
        int height = 20;
        int speed = 9;

        void Draw() {
            DrawRectangleRounded({pos.x, pos.y, (float)width, (float)height}, 0.7, 0, WHITE);
        }

        Rectangle getPaddle() {
            return Rectangle{pos.x, pos.y, (float)width, (float)height};
        }

        void Update() {
            Draw();

            if (IsKeyDown(KEY_LEFT)) {
                pos.x -= speed;
            }
            
            else if (IsKeyDown(KEY_RIGHT)) {
                pos.x += speed;
            }

            if (pos.x <= 0) {
                pos.x = 0;
            }

            else if (pos.x >= screenWidth - width) {
                pos.x = screenWidth - width;
            }
        }
};

class Bricks {
    public:
        int width = 70;
        int height = 70;
        Rectangle bricks[52];
        bool visible[52];

        void InitializeBricks() {
            const int rows = 3;
            const int columns = 14;

            for (int i = 0; i < 52; i++) {
                visible[i] = true;
            }

            for (int row = 0; row < rows; row++) {
                for (int col = 0; col < columns; col++) {
                    int index = row * columns + col;
                    bricks[index] = {(float)10 + col * 90, (float)10 + row * 90, (float)width, (float)height};
                }
            }
        }

        void Update(Vector2 ballPos, int radius, int &ballSpeedY) {
            for (int i = 0; i < 52; i++) {
                if (visible[i] && CheckCollisionCircleRec(Vector2{ballPos.x, ballPos.y}, radius, bricks[i])) {
                    visible[i] = false;
                    score += 5;
                    ballSpeedY *= -1;
                }
            }
        }

        void Draw() {
            for (int i = 0; i <= 52; i++) {
                if (visible[i]) {
                    DrawRectangleRounded(bricks[i], 0.5, 0, WHITE);
                }
            }
        }
};

class Ball {
    public:
        Vector2 pos = {screenWidth / 2, screenHeight - 250};
        int radius = 9;
        int speedX = 7;
        int speedY = 7;

        void Draw() {
            DrawCircle(pos.x, pos.y, radius, WHITE);
        }

        void Update(Rectangle paddle) {
            Draw();

            pos.x += speedX;
            pos.y -= speedY;

            if (pos.x >= screenWidth) {
                speedX *= -1;
            }

            if (pos.x <= 0) {
                speedX *= -1;
            }

            if (pos.y >= screenHeight) {
                score--;
                Reset();
            }

            if (pos.y <= 0) {
                speedY *= -1;
            }

            if (CheckCollisionCircleRec(Vector2{pos.x, pos.y}, radius, paddle)) {
                speedY *= -1;
            }
        }

        void Reset() {
            pos.x = screenWidth / 2;
            pos.y = screenHeight - 250;
        }
};

Paddle paddle;
Bricks bricks;
Ball ball;

int main () {

    InitWindow(screenWidth, screenHeight, "Brick Breaker");
    SetTargetFPS(60);

    bricks.InitializeBricks();

    while (WindowShouldClose() == false && running){
        BeginDrawing();

        ClearBackground(BLACK);

        DrawText(TextFormat("%i", score), screenWidth / 2 - 10, screenHeight - 200, 70, WHITE);

        paddle.Update();
        bricks.Update(Vector2{ball.pos.x, ball.pos.y}, ball.radius, ball.speedY);
        ball.Update(paddle.getPaddle());

        bricks.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}