#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "KOKESort.h"

#if _WIN32
#define SECURE_FUNCTIONS
#endif

#ifdef SECURE_FUNCTIONS
#define strtok_r strtok_s
#else
char* strtok_s_fake(char* _String, const char *_Delimiter, char **_Context){
    return strtok(_String, _Delimiter);
}

#define strtok_r strtok_s_fake
#endif

#ifdef SECURE_FUNCTIONS
#define strcpy_r strcpy_s
#else
int strcpy_s_fake(char *_Destination, rsize_t _SizeInBytes, const char *_Source)
{
    strcpy(_Destination, _Source);
    return 0;
}

#define strcpy_r strcpy_s_fake
#endif

#ifdef SECURE_FUNCTIONS
#define strcat_r strcat_s
#else
int strcat_s_fake(char *_Destination, rsize_t _SizeInBytes, const char *_Source)
{
    strcat(_Destination, _Source);
    return 0;
}

#define strcat_r strcat_s_fake
#endif

#ifdef _WIN32
#include <direct.h> // Windows için _getcwd()
#define getcwd _getcwd
#else
#include <unistd.h> // Linux/Unix için getcwd()
#endif

/// @brief 
/// @param fp 
/// @param filename 
/// @param mode 
/// @return 0 if success, another value if failed 
int OpenFile(FILE** fp, const char* filename, const char* mode)
{
    #ifdef SECURE_FUNCTIONS
    if(fopen_s(fp, filename, mode) != 0){
        return 1;
    }
    return 0;
    #else
    *fp = fopen(filename, mode);
    if(*fp == NULL)
    {
        return 1;
    }
    return 0;
    #endif
}

int SearchAndOpenFile(FILE** file, const char* filename, const char* mode)
{
    FILE *fp = NULL;
    int fileState = OpenFile(&fp, filename, "r");

    if (fileState != 0)
    {
        char filepath[256];
        snprintf(filepath, sizeof(filepath), "tests/%s", filename);
        fileState = OpenFile(&fp, filepath, "r");

        if (fileState != 0)
        {
            char cwd[256];
            if (getcwd(cwd, sizeof(cwd)) != NULL)
            {
                fprintf(stderr, "Working Directory: %s\n", cwd);
            }
            return 1;
        }
    }
    *file = fp;
    return 0;
}

const char* ChangeFileExtension(const char* fileName, const char* extension) 
{
    static char newFileName[256];
    strcpy_r(newFileName, 256, fileName);
    char* dotPos = strrchr(newFileName, '.');
    if (dotPos) 
    {
        *dotPos = '\0'; // Uzantıyı sil
    }
    strcat_r(newFileName, 256, extension); // Yeni uzantıyı ekle
    return newFileName;
}

#define TryOpenTestDataFile(file, mode) SearchAndOpenFile(file, ChangeFileExtension(__FILE__, ".txt"), mode)

int ReadArray(int* arr, char* str, const char* delimiter, char** context, int limit)
{
    int count = 0;
    char *token = strtok_r(str, delimiter, context);
    while (token && count < limit)
    {
        arr[count++] = atoi(token);
        token = strtok_r(NULL, delimiter, context);
    }

    return count;
}

/// @brief 
/// @param arr 
/// @param otherArr 
/// @param size 
/// @return 0 if equal, other value if not equal 
int ArrayEqual(int* arr, int* otherArr, int size)
{
    for(int i = 0; i < size; i++)
    {
        if(arr[i] != otherArr[i])
        {
            return 1;
        }
    }
    return 0;
}

void PrintArray(int* arr, int size)
{
    printf("[%d - ", size);
    printf("(");
    for (int i = 0; i < size; i++)
    {
        printf("%d", arr[i]);
        if (i < size - 1)
        {
            printf(", ");
        }
    }
    printf(")");
    printf("]");
}

