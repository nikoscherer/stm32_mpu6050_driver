#define DEBUG_LOG_ENABLED 1

#if DEBUG_LOG_ENABLED
#define LOG_INFO(fmt, ...) printf("[INFO] " fmt "\r\n", ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) printf("[ERROR] " fmt "\r\n", ##__VA_ARGS__)
#else
#define LOG_INFO(fmt, ...) ((void)0)
#define LOG_ERROR(fmt, ...) ((void)0)
#endif