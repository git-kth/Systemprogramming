#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
int main_menu_list();
void create_menu();
void query_menu();
int query_menu_list();
void query_menu_func();
void update_menu();
int fd;

struct song {
    char title[28];
    char singer[28];
    int year;
};

int main(int argc, char* argv[]) {
    int c;
    if (argc < 2) {
        fprintf(stderr, "사용법 : %s file\n", argv[0]);
        exit(1);
    }
    if ((fd = open(argv[1], O_RDWR | O_CREAT, 0640)) == -1) {
        perror(argv[1]);
        exit(2);
    }
    while (c = main_menu_list()) {
        switch (c) {
        case 1:
            create_menu();
            break;
        case 2:
            query_menu();
            break;
        case 3:
            update_menu();
            break;
        default:
            printf("메뉴값을 정확히 입력하세요.\n");
            printf("계속하려면 엔터 키를 누르세요.");
            while (getchar() != '\n');
        }
    }
    close(fd);
    return 0;
}

int main_menu_list() {
    int select;
    system("clear");
    printf("노래 리스트\n\n");
    printf("0. 종료\n");
    printf("1. 생성\n");
    printf("2. 질의\n");
    printf("3. 갱신\n");
    printf("메뉴번호 입력>>");
    if ((select = getchar()) == '\n')
        return 4;
    getchar();
    return select - '0';
}

void create_menu() {
    int select;
    struct song rec;
    system("clear");
    printf("생성\n");
    while (1) {
        printf("제목:");
        if (scanf("%[^\n]s", rec.title) != 1) break; getchar();
        printf("가수:");
        if (scanf("%[^\n]s", rec.singer) != 1) break; getchar();
        printf("년도:");
        if (scanf("%d", &rec.year) != 1) break; getchar();
        lseek(fd, 0L, SEEK_END);
        write(fd, &rec, sizeof(rec));
        printf("계속 하시려면 Y 그렇지 않으면 다른 키를 누르세요>>");
        char check = getchar();
        if (check == '\n')
            break;
        getchar();
        if (check != 'Y') 
            break;
    }
}

void query_menu() {
    int c;
    while (c = query_menu_list()) {
        int count = 0;
        switch (c) {
        case 1:
        case 2:
        case 3:
        case 4:
            query_menu_func(c);
            break;
        default:
            printf("메뉴값을 정확히 입력하세요.\n");
            printf("계속하려면 엔터 키를 누르세요.");
            while (getchar() != '\n');
        }
    }
}

int query_menu_list() {
    int select;
    system("clear");
    printf("질의\n");
    printf("0. 뒤로가기\n");
    printf("1. 모두보기\n");
    printf("2. 제목 검색\n");
    printf("3. 가수 검색\n");
    printf("4. 년도 검색\n");
    printf("메뉴번호 입력>>");   
    if ((select = getchar()) == '\n')
        return 5;
    getchar();
    return select - '0';
}

void query_menu_func(int select) {
    struct song rec;
    int count = 0;
    printf("%2s %s %s %s\n", "idx.", "[제목]", "{가수}", "(년도)");
    if (select == 1) {
        lseek(fd, 0L, SEEK_SET);
        while (read(fd, &rec, sizeof(rec)) > 0)
            printf("%2d. [%s] {%s} (%d)\n", ++count, rec.title, rec.singer, rec.year);
    }
    else if (select == 4) {
        int query;
        printf("검색할 년도 입력>>");
        scanf("%d", &query);
        getchar();
        lseek(fd, 0L, SEEK_SET);
        while (read(fd, &rec, sizeof(rec)) > 0)
            if (rec.year == query)
                printf("%2d. [%s] {%s} (%d)\n", ++count, rec.title, rec.singer, rec.year);
        if (count == 0) printf("년도가 %d인 정보는 없습니다.\n", query);
    }
    else {
        int compare;
        char query[28];
        if (select == 2) {
            printf("검색할 제목 입력>>");
            scanf("%[^\n]s", query);
            getchar();
            lseek(fd, 0L, SEEK_SET);
            while (read(fd, &rec, sizeof(rec)) > 0)
                if (!strcmp(rec.title, query))
                    printf("%2d. [%s] {%s} (%d)\n", ++count, rec.title, rec.singer, rec.year);
            if (count == 0) printf("제목이 %s인 정보는 없습니다.\n", query);
        }
        else {
            printf("검색할 가수 입력>>");
            scanf("%[^\n]s", query);
            getchar();
            lseek(fd, 0L, SEEK_SET);
            while (read(fd, &rec, sizeof(rec)) > 0)
                if (!strcmp(rec.singer, query))
                    printf("%2d. [%s] {%s} (%d)\n", ++count, rec.title, rec.singer, rec.year);
            if (count == 0) printf("가수가 %s인 정보는 없습니다.\n", query);
        }
    }
    printf("계속하려면 엔터 키를 누르세요.");
    while (getchar() != '\n');
}

void update_menu() {
    int count = 0;
    struct song rec,new_rec;
    int idx,mode;
    lseek(fd, 0L, SEEK_SET);
    while (read(fd, &rec, sizeof(rec)) > 0)
        printf("%2d. [%s] {%s} (%d)\n", ++count, rec.title, rec.singer, rec.year);
    puts("");
    do {
        char tmp;
        printf("갱신할 번호(1 ~ %d)를 입력 하세요>>",count);
        idx = (tmp = getchar()) - '0';
        if(tmp != '\n')
            getchar();
    } while (idx <= 0 || idx > count);
    lseek(fd, sizeof(rec) * idx, SEEK_SET);
    read(fd, &rec, sizeof(rec));
    printf("변경전제목:%s\n", rec.title);
    printf("변경후 제목:");
    if (scanf("%[^\n]s", new_rec.title) != 1) return; getchar();
    printf("변경후 가수:%s\n", rec.singer);
    printf("변경후 가수:");
    if (scanf("%[^\n]s", new_rec.singer) != 1) return; getchar();
    printf("변경전 년도:%d\n", rec.year);
    printf("변경후 년도:");
    if (scanf("%d", &new_rec.year) != 1) return; getchar();
    lseek(fd, -sizeof(rec) , SEEK_CUR);
    write(fd, &new_rec, sizeof(rec));
    printf("변경 되었습니다.\n");
    printf("계속하려면 엔터 키를 누르세요.");
    while(getchar() != '\n');
}   
