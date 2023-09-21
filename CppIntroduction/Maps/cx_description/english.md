# Maps
In this exercise you are supposed to do some operations with maps.

## Input

The first line of the input contains the number $t \leq 10$ of test
cases. Each of the $t$ test cases is described as follows.

-   It starts with a line that contains an integer `q`, such that $0
          \leq q \leq 10$.

-   The following $q$ lines each contain an integer and a string `a b`,
    separated by a space, such that $0 \leq a \leq 1000$ and $b$ is of
    length at most $10$, in one of the following forms:

    -   `0 b`: erase all entries with $b$ as the key;

    -   `x b`, for $x > 0$: add an entry with key $b$ and value $x$ to
        the map.

-   The following line contains a string $s$ consisting of length at
    most $10$.

## Output

For each test case output all the values with the key $s$ in a
non-decreasing order, separated by a space. If there are no elements
with the key $s$ output '`Empty`'.

## Points

There is one group of test sets, worth $100$ points in total.