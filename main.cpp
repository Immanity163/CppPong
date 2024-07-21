#include <iostream>
#include <raylib.h>

using namespace std;

Color Green = Color{38,185,154,255};
Color Dark_green = Color{20,160,133,255};
Color Light_green = Color{129,204,184,255};
Color Yellow = Color{243,213,91,255};

int player_score = 0;
int cpu_score = 0;

class Ball{
    public:
    float x,y;
    int speed_x, speed_y;
    int radius;

    void Draw(){
        DrawCircle(x, y, radius, Yellow);
    }

    void Update(){
        x += speed_x;
        y += speed_y;

        if(y+radius >= GetScreenHeight() || y - radius <= 0){
            speed_y *= -1;
        }

        if(x+radius >= GetScreenWidth())//point to griffindore
        {
            cpu_score++;
            ResetBall();
        }
        if (x - radius <= 0){
            player_score++;
            ResetBall();
        }
    }

    void ResetBall(){
        x = GetScreenWidth()/2;
        y = GetScreenHeight()/2;

        int speed_choices[2] = {-1,1};
        speed_x *= speed_choices[GetRandomValue(0,1)];
        speed_y *= speed_choices[GetRandomValue(0,1)];
    }
};

class Paddle{
    public:
    float x,y;
    float width,height;
    int speed;

    void Draw(){
        DrawRectangleRounded(Rectangle{x,y, width,height},0.8,0,WHITE);
    }

    void Update(){
        if(IsKeyDown(KEY_UP) && y >= 10){
            y -= speed;
        }
        if(IsKeyDown(KEY_DOWN) && y <= GetScreenHeight()-height-10){
            y += speed;
        }
    }
};

class CpuPaddle: public Paddle
{

    public:
    
    void Update(int ball_y){
        if(y+height/2 > ball_y && y >= 10){
            y -= speed;
        }
        if(y+height/2 < ball_y && y <= GetScreenHeight()-height-10){
            y += speed;
        }
    }

};

Ball ball;
Paddle player;
CpuPaddle cpu;

int main () {

    //Screen
    const int screen_width = 1280;
    const int screen_height = 800;
    const int screen_FPS = 60;
    InitWindow(screen_width, screen_height, "Pong");
    SetTargetFPS(screen_FPS);

    //Ball

    ball.radius = 20;
    ball.x = screen_width/2;
    ball.y = screen_height/2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    //Player
    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width - 10;
    player.y = screen_height/2 - player.height/2;
    player.speed = 6;

    //AI
    cpu.height = 120;
    cpu.width = 25;
    cpu.x = 10;
    cpu.y = screen_height/2 - cpu.height/2;
    cpu.speed = 6;
    
    while(WindowShouldClose() == false){
        BeginDrawing();

        ball.Update();
        player.Update();
        cpu.Update(ball.y);

        //collisions
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius,Rectangle{player.x-1, player.y,player.width,player.height})){
            ball.speed_x *= -1;
        }
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius,Rectangle{cpu.x+1, cpu.y,cpu.width,cpu.height})){
            ball.speed_x *= -1;
        }


        ClearBackground(Dark_green);
        DrawRectangle(screen_width / 2, 0, screen_width/2, screen_height,Green);
        DrawCircle(screen_width/2,screen_height/2,150,Light_green);
        ball.Draw();
        //paddles
        player.Draw();
        cpu.Draw();
        //score
        DrawText(TextFormat("%i", cpu_score),screen_width/4-20,20,80,WHITE);
        DrawText(TextFormat("%i", player_score),3*screen_width/4-20,20,80,WHITE);
        
        //field border
        DrawLine(screen_width/2,0,screen_width/2,screen_height,WHITE);


        EndDrawing();
    }

    CloseWindow();
    return 0;
}