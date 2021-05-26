# Zeliora-2DGame
2D game created as my end of year project during my c++ programming course.

![2021-05-26 12_18_38-Project Zeliora - Vincent, Thomas - 1DAE11](https://user-images.githubusercontent.com/76394390/119651073-df563100-be24-11eb-965f-f45752f08891.png)

At the end of my first year as a Game Development student at Howest Digital Art and Entertainment (Belgium), for our C++ programming course, we were tasked to (re)create a level or 2 from of a childhood 2D games or make our own. 

I decided to create a small dungeon crawler game build on top of a grid. The motivation behind it was to experiment with procedural 2D dungeon generation as it was something I had in the back of my mind  and was curious about. 
The game is fairly basic in itself but my goal was mostly to get out of my confort zone and try out different things, hoping for the best.

# Dungeon generation
The core of this project was to create a somewhat interesting random dungeon in realtime that was fully explorable, had different rooms and corridors with enemies and a few destructible items.
The base algorithm for the dungeon is based on technique used by Phigames' title TinyKeep (algo link below). I picked that algorithm as I found it was giving nice results in terms of room spacing, size differences and the shape of the overall dungeon could be fairly easily controlled.

Due to time constraint (and becasue I couldn't fully grasp it then) I swapped Delaunay's graph triangulation for a more naive approach in which I process each graph nodes one by one and trace an edge between the current node and all the other ones if and only if the new edges doesn't already exist (undirected graph) and does not intersect an existing edge. It's not a perfect solution but was surely good enough.

# Try out with menu
Another unknown I looked into was try to build some kind of generic menu system in which I could fairly easily add new menu pages. The design I ended up with was a hierarchal data structure where child menu screens reprents sub-menus and allowed me to move up and down the menu items. For buttons/navigation and sometimes passing data, I inspired myself from C#/asp.net button event handlers and created a templated Button object to which I registered a callback function, that is invoked on click and accepts a ButtonPayload struct holding information about the state of menu and user-defined data.

I'm not 100% this was the best solution (probably not hehe) but it did seem to do the trick decently back then. Even now I'm not sure how to properly build a menu system but still haven't given up on it.

# Path finding and enemy behaviors
6 months before we were introduced to path-finding algorithms (BFS, DFS and A*) so I saw this assignement as a good opportinuty to have a go at it. To make it a bit more interesting I wanted my enemies roaming the dungeon to randomly wander the rooms and corridor but also not get caught on walls because they were too dumb to find a way around it. Wikipedia to the rescue (also my Algorithm teacher get some credits) to implmement A* that is used on the world grid, considering all tiles connected to their 8 adjacent neighbors accept wall tiles that are unwalkable abviously. Each enemies pick a random destination in a radius around them and a path is them calculated towards it (are towards the closest reachable point as it is possible that the random destination end up on a wall or outside the dungeon). 
The second use case for A* was to have enemies follow the player if the player was in range. 
I encountered one major issue, and that was that sometimes, due to their bounding box, the enemies were getting stuck in corners. A work around I found was to create a 2nd "navmesh grid", a copy a the base dungeon grid but shrank inward by one tile which would be used for the path-finding. I gave enough head room to the enmies to avoid getting stuck. 

# Other topics
- The point of the assignment was for us to use the different topics seen in class within a project (rule fo 3/5, inheritance amnd polymorphism, std algorithm, load and display textures and sprite, sounds and text).
- Load game data (player characters and enemy info, items, spritesheet, etc...) from simple XML files.
- I played around with multiple types of enemies with their own behaviors:
            - Guard: follow the player, melee attacks
            - Witch: follow the player, range attacks, flee if the player come too close
- Adding some procedurally positionned items to populate the dungeons such as destructible pots and boxes. Due to time constraint I had to settle with placing them in a pattern in the corner of rooms. It's not that bad but not ideal.
- Inventory system: Building a small inventory system in which the player stores the different armor, weapon and items they collect. The player can then equip them to improve their stats.
- Level system, difficulty scaling and stats: To be fair this was added for the sake of being there, the player can level up, a multiplier based on the player level increases enemy stats (only when they respawn) and equiped weapons affects the damage the player deals but it's far from balanced; 2-3 levels you are close unkilable.
Let's say it was more to explore the concept than having the ability to make it work decently.
- Oh yeah spawners; each dungeon rooms get a spawner or too which maintain it's population and perform respawn of enemies based on a respawn time, max enemy spawned count and a spawn radius.

# Resources
- Tiny Keep Dungeon generation [Tiny Keep](https://www.gamasutra.com/blogs/AAdonaac/20150903/252889/Procedural_Dungeon_Generation_Algorithm.php)
- A* [Wikipedia](https://en.wikipedia.org/wiki/A*_search_algorithm)


 
