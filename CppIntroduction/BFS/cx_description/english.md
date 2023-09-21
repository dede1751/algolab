# BFS
Compute the distances of all vertices from a given starting vertex using
BFS.

## Input

The first line of the input contains the number $t \leq 10$ of test
cases. Each of the $t$ test cases is described as follows.

-   It starts with a line that contains three integers `n m v`,
    separated by a space, denoting the number of vertices, the number of
    edges, and the starting vertex, and such that $0 \leq n \leq 10^3$,
    $0
          \leq m \leq \binom{n}{2}$, and $0 \leq v \leq n - 1$.

-   The following $m$ lines each contain two integers `a b`, separated
    by a space, indicating that $\{a, b\}$ is an edge of the graph.

## Output

For each test case you should output one line containing the distance of
the vertices from $v$, ordered by increasing labels. If a vertex cannot
be reached, its distance is $-1$.

## Points

There is one group of test sets, worth $100$ points in total.