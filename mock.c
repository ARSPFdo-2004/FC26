#include <stdio.h>
int main() {
    printf("=========================================\n");
    printf("               SCORECARD                 \n");
    printf("=========================================\n");
    printf("%15s  %2d - %-2d  %-15s\n", "Player1", 2, 1, "Player2");
    printf("-----------------------------------------\n");
    printf("%15d   Shots   %-15d\n", 5, 3);
    printf("%15d   Fouls   %-15d\n", 2, 4);
    printf("%15d  Yellows  %-15d\n", 0, 1);
    printf("%15d   Reds    %-15d\n", 0, 0);
    printf("=========================================\n");
    return 0;
}
