#define DEBUG

#ifdef DEBUG
#define ERROR_PRINT
#define debugPrint(fmt, ...) printf("[%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define debugError(fmt, ...) fprintf(stderr, "[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#endif