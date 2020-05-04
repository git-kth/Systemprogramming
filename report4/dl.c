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

// � �ɼ��� �ִ��� Ȯ���ϴ� int�� ���� (�ʱⰪ = 0 -> false , 1 -> true)
int aflag = 0; // a option
int lflag = 0; // l option
int iflag = 0; // i option
int oneflag = 0; // 1 option

// getopt_long �Լ��� �Ѱ��� �ɼ��� ����
static struct option options[] =
{
    // {const char* name, int has_arg, int* flag, int val}
    // no_argument => 0
    // required_argument => 1
    {"all", 0, 0, 0},
    {"inode", 0, 0, 0},
    {"help", 0, 0, 0},
    {0, 0, 0, 0}
};

// function prototype declaration
char type(mode_t);
char* perm(mode_t);
void printStat(char*, char*, struct stat*);

// --help print format
void help() {
    printf("  %s, %-22s %-50s\n","-a", "--all", "do not ignore entries starting with .");
    printf("  %s  %-22s %-50s\n","-l", "", "use a long listing format");
    printf("  %s, %-22s %-50s\n","-i", "--inode", "print the index number of each file");
    printf("  %s  %-22s %-50s\n","-1", "", "list one file per line");
    exit(0);
}

int main(int argc, char** argv) {
    DIR* dp;
    char* dir;
    struct dirent* d;
    struct stat st;
    char path[BUFSIZ + 1];
    //	directory ����
    if (argc == 1 || argv[argc - 1][0] == '-')
        dir = ".";	// main arguments �������� ��θ� ���� ������ ���� ���͸��� ������� �Ѵ�.		
    else
        dir = argv[argc - 1];
    if ((dp = opendir(dir)) == NULL) {
        perror(dir);	//null ���� ���´ٸ� �������.
        exit(1);
    }

    while (1) {
        int option_index = 0;
        int opt = getopt_long(argc, argv, "ail1", options, &option_index);
        // ���̻� �Է¹��� �ɼ��� ���� �� while loop break
        if (opt == -1)
            break;
        switch (opt) {
        // --option
        case 0:
            switch (option_index) {
            case 0: //  options[0] : --all
                aflag = 1;
                break;
            case 1: //  options[2] : --inode
                iflag = 1;
                break;
            case 2: //  options[3] : --help
                help();
                break;
            }
            break;
        // -option
        case 'a': //    option : -a
            aflag = 1;
            break;
        case 'i': //    option : -i
            iflag = 1;
            break;
        case 'l': //    option : -l
            lflag = 1;
            break;
        case '1': //    option : -1
            oneflag = 1; 
            break;
        default: // option�� ��ġ�ϴ� ���ڰ� ���� ���
            printf("Try '%s --help' for more information.\n",argv[0]);
            exit(1);
        }
    }
    char hidden = '.';
    // a option�� �ִٸ� .�� �����ϴ� �����̸��� ����ϵ��� ����
    if (aflag)
        hidden = ' ';
    while ((d = readdir(dp)) != NULL) {	// file iterator in directory
        if (d->d_name[0] != hidden) {
            sprintf(path, "%s/%s", dir, d->d_name); // ���� ��θ� ����� 
            if (lstat(path, &st) < 0) {	// ���� ���� ��������  
                perror(path);
                exit(1);
            }
            else 
                printStat(path, d->d_name, &st);  // ���� ���� ���
        }
    }
    if (!lflag && !oneflag)
        printf("\n");
    closedir(dp);

    return 0;
}

// file state print
void printStat(char* pathname, char* file, struct stat* st) {
    if (iflag) //   inode print
        printf("%18ld ", st->st_ino);
    if (lflag) {
        printf("%c%s ", type(st->st_mode), perm(st->st_mode));
        printf("%3ld ", st->st_nlink);
        printf("%s %s ", getpwuid(st->st_uid)->pw_name, getgrgid(st->st_gid)->gr_name);
        printf("%9ld ", st->st_size);
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
    * ������ ����Ʈ
	 -https://www.joinc.co.kr/w/man/3/getopt
	 -https://soooprmx.com/archives/4991
	 -http://forum.falinux.com/zbxe/index.php?mid=C_LIB&document_srl=519764
     -https://linux.die.net/man/3/getopt_long
    * ������ source file(chap5.pdf)
    - list1.c
    - list2.c
*/