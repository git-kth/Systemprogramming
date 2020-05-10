#include <sys/types.h> 
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <getopt.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>

/*
    == use example ==
    ./dl -l
    ./dl -al . ..
    ./dl --almost-all -l . ..
    ./dl -ila ~ . ..
    ./dl ~ -hla . .. 

    이와 같이 다양하게 쓸 수 있다.
*/

// 어떤 옵션이 있는지 확인하는 int형 변수 (초기값 = 0 -> false , 1 -> true)
int aflag = 0; // a option
int Aflag = 0; // A option
int hflag = 0;  // h option
int iflag = 0; // i option
int lflag = 0; // l option
int oneflag = 0; // 1 option

// getopt_long 함수에 넘겨줄 옵션을 정의
static struct option options[] =
{
    // {const char* name, int has_arg, int* flag, int val}
    // no_argument => 0
    // required_argument => 1
    {"all", 0, 0, 0},
    {"almost-all", 0, 0, 0},
    {"inode", 0, 0, 0},
    {"help", 0, 0, 0},
    {"human-readable", 0, 0, 0},
    {0, 0, 0, 0}
};

// function prototype declaration
char type(mode_t);
char* perm(mode_t);
void print(char*, char*, struct stat*);

// --help print format
void help() {
    printf("  %s, %-22s %-50s\n","-a", "--all", "do not ignore entries starting with .");
    printf("  %s, %-22s %-50s\n","-A", "--almost-all", "do not list implied . and ..");
    printf("  %s, %-22s %-50s\n","-i", "--inode", "print the index number of each file");
    printf("  %s  %-22s %-50s\n","-l", "", "use a long listing format");
    printf("  %s, %-22s %-50s\n","-h", "--human-readable", "with -l and/or -s, print human readable sizes");
    printf("  %s  %-22s %-50s\n","-1", "", "list one file per line");
    exit(0);
}

int main(int argc, char** argv) {
    DIR* dp;
    char* dir;
    struct dirent* d;
    struct stat st;
    char path[BUFSIZ + 1];
    while (1) {
        int option_index = 0;
        int opt = getopt_long(argc, argv, "aAilh1", options, &option_index);
        // 더이상 입력받을 옵션이 없을 시 while loop break
        if (opt == -1)
            break;
        switch (opt) {
        // --option
        case 0:
            switch (option_index) {
            case 0: //  options[0] : --all
                aflag = 1;
                break;
            case 1: //  options[1] : --almost-all
                Aflag = aflag = 1;
                break;
            case 2: //  options[2] : --inode
                iflag = 1;
                break;
            case 3: //  options[3] : --help 
                help();
                break;
            case 4: //  options[4] : --human-readable
                hflag = 1;
                break;
            }
            break;
        // -option
        case 'a': //    option : -a
            aflag = 1;
            break;
        case 'A': //    option : -A
            Aflag = aflag = 1;
            break;
        case 'i': //    option : -i
            iflag = 1;
            break;
        case 'l': //    option : -l
            lflag = 1;
            break;
        case 'h': //    option : -h
            hflag = 1;
            break;
        case '1': //    option : -1
            oneflag = 1; 
            break;
        default: // option중 일치하는 문자가 없을 경우
            printf("Try '%s --help' for more information.\n",argv[0]);
            exit(1);
        }
    }
    int dircnt = 0;
    for (int i = 1;i < argc;i++)  // main 매개변수에 디렉터리 이름이 몇개 있는지 구한다.
        if (argv[i][0] != '-')
            dircnt++;
    for (int i = 1;;i++) {
        if (dircnt == 0)    // main 매개변수에 디렉터리 이름이 하나도 없다면 현재 디렉터리만 출력하도록 설정
            dir = ".";
        else if (argv[i][0] == '-')     // -으로 시작하는 매개변수 값은 옵션이므로 continue
            continue;
        else {
            dir = argv[i];      // 디렉터리 이름을 매개변수로 받는다
            printf("%s:\n", dir);
        }
        if ((dp = opendir(dir)) == NULL) {
            perror(dir);	//null 값이 나온다면 에러출력.
            exit(1);
        }
        char hidden = '.';
        // a option이 있다면 .로 시작하는 파일이름도 출력하도록 변경
        if (aflag)
            hidden = ' ';
        if (Aflag)  // A option이 있다면 . 그리고 .. 은 출력 되지 않고 숨긴 파일 출력
            for (int i = 0;i < 2;i++)
                d = readdir(dp);
        while ((d = readdir(dp)) != NULL) {	// file iterator in directory
            if (d->d_name[0] != hidden) {
                sprintf(path, "%s/%s", dir, d->d_name); // 파일 경로명 만들기 
                if (lstat(path, &st) < 0) {	// 파일 상태 가져오기  
                    perror(path);
                    exit(1);
                }
                else
                    print(path, d->d_name, &st);  // 옵션에 맞게 출력
            }
        }
        if (!lflag && !oneflag)
            printf("\n");
        if (--dircnt <= 0)      // 디렉터리를 하나 처리했으므로 값을 뺴고 0보다 작거나 같다면 더이상 명령할 디렉터리가 없으므로 루프를 종료한다
            break;
        printf("\n");
    }
    closedir(dp);
    return 0;
}

// file print
void print(char* pathname, char* file, struct stat* st) {
    if (iflag) //   inode print
        printf("%18ld ", st->st_ino);
    if (lflag) { //     l option apply
        printf("%c%s ", type(st->st_mode), perm(st->st_mode));
        printf("%3ld ", st->st_nlink);
        printf("%s %s ", getpwuid(st->st_uid)->pw_name, getgrgid(st->st_gid)->gr_name);
        if (hflag) { //     h option apply
            double size = (double)st->st_size;
            int idx = 0;
            char unit[] = { ' ','K','M','G','T','P' };
            while (size > 1024) { //    ' ' -> 'K' -> 'M' -> 'G' -> 'T' -> 'P' (Byte)
                size /= 1024;
                idx++;
            }
            if((long)size == size) //   정수값이라면
                printf("%10ld ", (long)size);
            else
                printf("%9.1lf%c ", size, unit[idx]);
        }
        else 
            printf("%10ld ", st->st_size);
        printf("%.12s ", ctime(&st->st_mtime) + 4);
    }
    printf("%s ", file);
    if (oneflag || lflag)
        printf("\n");
}

// file type return
char type(mode_t mode) {
    if (S_ISREG(mode))
        return('-');
    if (S_ISDIR(mode))
        return('d');
    if (S_ISCHR(mode))
        return('c');
    if (S_ISBLK(mode))
        return('b');
    if (S_ISLNK(mode))
        return('l');
    if (S_ISFIFO(mode))
        return('p');
    if (S_ISSOCK(mode))
        return('s');
}

// file permission return
char* perm(mode_t mode) {
    int i;
    static char perms[10];

    strcpy(perms, "---------");

    for (i = 0; i < 3; i++) {
        if (mode & (S_IREAD >> i * 3))
            perms[i * 3] = 'r';
        if (mode & (S_IWRITE >> i * 3))
            perms[i * 3 + 1] = 'w';
        if (mode & (S_IEXEC >> i * 3))
            perms[i * 3 + 2] = 'x';
    }
    return(perms);
}

/* 
    * 참조한 사이트
	 -https://www.joinc.co.kr/w/man/3/getopt
	 -https://soooprmx.com/archives/4991
	 -http://forum.falinux.com/zbxe/index.php?mid=C_LIB&document_srl=519764
     -https://linux.die.net/man/3/getopt_long
    * 참조한 source file(chap5.pdf)
    - list1.c
    - list2.c
*/