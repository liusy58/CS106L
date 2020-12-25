### Lecture 10: Classes I

#### File Layout

![](./images/1.png)


#### Type Aliases
```C++
using newType = oldType
```

#### Using Member Types

* Before class specifier, use typename.
```C++
template <typename T>
typename vector<T>::iterator vector<T>::insert(iterator pos,
int value) {
}
```

### Lecture 11: Const correctness

* If a function should be able to be called on a const object, it should be designated as const.


#### Summary of `const`
* const reference = a reference that cannot be used to modify the object that is being referenced.
* const method = a method of a class that can't change any class
variables and can only call other const methods.
* const object = an object declared as const that can only call its const methods.

>>Note: compiler will prefer the
non-const version if it’s not clear
which one is being invoked

#### `const_iterator`
`const_iterator` points to const objects,But the const_iterator object itself is not const,so we can perform `++` on a const_iterator but we cannot write to a const_iterator(`*iter=3`). A const iterator is a const object, cannot be reassigned but can write.


