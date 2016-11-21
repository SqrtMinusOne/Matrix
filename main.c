#include <stdio.h>
#include <stdlib.h>
#include "conio2.h"
#include "xgraph.h"
#include <windows.h>

#define CMD printf("MATRIX> ")
#define TEXTDEF textcolor(LIGHTGRAY); textbackground(BLACK)
#define TEXTHEADER textcolor(YELLOW); textbackground(BLUE)
#define TEXTMAT textcolor(BLACK); textbackground(WHITE);
#define TEXTHEADER2 textcolor(GREEN); textbackground(BLACK)
#define TEXTIMP textcolor(YELLOW); textbackground(BLACK);
#define TEXTCOL textcolor(WHITE); textbackground(BLUE);
#define DELAY _sleep(50);
#define STOP getchar(); getchar()
#define NL gotoxy(x, y++)

//PROTOTYPE
void swap(int *a, int *b); //Swaps a and b
void fswap(double *a, double *b); //Swaps floats
int mod(int a, int b);
int comd(int* a, int n); //Find common divisor
void randomgen(); //Generate random matrix
void enter(); //Enter matrix
void out(); //Out matrix
void savefile(); //Save matrix to file
void loadfile(); //Load from file
void save(); //Save to backup
void restore(); //Restore from backup
int NarayanaNextPerm (int *a, int n);
int inv(int *arr, int n);
int det(int **arr, int n); //Calculate determinant
void determinant(); //Determinant I/O
int fact(int n);
void correct();
void swaprow(int tgt, int src);
void swapcol(int tgt, int src);
void fswaprow(int tgt, int src);
void fswapcol(int tgt, int src);
void elementary();
void checkbackup();
void fout(int cx, int cy, int sel); //Out float matrix
void fmakeonefrom(int start); //Make 1s from start;start
void gauss(); //Gauss I/O
double makezero();


struct fmatrix{
    int rows;
    int columns;
    double **mat;
}farr;

struct matrix{
    int rows;
    int columns;
    int **mat; //mat[row][column]
}arr;

double makezero(double a){
    if ((a<0.000001) && (a>-0.000001))
        a=0;
    return a;
}

int bmade=0;
struct matrix backup;

