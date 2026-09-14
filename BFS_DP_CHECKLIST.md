# BFS + DP (state-space search) practice checklist

Track progress on grid/graph problems where BFS needs extra state
(bitmask, resource counter, direction, etc.) beyond plain `(r, c)`.

## Bitmask state — collect all targets

- [x] LC 847 — Shortest Path Visiting All Nodes — `(node, mask)`, multi-source BFS
- [x] LC 864 — Shortest Path to Get All Keys — `(r, c, mask)` — [src/N/main.cpp](src/N/main.cpp)
- [x] Cleaning Classroom — `(r, c, mask, energy)` — [src/L/main.cpp](src/L/main.cpp)

## Resource/counter state — dominance pruning

- [x] LC 1293 — Shortest Path in a Grid with Obstacles Elimination — `(r, c, k_remaining)` — [src/M/main.cpp](src/M/main.cpp)
- [ ] LC 778 — Swim in Rising Water — Dijkstra-style, `best[r][c]`
- [ ] Path With Maximum Minimum Value — Dijkstra-style, `best[r][c]`

## Direction/momentum state

- [ ] LC 1368 — Minimum Cost to Make at Least One Valid Path in a Grid — 0-1 BFS
- [ ] Minimum Number of Turns to Reach Destination — `(r, c, dir)`

## Multi-agent / synchronized state

- [ ] LC 1463 — Cherry Pickup II — DP state `(row, col1, col2)`

---

Mark an item `[x]` once solved and tested. Add new problems under the
matching category as you encounter them.
