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

