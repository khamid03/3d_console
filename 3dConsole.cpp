#include <iostream>
#include<Windows.h>
#include <chrono>
#include <math.h>
#include <vector>
#include <algorithm>
using namespace std;

int ScreenW = 120;
int ScreenH = 40;

float playerX = 8.0f;
float playerY = 8.0f;
float playerV = 0.0f;

int mapH = 16;
int mapW = 16;

float POV = 3.14159 / 4.0;
float depth = 16.0f;

int main(){
    bool running = true;
    wchar_t *screen = new wchar_t[ScreenH*ScreenW];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,0,NULL,CONSOLE_TEXTMODE_BUFFER,NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytes = 0;

    wstring map;

    map += L"################";
    map += L"#..............#";
    map += L"#####...###....#";
    map += L"#.........###..#";
    map += L"#.........#....#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..######......#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#.....#####....#";
    map += L"#..............#";
    map += L"################";

    auto tp1 = chrono::system_clock::now();
    auto tp2 = chrono::system_clock::now();
    while (running)
    {   
        tp2 = chrono::system_clock::now();
        chrono::duration<float> elapsedTime = tp2-tp1;
        tp1 = tp2;
        float felapsedTime = elapsedTime.count();

        //Controls
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
            running = false;
        if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
            playerV -= (0.8f * felapsedTime);
        if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
            playerV += (0.8f * felapsedTime);
        if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
        {
            playerX += sinf(playerV) * 5.0f * felapsedTime;
            playerY += cosf(playerV) * 5.0f * felapsedTime;

            if (map[(int)playerY * mapW + (int)playerX] == '#')
            {
                playerX -= sinf(playerV) * 5.0f * felapsedTime;
                playerY -= cosf(playerV) * 5.0f * felapsedTime;
            }
             
        }
        if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
        {
            playerX -= sinf(playerV) * 5.0f * felapsedTime;
            playerY -= cosf(playerV) * 5.0f * felapsedTime;
            if (map[(int)playerY * mapW + (int)playerX] == '#')
            {
                playerX += sinf(playerV) * 5.0f * felapsedTime;
                playerY += cosf(playerV) * 5.0f * felapsedTime;
            } 
        }
        
        for (int x = 0; x < ScreenW; x++)
        {
            float playAngle = (playerV - POV / 2.0f) + ((float)x / (float)ScreenW)* POV;
            
            float distanceWall = 0;
            bool hitWall = false;
            bool boundary = false;

            float eyeX = sinf(playAngle);
            float eyeY = cosf(playAngle);
            while (!hitWall && distanceWall < depth)
            {
                
                distanceWall += 0.1f;

                int testX = (int)(playerX + eyeX * distanceWall);
                int testY = (int)(playerY + eyeY * distanceWall);

                if (testX < 0 || testX >= mapW || testY < 0 || testY >= mapH)
                {
                    hitWall = true;
                    distanceWall = depth;
                }
                else
                {   // # is a wall
                    if (map[testY*mapW + testX] == '#')
                    {
                        hitWall = true;

                        vector<pair<float,float>> p; // distance, dot

                        for (int tx = 0; tx < 2; tx++)
                            for (int ty = 0; ty < 2; ty++)
                            {
                                float vy = (float)testY + ty - playerY;
                                float vx = (float)testX + tx - playerX;
                                float d = sqrt(vx*vx + vy*vy);
                                float dot = (eyeX * vx / d) + (eyeY * vy / d);
                                p.push_back(make_pair(d, dot));
                            }
                        
                        //sort pairs from closest to farthest
                        sort(p.begin(), p.end(), [](const pair<float, float> &left, const pair<float, float> &right) {return left.first < right.first;});

                        float fbound = 0.01;
                        if (acos(p.at(0).second) < fbound) boundary = true;
                        if (acos(p.at(1).second) < fbound) boundary = true;
                    }
                    
                }
                
            }
            // distance to ceiling and floor
            int ceiling = (float)(ScreenH / 2.0) - ScreenH / ((float)distanceWall);
            int floor = ScreenH - ceiling;

            short shade = ' ';
            //shade for walls
            if (distanceWall <= depth / 4.0f)       shade = 0x2588;//very close
            else if (distanceWall < depth / 3.0f)   shade = 0x2593;
            else if (distanceWall < depth / 2.0f)   shade = 0x2592;
            else if (distanceWall < depth)          shade = 0x2591;
            else                                    shade = ' ';   //far away
            
            if(boundary)  shade = ' '; //black out
            for (int y = 0; y < ScreenH; y++)
            {
                if (y < ceiling)
                    screen[y*ScreenW + x] = ' ';
                else if (y > ceiling && y <= floor)
                    screen[y*ScreenW + x] = shade;
                else
                {    
                    short fshade = ' ';
                    float b = 1.0f - (((float)y - ScreenH / 2.0f) / ((float)ScreenH / 2.0f));
                    if (b < 0.25)         fshade = '#';
                    else if (b < 0.5)     fshade = 'x';
                    else if (b < 0.75)    fshade = '.';
                    else if (b < 0.9)     fshade = '-';
                    else                  fshade = ' ';
                    screen[y*ScreenW + x] = fshade;
                }
                
                
            }
            
            
        }
        //info
        swprintf(screen, 40, L"X=%3.2f, Y=%3.2f, V=%3.2f FPS=%3.2f ", playerX, playerY, playerV, 1.0/felapsedTime);
        //map
        for (int nx = 0; nx < mapW; nx++)
            for (int ny = 0; ny < mapW; ny++)
            {
                screen[(ny + 1)*ScreenW + nx] = map[ny * mapW + nx];
            }
        char view;
        if (playerV >= -0.5 && playerV <= 0.5)
            view = 'v';
        else if(playerV >= 1.0 && playerV <= 2.0)
            view = '>';
        else if(playerV <= 3.5 && playerV >= 2.5)
            view = '^';
        else if(playerV <= -1.0 && playerV >= -2.0)
            view = '<';
        else
            view = 'P'; 
        screen[((int)playerY + 1) * ScreenW + (int)playerX] = view;
            
        screen[ScreenH*ScreenW - 1] = '\0';
        WriteConsoleOutputCharacterW(hConsole,screen,ScreenH*ScreenW,{0,0},&dwBytes);
    }
    return 0;
}