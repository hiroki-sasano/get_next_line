#include <fcntl.h>      
#include <stdio.h>      
#include <stdlib.h>     
#include <unistd.h>     
#include <string.h>     
#include "get_next_line.h"

void test_fd(int fd, const char *label)
{
    char *line;
    int line_num = 0;

    printf("===== Testing: %s =====\n", label);
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("[%02d] %s\n", ++line_num, line);
        free(line);
    }
    printf("===== End: %s =====\n\n", label);
}

int main(int argc, char **argv)
{
    // 1. ファイル読み取り (複数対応)
    for (int i = 1; i < argc; i++)
    {
        int fd = open(argv[i], O_RDONLY);
        if (fd == -1)
        {
            perror("open");
            continue;
        }
        test_fd(fd, argv[i]);
        close(fd);
    }

    // 2. 標準入力を手動テストしたいとき
    if (argc == 1)
    {
        printf("標準入力からテスト（Ctrl+Cで終了）:\n");
        test_fd(0, "stdin");
    }

    return 0;
}

// int main(void)
// {
// 	// int fd = open("test.txt", O_RDONLY);

// 	char *line;
//     int line_num = 0;

//     line = get_next_line(42);
//     {
//         printf("[%02d] %s", ++line_num, line);
//         free(line);
//     }
//     printf("===== End =====\n\n");

// }
