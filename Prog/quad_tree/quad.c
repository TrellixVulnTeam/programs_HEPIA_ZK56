#include "quad.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

quad_tree* create_node(int value) {
    quad_tree* node = calloc(1, sizeof(quad_tree));
    node->val = value;
    return node;
}

quad_tree* create_childrens(int value[4]) {
    quad_tree* node = calloc(4, sizeof(quad_tree));

    for (size_t i = 0; i < 4; i++) {
        node[i].val = value[i];
    }

    return node;
}

void tree_free(quad_tree* tree) {
    for (size_t i = 0; i < 4; i++) {
        if (tree->children[i] != NULL)
            tree_free(tree->children[i]);
    }

    free(tree);
}
/*
void to_matrix(quad_tree* tree, int size, int index, int** matrix) {
    if(index < 0)
        index = 0;

    printf("\n");
    for (int i = 0; i < 4; i++) {
        if (tree->children[i]->children[0] != NULL){

            int new_index = index;
            //if(tree->children[i]->children[0] != NULL)
            new_index = index/4*i-1;
                
            to_matrix(tree->children[i], size, new_index, matrix);
        }
        else{
            int x;
            if (index == 0)
                x = (index+i) % 2;
            else
                x = (index+1+i) % 2;

            if((index+1) % 8 != 0 && index != 0)
                x += 3;
                
            int y = (int)floor((index+1) / size);



            //if(i > 1)
            printf("if x:%d  y:%d   index:%d  i:%d  \n", x, (int)floor((index+5) / size), index+1, i);
            //  printf("else x:%d  y:%d   index:%d  i:%d  \n", x, y, index+1, i);

            if(i > 1)
                matrix[y][x] = tree->val;
            else
                matrix[y][x] = tree->val;
        }
    }
}
*/

bool is_leaf(quad_tree* qt) {
    return (qt->children[0] == NULL);
}

int max(int a, int b) {
    return (a >= b ? a : b);
}

int max_depth(int depths[4]) {
    int m = depths[0];
    for (int i = 1; i < 4; i += 1) {
        m = max(m, depths[i]);
    }
    return m;
}

int depth(quad_tree* qt) {
    int depths[] = {0, 0, 0, 0};
    if (is_leaf(qt)) {
        return 0;
    } else {
        for (int i = 0; i < 4; i += 1) {
            depths[i] = depth(qt->children[i]);
        }
        return max_depth(depths) + 1;
    }
}

quad_tree* position(int row, int col, quad_tree* qt) {
    int d = depth(qt);
    int index = 0;
    while (d > 1) {
        index = 2 * ((row % 2 ^ d) / 2 ^ (d - 1)) + (col % 2 ^ d) / 2 ^ (d - 1);
        qt = qt->children[index];
        d -= 1;
    }
    return qt;
}

void to_matrix(quad_tree* qt, int row, int col, int** matrix) {
    for (int r = 0; r < row; r += 1) {
        for (int c = 0; c < col; c += 1) {
            quad_tree* current = position(r, c, qt);
            matrix[r][c] = current->val;
        }
    }
}


int** alloc_array(int size) {
    int** array = malloc(sizeof(int*) * size);
    for (int i = 0; i < size; i++) {
        array[i] = malloc(sizeof(int*) * size);
    }
    return array;
}

int** copy_array_range(int size, int** mat, int start_x, int start_y, int new_size) {
    int** array = alloc_array(new_size);

    for (int i = 0; i < new_size; i++) {
        for (int j = 0; j < new_size; j++) {
            array[i][j] = mat[start_y + i][start_x + j];
        }
    }
    return array;
}

void free_array(int size, int** mat) {
    for (int i = 0; i < size; i++) {
        free(mat[i]);
    }
    free(mat);
}

quad_tree* to_quad_tree(int size, int** mat) {
    quad_tree* tree = create_node(0);

    if (size == 2) {
        tree->children[0] = create_node(mat[0][0]);
        tree->children[1] = create_node(mat[0][1]);
        tree->children[2] = create_node(mat[1][0]);
        tree->children[3] = create_node(mat[1][1]);
    } else {
        int new_size = size / 2;

        int** new_mat = copy_array_range(size, mat, 0, 0, new_size);
        // printf("allo %d\n",new_mat[0][0]);
        // printf("allo %d\n",new_mat[1][0]);
        // printf("allo %d\n",new_mat[0][1]);
        // printf("allo %d\n\n",new_mat[1][1]);
        tree->children[0] = to_quad_tree(new_size, new_mat);  // top left
        free_array(new_size, new_mat);

        new_mat = copy_array_range(size, mat, new_size, 0, new_size);
        tree->children[1] = to_quad_tree(new_size, new_mat);  // top right
        free_array(new_size, new_mat);

        new_mat = copy_array_range(size, mat, 0, new_size, new_size);
        tree->children[2] = to_quad_tree(new_size, new_mat);  // bottom left
        free_array(new_size, new_mat);

        new_mat = copy_array_range(size, mat, new_size, new_size, new_size);
        tree->children[3] = to_quad_tree(new_size, new_mat);  // bottom right
        free_array(new_size, new_mat);
    }
    return tree;
}

void symetrie(quad_tree* tree) {
    for (size_t i = 0; i < 4; i++) {
        if (tree->children[i] != NULL)
            symetrie(tree->children[i]);
    }

        quad_tree* tmp = tree->children[0];
        tree->children[0] = tree->children[1];
        tree->children[1] = tmp;
        tmp = tree->children[2];
        tree->children[2] = tree->children[3];
        tree->children[3] = tmp;
    
}

quad_tree* compress(quad_tree* tree) {
}

void pretty_print(quad_tree* tree, int depth) {
    if (!depth)
        printf("_____________________\n");

    for (int i = 0; i < 2; i++) {
        if (tree->children[i] != NULL)
            pretty_print(tree->children[i], depth + 1);
    }

    for (int i = 0; i < depth * 7; i++)
        printf(" ");

    if (tree->children[0] == NULL)
        printf("%d\n", tree->val);
    else
        printf("o\n");

    for (int i = 2; i < 4; i++) {
        if (tree->children[i] != NULL)
            pretty_print(tree->children[i], depth + 1);

        if(i == 3 && tree->children[0] != NULL && tree->children[0]->children[0] == NULL)
            printf("\n");
    }
}

/*
quad_tree* remove_node(quad_tree* tree) {
}

int tree_size(quad_tree* t) {
}

int tree_depth(quad_tree* tree) {
}

quad_tree* insert(quad_tree* t, int value) {
}

quad_tree* find(quad_tree* t, int value) {
}

quad_tree* find_parent(quad_tree* t, int value) {
}

void remove_element(quad_tree* t, int value) {
}
*/
