Gridmancer
==========

*Original problem can be found at [here](http://codecombat.com/play/level/gridmancer).*

Number of blocks: 32

I tried to go with a simple solution for this problem, before trying some recusive backtracking optimization mess. The simple solution I can up with is a greedy algorithm that searches for rows that have a width of one.

I search for rows of one because I know there MUST be a row of width one extending through that point. Pretty simple idea, but it's important.

From this fact I can build most of the grid. Any place there is a row that at some point has a width of one, I extend it as far as I can in either direction.

After there are no more places of single width, I try for greater widths until the entire grid is filled.
