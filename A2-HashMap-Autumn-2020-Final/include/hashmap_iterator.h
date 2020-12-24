#ifndef HASHMAPITERATOR_H
#define HASHMAPITERATOR_H
/* Milestone 6 (optional): implement iterators for your HashMap class */
#include "hashmap.h"

class Iterator{
private:
    std::shared_ptr<std::vector<node*>> array;
    size_t index;
    node*curr_node;
public:
    Iterator(std::vector<node*>&_buckets_array,size_t _index=0,node*_node= nullptr):index(_index),curr_node(_node){
        this->array = &_buckets_array;
        while(index<array.size()&&array[index]== nullptr){
            index++;
        }
        if(index != array.size()){
            curr_node = array[index];
        }
    }
    bool operator!=(Iterator temp){
        return curr_node!= temp.curr_node;
    }

    value_type& operator*(){
        return curr_node->value;
    }
    Iterator&operator++(){
        curr_node = curr_node->next;
        if(curr_node == nullptr){
            while(index<array.size()&&array[index]== nullptr){
                index++;
            }
        }
        if(index != array.size()){
            curr_node = array[index];
        }
        return *this;
    }
};








// "White. A blank page or canvas. His favorite. So many possibilities...""









#endif // HASHMAPITERATOR_H
