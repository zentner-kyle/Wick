#!/usr/bin/env python
from __future__ import print_function

import collections

import random

def unique_id():
    return random.getrandbits(128)

# n = 100
# a = 0
# b = 1
# while n > 0:
#   temp = a + b
#   a = b
#   b = temp
#   n = n - 1

# start ->
#  { fib100 <-
#     { n <- 100
#       a <- 0
#       b <- 0
#       n != 0 ->
#         { temp <- a + b
#           a <- b
#           b <- temp
#           n <- n - 1 } }.n
#    print << fib100 }

class WickTypeError(Exception):
    pass


class Value(object):

    def __init__(self, ID=None):
        self.ID = ID if ID is not None else unique_id()

    def write(self, transaction, value):
        transaction.write(self.result(value))

    def externalize(self):
        pass


class PythonValue(Value):

    def __init__(self, val, ID=None):
        Value.__init__(self, ID)
        self.val = val 

    def result(self, val):
        return PythonValue(val, self.ID)

    def external(self):
        return self.val


class Scope(Value):

    def __init__(self, ID=None):
        Value.__init__(self, ID)
        self.defs = {}

    def result(self, val):
        if not instanceof(val, Scope):
            raise WickTypeError('Write of non-scope value to scope.')
        else:
            scope = Scope(self.ID)
            for name, ID in self.defs:
                scope.write_val_in_place(name, ID)
            return scope

    def write_val_in_place(self, name, ID):
        self.defs[name] = ID

    def get(self, name):
        return self.defs[name]


class ExternalFunction(Value):

    def __init__(self, func, val=None, ID=None):
        Value.__init__(self, ID)
        self.func = func
        self.val = val

    def result(self, val):
        if self.val is not None:
            raise WickTypeError('Repeated writes to external function.')
        return ExternalFunction(self.func, val)

    def call(self, transaction, val):
        self.write(transaction, val)

    def externalize(self):
        if self.val is not None:
            if not isinstance(self.val, PythonValue):
                raise WickTypeError('Passing non-external value to external function.')
            self.func(self.val.external())


class Transaction(object):

    def __init__(self, defs=None):
        self.defs = defs if defs is not None else collections.defaultdict(list)

    def write(self, val):
        self.defs[val.ID].append(val)

    def finalize(self):
        for ID, vals in self.defs.items():
            if len(vals) != 1:
                raise WickTypeError('Commiting multiple values for one ID.')

    def commit(self):
        raise WickTypeError('Not Implemented')

    def get(self, ID, index=0):
        return self.defs[ID][index]


class SubTransaction(Transaction):

    def __init__(self, parent, defs=None):
        Transaction.__init__(self, defs)
        self.parent = parent

    def commit(self):
        self.finalize()
        for ID, vals in self.defs:
            self.parent.write(ID, vals[0])


class RootTransaction(Transaction):

    def __init__(self, defs=None):
        Transaction.__init__(self, defs)

    def commit(self):
        self.finalize()
        for ID, vals in self.defs.items():
            vals[0].externalize()


main = Scope()
root = RootTransaction()
p = ExternalFunction(print)
root.write(p)
main.write_val_in_place('print', p.ID)
root.get(main.get('print')).call(root, PythonValue('Test'))
root.commit()
