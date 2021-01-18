row column in model 
https://doc.qt.io/qt-5/model-view-programming.html#basic-concepts

Model indexes

Model indexes provide temporary references to pieces of information, 
and can be used to retrieve or modify data via the model. 
Since models may reorganize their internal structures from time to time, 
model indexes may become invalid, and should not be stored. 

If a long-term reference to a piece of information is required, a persistent model index must be created. 
This provides a reference to the information that the model keeps up-to-date. 
Temporary model indexes are provided by the QModelIndex class, 
and persistent model indexes are provided by the QPersistentModelIndex class.

https://doc.qt.io/qt-5/model-view-programming.html#model-indexes

Coordinate:top-left is origin
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
down |  up ^
     V     |

when set size of board
      <-      width      ->
      ---------------------   ^
      |                   |   |
      |                   | height
      |                   |   |
      ---------------------   v
when index cell inside board, use point(x, y)
       x ->
       0 1 2 ...
   y   _ _ _ _ _    
   | 0|_|_|_|_|_|   
   V 1|_|_|_|_|_|   
     2|_|_|_|_|_|   
  
rotation illustration
       _ _ _ 
      |o|o|_|
      |_|o|_|
      |_|o|_| 
       _ _ _ 
      |_|_|_|
      |o|o|o|
      |o|_|_|       
       _ _ _ 
      |_|o|o|
      |_|_|o|
      |_|_|o| 

       _ _ _ 
      |_|_|_|
      |_|o|o|o
      |_|o|_|