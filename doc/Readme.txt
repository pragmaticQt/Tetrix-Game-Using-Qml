Coordinate:top left is origin
(0, 0)
-----------------> X
|
|
|
|
|
v Y
Piece Control
left <- right->
down | up ^
     V    |

when set size of board
      <-      width      ->
      ---------------------   ^
      |                   |   |
      |                   | height
      |                   |   |
      ---------------------   v
when index cell inside board
       column ->
       0 1 2 ...
       _ _ _ _ _    row
      |_|_|_|_|_| 0  |
      |_|_|_|_|_| 1  V
      |_|_|_|_|_| 2  
  