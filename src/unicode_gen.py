#!/usr/bin/env python2
from __future__ import print_function
from collections import namedtuple

Node = namedtuple('Node', ['color', 'left', 'right', 'low', 'high', 'key'])

def mid(node):
    return (node.high + node.low) / 2.0

def makeRed(node):
    return Node('red', node.left, node.right, node.low, node.high, node.key)

def makeBlack(node):
    return Node('black', node.left, node.right, node.low, node.high, node.key)

def get(node, val, default):
    while node:
        if node.low <= val and val <= node.high:
            return node.key
        if val > mid(node):
            node = node.right
        else:
            node = node.left
    return default

var_name = 'to_find'

code_template = \
'''
if ({0.low} <= {1} && {1} <= {0.high}) {{
{5}  return {0.key};
{5}  }}
{5}if ({1} > {2}) {{
{5}  {3}
{5}  }}
{5}else {{
{5}  {4}
{5}  }}
'''[1:-1]

code_template2 = \
'''
if ({0.low} <= {1} && {1} <= {0.high}) {{
{3}  return {0.key};
{3}  }}
{3}else {{
{3}  {2}
{3}  }}
'''[1:-1]

def gen_code(node, default, indent=0):
    if not node:
        return '''return {};'''.format(default)
    if node.key == default and not node.left and not node.right:
        return '''return {};'''.format(default)
    left_code = gen_code(node.left, default, indent + 1);
    right_code = gen_code(node.right, default, indent + 1)
    if left_code != right_code:
        return code_template.format(node, var_name, int(mid(node)),
                gen_code(node.left, default, indent + 1),
                gen_code(node.right, default, indent + 1),
                '  ' * indent)
    else:
        return code_template2.format(node, var_name,
                left_code, '  ' * indent)

def get_node(node, val):
    #prev_node = None
    while node:
        #prev_node = node
        if node.low <= val and val <= node.high:
            return node
        if val > mid(node):
            node = node.right
        else:
            node = node.left
    return None
    #return prev_node

def insert(low, high, key, node):
    val = (low + high) / 2.0
    def ins(n):
        if not n:
            return Node('red', None, None, low, high, key)
        if val <= mid(n):
            return balance(n.color, ins(n.left), n.right, n.low, n.high, n.key)
        else:
            return balance(n.color, n.left, ins(n.right), n.low, n.high, n.key)
    return makeBlack(ins(node))

def balance(color, left, right, low, high, key):
    if left and left.color == 'red':
        if left.left and left.left.color == 'red':
            new_left = makeBlack(left.left)
            new_right = Node('black', left.right, right, low, high, key)
            return Node('red', new_left, new_right, left.low, left.high,
                left.key)
        elif left.right and left.right.color == 'red':
            new_left = Node('black', left.left, left.right.left, left.low,
                    left.high, left.key)
            new_right = Node('black', left.right.right, right, low, high,
                    key)
            return Node('red', new_left, new_right, left.right.low,
                    left.right.high, left.right.key)
    elif right and right.color == 'red':
        if right.left and right.left.color == 'red':
            new_left = Node('black', left, right.left.left, low, high,
                    key)
            new_right = Node('black', right.left.right, right.right, right.low,
                    right.high, right.key)
            return Node('red', new_left, new_right, right.left.low,
                    right.left.high, right.left.key)
        elif right.right and right.right.color == 'red':
            new_left = Node('black', left, right.left, low, high, key)
            new_right = makeBlack(right.right)
            return Node('red', new_left, new_right, right.low, right.high,
                    right.key)
    return Node(color, left, right, low, high, key)

def print_node(node):
    if not node:
        print('.')
    else:
        print('{}..{}:{}'.format(node.low, node.high, node.key))

def print_tree(node, indent=0):
    print('  ' * indent, end='')
    print_node(node)
    if node:
        print_tree(node.left, indent + 1)
        print_tree(node.right, indent + 1)


