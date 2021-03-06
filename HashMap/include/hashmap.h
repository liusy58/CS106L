/*
* Assignment 2: HashMap template interface
*      TODO: write a comment here.
*
*      You'll notice that the commenting provided is absolutely stunning.
*      It was really fun to read through the starter code, right?
*      Please emulate this commenting style in your code, so your grader
*      can have an equally pleasant time reading your code. :)
*/

#ifndef HASHMAP_H
#define HASHMAP_H

#include <iostream>             // for cout
#include <iomanip>              // for setw, setprecision, setfill, right
#include <sstream>              // for istringstream
#include <vector>               // for vector
#include "hashmap_iterator.h"

// add any other includes that are necessary

/*
* Template class for a HashMap
*
* K = key type
* M = mapped type
* H = hash function type used to hash a key; if not provided, defaults to std::hash<K>
*
* Notes: When dealing with the Stanford libraries, we often call M the value
* (and maps store key/value pairs).
*
* However, we name it M for mapped type to avoid confusion with value_type.
* value_type is what the container is storing, which is a std::pair<const K, M>.
*
* All STL containers have a value_type and STL algorithms may use the value_type alias,
* so we try our best to follow that convention.
*
* Example:
*      HashMap<std::string, int>
*      This means K = key = std::string,
*                 M = mapped = int,
*                 value_type = std::pair<const std::string, int>.
*
* Concept requirements:
*      - H is function type that takes in some type K, and outputs a size_t.
*      - K and M must be regular (copyable, default constructible, and equality comparable).
*/
template <typename K, typename M, typename H = std::hash<K>>
class HashMap {


public:
    /*
    * Alias for std::pair<const K, M>, used by the STL (such as in std::inserter)
    * As noted above, value_type is not the same as the mapped_type!
    *
    * Usage:
    *      HashMap::value_type val = {3, "Avery"};
    *      map.insert(val);
    */
    friend class iterator;
    using value_type = std::pair<const K, M>;

    /*
    * Default constructor
    * Creates an empty HashMap with default number of buckets and hash function.
    *
    * Usage:
    *      HashMap map;
    *      HashMap map{};
    *
    * Complexity: O(B), B = number of buckets
    */
    HashMap();

    /*
    * Constructor with bucket_count and hash function as parameters.
    *
    * Creates an empty HashMap with a specified initial bucket_count and hash funciton.
    * If no hash function provided, default value of H is used.
    *
    * Usage:
    *      HashMap(10) map;
    *      HashMap map(10, [](const K& key) {return key % 10; });
    *      HashMap map{10, [](const K& key) {return key % 10; });
    *
    * Complexity: O(B), B = number of buckets
    *
    * Notes : what is explicit? Explicit specifies that a constructor
    * cannot perform implicit conversion on the parameters, or use copy-initialization.
    * That's good, as nonsense like the following won't compile:
    *
    * HashMap<int, int> map(1.0);  // double -> int conversion not allowed.
    * HashMap<int, int> map = 1;   // copy-initialization, does not compile.
    */
    explicit HashMap(size_t bucket_count, const H& hash = H());

    HashMap(const HashMap &other);
    HashMap(HashMap &&other);

    HashMap(std::initializer_list<std::pair<K, M> >list);

    template <typename interator_input>
    HashMap(interator_input begin,interator_input end);
    /*
    * Destructor.
    *
    * Usage: (implicitly called when HashMap goes out of scope)
    *
    * Complexity: O(N), N = number of elements
    */
    ~HashMap();

    /*
    * Returns the number of (K, M) pairs in the map.
    *
    * Parameters: none
    * Return value: size_t
    *
    * Usage:
    *      if (map.size() < 3) { ... }
    *
    * Complexity: O(1) (inlined because function is short)
    */
    inline size_t size() const noexcept;

    /*
    * Returns whether the HashMap is empty.
    *
    * Parameters: none
    * Return value: bool
    *
    * Usage:
    *      if (map.empty()) { ... }
    *
    * Complexity: O(1) (inlined because function is short)
    */
    inline bool empty() const noexcept;

    /*
    * Returns the load_factor, defined as size/bucket_count.
    *
    * Parameters: none
    * Return value: float
    *
    * Usage:
    *      float load_factor = map.load_factor();
    *
    * Complexity: O(1) (inlined because function is short)
    *
    * Notes: our minimal implementation does not automatically rehash when the load
    * factor is too high. If you want as an extension, you can implement automatic rehashing.
    */
    inline float load_factor() const noexcept;

