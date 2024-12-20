Based on an [algorithm described by u/JochCool on Reddit](https://www.reddit.com/r/adventofcode/comments/1hhiawu/2024_day_18_part_2_visualization_of_my_algorithm/)
I implemented a [DSU version](./dsu.c).

My old version (with part 1 disabled)
```
$ time ./both < input
part 2: xx,xx

real	0m0,244s
user	0m0,243s
sys	0m0,000s
```

New version:
```
$ time ./dsu < input 
xx,xx

real	0m0,005s
user	0m0,005s
sys	0m0,001s
```