class Tree(object):

    def __init__(self):
        self.root = None

    def get(self, val, default):
        return get(self.root, val, default)

    def get_node(self, val):
        return get_node(self.root, val)

    def insert(self, low, high, key):
        self.root = insert(low, high, key, self.root)

    def print(self):
        print_tree(self.root)

    def get_lowest(self):
        node = self.root
        while node.left:
            node = node.left
        return node.low

    def get_highest(self):
        node = self.root
        while node.right:
            node = node.right
        return node.high

    def size(self):
        def size(node):
            if node is None:
                return 0
            else:
                return 1 + size(node.left) + size(node.right)
        return size(self.root)

    def gen_code(self, default):
        return gen_code(self.root, default)

def test_tree():
    tree = Tree()
    iterations = 1000 * 10
    for i in range(1, iterations):
        tree.insert(i * 10, i * 10 + 9, 1.0 / i)

    for i in range(1, iterations):
        assert tree.get(i * 10, 0) == 1.0 / i

    tree.print()

cat_map = { 'Lu': 'true', 'Ll': 'true', 'Lm':'true', 'Lo':'true'}

def insert_line(line, tree):
    if not line or not line[0].isdigit():
        return
    no_comment = line.split('#')[0]
    parts = [t.strip() for t in no_comment.split(';')]
    #print(parts)
    low = None
    high = None
    cat = parts[1].strip()
    cat = cat_map.get(cat, 'false')
    if '..' in parts[0]:
        r = parts[0].split('..')
        low = int(r[0], base=16)
        high = int(r[1], base=16)
    else:
        low = int(parts[0], base=16)
        high = int(parts[0], base=16)
    tree.insert(low, high, cat)

def tree_from_filename(filename):
    tree = Tree()
    with open(filename) as f:
        for line in f:
            insert_line(line, tree)
    return tree

def merge_ranges(tree):
    new_tree = Tree()
    stop = tree.get_highest()
    low = tree.get_lowest()
    while low < stop:
        high = low
        #key = tree.get_node(low).key
        key = tree.get(low, 'false')
        #while high < stop and tree.get(high + 1, 'false') == key:
        while high < stop:
            node = tree.get_node(high + 1)
            if node is None and key is 'false':
                high += 1
            elif node.key == key:
                high = node.high
            else:
                break
            #if node is not None or key is 'false':
                #high += 1

            # Try to increase high by one

            #node = tree.get_node(high + 1)
            #if node.key != key:
            #if tree.get(high, 'false') != key:
                
# If that would result in a new key, stop and do not increase high.

                #break
            #else:

# Otherwise, increase high to this node's highest value.

                #high = node.high
                #high += 1
        new_tree.insert(low, high, key)
        low = high + 1
    return new_tree

def find_key(node, key):
    if not node:
        return False
    if node.key == key:
        return True
    else:
        return find_key(node.left, key) or find_key(node.right, key)

def other(key):
    if key == 'true':
        return 'false'
    else:
        return 'true'

def check_merges(node):
    if node is None:
        return True
    out = True
    if node.left:
        out = out and find_key(node.left, other(node.key))
    if node.right:
        out = out and find_key(node.right, other(node.key))
    return out and check_merges(node.left) and check_merges(node.right)

def get_merged_tree(filename):
    tree = tree_from_filename(filename)
    d1 = {}
    for i in range(0, tree.get_highest()):
        d1[i] = tree.get(i, 'false')
    print("unmerged size", tree.size())
    mtree = merge_ranges(tree)
    d2 = {}
    for i in range(0, tree.get_highest()):
        d2[i] = mtree.get(i, 'false')
    start = None
    end = None
    for i in range(0, tree.get_highest()):
        if d1[i] != d2[i] and start is None:
            start = i
        if d1[i] == d2[i] and start is not None:
            end = i
        if start is not None and end is not None:
            print('{}..{}: {} != {}'.format(start, end, d1[start], d2[start]))
            start = None
            end = None
    #if start is not None and end is None:
        #end = tree.get_highest()
        #print('{}..{}: {} != {}'.format(start, end, d1[start], d2[start]))
        #start = None
        #end = None
    #assert d1 == d2
    assert check_merges(mtree.root)
    #tree.print()
    #mtree.print()
    print("merged size", mtree.size())
    return mtree

mtree = get_merged_tree('deps/UCD/extracted/DerivedGeneralCategory.txt')
print(mtree.gen_code('false'))
