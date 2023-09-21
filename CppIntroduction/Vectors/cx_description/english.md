# Vectors
In this exercise you are supposed to do some operations with vectors.

## Input

The first line of the input contains the number $t \leq 10$ of test
cases. Each of the $t$ test cases is described as follows.

-   It starts with a line that contains an integer `n`, such that $0
          \leq n \leq 10$.

-   The following line contains $n$ integers $\verb|a|_0\ \ldots\
          \verb|a|_{n - 1}$, separated by a space, such that
    $-1000 \leq a_i \leq
          1000$, for all $i \in \{0, \ldots, n - 1\}$.

-   The following line contains an integer `d`, denoting the index of an
    element that is to be removed from the vector, and such that
    $0 \leq d
          \leq n - 1$.

-   The following line contains two integers `a b`, separated by a
    space, denoting the range of indices of the elements that should be
    removed from the *remaining* vector (both inclusive), and such that
    $0 \leq a \leq b \leq n - 2$.

## Output

For each test case output one line with the remaining elements of the
vector separated by a space. If there are no elements remaining in the
vector output '`Empty`'.

## Points

There is one group of test sets, worth $100$ points in total.