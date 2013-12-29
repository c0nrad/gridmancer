Gridmancer
==========

*Original problem can be found at [here](http://codecombat.com/play/level/gridmancer).*

Number of blocks: 32

<pre>
  +--------------------+
  |AAAAAAAAAAAAAAAAAAAA|
  |BBBB     C   D  EEE |
  |         C   D   ff |
  |         C   D   ff |
  |      FFFCGGGD   ff |
  |  H    ccC  IDJ  ff |
  |  H    ccC  IDJ  ff |
  | KH    ccC  IDJ  ff |
  | KHLLLLLLCMMIDJdddd |
  |NK     OO      dddd |
  |NK     P       dddd |
  |N              dddd |
  |    Q      RR  dddd |
  |    Q          dddd |
  |   SQTTT       dddd |
  |   S   U       eee  |
  |   S   UVVVVVVVVVV  |
  |bbbS   UWWW  XXXX   |
  |bbbS  YUZ       a   |
  |bbbS   U            |
  +--------------------+
</pre>

I tried to go with a simple solution for this problem, before trying some recusive backtracking optimization mess. The simple solution I can up with is a greedy algorithm that searches for rows that have a width of one.

I search for rows of one because I know there MUST be a row of width one extending through that point. Pretty simple idea, but it's important.

From this fact I can build most of the grid. Any place there is a row that at some point has a width of one, I extend it as far as I can in either direction.

After there are no more places of single width, I try for greater widths until the entire grid is filled.
