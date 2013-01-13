#!/usr/bin/env python3

from collections import namedtuple

class UndefinedBehavior(Exception):
    '''Represents when Undefined Behavior would occur in the C program.'''
    pass

class Ptr(object):

    def deref(self):
        raise NotImplementedError('Call to Ptr.deref')

    def asint(self):
        raise NotImplementedError('Call to Ptr.asint')

class ListPtr(Ptr):

    def __init__(self, lst, index=0):
        self.lst = lst
        self.index = index

    def advanced(self, amount=1):
        return type(self)(self.lst, self.index + amount)

class CharPtr(Ptr):
    '''Represents char *
    >>> c = CharPtr('test', 1)
    >>> c.deref()
    'est'
    >>> c.advanced(1).deref()
    'st'
    >>> c.advanced(1).diff(c)
    1
    >>> c.diff(c.advanced(2))
    -2
    '''

    def __init__(self, string, index=0):
        self.string = string
        self.index = index

    def advanced(self, amount=1):
        return type(self)(self.string, self.index + amount)

    def deref(self):
        return self.string[self.index:]

    def asint(self):
        return id(self.string) + self.index

    def diff(self, other):
        if other.string != self.string:
            raise UndefinedBehavior('Comparison of CharPtr\'s from different strings.')
        else:
            return self.index - other.index

class String(object):

    def __init__(self, sptr, peptr):
        self.sptr = sptr
        self.peptr = peptr

NullPtr = None

class Match(namedtuple('Match', 'match string')):

    def __init__(self, match, string):
        assert isinstance(string, String)
        super(self).__init__(self, match, string)

# Match = namedtuple('Match', 'match string')

NO_MATCH = Match(False, NullPtr, NullPtr)

class Undef(object):
    '''Represents an undefined object.'''

    def __init__(self, name='unknown'):
        self.name = name

    def __bool__(self):
        raise UndefinedBehavior('__bool__ called on undefined object')

    def __getattr__(self, attrname):
        raise UndefinedBehavior('attempt to get attribute {} on undefined object {}'.format(attrname, self.name))

class Matcher(object):
    '''Abstract base class.'''

    def match(self, string):
        '''Check whether the string starting at charptr matches.
        Returns a valid Match or NO_MATCH.'''
        raise NotImplementedError('Call to Matcher.match')

class Str(Matcher):

    def __init__(self, string):
        self.string = string

    def match(self, string):
        return Match(string.sptr.deref().startswith(self.string), String(charptr, charptr.advanced(len(self.string))))

class Rep(Matcher):

    def __init__(self, sub, minc=0, maxc=-1):
        assert isinstance(sub, Matcher)
        self.sub = sub
        self.minc = minc
        self.maxc = maxc

    def match(self, string):
        counter = 0
        match = self.sub.Match(string)
        while match.match:
            if maxc > 0 and counter == maxc:
                break
            counter += 1
            nmatch = self.sub.match(match.string.peptr)
            match = Match(True, String(string.sptr, nmatch.string.peptr))
        if counter < minc:
            match = NO_MATCH
        return match

class Opt(Matcher):

    def __init__(self, *args):
        self.subs = args

    def match(self, string):
        for sub in self.subs:
            match = sub.match(string)
            if match.match:
                return match
        return NO_MATCH

class Cache(Matcher):

    def __init__(self, sub):
        self.sub = sub
        self.clear()

    def match(self, string):
        if self.string is None:
            self.string = string.sptr.string
        else:
            assert self.string == string.sptr.string
        try:
            # Note the lack of reference to string
            return self.cache[string.sptr.asint()]
        except KeyError:
            match = self.sub.match(string)
            self.cache[string.sptr.asint()] = match
            return match

    def clear(self):
        self.cache = dict()
        self.string = None

class Seq(Matcher):

    def __init__(self, *args):
        self.subs = args
        for sub in self.subs:
            assert isinstance(sub, Matcher)

    def match(self, string):
        peptr = string.sptr
        for sub in self.subs:
            match = sub.match(peptr)
            peptr = match.string.peptr
            if not match.match:
                return NO_MATCH
        return Match(True, String(string.sptr, peptr))
