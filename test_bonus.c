#include <fcntl.h>      // open
#include <stdio.h>      // printf, perror
#include <stdlib.h>     // free
#include <unistd.h>     // close
#include <string.h>     // strcmp
#include "get_next_line.h"  // get_next_lineのプロトタイプ

void test_fd(int fd, const char *label)
{
    char *line;
    int line_num = 0;

    printf("===== Testing: %s =====\n", label);
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("[%02d] %s", ++line_num, line);
        free(line);
        if (line_num >= 10 && strcmp(label, "/dev/zero") == 0)
        {
            printf("... (cut off after 10 lines)\n");
            break;
        }
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
        printf("標準入力からテスト（Ctrl+Dで終了）:\n");
        test_fd(0, "stdin");
    }

    // 3. 応用：/dev/zero テスト（最大10行で打ち切り）
    int zero_fd = open("/dev/zero", O_RDONLY);
    if (zero_fd != -1)
    {
        test_fd(zero_fd, "/dev/zero");
        close(zero_fd);
    }

    return 0;
}
