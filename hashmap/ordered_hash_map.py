#!/usr/bin/env python2


# https://wiki.mozilla.org/User:Jorend/Deterministic_hash_tables
# https://www.youtube.com/watch?v=BRO7mVIFt08&index=9&list=PLUl4u3cNGP61Oq3tWYp6V_F-5jb5L2iHb


from collections import OrderedDict
from random import randrange
import timeit


class Node:
    def __init__(self, key, value, next=None, prev=None):
        self.key = key
        self.value = value
        self.next = next
        self.prev = prev


class HashMap:
    def __init__(self):
        self.size = 0
        self.data_head = None
        self.data_tail = None
        self.hash_table_size = 8
        self.create_table()

    def create_table(self):
        self.hash_table = [[] for _ in xrange(self.hash_table_size)]
        node = self.data_head
        while node is not None:
            h = self.compute_hash(node.key)
            self.hash_table[h].append(node)
            node = node.next

    def maybe_grow_table(self):
        if self.size > self.hash_table_size:
            self.hash_table_size *= 2
            self.create_table()

    def maybe_shrink_table(self):
        if self.size <= self.hash_table_size / 4:
            self.hash_table_size /= 2
            self.create_table()

    def iteritems(self):
        node = self.data_head
        while node is not None:
            yield (node.key, node.value)
            node = node.next

    def items(self):
        return list(self.iteritems())

    def keys(self):
        return [key for key, _ in self.iteritems()]

    def __setitem__(self, key, value):
        h = self.compute_hash(key)

        for node in self.hash_table[h]:
            if node.key == key:
                node.value = value
                return

        node = Node(key, value, next=None, prev=self.data_tail)
        if self.data_head is None:
            self.data_head = node
        if not (self.data_tail is None):
            self.data_tail.next = node
        self.data_tail = node

        self.hash_table[h].append(node)
        self.size += 1
        self.maybe_grow_table()

    def __getitem__(self, key):
        h = self.compute_hash(key)

        for node in self.hash_table[h]:
            if node.key == key:
                return node.value

        raise KeyError(key)

    def __delitem__(self, key):
        h = self.compute_hash(key)
        for i, node in enumerate(self.hash_table[h]):
            if node.key == key:
                is_head = node == self.data_head
                is_tail = node == self.data_tail

                if not (node.prev is None):
                    node.prev.next = node.next
                if not (node.next is None):
                    node.next.prev = node.prev

                if is_head:
                    self.data_head = node.next
                if is_tail:
                    self.data_tail = node.prev

                del self.hash_table[h][i]
                del node

                self.size -= 1
                self.maybe_shrink_table()

                return

        raise KeyError(key)

    def compute_hash(self, key):
        return hash(key) % self.hash_table_size


def random_word(maxLen):
    result = ''
    n = randrange(1, maxLen + 1)
    for i in xrange(n):
        char = chr(randrange(ord('a'), ord('z') + 1))
        result += char
    return result


def run_tests():
    for size in xrange(100):
        keys = list({random_word(3) for i in xrange(size)})

        hm = HashMap()
        od = OrderedDict()

        for k in keys:
            value = randrange(10)
            od[k] = value
            hm[k] = value
            assert hm[k] == value

        assert od.keys() == hm.keys()
        assert od.items() == hm.items()

        for j in xrange(5):
            n = len(keys)
            if n > 0:
                index = randrange(n)
                key = keys[index]
                del keys[index]
                del od[key]
                del hm[key]

        assert od.keys() == hm.keys()
        assert od.items() == hm.items()


def benchmark(hm, n, word_length=10):
    items = {(random_word(word_length), randrange(10)) for i in xrange(n)}
    items = list(items)

    for k, v in items:
        hm[k] = v


def run_benchmark(hm, n):
    stmt = 'benchmark(%s(), %d)' % (hm, n)
    setup = 'from __main__ import benchmark, OrderedDict, HashMap'
    return timeit.timeit(stmt, setup=setup, number=10)


def run_benchmarks():
    maxN = 20000 + 1
    step = 1000
    for n in xrange(0, maxN, step):
        t1 = run_benchmark('OrderedDict', n)
        t2 = run_benchmark('HashMap', n)
        print n, t1, t2


if __name__ == '__main__':
    run_tests()
    run_benchmarks()


# 0 5.00679016113e-05 2.09808349609e-05
# 1000 0.0722670555115 0.0916271209717
# 2000 0.1428129673 0.189970016479
# 3000 0.224137067795 0.301007032394
# 4000 0.293581962585 0.368829965591
# 5000 0.364743947983 0.494722127914
# 6000 0.440720081329 0.581702947617
# 7000 0.515568971634 0.656297922134
# 8000 0.604732990265 0.756973981857
# 9000 0.670207023621 0.828922986984
# 10000 0.731194019318 1.02426791191
# 11000 0.804370880127 1.09801793098
# 12000 0.887900114059 1.19244885445
# 13000 0.944222927094 1.26397705078
# 14000 1.033452034 1.36154294014
# 15000 1.09127283096 1.41246795654
# 16000 1.16520500183 1.51412701607
# 17000 1.23193097115 1.62163591385
# 18000 1.3085308075 1.68243217468
# 19000 1.38884282112 1.76016998291
# 20000 1.47006607056 2.07331299782