void PrintBinaryTreeInOrderTraversal(struct BinaryTree* root) {
    if (root != NULL) {
        PrintBinaryTreeInOrderTraversal(root->Left);  // Visit left subtree
        printf("%d ", root->Data);    // Print root
        PrintBinaryTreeInOrderTraversal(root->Right); // Visit right subtree
    }
}

// Pre-order traversal of the tree (root, left, right)
void PrintBinaryTreePreOrderTraversal(struct BinaryTree* root) {
    if (root != NULL) {
        printf("%d ", root->Data);    // Print root
        PrintBinaryTreePreOrderTraversal(root->Left);  // Visit left subtree
        PrintBinaryTreePreOrderTraversal(root->Right); // Visit right subtree
    }
}

// Post-order traversal of the tree (left, right, root)
void PrintBinaryTreePostOrderTraversal(struct BinaryTree* root) {
    if (root != NULL) {
        PrintBinaryTreePostOrderTraversal(root->Left);  // Visit left subtree
        PrintBinaryTreePostOrderTraversal(root->Right); // Visit right subtree
        printf("%d ", root->Data);      // Print root
    }
}

BinaryTree* CreateBinaryTreeNode(int data) {
    BinaryTree *newNode = malloc(sizeof(BinaryTree));
    if (!newNode) {
        fprintf(stderr, "Allocation Error\n");
        return NULL;
    }
    newNode->Data = data;
    newNode->Left = newNode->Right = NULL;
    return newNode;
}

void SerializeBinaryTree(BinaryTree *root, char *buffer) {
    if (root == NULL) {
        strcat(buffer, "# ");
        return;
    }
    char temp[20];
    sprintf(temp, "%d ", root->Data);
    strcat(buffer, temp);
    SerializeBinaryTree(root->Left, buffer);
    SerializeBinaryTree(root->Right, buffer);
}

BinaryTree* DeserializeBinaryTree(char **str) 
{
    while (**str == ' ') 
    {
        (*str)++;
    }

    if (**str == '\0')
    {
        return NULL;
    }
    
    char token[20];
    int i = 0;

    while (**str != ' ' && **str != '\0') 
    {
        token[i++] = **str;
        (*str)++;
    }
    token[i] = '\0';
    
    while (**str == ' ')
    {
        (*str)++;
    }
    
    if (strcmp(token, "#") == 0)
    {
        return NULL;
    }
    
    int data = atoi(token);
    BinaryTree *node = CreateBinaryTreeNode(data);
    node->Left = DeserializeBinaryTree(str);
    node->Right = DeserializeBinaryTree(str);
    return node;
}

int CompareBinaryTree(BinaryTree *root1, BinaryTree *root2) {
    if (root1 == NULL && root2 == NULL)
    {
        return 1;
    }

    if (root1 == NULL || root2 == NULL)
    {
        return 0;
    }

    if (root1->Data != root2->Data)
    {
        return 0;
    }

    return CompareBinaryTree(root1->Left, root2->Left) &&
           CompareBinaryTree(root1->Right, root2->Right);
}

void FreeTree(BinaryTree *root) {
    if (root == NULL)
    {
        return;
    }

    FreeTree(root->Left);
    FreeTree(root->Right);
    free(root);
}

void SaveBinaryTree(BinaryTree *root, FILE *fp) 
{
    //QA: Using SerializeBinaryTree Function?

    if (root == NULL) 
    {
        fprintf(fp, "# ");
        return;
    }
    fprintf(fp, "%d ", root->Data);
    SaveBinaryTree(root->Left, fp);
    SaveBinaryTree(root->Right, fp);
}

BinaryTree* LoadBinaryTree(FILE *fp) 
{
    //QA: Using DeserializeBinaryTree Function?

    char token[20]; 
    if (fscanf(fp, "%s", token) != 1)
    {
        return NULL;
    }
    
    if (strcmp(token, "#") == 0)
    {
        return NULL;
    }
    
    int data = atoi(token);
    BinaryTree *node = CreateBinaryTreeNode(data);
    node->Left = LoadBinaryTree(fp);
    node->Right = LoadBinaryTree(fp);
    return node;
}


