#include <iostream>
#include "b.h"

class A{
    private:
    class friend B;
    struct node{
        node*next;
        int value;
    }

}