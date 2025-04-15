---
marp: true
#size: 4:3
style: |
    img[alt~="center"] {
      display: block;
      margin: 0 auto;
    }
---

# **Laboratory 08**

## Paolo Joseph Baioni

### 09/04/2025


---
# Outline
- recalls on const and pointers (from Lippman, 2013)
- remarks on auto qualifier
- use case of polymorphism: building a Deep Neural Network (DNN) from scratch with Eigen
---
# Const
## warm-up

This section inherits from "C++ Primer, Fifth Edition, Lippman et al, 2013", to which is possible to refer for further insights.


```
const int i = get_size(); // ok: initialized at run time
const int j = 42; // ok: initialized at compile time
const int k; // error: k is uninitialized const
```

---
# Const
## extern
By default, const objects are local to a file.
To define a single instance of a const variable, we use the keyword extern on both its definition and declaration(s):
```
// file_1.cc 
//defines and initializes a const that is accessible to other files
extern const int bufSize = fcn();
```
```
// file_1.h
extern const int bufSize; // same bufSize as defined in file_1.cc
```
---
# Const
## References and const

```
int i = 42; 
int &r1 = i; //r1 bound to i
const int &r2 = i; //r2 also bound to i; but cannot be used to change i
r1 = 0; //r1 is not const; i is now 0
r2 = 0; //error: r2 is a reference to const

```

---
# Const
## Pointers to const
Like a reference to const, a pointer to const says nothing about whether the object to which the pointer points is const. Defining a pointer as a pointer to const affects only what we can do with the pointer.
```
const double pi = 3.14; //pi is const; its value may not be changed
double *ptr = &pi; //error: ptr is a plain pointer
const double *cptr = &pi; //ok: cptr may point to a double that is const
*cptr = 42; //error: cannot assign to *cptr
double dval = 3.14; // dval is a double; its value can be changed
cptr = &dval; // ok: but can’t change dval through cptr

```
---
# Const
## Const pointers
Unlike references, pointers are objects. Hence, as with any other object type, we can have a pointer that is itself const. Like any other const object, a const pointer must be initialized, and once initialized, its value (i.e., the address that it holds) may not be changed. We indicate that the pointer is const by putting the const after the *. This placement indicates that it is the pointer, not the pointed-to type, that is const:
```
int errNumb = 0;
int *const curErr = &errNumb; // curErr will always point to errNumb
const double pi = 3.14159;
const double *const pip = &pi; // pip is a const pointer to a const object
```
---
# Const
## Top- and low- level const
We use the term top-level const to indicate that the pointer itself is a const. When a pointer can point to a const object, we refer to that const as a low-level const.
```
int i = 0;
int *const p1 = &i; // we can’t change the value of p1; const is top-level
const int ci = 42; // we cannot change ci; const is top-level
const int *p2 = &ci; // we can change p2; const is low-level
const int *const p3 = p2; // right-most const is top-level, left-most is not
const int &r = ci; // const in reference types is always low-level
```
Tip: read from right to left

---
# Const
## Top- and low- level const
The distinction between top-level and low-level matters when we copy an object. When we copy an object, top-level consts are ignored:
```
i = ci; // ok: copying the value of ci; top-level const in ci is ignored
p2 = p3; // ok: pointed-to type matches; top-level const in p3 is ignored
```
---
# Const
## Top- and low- level const
On the other hand, low-level const is never ignored. When we copy an object, both objects must have the same low-level const qualification or there must be a conversion between the types of the two objects. In general, we can convert a nonconst to const but not the other way round:
```
int *p = p3; // error: p3 has a low-level const but p doesn’t
p2 = p3; // ok: p2 has the same low-level const qualification as p3
p2 = &i; // ok: we can convert int* to const int*
int &r = ci; // error: can’t bind an ordinary int& to a const int object
const int &r2 = i; // ok: can bind const int& to plain int
```
---
# Auto
Remarks:
 - `auto` stripes out qualifiers and &; if you want to use it, you should add by hand what is missing
 - `const &` as a function return value only makes sense if and only if all the following are satisfied:
    - the function is a member of a class, 
    - the return value can be large,
    - you expect to be able to access it "on the fly" without copying it into another variable.
---
# Auto
Go to 
[https://github.com/pacs-course/pacs-Labs/tree/main/Labs/2025/08-neural-network/auto/src/](https://github.com/pacs-course/pacs-Labs/tree/main/Labs/2025/08-neural-network/auto/src/)
and correct the `const_ref.cpp` example.

Remark: in c++20, it is possible to use
`std::source_location` instead of `__PRETTY_FUNCTION__`

---
# Neural Network
## Polymorphism example
In 
[https://github.com/pacs-course/pacs-Labs/tree/main/Labs/2025/08-neural-network/dnn/](https://github.com/pacs-course/pacs-Labs/tree/main/Labs/2025/08-neural-network/dnn/)
you can find an (incomplete) implementation of a Deep Neural Network, based on C++ STL & Eigen.

---
# Neural Network
## Polymorphism example

Namely, you have:
 - an include directory, which contains the neural network and the optimizers headers
 - a src directory, which contains the main and other source files, among which the neural network one
 - a data directory, with some data ready for tests.

---
# Neural Network
## Polymorphism example
You have to:
 - implement the optimizers as a hierarchy
 - complete the neural network implementation, paying attention to exploit polymorphism for the optimization stage

You can refer to 
[https://github.com/pjbaioni/neural-net](https://github.com/pjbaioni/neural-net)
for a possible solution and some further insight.