    /*
    * Returns the number of buckets.
    *
    * Parameters: none
    * Return value: size_t - number of buckets
    *
    * Usage:
    *      size_t buckets = map.bucket_count();
    *
    * Complexity: O(1) (inlined because function is short)
    *
    * Notes: our minimal implementation does not automatically rehash when the load
    * factor is too high. If you want, you can implement automatic rehashing.
    *
    * What is noexcept? It's a guarantee that this function does not throw
    * exceptions, allowing the compiler to optimize this function further.
    * A noexcept function that throws an exception will automatically
    * terminate the program.
    */
    inline size_t bucket_count() const noexcept;

    /*
    * Returns whether or not the HashMap contains the given key.
    *
    * Parameters: const l-value reference to type K, the given key
    * Return value: bool
    *
    * Usage:
    *      if (map.contains("Avery")) { map.at("Avery"); ... }
    *
    * Complexity: O(1) amortized average case, O(N) worst case, N = number of elements
    *
    * Notes: Recall that when using a std::map, you use the map.count(key) function
    * (returns 0 or 1) to check if key exists. In C++20, map.contains(key) will be available.
    * Since contains feels more natural to students who've used the Stanford libraries
    * and will be available in the future, we will implement map.contains(key).
    */
    bool contains(const K& key) const noexcept;

    /*
    * Removes all K/M pairs the HashMap.
    *
    * Parameters: none
    * Return value: none
    *
    * Usage:
    *      map.clear();
    *
    * Complexity: O(N), N = number of elements
    *
    * Notes: clear removes all the elements in the HashMap and frees the memory associated
    * with those elements, but the HashMap should still be in a valid state and is
    * ready to be inserted again, as if it were a newly constructed HashMap with no elements.
    * The number of buckets should stay the same.
    */
    void clear() noexcept;

    /*
    * Inserts the K/M pair into the HashMap, if the key does not already exist.
    * If the key exists, then the operation is a no-op.
    *
    * Parameters: const l-value reference to value_type (K/M pair)
    * Return value:
    *          pair<value_type*, bool>, where:
    *              value_type* - pointer to the value_type inside a node that is just constructed.
    *              bool - true if the element was successfully added,
    *                      false if the element already existed.
    *
    * Note: in general it is a poor idea to return raw pointers to the internal nodes.
    * It is much better to return an iterator instead, which you can optionally do in milestone 8.
    *
    * Usage:
    *      HashMap<int, std::string> map;
    *      map.insert({3, "Avery"});        // inserts key = 3, value = "Avery"
    *      map.insert({3, "Anna"});         // key = 3 already exists, no-op
    *
    * Complexity: O(1) amortized average case
    */
    std::pair<value_type*, bool> insert(const value_type& value);

    /*
    * Erases a K/M pair (if one exists) corresponding to given key from the HashMap.
    * This is a no-op if the key does not exist.
    *
    * Parameters: const l-value reference to K, key to be removed.
    * Return value: true if K/M pair was found and removed, false if key was not found.
    *
    * Usage:
    *      map.erase(3);           // assuming K = int, erases element with key 3, returns true
    *
    * Complexity: O(1) amortized average case, O(N) worst case, N = number of elements
    *
    * Notes: after you implement iterators, a call to erase should maintain the
    * order of existing iterators, other than iterators to the erased K/M.
    */
    bool erase(const K& key);

    /*
    * Returns a l-value reference to the mapped value given a key.
    * If no such element exists, throws exception of type std::out_of_range.
    *
    * Parameters: key of type K.
    * Return value: l-value reference to type V, the mapped value of key.
    *
    * Usage:
    *      map.at(3) = "Avery";            // assuming {3, "Avery"} is in the map.
    *      std::string s = map.at(3);      // s = "Avery"
    *
    * Exceptions: std::out_of_range if key is not in the map.
    *
    * Complexity: O(1) amortized average case, O(N) worst case, N = number of elements
    *
    * Notes: recall that operator[], which you will implement, does not throw exceptions,
    * if a key is not found. Instead, it will create a K/M pair for that key with a default
    * mapped value.
    */
    M& at(const K& key)const;