void swap(int* a, int* b){
    int c;
    c=*a; *a=*b; *b=c;
}
void fswap(double *a, double *b){
    double c;
    c=*a; *a=*b; *b=c;
}
int mod(int a, int b){
    while (a>=b)
        a=a-b;
    return a;
}
int comd(int* a, int n){
    int i; int cd; int max = a[1]; int b; int rcd = 1;
    for (i=2; i<=n; i++){
        if (a[i]>max)
            max=a[i];
    }
    for (cd=max; cd>1; cd--){
        b=1;
        for (i=1; i<=n; i++)
            if (mod(a[i], cd) == 0)
                b=0;
        if (b==1)
            rcd=cd;
    }
    return rcd;
}
void randomgen(){
    int i; int k; int c;
    int x = 55; int y = 10;
    window("Randomizer", x++, y++, x+30, y+15, YELLOW, BLACK);
    gotoxy(x, y++);
    printf("Enter columns number: ");
    scanf("%d", &arr.columns);
    gotoxy(x, y++);
    printf("Enter rows number: ");
    scanf("%d", &arr.rows);
    gotoxy(x, y++);
    printf("Enter maximum: ");
    scanf("%d", &c);
    arr.mat = (int**)malloc((arr.rows+1)*sizeof(int*));
    for (i=1; i<=arr.rows; i++){
        arr.mat[i]=(int*)malloc((arr.columns+1)*sizeof(int));
        for (k=1; k<=arr.columns; k++){
            arr.mat[i][k]=rand() %(c*2) - c;
        }
    }
}
void enter(){
    int i; int k; int x = 30; int y = 10;
    window("Enter matrix", x++, y++, x+60, y+15, YELLOW, BLACK);
    gotoxy(x, y++); textcolor(YELLOW); textbackground(BLACK);
    printf("Enter columns number: ");
    scanf("%d", &arr.columns);
    gotoxy(x, y++);
    printf("Enter rows number: ");
    scanf("%d", &arr.rows);
    arr.mat = (int**)malloc((arr.rows+1)*sizeof(int*));
    for (i=1; i<=arr.rows; i++){
        arr.mat[i]=(int*)malloc((arr.columns+1)*sizeof(int));
        gotoxy(x, y++);
        printf("%d: ", i);
        for (k=1; k<=arr.columns; k++){
            scanf("%d", &arr.mat[i][k]);
        }
    }
}
void out(){
    int i; int k; int x=60; int y=3;
    window("Matrix", x, y, x+(arr.columns+1)*4+2, y+arr.rows+2, LIGHTGRAY, BLACK);
    x++; y++;
    gotoxy(x, y++); TEXTMAT;
    printf("     ");
    for (i=1; i<=arr.columns; i++){
        printf("%d   ", i);
    }
    gotoxy(x, y++);
    for (i=1; i<=arr.rows; i++){
        TEXTMAT; printf("%d:", i); TEXTDEF;
        for (k=1; k<=arr.columns; k++)
            printf("%4d", arr.mat[i][k]);
        gotoxy(x, y++);
    }
    gotoxy(1, 2);
}
void savefile(){
    int x=30; int y = 10;
    window("Enter name", x++, y++, x+60, y+3, YELLOW, BLACK);
    char *name = (char*)malloc(256*sizeof(char));
    int i; int k;
    gotoxy(x, y); textcolor(YELLOW);
    scanf("%s", name);
    FILE *file;
    file = fopen(name, "w");
    fprintf(file, "%d\n%d\n", arr.rows, arr.columns);
    for (i=1; i<=arr.rows; i++){
        for (k=1; k<=arr.columns; k++)
            fprintf(file, "%d ", arr.mat[i][k]);
        fprintf(file, "\n");
    }
    fclose(file);
}
void loadfile(){
    int x=30; int y = 10;
    window("Enter name", x++, y++, x+60, y+3, YELLOW, BLACK);
    char* name = (char*)malloc(256*sizeof(char)); int i; int k;
    gotoxy(x, y++); textcolor(YELLOW);
    scanf("%s", name);
    FILE *file;
    file = fopen(name, "r");
    if (file == NULL){
        gotoxy(x, y);
        printf("File does not exists!\n");
        getchar(); getchar();
    }
    else{
        fscanf(file, "%d %d", &arr.rows, &arr.columns);
        arr.mat = (int**)malloc((arr.rows+1)*sizeof(int*));
        for (i=1; i<=arr.rows; i++){
            arr.mat[i]=(int*)malloc((arr.columns+1)*sizeof(int));
            for (k=1; k<=arr.columns; k++)
                fscanf(file, "%d", &arr.mat[i][k]);
        }
    }
    fclose(file);
}
void save(){
    int i; int k; bmade=1;
    if (backup.rows!=arr.rows){
        backup.rows=arr.rows;
        backup.columns=arr.columns;
        backup.mat = (int**)malloc((arr.rows+1)*sizeof(int*));
    }
    for (i=1; i<=arr.rows; i++){
        backup.mat[i]=(int*)malloc((arr.columns+1)*sizeof(int));
        for (k=1; k<=arr.columns; k++)
            backup.mat[i][k]=arr.mat[i][k];
    }
}
void restore(){
    int i; int k;
    if (bmade==1){
        arr.mat = (int**)malloc((backup.rows+1)*sizeof(int*));
        for (i=1; i<=arr.rows; i++){
            arr.mat[i]= (int*)malloc((backup.columns+1)*sizeof(int));
            for (k=1; k<=arr.columns; k++)
                arr.mat[i][k]=backup.mat[i][k];
        }
    }

}
int NarayanaNextPerm (int *a, int n){
    int i, k, t, tmp;
    for (k=n-2; (k >= 0) && (a[k]>=a[k+1]); k--);
    if (k == -1)
        return 0;
    for (t=n-1; (a[k]>=a[t]) && (t>=k+1); t--);
    tmp=a[k], a[k]=a[t], a[t]=tmp;
    for (i=k+1; i<=(n+k)/2; i++)
    {
        t=n+k-i;
        tmp=a[i], a[i]=a[t], a[t]=tmp;
    }
    return i;
}
int inv(int *arr, int n){
    int i, k; int b=0;
    for (i=1; i<n; i++){
        for (k=(i+1); k<=n; k++){
            if (arr[i]>arr[k])
                b++;
        }
    }
    while (b>=2)
        b=b-2;
    return b;
}
int det(int **arr, int n){
    int i=0; int k=0; int res=0; int np=0; int tmp=0; int nnp = 0;
    int **per; int *abc;
    np=fact(n);
    abc = (int*) malloc(n*sizeof(int));
    for (i=0; i<n; i++)
        abc[i]=i+1;
    per = (int**)malloc((np+1)*sizeof(int*));
    for (i=1; i<=np; i++){
        per[i]=(int*)malloc((n+1)*sizeof(int));
    }
    for (i=1; i<=np; i++){
        for (k=1; k<=n; k++){
            per[i][k]=abc[k-1];
        }
        nnp=NarayanaNextPerm(abc, n);
    }
    for (i=1; i<=np; i++){
        tmp=1;
        for (k=1; k<=n; k++){
            tmp=tmp*arr[k][per[i][k]];
        }
        if (inv(per[i], n) == 1)
            tmp = -tmp;
        res=res+tmp;
    }
    return res;
    free(arr);
    free(per);
    free(abc);
}
void determinant(){
    int x=45; int y=17;
    window("Determinant", x++, y++, x+70, y+5, YELLOW, BLACK);
    if (arr.rows == arr.columns){
        gotoxy(x, y);
        printf("Calculated determinant: %d", det(arr.mat, arr.rows));
        getchar(); getchar();
    }
    else if (arr.rows > arr.columns){
        int** tarr = (int**)malloc((arr.columns+1)*sizeof(int*));
        int i; int t; int k;
        gotoxy(x, y++);
        printf("Too many rows! Choose which (%d) to include: ", arr.columns);
        for (i=1; i<=arr.columns; i++){
            scanf("%d", &t);
            tarr[i]=(int*)malloc((arr.columns+1)*sizeof(int));
            for (k=1; k<=arr.columns; k++)
                tarr[i][k]=arr.mat[t][k];
        }
        gotoxy(x, y++);
        printf("Calculated determinant: %d", det(tarr, arr.columns));
        getchar(); getchar();
        free(tarr);
    }
    else if (arr.rows < arr.columns){
        int** tarr = (int**)malloc((arr.rows+1)*sizeof(int*));
        int i; int t; int k;
        for (i=1; i<=arr.rows; i++)
            tarr[i]=(int*)malloc((arr.rows+1)*sizeof(int));
        gotoxy(x, y++);
        printf("Too many columns! Choose which (%d) to include: ", arr.rows);
        for (i=1; i<=arr.rows; i++){
            scanf("%d", &t);
            for (k=1; k<=arr.rows; k++)
                tarr[k][i]=arr.mat[k][t];
        }
        gotoxy(x, y++);
        printf("Calculated determinant: %d", det(tarr, arr.rows));
        getchar(); getchar();
        free(tarr);
    }
}
int fact(int n){
    int i; int f=1;
    for (i=1; i<=n; i++){
        f=f*i;
    }
    return f;
}
void correct(){
    int x; int y; int wx=30; int wy=10;
    window("Correct element", wx++, wy++, wx+50, wy+5, YELLOW, BLACK);
    textcolor(YELLOW);
    gotoxy(wx, wy++);
    printf("Enter number of required element: \n");
    gotoxy(wx, wy++);
    scanf("%d %d", &x, &y);
    gotoxy(wx, wy++);
    if ((x<=arr.columns) && (y<=arr.rows)){
        printf("Old number: %d. Enter new number: ", arr.mat[x][y]);
        scanf("%d", &arr.mat[x][y]);
    }
    else
        printf("Incorrect number\n");
}
void swaprow(int tgt, int src){
    int i;
    for (i=1; i<=arr.columns; i++)
        swap(&arr.mat[src][i], &arr.mat[tgt][i]);
}
void swapcol(int tgt, int src){
    int i;
    for (i=1; i<=arr.rows; i++)
        swap(&arr.mat[i][src], &arr.mat[i][tgt]);
}
void fswaprow(int tgt, int src){
    int i;
    for (i=1; i<=farr.columns; i++)
        fswap(&farr.mat[src][i], &farr.mat[tgt][i]);
}
void fswapcol(int tgt, int src){
    int i;
    for (i=1; i<=farr.rows; i++)
        fswap(&farr.mat[i][src], &farr.mat[i][tgt]);
}
void elementary(){

    int x=2; int y=2; int i; char** mn;
    window("Elementary", x++, y++, x+40, y+20, YELLOW, BLUE);
    TEXTHEADER;
    mn = (char**)malloc(10*sizeof(char*));
    for (i=0; i<20; i++){
        mn[i]=(char*)malloc(40*sizeof(char));
    }
    mn[0]="Return\0";
    mn[1]="Plus one column to another\0";
    mn[2]="Plus one row to another\0";
    mn[3]="Swap columns\0";
    mn[4]="Swap rows\0";
    mn[5]="Multiply column\0";
    mn[6]="Multiply row\0";
    int opt = menu(mn, 7, x, y, YELLOW, BLUE, BLUE, YELLOW);
    y=y+8;
    gotoxy(x, y++);
    /*printf("1. Plus one column to another\n");
    printf("2. Plus one row to another\n");
    printf("3. Swap columns\n");
    printf("4. Swap rows\n");
    printf("5. Multiply column\n");
    printf("6. Multiply row\n");*/
    int mult; int src; int tgt; int k;
    switch (opt){
        case 1: printf("Enter source column: ");
                scanf("%d", &src); NL;
                printf("Enter multiplier: ");
                scanf("%d", &mult); NL;
                printf("Enter target column: ");
                scanf("%d", &tgt);
                for (i=1; i<=arr.rows; i++)
                    arr.mat[i][tgt]=arr.mat[i][tgt]+arr.mat[i][src]*mult;
                break;
        case 2: printf("Enter source row: ");
                scanf("%d", &src); NL;
                printf("Enter multiplier: ");
                scanf("%d", &mult); NL;
                printf("Enter target row: ");
                scanf("%d", &tgt);
                for (i=1; i<=arr.columns; i++)
                    arr.mat[tgt][i]=arr.mat[tgt][i]+arr.mat[src][i]*mult;
                break;
        case 3: printf("Enter source column: ");
                scanf("%d", &src); NL;
                printf("Enter target column: ");
                scanf("%d", &tgt);
                swapcol(tgt, src);
                break;
        case 4: printf("Enter source row: ");
                scanf("%d", &src); NL;
                printf("Enter target row: ");
                scanf("%d", &tgt);
                swaprow(tgt, src);
                break;
        case 5: printf("Enter required column: ");
                scanf("%d", &tgt); NL;
                float m;
                printf("Enter multiplier: ");
                scanf("%f", &m);
                for (i=1; i<=arr.rows; i++)
                    arr.mat[i][tgt] = arr.mat[i][tgt]*m;
                break;
        case 6: printf("Enter required row: ");
                scanf("%d", &tgt); NL;
                float mt;
                printf("Enter multiplier: ");
                scanf("%f", &mt);
                for (i=1; i<=arr.columns; i++)
                    arr.mat[tgt][i] = arr.mat[tgt][i]*mt;
                break;
        }
    }
