#include <stdio.h> 
#include <math.h> 
 
int main() { 
 
    int a = 0, b = 0, c = 0; 
    int x = 0, y = 0; 
 
    scanf("%d %d %d", &a, &b, &c); 
    scanf("%d %d", &x, &y); 
 
    if ((a > b && a < c) || (a < b && a > c))  
            printf("%d\n", a); 
 
    else if ((b > a && b < c) || (b < a && b > c))  
            printf("%d\n", b); 
 
    else if ((c > a && c < b) || (c < a && c > b))  
            printf("%d\n", c); 
 
    else if (a == b && b == c)  
            printf("%d=%d=%d\n", a, b, c); 
 
    else if ((a == b && a > c))  
            printf("%d=%d>%d\n", a, b, c); 
 
    else if ((a == c && a > b))  
            printf("%d=%d>%d\n", a, c, b); 
 
    else if ((b == c && b > a))  
            printf("%d=%d>%d\n", b, c, a); 
 
    else if ((a == b && a < c))  
            printf("%d=%d<%d\n", a, b,c); 
 
    else if ((a == c && a < b))  
            printf("%d=%d<%d\n", a, c, b); 
 
    else if ((b == c && b < a))  
            printf("%d=%d<%d\n", b, c, a); 
 
   if ( (y > 150 && x*x + y*y < 300*300 && abs(x) < y) || (y < 0 && x*x + y*y > 300*300 && abs(x) < 300 && abs(y) < 300) ) 
        printf("inside\n"); 
    else  
        printf("outside\n"); 
 
    return 0; 
}