    /*
    * Function that will print to std::cout the contents of the hash table as
    * linked lists, and also displays the size, number of buckets, and load factor.
    *
    * Parameters: none
    * Return value: none
    *
    * Usage:
    *      map.debug();
    *
    * Complexity: O(N), N = number of elements.
    *
    * Notes: debug will not compile if either K or V does not support operator<< for std::ostream.
    * this function will crash if your linked list logic is incorrect (eg. forgot to reset the
    * last node's next to nullptr). Check where the source of the compiler error comes from
    * before complaining to us that our starter code doesn't work!
    *
    * Tip: place map.debug() in various places in the test cases to figure out which operation
    * is failing. Super useful when we debugged our code.
    */
    void debug() const;

    /*
    * Resizes the array of buckets, and rehashes all elements. new_buckets could
    * be larger than, smaller than, or equal to the original number of buckets.
    *
    * Parameters: new_buckets - the new number of buckets. Must be greater than 0.
    * Return value: none
    *
    * Usage:
    *      map.rehash(30)
    *
    * Exceptions: std::out_of_range if new_buckets = 0.
    *
    * Complexity: O(N) amortized average case, O(N^2) worst case, N = number of elements
    *
    * Notes: our minimal HashMap implementation does not support automatic rehashing, but
    * std::unordered_map will automatically rehash, even if you rehash to
    * a very small number of buckets. For this reason, std::unordered_map.rehash(0)
    * is allowed and forces an unconditional rehash. We will not require this behavior.
    * If you want, you could implement this.
    *
    * This function is incomplete, and for milestone 1 you should complete the implementation.
    * The test cases provided use a probabilistic time test. If the test fails very occasionally,
    * that is totally fine, as long as most times you run the test cases, it passes that test.
    */
    void rehash(size_t new_buckets);

    M& operator[](const K& key);

    HashMap&operator=(const HashMap& other);
    HashMap&operator=(HashMap&& other);


private:

    /*
    * node structure represented a node in a linked list.
    * Each node consists of a value_type (K/M pair) and a next pointer.
    *
    * This is implemented in the private section as clients should not be dealing
    * with anything related to the node struct.
    *
    * Usage;
    *      HashMap<K, M, H>::node n;
    *      n->value = {3, 4};
    *      n->next = nullptr;
    */
    struct node {
        value_type value;
        node* next;

        /*
        * Constructor with default values, so even if you forget to set next to nullptr it'll be fine.
        *
        * Usage:
        *      node* new_node = node({key, mapped}, next_ptr);
        */
        node(const value_type& value = value_type(), node* next = nullptr) :
            value(value), next(next) {}
    };

    /*
    * Type alias for a pair of node*'s.
    *
    * This is used in find_node.
    *
    * Usage:
    *      auto& [prev, curr] = node_pair{nullptr, new node()};
    */
    using node_pair = std::pair<typename HashMap::node*, typename HashMap::node*>;

    /*
    * Finds the node N with given key, and returns a node_pair consisting of
    * the node whose's next is N, and N. If node is not found, {nullptr, nullptr}
    * is returned. If node found is the first in the list, {nullptr, node} is returned.
    *
    * Example given list: front -> [A] -> [B] -> [C] -> /
    * where A, B, C, D are pointers, then
    *
    * find_node(A_key) = {nullptr, A}
    * find_node(B_key) = {A, B}
    * find_node(C_key) = {B, C}
    * find_node(D_key) = {nullptr, nullptr}
    *
    * Usage:
    *      auto& [prev, curr] = find_node(3);
    *      if (prev == nullptr) { ... }
    *
    * Complexity: O(1) amortized average case, O(N) worst case, N = number of elements
    *
    * Notes: this function is necessary because when erasing, we need to change the
    * next pointer of the node before the one we are erasing.
    */
    node_pair find_node(const K& key) const;


    /* Private member variables */

    /*
    * instance variable: _size, the number of elements, which are K/M pairs.
    * Don't confuse this with the number of buckets!
    */
    size_t _size;

    /*
    * instance variable: _hash_function, a function (K -> size_t) that is used
    * to hash K's to determine which bucket they should be inserted/found.
    *
    * Remember to mod the output of _hash_function by _bucket_count!
    *
    * Usage:
    *      K element = // something;
    *      size_t index = _hash_function(element) % _bucket_count;
    *
    */
    H _hash_function;

