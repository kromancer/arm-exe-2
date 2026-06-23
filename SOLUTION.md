# Timeline

I have "currated" my work as a commit-by-commit history, the .git dir is included.

# A small bug in the tests

The given code `tests/unit.cpp` is otherwise untouched (added some extra tests), 
except from a one-character fix to a copy-paste bug in `TEST(Restaurant, Order)`: 
it asserted `order1` was non-null twice, where the second check was propably meant for `order2`.