void checkbackup(){
    if (bmade == 0){
        printf("No backup has been made\n");
    }
    else
        printf("Backup has been made\n");
}
void fout(int cx, int cy, int sel){
    int i; int k; int x=60; int y=3;
    gotoxy(cx, cy++); TEXTMAT;
    printf("       ");
    for (i=1; i<=farr.columns; i++){
        printf("%d        ", i);
    }
    gotoxy(cx, cy++);
    for (i=1; i<=farr.rows; i++){
        TEXTMAT; printf("%d: ", i); TEXTDEF;
        for (k=1; k<=farr.columns; k++){
            if ((i==k) && (farr.mat[i][k]!=0))
                {TEXTIMP;}
            if (k==sel)
                {TEXTCOL;}
            printf("%8.2f ", farr.mat[i][k]); TEXTDEF;
        }
        gotoxy(cx, cy++);
    }
}
void zerorrow(int r){
    int i;
    for (i=1; i<=farr.columns; i++){
        farr.mat[r][i] = makezero(farr.mat[r][i]);
    }
}
void fmakeonefrom(int start){
    int i; int k; double c;
    for (i=start; i<=arr.rows; i++){
        c = farr.mat[i][start];
        if (c!=0){
            for (k=start; k<=arr.columns; k++)
                farr.mat[i][k]=farr.mat[i][k]/c;
        }
    }
}
void gauss(){
    int i; int k; int step = 0; int z; float max; int imax; double c; int d;
    farr.columns=arr.columns; farr.rows=arr.rows;
    farr.mat=(double**)malloc((farr.rows+1)*sizeof(double*));
    for (i=1; i<=arr.rows; i++){
        farr.mat[i]=(double*)malloc((farr.columns+1)*sizeof(double));
        for (k=1; k<=arr.columns; k++)
            farr.mat[i][k]=arr.mat[i][k];
    }
    int x=20; int y=10;
    for (z=1; z<=farr.rows; z++){
        clrscr();
        TEXTHEADER2;
        x = 20; y = 10;
        gotoxy(x, y++);
        printf("<===|Gauss method|===>\n");
        TEXTDEF;
        //Make max first;
        NL;
        printf("Step : %d", z); NL;
        fout(wherex(), wherey(), z);
        DELAY;
        for (i=z; i<=arr.rows; i++){
            if (farr.mat[i][z]<0)
                for (k=z; k<=arr.columns; k++)
                    farr.mat[i][k]=farr.mat[i][k]*(-1);
        }
        max = farr.mat[z][z]; imax = z;
        for (i=z+1; i<=arr.rows; i++){
            if (farr.mat[i][z]>max){
                max = farr.mat[i][z]; imax = i;
            }
        }
        fswaprow(z, imax);
        //Making 1s
        fmakeonefrom(z);
        for (i=z+1; i<=farr.rows; i++){
            if (farr.mat[i][z]!=0)
            for (k=1; k<=farr.columns; k++){
                    farr.mat[i][k]=farr.mat[i][k]-farr.mat[z][k];
            }
        }
        //Making zeros
        for (i=1; i<=farr.rows; i++)
            zerorrow(i);
    }
    for (z=arr.rows; z>=1; z--){
        clrscr();
        x = 20; y = 10;
        gotoxy(x, y++);
        TEXTHEADER2;
        printf("<===|Gauss method|===>\n");
        TEXTDEF;
        //Make max first;
        NL;
        printf("Reverse step : %d", z); NL;
        fout(wherex(), wherey(), z);
        DELAY;
        for (i=1; i<z; i++){
            c = farr.mat[i][z];
            for (k=z; k<=arr.columns; k++){
                farr.mat[i][k]=farr.mat[i][k]-farr.mat[z][k]*c;
            }

        }
    }
    clrscr();
    TEXTHEADER2;
    x = 20; y = 10;
    gotoxy(x, y++);
    printf("<===|Gauss method|===>\n");
    TEXTDEF;
    NL;
    printf("Result\n"); NL;
    fout(wherex(), wherey(), 0);
    getchar(); getchar();


}
int main()
{
    int option; arr.rows=0; backup.rows=0; char** mn; int i; int x; int y;
    mn = (char**)malloc(8*sizeof(char*));
    for (i=0; i<20; i++){
        mn[i]=(char*)malloc(40*sizeof(char));
    }
    while (option!=0){
        TEXTDEF; clrscr();
        if (arr.rows == 0){
            x = 45; y = 2;
            window("Matrix 4.0", x++, y, x+30, y+18, YELLOW, BLUE);
            TEXTHEADER; gotoxy(x, ++y);
            printf("No matrix entered!");
            gotoxy(x, ++y);
            checkbackup();
            mn[0]="Exit\0";
            mn[1]="Enter\0";
            mn[2]="Restore\0";
            mn[3]="Load\0";
            mn[4]="Randomize\0";
            option = menu(mn, 5, x, ++y, YELLOW, BLUE, BLUE, YELLOW);
            switch (option){
                case 0: gotoxy(1, 25); break;
                case 1: enter(); break;
                case 2: if (bmade == 1) restore(); break;
                case 3: loadfile(); break;
                case 4: randomgen(); break;
            }
        }
        else{
            out();
            x = 2; y = 2;
            window("Matrix 4.0", x++, y++, x+30, y+20, YELLOW, BLUE);
            TEXTHEADER;
            gotoxy(x, y++);
            checkbackup();
            mn[0]="Exit\0";
            mn[1]="Clear\0";
            mn[2]="Backup\0";
            mn[3]="Restore\0";
            mn[4]="Save\0";
            mn[5]="Load\0";
            mn[6]="Correct element\0";
            mn[7]="Calculate determinant\0";
            mn[8]="Elementary operations\0";
            mn[9]="Gauss method";
            option = menu(mn, 10, x, y, YELLOW, BLUE, BLUE, YELLOW);
            /*printf("\n Choose option:\n");
            printf("0. Exit\n");
            printf("1. Clear\n");
            printf("2. Backup\n");
            printf("3. Restore\n");
            printf("4. Save\n");
            printf("5. Load\n");
            printf("6. Correct element\n");
            printf("7. Calculate determinant\n");
            printf("8. Elementary operations\n");
            printf("9. Gauss method\n");
            CMD;
            scanf("%d", &option);*/
            switch (option){
                case 0: break;
                case 1: arr.rows=0; arr.columns=0; break;
                case 2: save(); break;
                case 3: restore(); break;
                case 4: savefile(); break;
                case 5: loadfile(); break;
                case 6: correct(); break;
                case 7: determinant(); break;
                case 8: elementary(); break;
                case 9: gauss(); break;
            }
        }
    }
    return 0;
}
