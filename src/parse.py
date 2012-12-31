#!/usr/bin/env python3

from collections import namedtuple

Match = namedtuple('Match', 'match string sindex peindex')

NO_MATCH = Match(False, '', 0, 0)

class Matcher(object):

    def match(self, string, index):
        raise NotImplementedError('Call to abstract method Matcher.match')
        

class Str(matcher):

    def __init__(self, string):
        self.string = string

    def match(self, string, index):
        return Match(string[index:].startswith(self.string), self.string, index, index + len(self.string))

Null = lambda: Str('')

class Rep(matcher):

    def __init__(self, sub, minc=0, maxc=-1):
        self.sub = sub
        self.minc = minc
        self.maxc = maxc

    def match(self, string, index):
        counter = 0
        match = self.sub.match(string, index)
        while match.match:
            if maxc > 0 and counter == maxc:
                break
            counter += 1
            nmatch = self.sub.match(string, match.peindex)
            match = Match(True, match.string + nmatch.string, index, nmatch.peindex)
        if counter < minc:
            match = NO_MATCH
        return match

class Opt(matcher):

    def __init__(self, args*):
        self.subs = args

    def match(self, string, index):
        for sub in self.subs:
            match = sub.match(string, index)
            if match.match:
                return match
        return NO_MATCH

class Cache(matcher):

    def __init__(self, sub):
        self.sub = sub
        self.clear()

    def match(self, string, index):
        try:
            # Note the lack of reference to string
            return self.cache[index]
        except KeyError:
            match = self.sub.match(string, index)
            self.cache[index] = match
            return match

    def clear(self):
        self.cache = dict()

class Seq(matcher):

    def __init__(self, *args):
        self.subs = args

    def match(self, string, index):
        match = Match(True, '', index, index)
        cindex = index
        for sub in self.subs:
            match = sub.match(string, cindex)
            cindex = match.peindex
            if not match.match:
                return NO_MATCH
        return Match(True, string[index:cindex], index, cindex)
