#include "conio2.h"

void window(char* header, int xl, int yl, int xr, int yr, int color, int background){
    int a = xr-xl; int len = strlen(header)+2;
    int i; int k;
    //up
    gotoxy(xl, yl);
    textcolor(color); textbackground(background);
    printf("%c", 201);
    for (i=xl; i<xl+(a-len)/2; i++)
        printf("%c", 205);
    printf("%c%s%c", 185, header, 204);
    for (i=wherex(); i<xr; i++)
        printf("%c", 205);
    printf("%c", 187);
    //left
    for (i=yl+1; i<yr; i++){
        gotoxy(xl, i);
        printf("%c", 186);
    }
    //right
    for (i=yl+1; i<yr; i++){
        gotoxy(xr, i);
        printf("%c", 186);
    }
    //down
    gotoxy(xl, yr);
    printf("%c", 200);
    for (i=xl+1; i<xr; i++)
        printf("%c", 205);
    printf("%c", 188);
    //fill
    for (i=yl+1; i<yr; i++){
        for (k=xl+1; k<xr; k++){
            gotoxy(k, i);
            printf(" ");
        }
    }
    textcolor(LIGHTGRAY); textbackground(BLACK);
    gotoxy(xl, yr+1);
}

int menu(char **menu, int len, int x, int y, int ucolor, int uback, int scolor, int sback){
    int i; int point = 0; char ch;
    textcolor(scolor); textbackground(sback);
    gotoxy(x, y);
    printf("%s", menu[0]);
    textcolor(ucolor); textbackground(uback);
    for (i=y+1; i<y+len; i++){
        gotoxy(x, i);
        printf("%s", menu[i-y]);
    }
    while ((ch=getch())!=13){
        if ((ch == 72) && (point>0)){
            gotoxy(x, y+point);
            textcolor(ucolor); textbackground(uback);
            printf("%s", menu[point--]);
            textcolor(scolor); textbackground(sback);
            gotoxy(x, y+point);
            printf("%s", menu[point]);
        }
        if ((ch == 80) && (point<(len-1))){
            gotoxy(x, y+point);
            textcolor(ucolor); textbackground(uback);
            printf("%s", menu[point++]);
            textcolor(scolor); textbackground(sback);
            gotoxy(x, y+point);
            printf("%s", menu[point]);
        }
        gotoxy(x, y+len);
    }
    textcolor(ucolor);
    textbackground(uback);
    return point;
}

void nl(){
    gotoxy(wherex(), wherey()+1);
}
