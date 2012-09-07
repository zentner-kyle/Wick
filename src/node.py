#!/usr/bin/env python2
# -*- coding: utf-8 -*-

from __future__ import print_function

from random import getrandbits

import serialize

class HexIdentified(object):

    def gen_identifier(self):
        return getrandbits(256)

    @property
    def hex_identifier(self):
        return hex(self.identifier)[2:].split('L')[0]

class Node(HexIdentified):
    '''
    Uses the following naming scheme, like all other code in this module:
        y
    a  --->  b 
    However, a, y, and b are treated identically by the system.
    '''

    def __init__(self, a_links=None, b_links=None, y_links=None,
            identifier=None):
        self.a_links = a_links if a_links else []
        self.b_links = b_links if b_links else []
        self.y_links = y_links if y_links else []
        self.identifier = identifier if identifier is not None else self.gen_identifier()

    def add_a_link(self, a_link):
        self.a_links.append(a_link)

    def add_b_link(self, b_link):
        self.b_links.append(b_link)

    def add_y_link(self, y_link):
        self.y_links.append(y_link)

    def __hash__(self):
        return hash(self.identifier)

    def __eq__(self, other):
        if isinstance(other, Node):
            return self.identifier == other.identifier
        else:
            return False

    def __ne__(self, other):
        if isinstance(other, Node):
            return self.identifier != other.identifier
        else:
            return True

    serialization_path = 'wick.node'

    def serialize(self, serializer, ref):
        serializer.schedule_list(self.a_links)
        serializer.schedule_list(self.b_links)
        serializer.schedule_list(self.y_links)
        return  {
                'a_links' : [ref(link) for link in self.a_links],
                'b_links' : [ref(link) for link in self.b_links],
                'y_links' : [ref(link) for link in self.y_links],
                }

    @classmethod
    def deserialize(cls, data, deserializer, hex_id):
        result = cls(data['a_links'], data['b_links'], data['y_links'],
        hex_id)
        deserializer.schedule_update(result)
        return result

    def deserialization_update(self):
        self.a_links = [ref.resolve() for ref in self.a_links]
        self.b_links = [ref.resolve() for ref in self.b_links]
        self.y_links = [ref.resolve() for ref in self.y_links]

class NodeVisitor(object):
    def __init__(self, start_node):
        self.start_node = start_node
        self.done = set()
        self.scheduled = []

    def reduce(self, func):
        self.done = set()
        self.scheduled = [start_node]
        result = None
        while len(self.scheduled_nodes) > 0:
            node = self.scheduled_nodes.pop()
            self.done.add(node)
            result = func(node, result)
            self._add_links(node)
        return result

    def _add_links(node):
        for link in node.a_links:
            if link.b not in self.done and link.b not in self.scheduled:
                self.scheduled.append(link.b)
            if link.y not in self.done and link.y not in self.scheduled:
                self.scheduled.append(link.y)
        for link in node.b_links:
            if link.a not in self.done and link.a not in self.scheduled:
                self.scheduled.append(link.a)
            if link.y not in self.done and link.y not in self.scheduled:
                self.scheduled.append(link.y)
        for link in node.y_links:
            if link.a not in self.done and link.a not in self.scheduled:
                self.scheduled.append(link.a)
            if link.b not in self.done and link.b not in self.scheduled:
                self.scheduled.append(link.b)

from collections import namedtuple


class Link(HexIdentified):

    serialization_path = 'wick.link'

    def __init__(self, a, b, y, identifier=None, deserializing=False):
        self.identifier = identifier if identifier else self.gen_identifier()
        self.a = a
        self.b = b
        self.y = y
        if not deserializing:
            a.add_a_link(self)
            b.add_b_link(self)
            y.add_y_link(self)

    def serialize(self, serializer, ref):
        serializer.schedule_list((self.a, self.b, self.y))
        return  {'a' : ref(self.a), 'b' : ref(self.b), 'y' : ref(self.y)}

    @classmethod
    def deserialize(cls, data, deserializer, hex_id):
        result =  cls(data['a'], data['b'], data['y'],
        hex_id, deserializing=True)
        deserializer.schedule_update(result)
        return result

    def deserialization_update(self):
        self.a = self.a.resolve()
        self.b = self.b.resolve()
        self.y = self.y.resolve()

    def __eq__(self, other):
        if not isinstance(other, Link):
            return False
        else:
            return (self.a == other.a and self.b == other.b and self.y ==
        other.y)


