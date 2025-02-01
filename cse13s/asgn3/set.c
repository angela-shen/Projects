#include "set.h"

#include <stdbool.h>
#include <stdint.h>

Set set_empty(void) {
    Set s = 0;
    return s;
}

Set set_universal(void) {
    Set s = set_empty();
    return ~s;
}

Set set_insert(Set s, uint8_t x) {
    Set t = 1;
    t <<= x;
    return (t | s);
}

Set set_remove(Set s, uint8_t x) {
    Set t = 1;
    t <<= x;
    return ((~t) & s);
}

bool set_member(Set s, uint8_t x) {
    Set t = 1;
    t <<= x;
    bool member = ((t & s) != 0);
    return member;
}

Set set_union(Set s, Set t) {
    return (s | t);
}

Set set_intersect(Set s, Set t) {
    return (s & t);
}

Set set_difference(Set s, Set t) {
    return (s & ~t);
}

Set set_complement(Set s) {
    return ~s;
}