    /*
    * The array (vector) of buckets. Each bucket is a linked list,
    * and the item stored in the bucket is the front pointer of that linked list.
    *
    * Usage:
    *      node* ptr = _buckets_array[index];          // _buckets_array is array of node*
    *      const auto& [key, mapped] = ptr->value;     // each node* contains a value that is a pair
    */
    std::vector<node*> _buckets_array;

    /*
    * A constant for the default number of buckets for the default constructor.
    */
    static const size_t kDefaultBuckets = 10;

    template <typename K_, typename M_, typename H_>
    friend std::ostream& operator<<(std::ostream& os, const HashMap<K_, M_, H_>& map);

    template <typename K_, typename M_, typename H_>
    friend bool operator==(const HashMap<K_, M_, H_>& lhs,
       const HashMap<K_, M_, H_>& rhs);

    template <typename K_, typename M_, typename H_>
    friend bool operator!=(const HashMap<K_, M_, H_>& lhs,
       const HashMap<K_, M_, H_>& rhs);

public:
    class iterator :public std::iterator<std::input_iterator_tag,value_type>{
    private:
        const HashMap*hashMap;
        bool is_end = true;
        int index = 0;
        node*curr_node= nullptr;
    public:
        iterator(const HashMap*mp,bool end=false):hashMap(mp),is_end(end){
            hashMap = mp;
            if(!is_end){
                while(index<hashMap->bucket_count()&&hashMap->_buckets_array[index]== nullptr)
                    ++index;
                if(index<hashMap->bucket_count()){
                    curr_node = hashMap->_buckets_array[index];
                }else{
                    is_end = true;
                }
            }
        }
        iterator(const iterator&it){
            hashMap = it.hashMap;
            index = it.index;
            is_end = it.is_end;
            curr_node = it.curr_node;
        }
        bool operator==( iterator&temp){
            return (is_end&temp.is_end)||(curr_node == temp.curr_node);
        }

        bool operator==(const iterator&temp){
            return (is_end&temp.is_end)||(curr_node == temp.curr_node);
        }
        bool operator!=( iterator&temp){
            return !((*this)==temp);
        }
        bool operator!=(const iterator&temp){
            return !((*this)==temp);
        }
        value_type& operator*()const{
            if(curr_node== nullptr)
                throw "Error! ";
            return curr_node->value;
        }
        iterator&operator++(){
            if(curr_node== nullptr){
                is_end = true;
                return *this;
            }
            curr_node = curr_node->next;
            if(curr_node== nullptr){
                ++index;
                while(index<hashMap->bucket_count()&&hashMap->_buckets_array[index]== nullptr)
                    ++index;
                if(index<hashMap->bucket_count()){
                    curr_node = hashMap->_buckets_array[index];
                }else{
                    is_end=true;
                }
            }
            return *this;
        }
        iterator operator++(int){
            iterator copy(*this);
            operator++();
           return copy;
        }

        iterator&operator=(iterator&other){
            if(*this == other)
                return *this;
            this->curr_node = other.curr_node;
            this->is_end = other.is_end;
            this->index = other.index;
            this->hashMap = other.hashMap;
            return *this;
        }

        iterator&operator=(const iterator&other){
            if(*this == other)
                return *this;
            this->curr_node = other.curr_node;
            this->is_end = other.is_end;
            this->index = other.index;
            this->hashMap = other.hashMap;
            return *this;
        }
        value_type*operator->() const{
            if(curr_node == nullptr)
                throw "error!";
            return &(curr_node->value);
        }
        bool key_equal(const K&key){
            if(is_end||curr_node== nullptr)
                return false;
            return curr_node->value.first == key;
        }
    };
    class const_iterator :public std::iterator<std::input_iterator_tag,value_type>{
    private:
        const HashMap*hashMap;
        bool is_end = true;
        int index = 0;
        node*curr_node= nullptr;
    public:
        explicit const_iterator(const HashMap*mp,bool end=false):hashMap(mp),is_end(end){
            hashMap = mp;
            if(!is_end){
                while(index<hashMap->bucket_count()&&hashMap->_buckets_array[index]== nullptr)
                    ++index;
                if(index<hashMap->bucket_count()){
                    curr_node = hashMap->_buckets_array[index];
                }else{
                    is_end = true;
                }
            }
        }
        const_iterator(const_iterator&it){
            hashMap = it.hashMap;
            index = it.index;
            is_end = it.is_end;
            curr_node = it.curr_node;
        }
        bool operator==( const_iterator&temp){
            return (is_end&temp.is_end)||(curr_node == temp.curr_node);
        }
        bool operator==(const const_iterator&temp){
            return (is_end&temp.is_end)||(curr_node == temp.curr_node);
        }
        bool operator!=(const_iterator&temp){
            return !((*this)==temp);
        }
        bool operator!=(const const_iterator&temp){
            return !((*this)==temp);
        }
        bool key_equal(const K&key){
            if(!is_end||curr_node== nullptr)
                return false;
            return curr_node->value.first == key;
        }

