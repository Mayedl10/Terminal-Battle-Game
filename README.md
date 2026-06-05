### Level Files
Level files are very visual. Every character represents a tile.
- `#` Wall
    - blocks movement, attacks
- `O` Hole
    - blocks movement, allows attacks
- `.` Regular floor
    - allows movement, allows attacks
- `@` Spawn point
    - displayed as regular floor, acts as regular floor
    - this is where characters can spawn
    - number of `@`s determines max players per level

The edges of the map act like walls. Tiles with items are displayed as `!`.

Example level:
```
###......
#........
#.@..#...
#...#....
...#.....
......OO.
....OO...
...O...@.
.........
```

Make sure there is no extra whitespace in any custom levels you create or download.