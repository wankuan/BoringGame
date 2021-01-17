#include <stdio.h>
#include <stdint.h>
#include <stack>


#define MAZE_X 10
#define MAZE_Y 10

typedef struct{
    uint8_t is_pass_by;
    uint8_t is_up;
    uint8_t is_right;
    uint8_t is_down;
    uint8_t is_left;
}cal_t;

cal_t cal_map[MAZE_Y][MAZE_X] = {0};
uint8_t maze_map_with_cal[MAZE_Y][MAZE_X];
uint8_t maze_map[MAZE_Y][MAZE_X] =
{
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 1, 0, 0, 0, 1, 0, 1},
    {1, 0, 0, 1, 0, 0, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 1, 1, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 1, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 1, 0, 1},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

class point_t{
public:
    point_t(){}
    point_t(uint8_t _x, uint8_t _y){x=_x; y=_y;}
    uint8_t x;
    uint8_t y;
};

void printf_map()
{
    printf("=========MAP==========\n");
    for(uint8_t i=0;i<MAZE_Y;i++)
    {
        for(uint8_t j=0;j<MAZE_X;j++)
        {
            if(maze_map[i][j])
            {
                printf("* ");
            }
            else
            {
                printf("  ");
            }
        }
        printf("\n");
    }
    printf("=========MAP==========\n");
}
void printf_map_with_cal(std::stack<point_t> &_stack)
{
    memcpy(maze_map_with_cal, maze_map, sizeof(maze_map));
    while(!_stack.empty())
    {
        point_t p = _stack.top();
        maze_map_with_cal[p.y][p.x] = 2;
        _stack.pop();
    }
    printf("=========CAL MAP==========\n");
    for(uint8_t i=0;i<MAZE_Y;i++)
    {
        for(uint8_t j=0;j<MAZE_X;j++)
        {
            if(maze_map_with_cal[i][j] == 1)
            {
                printf("* ");
            }
            else if(maze_map_with_cal[i][j] == 2)
            {
                printf("0 ");
            }
            else
            {
                printf("  ");
            }
        }
        printf("\n");
    }
    printf("=========CAL MAP==========\n");
}

void printf_stack_point(std::stack<point_t> &_stack)
{
    printf("=========STACK POINT==========\n");
    while(!_stack.empty())
    {
        point_t p = _stack.top();
        printf("(%d, %d)\n", p.x, p.y);
        _stack.pop();
    }
    printf("=========STACK POINT==========\n");
}


uint8_t is_pass_by(uint8_t x, uint8_t y)
{
    return cal_map[y][x].is_pass_by;
}

uint8_t is_finshed = 0;



int main()
{
    uint8_t cur_x = 0;
    uint8_t cur_y = 0;
    uint8_t last_x = 0;
    uint8_t last_y = 0;
    point_t maze_start(1, 1);
    point_t maze_end(MAZE_X-2, MAZE_Y-2);
    printf_map();
    cur_x = maze_start.x;
    cur_y = maze_start.y;
    std::stack<point_t> my_stack;

    my_stack.emplace(point_t(maze_start.x, maze_start.y));
    // printf_stack_point(my_stack);
    static uint8_t is_can_pass = 0;
    while(!is_finshed)
    {
        last_x = cur_x;
        last_y = cur_y;
        // printf("cur:(%d, %d)\n", cur_x, cur_y);
            is_can_pass = 0;
            // right
            if(!maze_map[cur_y][cur_x+1] && !cal_map[cur_y][cur_x].is_right)
            {
                cur_x++;

                cal_map[cur_y+1][cur_x].is_up = 1;
                cal_map[cur_y][cur_x+1].is_left = 1;
                cal_map[cur_y-1][cur_x].is_down = 1;
                cal_map[cur_y][cur_x-1].is_right = 1;

                is_can_pass = 1;
            }
            // down
            else if(!maze_map[cur_y+1][cur_x] && !cal_map[cur_y][cur_x].is_down)
            {
                cur_y++;

                cal_map[cur_y+1][cur_x].is_up = 1;
                cal_map[cur_y][cur_x+1].is_left = 1;
                cal_map[cur_y-1][cur_x].is_down = 1;
                cal_map[cur_y][cur_x-1].is_right = 1;

                is_can_pass = 1;
            }
            // left
            else if(!maze_map[cur_y][cur_x-1] && !cal_map[cur_y][cur_x].is_left)
            {
                cur_x--;
                cal_map[cur_y+1][cur_x].is_up = 1;
                cal_map[cur_y][cur_x+1].is_left = 1;
                cal_map[cur_y-1][cur_x].is_down = 1;
                cal_map[cur_y][cur_x-1].is_right = 1;
                is_can_pass = 1;
            }
            // up
            else if(!maze_map[cur_y-1][cur_x] && !cal_map[cur_y][cur_x].is_up)
            {
                cur_y--;
                cal_map[cur_y+1][cur_x].is_up = 1;
                cal_map[cur_y][cur_x+1].is_left = 1;
                cal_map[cur_y-1][cur_x].is_down = 1;
                cal_map[cur_y][cur_x-1].is_right = 1;

                is_can_pass = 1;
            }
            else
            {
                is_can_pass = 0;
                my_stack.pop();
                cur_x = my_stack.top().x;
                cur_y = my_stack.top().y;
            }
            if(is_can_pass)
            {
                my_stack.emplace(point_t(cur_x, cur_y));
                cal_map[last_y][last_x].is_pass_by = 1;
            }
        // }
        if(cur_x == maze_end.x && cur_y == maze_end.y)
        {
            is_finshed = 1;
        }
    }
    printf_map_with_cal(my_stack);
    return 0;
}