        const value_type& operator*()const{
            if(curr_node== nullptr)
                throw "Error! ";
            return curr_node->value;
        }
        const_iterator&operator++(){
            if(curr_node== nullptr){
                is_end = true;
                return *this;
            }
            curr_node = curr_node->next;
            if(curr_node== nullptr){
                ++index;
                while(index<hashMap->bucket_count()&&hashMap->_buckets_array[index]== nullptr)
                    ++index;
                if(index<hashMap->bucket_count()){
                    curr_node = hashMap->_buckets_array[index];
                }else{
                    is_end=true;
                }
            }
            return *this;
        }
        const_iterator operator++(int){
            const_iterator copy(*this);
            operator++();
            return copy;
        }

        const_iterator&operator=(const_iterator&other){
            if(*this == other)
                return *this;
            this->curr_node = other.curr_node;
            this->is_end = other.is_end;
            this->index = other.index;
            this->hashMap = other.hashMap;
            return *this;
        }

        const value_type*operator->()const{
            if(curr_node == nullptr)
                throw "error!";
            return &(curr_node->value);
        }
    };
    iterator begin(){
        return iterator(this,false);
    }
    iterator end(){
        return iterator(this,true);
    }

    const_iterator begin()const{
        return const_iterator(this,false);
    }
    const_iterator end()const{
        return const_iterator(this,true);
    }
    iterator find ( const K& k );
    const_iterator find ( const K& k ) const;


    iterator erase ( iterator position );
    iterator erase ( iterator first, iterator last );

};



/*
* Ask compiler to put the template implementation here.
*
* Typically we'd just put everything (interface + implementation) in the .h file
* but the file got a bit too long with the comments, so we split it up.
*/

template <typename K, typename M, typename H>
HashMap<K, M, H>::HashMap() : HashMap(kDefaultBuckets) { }

template <typename K, typename M, typename H>
HashMap<K, M, H>::HashMap(size_t bucket_count, const H& hash) :
        _size(0),
        _hash_function(hash),
        _buckets_array(bucket_count, nullptr) { }

template <typename K, typename M, typename H>
HashMap<K, M, H>::~HashMap() {
    clear();
}

template <typename K, typename M, typename H>
inline size_t HashMap<K, M, H>::size() const noexcept {
    return _size;
}

template <typename K, typename M, typename H>
inline bool HashMap<K, M, H>::empty() const noexcept {
    return size() == 0;
}

template <typename K, typename M, typename H>
inline float HashMap<K, M, H>::load_factor() const noexcept {
    return static_cast<float>(size())/bucket_count();
};

template <typename K, typename M, typename H>
inline size_t HashMap<K, M, H>::bucket_count() const noexcept {
    return _buckets_array.size();
};

template <typename K, typename M, typename H>
bool HashMap<K, M, H>::contains(const K& key) const noexcept {
    return find_node(key).second != nullptr;
}

template <typename K, typename M, typename H>
void HashMap<K, M, H>::clear() noexcept {
    for (auto& curr : _buckets_array) {
        while (curr != nullptr) {
            auto trash = curr;
            curr = curr->next;
            delete trash;
        }
    }
    _size = 0;
}

template <typename K, typename M, typename H>
std::pair<typename HashMap<K, M, H>::value_type*, bool>
HashMap<K, M, H>::insert(const value_type& value) {
    const auto& [key, mapped] = value;
    auto [prev, node_to_edit] = find_node(key);
    size_t index = _hash_function(key) % bucket_count();

    if (node_to_edit != nullptr) return {&(node_to_edit->value), false};
    _buckets_array[index] = new node(value, _buckets_array[index]);

    ++_size;
    return {&(_buckets_array[index]->value), true};
}

