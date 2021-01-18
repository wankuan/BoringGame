/**
 * @file maze.cpp
 * @author kendrick (wankuan98@gmail.com)
 * @brief
 * @version 0.1
 * @date 2021-01-19
 *
 * @copyright Copyright (c) 2021
 *
 */
#include <stdio.h>
#include <stdint.h>
#include <stack>

// 迷宫长度
//y ^
//  |
//  |
//  |
//  |
//  |
//  o------------------>
//                    x
#define MAZE_X 10
#define MAZE_Y 10
// 迷宫地图map 1-有障碍物 0-无障碍物
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

//      up
// left o right
//     down
typedef struct{
    // 是否经过该点
    uint8_t is_pass_center;
    uint8_t is_pass_up;
    uint8_t is_pass_right;
    uint8_t is_pass_down;
    uint8_t is_pass_left;
}point_cal_t;
// 保存点计算的map
point_cal_t cal_map[MAZE_Y][MAZE_X] = {0};
// 显示结果的map
uint8_t dis_result_map[MAZE_Y][MAZE_X];

// 迷宫单个点
class point_t{
public:
    point_t(){}
    point_t(uint8_t _x, uint8_t _y){x=_x; y=_y;}
    uint8_t x;
    uint8_t y;
};
// 打印原始迷宫地图
void printf_init_map()
{
    printf("=======INIT MAP==========\n");
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
    printf("=======INIT MAP==========\n");
}

// 打印带轨迹结果的地图
void printf_result_map(std::stack<point_t> &_stack)
{
    memcpy(dis_result_map, maze_map, sizeof(maze_map));
    while(!_stack.empty())
    {
        point_t p = _stack.top();
        dis_result_map[p.y][p.x] = 2;
        _stack.pop();
    }
    printf("=========CAL MAP==========\n");
    for(uint8_t i=0;i<MAZE_Y;i++)
    {
        for(uint8_t j=0;j<MAZE_X;j++)
        {
            if(dis_result_map[i][j] == 1)
            {
                printf("* ");
            }
            else if(dis_result_map[i][j] == 2)
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

// 设置点的四周为走过
void set_point_round_pass(uint8_t cur_x, uint8_t cur_y)
{
    cal_map[cur_y+1][cur_x].is_pass_up = 1;
    cal_map[cur_y][cur_x+1].is_pass_left = 1;
    cal_map[cur_y-1][cur_x].is_pass_down = 1;
    cal_map[cur_y][cur_x-1].is_pass_right = 1;
}

// 是否结束标志位
// 0-未结束  1-结束并找到出口 2-结束未找到出口
uint8_t is_finshed = 0;

int main()
{
    // 实例化起始点和终点
    point_t maze_start(1, 1);
    point_t maze_end(8, 8);
    // 输出原始迷宫地图
    printf_init_map();
    std::stack<point_t> my_stack;
    my_stack.emplace(point_t(maze_start.x, maze_start.y));
    while(!is_finshed)
    {
        // 当前点的x/y
        static uint8_t cur_x = maze_start.x;
        static uint8_t cur_y = maze_start.y;
        // 是否可以前进标志位
        static uint8_t is_can_pass = 0;
        // 设置该点已经过
        cal_map[cur_y][cur_x].is_pass_center = 1;

        // printf("cur:(%d, %d)\n", cur_x, cur_y);
        is_can_pass = 0;
        // right 先向右搜索，如果没有障碍物&右边的点没有走过
        if(!maze_map[cur_y][cur_x+1] && !cal_map[cur_y][cur_x].is_pass_right)
        {
            // 当前点右移
            cur_x++;
            // 设置移动后的四周点为经过
            set_point_round_pass(cur_x, cur_y);
            // 可以前进
            is_can_pass = 1;
        }
        // down 同理向下搜索
        else if(!maze_map[cur_y+1][cur_x] && !cal_map[cur_y][cur_x].is_pass_down)
        {
            cur_y++;
            set_point_round_pass(cur_x, cur_y);
            is_can_pass = 1;
        }
        // left 同理向左搜索
        else if(!maze_map[cur_y][cur_x-1] && !cal_map[cur_y][cur_x].is_pass_left)
        {
            cur_x--;
            set_point_round_pass(cur_x, cur_y);
            is_can_pass = 1;
        }
        // up 同理向上搜索
        else if(!maze_map[cur_y-1][cur_x] && !cal_map[cur_y][cur_x].is_pass_up)
        {
            cur_y--;
            set_point_round_pass(cur_x, cur_y);
            is_can_pass = 1;
        }
        else
        {
            is_can_pass = 0;
        }

        // 可以前进
        if(is_can_pass)
        {
            // 将该点入栈
            my_stack.emplace(point_t(cur_x, cur_y));
        }
        else
        {
            // 找不到前进的点，则后退，将当前点弹出
            my_stack.pop();
            // 找不到出口点，所有走过的点已被弹出
            if(my_stack.empty())
            {
                is_finshed = 2;
                break;
            }
            // 记录弹出后的点x/y，进入下一次循环
            cur_x = my_stack.top().x;
            cur_y = my_stack.top().y;
        }
        // 判断是否到达终点
        if(cur_x == maze_end.x && cur_y == maze_end.y)
        {
            is_finshed = 1;
        }
    }
    // 找到出口
    if(is_finshed == 1)
    {
        printf("[INFO] find dst entrance!\n");
        printf_result_map(my_stack);
    }
    else // 未找到
    {
        printf("[ERROR] can not find dst entrance!\n");
    }
    return 0;
}

