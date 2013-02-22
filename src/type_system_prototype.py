#!/usr/bin/env python2
from __future__ import print_function

class Form(object);

    def __hash__(self):
        return hash(str(self))

class A(Form):

    meta = "a"

    def __init__(self, name):
        self.name = name

    def __str__(self):
        return "*" + self.name

    def __eq__(self, other):
        return isinstance(other, A) and self.name == other.name

class B(Form):

    meta = "b"

    def __init__(self, name):
        self.name = name

    def __str__(self):
        return self.name

    def __eq__(self, other):
        return isinstance(other, B) and self.name == other.name

class T(Form):

    meta = "t"

    def __init__(self, tag, subtype):
        self.tag = tag
        self.subtype = subtype

    def __str__(self):
        return "{}( {} )".format(self.tag, str(self.subtype))

    def clone(self):
        return T(self.tag, self.subtype)

    def __eq__(self, other):
        return isinstance(other, T) and other.tag == self.tag and other.subtype == self.subtype

class O(Form):

    meta = "o"

    def __init__(self, *args):
        self.subtypes = args

    def clone(self, additional_subtypes):
        return O(self.subtypes + additional_subtypes)

    def __eq__(self):
        

    def __str__(self):
        return " | ".join(str(s) for s in self.subtypes)

class S(Form):

    meta = "s"

    def __init__(self, *args, **kwargs):
        self.cells = set(args)
        self.fields = kwargs

    def __eq__(self, other):
        if isinstance(other, S):
            return (self.cells == other.cells and
                    self.fields == other.fields)
        else:
            return False

    def union(self, other):
        if self == other:
            return self
        else:
            return O(self, other)

    def __str__(self):
        return "({})".format(", ".join(self.cells + ["{} :: {}".format(k, v) for k,v in self.fields.items()]))

# class Type(object):

#     def __init__(self, tag, options, fields):
#         self.tag = tag
#         self.options = options
#         self.fields = fields

# def union(t1, t2):
#     if t1.meta == t2.meta:
#         return t1.union(t2)
#     if t1.meta == "o" and t2.meta == "t":
#         return t1.clone(t2)
#     if t1.meta == "t" and t2.meta == "o":
#         return t2.clone(t1)
#     if t1.tag == "s":
#         return t1.union(t2)
#     if t2.tag == "s":
#         return t2.union(t1)

# i = T("i")
# f = T("f")
# print(i)
# print(f)
# print(union(i, f))
