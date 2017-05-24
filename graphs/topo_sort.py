#!/usr/bin/env python2


# https://www.youtube.com/watch?v=ddTC4Zovtbc


def topo_sort(graph):
    def dfs(node, visited, acc):
        visited.add(node)
        for child in graph[node]:
            if child not in visited:
                dfs(child, visited, acc)
        acc.insert(0, node)

    acc = []
    visited = set()
    for key in graph:
        if key not in visited:
            dfs(key, visited, acc)

    return acc


def test():
    graph = {
        'a': ['c'],
        'b': ['c', 'd'],
        'c': ['e'],
        'd': ['f'],
        'e': ['h', 'f'],
        'f': ['g'],
        'g': [],
        'h': []
    }

    assert topo_sort(graph) == ['b', 'd', 'a', 'c', 'e', 'f', 'g', 'h']


test()