class APIError(Exception):
    pass


def get_links(a=None, b=None, y=None):
    '''
    Get links found directly between the given nodes.
    Every argument is optional, and only links fullfilling all the
    requirements will be returned.
    '''

    results = []
    link_candidates = set((a.a_links if a else []) +
                          (b.b_links if b else []) +
                          (y.y_links if y else []))
    for link in link_candidates:
        if (not a or link.a == a and
            not b or link.b == b and
            not y or link.y == y):
            results.append(link)
    return results

class ForeignData(Node):
    '''
    Contains foreign python data, but is otherwise a normal Node.
    Doesn't implement anything specific for serializing the foreign data it
    holds, but does wrap it in a ref.
    '''

    serialization_path = 'wick.foreign_data'

    def __init__(self, a_links=None, b_links=None, y_links=None,
            identifier=None, deserializing=False, data=''):
        self.a_links = a_links if a_links else []
        self.b_links = b_links if b_links else []
        self.y_links = y_links if y_links else []
        self.identifier = identifier if identifier else self.gen_identifier()
        Node.__init__(self, a_links, b_links, y_links, identifier)
        self.data = data

    def serialize(self, serializer, ref):
        result = Node.serialize(self, serializer, ref)
        result['data'] = ref(self.data)
        return result

    @classmethod
    def deserialize(cls, data, deserializer, hex_id):
        result =  cls(data['a_links'], data['b_links'], data['y_links'],
        hex_id, data=data['data'], deserializing=True)
        deserializer.schedule_update(result)
        return result

    def deserialization_update(self):
        self.data = self.data.resolve()
        Node.deserialization_update(self)


def run(func):
    if __name__ == '__main__':
        func()
    return func

def add_lookup(parent, name, node_table):
    n_name = ForeignData(data=name)
    Link(n_name, node_table['mimic'], node_table['lookup'])
    Link(parent, n_name, node_table[name])
def dif_string(s1, s2):
    for c1, c2 in zip(s1, s2):
        if c1 != c2:
            print(c1[:])
            print(c2[:])

@run
def test_nodes():
    nodes = {'globals' : Node(), 'lookup' : Node(), 'mimic' :
            Node()}
    add_lookup(nodes['globals'], 'globals', nodes)
    add_lookup(nodes['globals'], 'mimic', nodes)
    add_lookup(nodes['globals'], 'lookup', nodes)
    print(serialize.Serializer().schedule(nodes['globals']).dumps())

@run
def test_nodes():
    nodes = {'globals' : Node(), 'lookup' : Node(), 'mimic' :
            Node()}
    add_lookup(nodes['globals'], 'globals', nodes)
    add_lookup(nodes['globals'], 'mimic', nodes)
    add_lookup(nodes['globals'], 'lookup', nodes)
    #print(serialize.Serializer().schedule(nodes['globals']).dumps())
    node = nodes['globals']
    s = serialize.Serializer().schedule(node).dumps()
    node_again1 = serialize.Deserializer([Node, Link, ForeignData]).loads(s)
    #print(node_again1)
    #print(node_again1.identifier)
    #print(type(node_again1.identifier))
    #print(serialize.Serializer().schedule(nodes_again1).dumps())
    s2 = serialize.Serializer().schedule(node_again1).dumps()
    node_again2 = serialize.Deserializer([Node, Link, ForeignData]).loads(s2)
    #print('\n' * 20)
    #print(s)
    #print(len(s))
    #print(repr(s))
    #print(repr(s2))
    #dif_string(repr(s), repr(s2))
    #assert(s == s2)
    #assert(node_again1 == node_again2)

def profile():
    import cProfile
    cProfile.run('for i in range(0, 100): test_nodes()', 'parse_prof')

    import pstats
    p = pstats.Stats('parse_prof')
    p.strip_dirs().sort_stats('cumulative').print_stats(10)