template <typename K, typename M, typename H>
M& HashMap<K, M, H>::at(const K& key)const {
    auto [prev, node_found] = find_node(key);
    if (node_found == nullptr) {
        throw std::out_of_range("HashMap<K, M, H>::at: key not found");
    }
    return node_found->value.second;
}

template <typename K, typename M, typename H>
typename HashMap<K, M, H>::node_pair HashMap<K, M, H>::find_node(const K& key) const {
    size_t index = _hash_function(key) % bucket_count();
    auto curr = _buckets_array[index];
    node* prev = nullptr; // if first node is the key, return {nullptr, front}
    while (curr != nullptr) {
        const auto& [found_key, found_mapped] = curr->value;
        if (found_key == key) return {prev, curr};
        prev = curr;
        curr = curr->next;
    }
    return {nullptr, nullptr}; // key not found at all.
}

template <typename K, typename M, typename H>
void HashMap<K, M, H>::debug() const {
    std::cout << std::setw(30) << std::setfill('-') << '\n' << std::setfill(' ')
              << "Printing debug information for your HashMap implementation\n"
              << "Size: " << size() << std::setw(15) << std::right
              << "Buckets: " << bucket_count() << std::setw(20) << std::right
              << "(load factor: " << std::setprecision(2) << load_factor() << ") \n\n";

    for (size_t i = 0; i < bucket_count(); ++i) {
        std::cout << "[" << std::setw(3) << i << "]:";
        auto curr = _buckets_array[i];
        while (curr != nullptr) {
            const auto& [key, mapped] = curr->value;
            // next line will not compile if << not supported for K or M
            std::cout <<  " -> " << key << ":" << mapped;
            curr = curr->next;
        }
        std::cout <<  " /" <<  '\n';
    }
    std::cout << std::setw(30) << std::setfill('-') << '\n';
}

template <typename K, typename M, typename H>
bool HashMap<K, M, H>::erase(const K& key) {
    auto [prev, node_to_erase] = find_node(key);
    if (node_to_erase == nullptr) {

        return false;
    } else {
        size_t index = _hash_function(key) % bucket_count();
        (prev ? prev->next : _buckets_array[index]) = node_to_erase->next;
        delete node_to_erase;
        --_size;
        return true;
    }
}

template <typename K, typename M, typename H>
void HashMap<K, M, H>::rehash(size_t new_bucket_count) {
    if (new_bucket_count == 0) {
        throw std::out_of_range("HashMap<K, M, H>::rehash: new_bucket_count must be positive.");
    }

    std::vector<node*> new_buckets_array(new_bucket_count);
    /* Optional Milestone 1: begin student code */

    // Hint: you should NOT call insert, and you should not call
    // new or delete in this function. You must reuse existing nodes.
    (void) new_buckets_array; // remove this line after you start implementing\

    /* end student code */
    for (size_t i = 0; i < bucket_count(); ++i) {
        auto curr = _buckets_array[i];
        while (curr != nullptr) {
            auto node = curr;
            auto value = node->value;
            curr = curr->next;
            auto index = _hash_function(value.first)%new_bucket_count;
            node->next = new_buckets_array[index];
            new_buckets_array[index] = node;
        }
    }
    _buckets_array = new_buckets_array;
}
template <typename K, typename M, typename H>
M& HashMap<K, M, H>::operator[](const K& key){
    if(!contains(key)) {
        auto res = new value_type(key, M());
        insert(*res);
    }
    return at(key);
}

template <typename K, typename M, typename H>
std::ostream& operator<<(std::ostream& os, const HashMap<K, M, H>& map){
    os<<"{";
    std::string str = "";
    for (size_t i = 0; i < map.bucket_count(); ++i) {
        auto curr = map._buckets_array[i];
        while (curr != nullptr) {
            auto node = curr;
            auto value = node->value;
            os<<str<<value.first<<":"<<value.second;
            str = ", ";
            curr = curr->next;
        }
    }
    os<<"}";
    return os;
}

template <typename K, typename M, typename H>
bool operator==(const HashMap<K, M, H>& lhs,
                const HashMap<K, M, H>& rhs){
    if(lhs.size()!=rhs.size())
        return false;
    for (size_t i = 0; i < lhs.bucket_count(); ++i) {
        auto curr = lhs._buckets_array[i];
        while (curr != nullptr) {
            auto node = curr;
            auto value = node->value;
            if(!rhs.contains(value.first)||(value.second != rhs.at(value.first))){
                return false;
            }
            curr = curr->next;
        }
    }
    return true;
}

