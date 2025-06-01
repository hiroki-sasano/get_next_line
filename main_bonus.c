#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> 
#include "get_next_line_bonus.h"

void test_fd(int fd, const char *label)
{
    char *line;
    int line_num = 0;

    printf("===== Testing: %s =====\n", label);
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("[%02d] [%s]\n", ++line_num, line);
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

    return 0;
}

// int main(void)
// {
//     int fd1 = open("test1.txt", O_RDONLY);
//     int fd2 = open("test2.txt", O_RDONLY);

//     if (fd1 == -1 || fd2 == -1)
//     {
//         perror("open");
//         return 1;
//     }

//     char *line1 = NULL;
//     char *line2 = NULL;
//     while(1)
//     {
// 		line1 = get_next_line(fd1);
//         if (line1)
//         {
//             printf("file1: %s", line1);
//             free(line1);
//         }

// 		line2 = get_next_line(fd2);
//         if (line2)
//         {
//             printf("file2: %s", line2);
//             free(line2);
//         }

// 		if (!line1 && !line2)
// 			break;
//     }fd);
// }

//     close(fd1);
//     close(fd2);
//     return 0;
// }

//-D BUFFER_SIZE=n

// リダイレクト　./a.out < input.txt
// パイプで受け取る	echo　-n -e "a\nb\n" | ./test_bonus

/*
種類	read 可能	備考
通常ファイル	✔️	open()で取得
標準入力（キーボード）	✔️	fd = 0
パイプ／FIFO	✔️	pipe() / mkfifo()
ソケット	✔️	socket(), accept() 後の fd/
/dev/zero など	✔️	特殊デバイス（デバイスドライバに依存）
/proc/cpuinfo	✔️	仮想ファイルシステム（procfs）
*/

// lseekとはファイルの何バイト目から読み込むか指定できる関数　
// off_t lseek(int fd, off_t offset, int whence);

/*「ファイルディスクリプタは通常のファイル以外を指すこともある」*/
// 名前付きパイプ	mkfifo で作成した FIFO を経由してデータを送る
//FIFOとは名前付きの | (パイプ)	
// mkfifo mypipe<
//cat input.txt > mypipe &
//./a.out mypipe

/*****
cc -Wall -Wextra -Werror -D BUFFER_SIZE=1 \
bonus_src/get_next_line_bonus.c bonus_src/get_next_line_utils_bonus.c main_bonus.c -o gnl_test
cc -Wall -Wextra -Werror -D BUFFER_SIZE=9999 \
get_next_line_bonus.c get_next_line_utils_bonus.c main_bonus.c -o gnl_test
cc -Wall -Wextra -Werror -D BUFFER_SIZE=10000000 \
get_next_line_bonus.c get_next_line_utils_bonus.c main_bonus.c -o gnl_test
*/
