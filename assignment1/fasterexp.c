#include <stdio.h>
#include <time.h>
clock_t start,end1,end2;
float cpu1,cpu2;

int power_fast(int x, unsigned int y){
    int temp;
    if (y==0){
        return 1;
    }
    else{
        temp = power_fast(x,y/2);
        if(y%2==0){
            return temp*temp;
        }else{
            return x*temp*temp;
        }
    }
}

int power(int x , int y){
    if (y==0){
        return 1;
    }else{
        return x*power(x,y-1);
    }
}
int main(){
    start = clock();
    printf("%d\n",power(55,6));
    end1 = clock();
    printf("%lu\n",end1-start);
    printf("%d\n",power_fast(55,6));
    end2 = clock();
    printf("%lu\n",end2-end1);


}