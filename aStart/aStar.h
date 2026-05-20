#ifndef ASTAR_ASTAR_H
#define ASTAR_ASTAR_H

#include "typedef.h"

typedef struct HeapNode{
    i32 y , x;
    i32 f , step , manhattan;
    struct HeapNode *parent;
    i32 parent_y , parent_x;
}HeapNode;

typedef struct HeapNodeArr{
    HeapNode **arr;
    i32        tail;
}HeapNodeArr;

#define HEIGHT  20
#define WIDTH  20

i32 manhattan_formula_exe(i32 start_x, i32 start_y, i32 t_x, i32 t_y);

i32 enter_heap_formula(i32 curr_step, i32 manhattan_reuslt);

void heap_by_quckly_sort(HeapNodeArr *h, i32 left, i32 right);

HeapNode *create_HeapNode(HeapNode *parent ,i32 y , i32 x , i32 f , i32 step , i32 manhattan ,i32 parent_y , i32 parent_x);

void aStar_method(i32 map[HEIGHT][WIDTH],HeapNode *parent_node, HeapNodeArr *heap, i32 step, i32 *bol, i32 start_y, i32 start_x, i32 t_y, i32 t_x) ;

void aStar_main(i32 map[HEIGHT][WIDTH], i32 start_y, i32 start_x, i32 t_y, i32 t_x);

void print_map_path(i32 map[HEIGHT][WIDTH],HeapNode *end_node);

void print_map(i32 map[HEIGHT][WIDTH]);
#endif //ASTAR_ASTAR_H
