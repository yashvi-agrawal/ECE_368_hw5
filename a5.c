#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct point {
    int x, y;
} Point;

typedef struct tNode {
    Point point;
    struct tNode *left;
    struct tNode *right;
    int height;
} TNode;

// Function prototypes
int height(TNode* node);
int getBalance(TNode* node);
int max(int a, int b);
int isInCircle(Point p, int h, int k, int r);
void search(TNode* node, int h, int k, int r, int* count);
void freeTree(TNode* node);

// Helper function to construct a new node
TNode *constructNode(Point point){
    TNode *node = malloc(sizeof(TNode));
    node->point = point;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

int height(TNode* node) {
    if (node == NULL)
        return 0;
    return node->height;
}

int getBalance(TNode* node) {
    if (node == NULL) return 0;
    return height(node->left) - height(node->right);
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

// Right rotation
TNode* rotateRight(TNode* oldRoot) {
    TNode* newRoot = oldRoot->left;
    TNode* temp = newRoot->right;
    newRoot->right = oldRoot;
    oldRoot->left = temp;

    // Update heights
    oldRoot->height = max(height(oldRoot->left), height(oldRoot->right)) + 1;
    newRoot->height = max(height(newRoot->left), height(newRoot->right)) + 1;

    return newRoot;
}

// Left rotation
TNode* rotateLeft(TNode* oldRoot) {
    TNode* newRoot = oldRoot->right;
    TNode* temp = newRoot->left;
    newRoot->left = oldRoot;
    oldRoot->right = temp;

    // Update heights
    oldRoot->height = max(height(oldRoot->left), height(oldRoot->right)) + 1;
    newRoot->height = max(height(newRoot->left), height(newRoot->right)) + 1;

    return newRoot;
}

TNode* insert(TNode* node, Point point) {
    if (node == NULL)
        return constructNode(point);

    if (point.x < node->point.x || (point.x == node->point.x && point.y < node->point.y)) {
        node->left = insert(node->left, point);
    } else if (point.x > node->point.x || (point.x == node->point.x && point.y > node->point.y)) {
        node->right = insert(node->right, point);
    } else {
        return node; // Point already exists, no insertion
    }

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    // Left Left Case
    if (balance > 1 && (point.x < node->left->point.x || (point.x == node->left->point.x && point.y < node->left->point.y))) {
        return rotateRight(node);
    }
    // Right Right Case
    if (balance < -1 && (point.x > node->right->point.x || (point.x == node->right->point.x && point.y > node->right->point.y))) {
        return rotateLeft(node);
    }
    // Left Right Case
    if (balance > 1 && (point.x > node->left->point.x || (point.x == node->left->point.x && point.y > node->left->point.y))) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    // Right Left Case
    if (balance < -1 && (point.x < node->right->point.x || (point.x == node->right->point.x && point.y < node->right->point.y))) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

int isInCircle(Point p, int h, int k, int r) {
    int xNew = p.x - h;
    int yNew = p.y - k;
    return ((xNew * xNew) + (yNew * yNew)) <= r * r;
}

void search(TNode* node, int h, int k, int r, int* count) {
    if (node == NULL)
        return;

    if (isInCircle(node->point, h, k, r)) {
        (*count)++;
    }

    if (h - r <= node->point.x)
        search(node->left, h, k, r, count);

    if (h + r >= node->point.x)
        search(node->right, h, k, r, count);
}

void freeTree(TNode* node) {
    if (node == NULL) {
        return;
    }
    freeTree(node->left);
    freeTree(node->right);
    free(node);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
        return 1;
    }

    TNode* root = NULL;
    int x, y;

    while (fscanf(file, "%d %d", &x, &y) != EOF) {
        Point point = {x, y};
        root = insert(root, point);
    }
    fclose(file);

    int h, k, r;
    while (scanf("%d %d %d", &h, &k, &r) != EOF) {
        int count = 0;
        search(root, h, k, r, &count);
        printf("%d\n", count);
    }

    freeTree(root);

    return 0;
}