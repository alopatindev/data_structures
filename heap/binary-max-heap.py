#!/usr/bin/env python2


from random import randrange


def extract_max(heap):
    if is_empty(heap):
        raise IndexError('heap is empty')
    result = heap[root()]
    last_item = heap.pop()
    if not is_empty(heap):
        heap[root()] = last_item
        sift_down(heap)
    return result


def sift_down(heap, i=0):
    while True:
        min_child, max_child = arg_min_max(heap, (left(i), right(i)))
        if (max_child is None) or heap[i] >= heap[max_child]:
            break
        else:
            swap(heap, i, max_child)
            i = max_child


def arg_min_max(heap, indexes):
    result_min = None
    result_max = None
    for i in indexes:
        if i < len(heap):
            if (result_min is None) or heap[i] < heap[result_min]:
                result_min = i
            if (result_max is None) or heap[i] > heap[result_max]:
                result_max = i
    return result_min, result_max


def add(heap, key):
    heap.append(key)
    sift_up(heap)


def sift_up(heap):
    i = len(heap) - 1
    while i > 0:
        p = parent(i)
        if heap[p] < heap[i]:
            swap(heap, p, i)
            i = p
        else:
            break


def heapsort(heap):
    result = []
    while not is_empty(heap):
        item = extract_max(heap)
        result.append(item)
    return result


def build_heap(heap):
    n = len(heap)
    for i in xrange(n - 1, 0, -2):
        low, high = (i - 1, i) if i % 2 == 0 else (i, i)
        heapify(heap, low, high)


def heapify(heap, low, high):
    n = len(heap)
    i = high
    while i < n and i >= low:
        p = parent(i)
        min_child, max_child = arg_min_max(heap, (left(p), right(p)))
        if not (min_child is None) and heap[p] < heap[max_child]:
            swap(heap, p, max_child)
            sift_down(heap, max_child)
            i = p
        else:
            break


def is_correct_heap(heap):
    for i in xrange(len(heap)):
        if i != root():
            p = heap[parent(i)]
            min_child, max_child = arg_min_max(heap, (left(i), right(i)))
            if not (min_child is None) and \
               (p < heap[min_child] or p < heap[max_child]):
                return False
    return True


def swap(heap, i, j):
    heap[i], heap[j] = heap[j], heap[i]


def is_empty(heap):
    return len(heap) == 0


def root(low=0):
    return low


def parent(i):
    return (i - 1) / 2


def left(i):
    return 2 * i + 1


def right(i):
    return 2 * i + 2


def test_all():
    random = lambda: randrange(0, 100)

    for size in xrange(100):
        heap = []
        for i in xrange(size):
            add(heap, random())
        assert is_correct_heap(heap)
        expected = sorted(heap, reverse=True)
        result = heapsort(heap)
        assert is_correct_heap(heap)
        assert result == expected

    for size in xrange(100):
        heap = [random() for i in xrange(size)]
        build_heap(heap)
        assert is_correct_heap(heap)


test_all()
