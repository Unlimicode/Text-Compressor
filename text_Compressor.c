#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for a node in the Huffman tree
typedef struct Node {
    char data;
    unsigned freq;
    struct Node *left, *right;
} Node;

// Function to create a new node
Node* createNode(char data, unsigned freq) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->freq = freq;
    node->left = node->right = NULL;
    return node;
}

// Function to swap two nodes
void swap(Node** a, Node** b) {
    Node* temp = *a;
    *a = *b;
    *b = temp;
}

// Function to heapify the min-heap
void heapify(Node* nodes[], int size, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < size && nodes[left]->freq < nodes[smallest]->freq)
        smallest = left;

    if (right < size && nodes[right]->freq < nodes[smallest]->freq)
        smallest = right;

    if (smallest != i) {
        swap(&nodes[i], &nodes[smallest]);
        heapify(nodes, size, smallest);
    }
}

// Function to build the min-heap
void buildHeap(Node* nodes[], int size) {
    int i;
    for (i = size / 2 - 1; i >= 0; i--)
        heapify(nodes, size, i);
}

// Function to extract the minimum node from the heap
Node* extractMin(Node* nodes[], int* size) {
    Node* minNode = nodes[0];
    nodes[0] = nodes[--(*size)];
    heapify(nodes, *size, 0);
    return minNode;
}

// Function to insert a new node into the heap
void insert(Node* nodes[], int* size, Node* node) {
    nodes[(*size)++] = node;
    int i;
    for (i = (*size) / 2 - 1; i >= 0; i--)
        heapify(nodes, *size, i);
}

// Function to build the Huffman tree
Node* buildHuffmanTree(char data[], unsigned freq[], int size) {
    Node** nodes = (Node**)malloc(size * sizeof(Node*));
    int i;
    for (i = 0; i < size; i++)
        nodes[i] = createNode(data[i], freq[i]);

    int heapSize = size;
    buildHeap(nodes, heapSize);

    while (heapSize > 1) {
        Node* left = extractMin(nodes, &heapSize);
        Node* right = extractMin(nodes, &heapSize);
        Node* parent = createNode('$', left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        insert(nodes, &heapSize, parent);
    }

    Node* root = extractMin(nodes, &heapSize);
    free(nodes); // Free the allocated memory for nodes
    return root;
}

// Function to print Huffman codes
void printCodes(Node* root, int code[], int top) {
    if (root->left) {
        code[top] = 0;
        printCodes(root->left, code, top + 1);
    }
    if (root->right) {
        code[top] = 1;
        printCodes(root->right, code, top + 1);
    }
    if (!root->left && !root->right) {
        printf("%c: ", root->data);
        int i;
        for (i = 0; i < top; i++)
            printf("%d", code[i]);
        printf("\n");
    }
}

// Function to calculate the size of the compressed text
int calculateCompressedSize(Node* root, int depth) {
    if (!root) return 0;
    if (!root->left && !root->right)
        return root->freq * depth;
    return calculateCompressedSize(root->left, depth + 1) + calculateCompressedSize(root->right, depth + 1);
}

int main() {
    char text[1000];
    printf("Enter text to compress: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = 0; // Remove newline character

    unsigned freq[256] = {0};
    char data[256];
    int size = 0;

    // Calculate frequency of each character
    int i;
    for (i = 0; text[i]; i++)
        freq[(int)text[i]]++;
    for (i = 0; i < 256; i++) {
        if (freq[i]) {
            data[size] = (char)i;
            freq[size] = freq[i];
            size++;
        }
    }

    // Build Huffman tree
    Node* root = buildHuffmanTree(data, freq, size);

    // Print Huffman codes
    printf("Huffman Codes:\n");
    int code[100], top = 0;
    printCodes(root, code, top);

    // Calculate and print sizes
    int compressedSize = calculateCompressedSize(root, 0);
    printf("Original size: %lu bits\n", strlen(text) * 8);
    printf("Compressed size: %d bits\n", compressedSize);

    return 0;
}
