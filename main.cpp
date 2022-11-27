//
// Created by shlom on 27/11/2022.
//
#include "player.h"
#include <iostream>
int main(){
    Player p(1,2,3,4);
    Player p1(1,4,3,4);
    std::cout<<(p1<p);
}
