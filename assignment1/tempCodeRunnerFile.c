#include <stdio.h>
int power(int x, unsigned int y){
    int temp;
    if (y==0){
        return 1;
    }
    else{
        temp = power(x,y/2);
        if(y%2==0){
            return temp*temp;
        }else{
            return x*temp*temp;
        }
    }
}
int main(){
    power(5,3);
}