template <typename K, typename M, typename H>
bool operator!=(const HashMap<K, M, H>& lhs,
                const HashMap<K, M, H>& rhs){
    return !(lhs == rhs);
}

template <typename K, typename M, typename H>
HashMap<K, M, H>::HashMap(HashMap const &other){
    this->_hash_function = other._hash_function;
    this->_buckets_array = std::vector<node*>(other.bucket_count(), nullptr);
    this->_size = 0;
    for (size_t i = 0; i < other.bucket_count(); ++i) {
        auto curr = other._buckets_array[i];
        while (curr != nullptr) {
            auto value = curr->value;
            auto node = std::make_pair(value.first,value.second);
            insert(node);
            curr = curr->next;
        }
    }
}

template <typename K, typename M, typename H>
HashMap<K, M, H>::HashMap(HashMap &&other){
    this->_hash_function = other._hash_function;
    this->_size = other._size;
    this->_buckets_array = other._buckets_array;

    other._buckets_array = {};
    other._size = 0 ;

}


template<typename K, typename M, typename H>
HashMap<K,M,H> &HashMap<K, M, H>::operator=(const HashMap &other) {
    if(*this == other) return *this;
    this->_hash_function = other._hash_function;
    this->_size = 0;
    this->_buckets_array = std::vector<node*>(other.bucket_count(), nullptr);
    for (size_t i = 0; i < other.bucket_count(); ++i) {
        auto curr = other._buckets_array[i];
        while (curr != nullptr) {
            auto value = curr->value;
            auto node = std::make_pair(value.first,value.second);
            insert(node);
            curr = curr->next;
        }
    }
    return *this;
}

template<typename K, typename M, typename H>
HashMap<K,M,H> &HashMap<K, M, H>::operator=(HashMap &&other) {
    if(*this == other) return *this;
    this->_hash_function = other._hash_function;
    this->_size = other._size;
    this->_buckets_array = other._buckets_array;

    other._buckets_array = {};
    other._size = 0 ;
    return *this;
}

template<typename K, typename M, typename H>
HashMap<K, M, H>::HashMap(std::initializer_list<std::pair<K, M>>list) {
    this->_size = 0;
    this->_buckets_array = std::vector<node*>(kDefaultBuckets, nullptr);
    for(auto &node:list){
        insert(node);
    }
}

template<typename K, typename M, typename H>
template<typename interator_input>
HashMap<K, M, H>::HashMap(interator_input begin,interator_input end) {
    this->_size = 0;
    this->_buckets_array = std::vector<node*>(kDefaultBuckets, nullptr);
    while(begin!=end){
        insert(*begin++);
    }
}

template<typename K, typename M, typename H>
typename HashMap<K,M,H>::iterator HashMap<K, M, H>::find(const K &k) {
    for(auto iter = begin();iter!=end();++iter){
        if(iter.key_equal(k))
            return iter;
    }
    return end();
}
template<typename K, typename M, typename H>
typename HashMap<K,M,H>::const_iterator HashMap<K, M, H>::find(const K &k)const {
    for(auto iter = begin();iter!=end();++iter){
        if(iter.key_equal(k))
            return iter;
    }
    return end();
}

template<typename K, typename M, typename H>
typename HashMap<K,M,H>::iterator HashMap<K, M, H>::erase(HashMap::iterator position) {
    if(position == end())
        return end();
    auto res = position;
    if(erase((*res).first))
        return position++;
    return position;
}

template<typename K, typename M, typename H>
typename HashMap<K,M,H>::iterator HashMap<K, M, H>::erase(HashMap::iterator first, HashMap::iterator last) {
    auto res = first;
    for(auto iter = first;iter!=last;++iter){
        res = erase(iter);
    }
    return res;
}




/*
    Milestone 2-3: begin student code

    Here is a list of functions you should implement:
    Milestone 2
        - operator[]
        - operator<<
        - operator== and !=
        - make existing functions const-correct

    Milestone 3
        - copy constructor
        - copy assignment
        - move constructor
        - move assignment
*/

#endif // HASHMAP_H
