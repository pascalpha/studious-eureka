# studious-eureka

Was to implement some traits for some numerical project, and end up with full support for type_traits. 
At that moment, I thought why not and embarked on implementing all STL, or at least some of it.

Almost done:
   1. type traits, with the exception of the trivial family of traits, along with others, which seem not to have an implementation in the language per se.
   2. move semantics, perfect forwarding
In progress
   1. allocator and dynamic memory management (smart pointers etc), (un-pushed and still debugging before push)
   2. variant, optional, tuple, pair, etc
TBD
   1. containers & algorithms