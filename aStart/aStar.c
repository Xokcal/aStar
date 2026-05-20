#include "typedef.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "aStar.h"

#define HEIGHT 20
#define WIDTH  20

/* A* 的核心格式 ： f(h) = g(h) + m(h)
 * f(h) ： 指的是这个坐标的权重，数值越小，就越值得走
 * g(h) ； 值从原点到当前一共走了多少步
 * m(h) ： ”曼哈顿公式“ 计算原点到终点的最小距离
 * m(h) = | start_x - t_x | + | start_y - t_y | */

i32 manhattan_formula_exe(i32 start_x, i32 start_y, i32 t_x, i32 t_y) {
    return abs(start_x - t_x) + abs(start_y - t_y);
}

i32 enter_heap_formula(i32 curr_step, i32 manhattan_reuslt) {
    return curr_step + manhattan_reuslt;
}

void heap_by_quckly_sort(HeapNodeArr *h, i32 left, i32 right) {
    if (left >= right)return;
    i32 l = left;
    i32 r = right;
    i32 mid = h->arr[left + ((right - left) / 2)]->f;
    while (left <= right) {
        while (left <= right && h->arr[left]->f < mid)left++; // < mid 大于等于就停下来
        while (left <= right && h->arr[right]->f > mid)right--;
        if (left <= right) {
            HeapNode *t = h->arr[left];
            h->arr[left] = h->arr[right];
            h->arr[right] = t;
            left++;
            right--;
        }
    }
    heap_by_quckly_sort(h, l, right);
    heap_by_quckly_sort(h, left, r);
}

HeapNode *create_HeapNode(HeapNode *parent , i32 y, i32 x, i32 f, i32 step,
        i32 manhattan, i32 parent_y, i32 parent_x) {
    HeapNode *h = (HeapNode *) malloc(sizeof(HeapNode));
    h->y = y;
    h->x = x;
    h->f = f;
    h->step = step;
    h->manhattan = manhattan;
    h->parent = parent;
    h->parent_y = parent_y;
    h->parent_x = parent_x;
    return h;
}

HeapNodeArr *create_HeapNodeArr() {
    HeapNodeArr *heapNodeArr = (HeapNodeArr *) malloc(sizeof(HeapNodeArr));
    heapNodeArr->arr = (HeapNode **) malloc(HEIGHT * WIDTH * sizeof(HeapNode *));
    heapNodeArr->tail = 0;
    return heapNodeArr;
}

void enheapNodeArr(HeapNodeArr *heapNodeArr, HeapNode *heapNode) {
    heapNodeArr->arr[heapNodeArr->tail++] = heapNode;
}

void aStar_method
(i32 map[HEIGHT][WIDTH],HeapNode *parent_node ,HeapNodeArr *heap, i32 curr_step,
        i32 *bol, i32 start_y,i32 start_x, i32 t_y, i32 t_x) {
    i32 n_y[] = {-1, 1, 0, 0};
    i32 n_x[] = {0, 0, -1, 1};
    bol[start_y * WIDTH + start_x] = 1;
    if (start_y == t_y && start_x == t_x) {
        printf("step : %d\n", curr_step);
        printf("\n[A* RESULT] : place is [%d][%d]\n", t_y, t_x);
        print_map_path(map , parent_node);
        return;
    }
    for (int i = 0; i < 4; ++i) {
        i32 new_y = start_y + n_y[i];
        i32 new_x = start_x + n_x[i];
        if (new_y < HEIGHT && new_y >= 0 && new_x < WIDTH && new_x >= 0 &&
            map[new_y][new_x] != 1 && bol[new_y * WIDTH + new_x] == 0) {
            bol[new_y * WIDTH + new_x] = 1;
            i32 manhattanFormulaExe = manhattan_formula_exe(new_x, new_y, t_x, t_y);
            i32 f = enter_heap_formula(curr_step, manhattanFormulaExe);
            HeapNode *heapNode = create_HeapNode(parent_node , new_y, new_x, f, curr_step + 1
                , manhattanFormulaExe, start_y, start_x);
            enheapNodeArr(heap, heapNode);
        }
    }
    if (heap->tail == 0)return;
    heap_by_quckly_sort(heap, 0, heap->tail - 1);
    HeapNode *min_heap_node = heap->arr[0];
    //删除数组最小值
    heap->arr[0] = heap->arr[heap->tail - 1];
    heap->tail--;
    aStar_method(map, min_heap_node, heap , min_heap_node->step, bol,
        min_heap_node->y, min_heap_node->x, t_y, t_x);
}

void print_map_path(i32 map[HEIGHT][WIDTH],HeapNode *end_node) {
    printf("enter the function print_map_path!!\n");
    HeapNode *curr_node = end_node;
    while (curr_node->step > 1){
        map[curr_node->y][curr_node->x] = 5;
        printf("curr_node : [%d][%d]\n" , curr_node->y , curr_node->x);
        curr_node = curr_node->parent;
    }
    print_map(map);
}

void aStar_main(i32 map[HEIGHT][WIDTH], i32 start_y, i32 start_x, i32 t_y, i32 t_x) {
    HeapNodeArr *heap = create_HeapNodeArr();
    i32 *bol = (i32 *) calloc(HEIGHT * WIDTH, sizeof(i32));
    aStar_method(map,NULL, heap, 0, bol, start_y, start_x, t_y, t_x);
}

void print_map(i32 map[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; ++i) {
        printf("[");
        for (int j = 0; j < WIDTH; ++j) {
            if (j == WIDTH - 1) {
                printf("%d", map[i][j]);
            } else {
                printf("%d,", map[i][j]);
            }
        }
        printf("]\n");
    }
}


/*假如我走到了一步，这个地方下面和左边f都一样（可能下面更低一点），
 * 然后我走下面，但是下面是被堵住的之后右边可以走，但是右边f值更高，
 * 然后我扫heap里面发现上一步的左边是目前最小f值，然后我又走上一步的左边，
 * 那这样parent不就乱了吗，等等好像没乱，上一步的左边的父座标就是上一步，
 * 然后我回溯路径，直接就是上一步左边坐标到上一步的坐标，然后再到上上一步*/



/*
          . ---- q为当前位置                                                 . ---- b为当前位置
        /   . ----a 是f值第二大的位置                                      /   . ---- 此时a的f值是heap里面最小的，那么就会跳到这一步
      /   /                                                           /   /
  1 ，q ，a ，0 ，0 ，0                                            1 ，q ，a ，0 ，0 ，0
  c ，b ，1 ，0 ，0 ，0                                            c ，b ，1 ，0 ，0 ，0
  1 ，1 ，1 ，0 ，0 ，0                                            1 ，1 ，1 ，0 ，0 ，0
  0 ，0 ，0 ，0 ，0 ，终点                                         0 ，0 ，0 ，0 ，0 ，终点
      |                                                          |
      `---- 但是b位置是f值最小的，按理来说会走这一步！                 `-------- 然后发现c位置是死胡同或者f值竟然比a更大（说明a比c更值得走）
*/

