# NOTES

## Data

Size of the map relative to the bot is (n - 3) * (m - 3)
To find cell the bot can move to by comparing every 3x3 to a 3x3 of '.'
create a graph while comparing the 3x3 cells, must keep track of directions
Every step has the same cost so bfs gives shortest path to destination
Start is always top-left
Destination is always bottom-right
The graph is non-directional and every node has a maximum of 4 edges, one for every cardinal direction

## Algorithm

Read input file and generate graph (every 3x3 is a node, every node has a type EMPTY or WALL, two EMPTY nodes next to each other have an edge that connects them).
Bfs from source (always \[0\]\[0\]), if \[n-3\]\[m-3\] has distance < 0 the destination can't be reached.
Populate a char array with the opposite of the direction of travel during path traversal from dest to source (recursive like print_path from bfs.c)
Print distance of destination and path from source to dest.

## Better Algorithm

Expand wall by moving one block and checking 3x3 with with current block as center, if there's a wall put wall in current block
Do Bfs on map with expanded walls.
Check distance of destination if >= 0, it can be reached.
Go back by finding previous node (the one that has distance == current distance - 1).
