# DFS

Compute the DFS timestamps of discovery and finishing of all vertices
starting from a given vertex. The order in which the DFS traversal
visits the vertices should be such that it *always* visits the unvisited
neighbour of the current vertex *with the smallest identifier*.

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

For each test case you should output two lines: the first containing the
timestamps of discovery separated by a space and ordered by increasing
labels; the second containing timestamps of finishing separated by a
space and ordered by increasing labels. If a vertex cannot be reached,
both of its timestamps are $-1$.

## Points

There is one group of test sets, worth $100$ points in total.