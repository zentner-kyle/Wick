#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import print_function
#import node
from collections import defaultdict


def run(func):
    if __name__ == '__main__':
        func()
    return func


class SymbolTableMatcher(object):

    def __init__(self, syms):
        self.syms = syms
        self.max_lens = defaultdict(int)
        for sym in self.syms.keys():
            if len(sym) > self.max_lens[sym[0]]:
                self.max_lens[sym[0]] = len(sym)

    def __call__(self, text):
        longest = 0
        for i in range(1, self.max_lens[text[0]] + 1):
            if text[:i] in self.syms:
                longest = i
        if longest > 0:
            return True, self.syms[text[:longest]](text[:longest]), text[longest:]
        else:
            return False, None, None

import re


def match_regex(matcher, text, token_t):
    match = matcher.match(text)
    if match:
        return True, token_t(text[:match.end()]), text[match.end():]
    else:
        return False, None, None


_int_matcher = re.compile('[\d_]+')
_decimal_matcher = re.compile('\.[\d_]*')
_exponent_matcher = re.compile('(e|E)[\d_]+')


def NumberMatcher(text):
    result, token, text = match_regex(_int_matcher, text, recv)
    #result2 = None
    if result and text and text[0] == '.':
        result2, token2, text = match_regex(_decimal_matcher, text,
                transparent)
        if result2:
            token.text = token.text + token2.text
    if result and text and text[0] in 'eE':
        result2, token2, text = match_regex(_exponent_matcher, text,
                transparent)
        if result2:
            token.text = token.text + token2.text
    return result, token, text


_symbol_matcher = re.compile('''[^#[\]{}()\s`'"]*''')


def SymbolMatcher(text):
    return match_regex(_symbol_matcher, text, recv)

_whitespace_matcher = re.compile('\s+')


def WhitespaceMatcher(text):
    return match_regex(_whitespace_matcher, text, transparent)

_comment_matcher = re.compile('#[^\n]*')


def CommentMatcher(text):
    return match_regex(_comment_matcher, text, transparent)


def QuoteMatcher(text):
    if text[0] != "'" and text[0] != '"':
        return False, None, None
    i = 1
    end = None
    try:
        while True:
            if text[i] == '\\':
                i += 1
                get_escape_char(text[i])  # TODO
            if text[i] == text[0]:
                i += 1
                break
            i += 1
    except IndexError:
        raise InvalidToken('Unfinished string: ' + text[:16] +
                '...')
    return True, recv(text[:i]), text[i:]


class TokenGenerator(object):

    def __init__(self, functions, text):
        self.functions = functions
        self.text = text

    def next(self):
        if not self.text:
            raise StopIteration()
        for func in self.functions:
            matched, token, text = func(self.text)
            if matched:
                self.text = text
                return token
        raise InvalidToken(text[:16] + '...')

    def __iter__(self):
        return self


class InvalidToken(Exception):
    pass

class State(object):

    def __init__(self):
        self.automata = None
        self.states = dict()

    def react(self, event_type=None, *args, **kwargs):
        pass

    def add_transition(self, name, state):
        self.states[name] = state
        return self

    def transition(self, target_name):
        return self.states[target_name]

class Automata(object):

    def __init__(self, starting_state=None):
        self.states = dict()
        if starting_state is None:
            self.add_state('start', State())
        else:
            self.add_state('start', starting_state)
        self.current_state = self.state('start')

    def add_state(self, state_name, state):
        state.automata = self
        self.states[state_name] = state
        return self

    def state(self, state_name):
        return self.states[state_name]

    def enter_state(self, state_name):
        new_state = self.state(state_name)
        return self._enter_state(new_state)

    def _enter_state(self, new_state):
        self.current_state.react('exit')
        new_state.react('enter')
        self.current_state = new_state

    def react(self, event_type=None, *args, **kwargs):
        return self.current_state.react(event_type, *args, **kwargs)

    def transition(self, target_name):
        target = self.current_state.transition(target_name)
        self._enter_state(target)


class BindSite(object):

    def __init__(self, direction, strength=0, filter_func=None):
        self.direction = direction
        self.strength = strength
        self.filter_func = filter_func

    def __repr__(self):
        return ('BindSite({0}, {1})'
            .format(repr(self.direction),
            self.strength))

class TokenState(State):

    def __init__(self, bind_sites):
        self.bind_sites = bind_sites
        State.__init__(self)

class AutoToken(Automata):

    def __init__(self, text, starting_state=None):
        Automata.__init__(self, starting_state)
        self.parent = None
        self.text = text

    def get_bind_sites(self):
        return self.current_state.bind_sites

    def __repr__(self):
        return ('AutoToken({0})'.format(repr(self.text)))

    def __str__(self):
        return self.text

class RecvToken(AutoToken):

    def __repr__(self):
        return ('RecvToken({0})'.format(repr(self.text)))

    def react(self, event_type, *args, **kwargs):
        if event_type == 'token':
            self.transition('token')
        elif event_type == 'recv':
            self.transition('recv')

class InfixToken(AutoToken):

    def __init__(self, text, starting_state=None):
        AutoToken.__init__(self, text, starting_state)
        self.left = None
        self.right = None

    def __repr__(self):
        return ('InfixToken({0})'.format(repr(self.text)))

    def __str__(self):
        return '({0} {1} {2})'.format(self.text, str(self.left),
                str(self.right))

    def react(self, event_type, token=None, *args, **kwargs):
        if event_type == 'token':
            self.transition('token')
        elif event_type == 'recv':
            self.transition('recv')
        elif event_type == 'right':
            self.right = token
            token.parent = self
            self.transition('right')
            token.react('recv')
        elif event_type == 'left':
            self.left = token
            self.transition('left')
            token.parent = self
            token.react('recv')

class AutomataFactory(object):

    automata_type = Automata

    def __init__(self, graph, states, automata_type=None):
        self.states = states
        if automata_type is not None:
            self.automata_type = automata_type
        for state_name,transitions in graph.items():
            for transition, inner_state in transitions.items():
                self.states[state_name].add_transition(transition,
                        self.states[inner_state])

    def __call__(self, *args, **kwargs):
        return self.automata_type(*args, starting_state=self.states['start'],
                **kwargs)

class Recv(object):

    def __init__(self):
        recv_bindsite = BindSite('recv')
        self.automata_factory = AutomataFactory(
                {   
                    'start' : {'token' : 'token'},
                    'token' : {'recv' : 'end'}},
                {
                    'start' : TokenState([BindSite('token')]),
                    'token' : TokenState([recv_bindsite]),
                    'end'   : TokenState([])
                },
                automata_type=RecvToken)

    def __call__(self, text):
        return self.automata_factory(text)

    def react(self, event_type, token=None, *args, **kwargs):
        if event_type == 'token':
            self.transition('token')

recv = Recv()


class Infix(object):

    def __init__(self, left_strength, right_strength=None):
        if right_strength is None:
            right_strength = left_strength
        lbs = BindSite('left', left_strength)
        rbs = BindSite('right', right_strength)
        recv_bindsite = BindSite('recv')
        self.automata_factory = AutomataFactory(
                {   
                    'start' : {'token' : 'token'},
                    'token' : {'left'  : 'right', 'right' : 'left'},
                    'right' : {'right' : 'recv'},
                    'left'  : {'left'  : 'recv'},
                    'recv'  : {'recv'  : 'end'}
                },
                {
                    'start' : TokenState([BindSite('token')]),
                    'token' : TokenState([lbs, rbs]),
                    'right' : TokenState([rbs]),
                    'left'  : TokenState([lbs]),
                    'recv'  : TokenState([recv_bindsite]),
                    'end'   : TokenState([])
                },
                automata_type=InfixToken)

    def __call__(self, text):
        return self.automata_factory(text)

class PrefixToken(AutoToken):

    def __init__(self, text, right_strength):
        AutoToken.__init__(self, text, TokenState([BindSite('right',
            right_strength).add_transition('right', TokenState([]))]))

    def __repr__(self):
        return ('PreixToken({0}, {1})'.format(repr(self.text),
            self.right_strength))

    def __str__(self):
        return '({0} {1})'.format(self.text, str(self.right))

    def react(self, event_type, token=None, *args, **kwargs):
        if event_type == 'token':
            self.transition('token')
        elif event_type == 'right':
            self.right = token
            self.transition('right')

class TransparentToken(AutoToken):

    def react(self, event_type, token=None, *args, **kwargs):
        if event_type == 'token':
            self.transition('token')

    def __repr__(self):
        return ('TransparentToken({0})'.format(repr(self.text)))

class Transparent(object):

    def __init__(self):
        self.automata_factory = AutomataFactory(
                {   
                    'start' : {'token' : 'token'}},
                {
                    'start' : TokenState([BindSite('token')]),
                    'token' : TokenState([])
                },
                automata_type=TransparentToken)

    def __call__(self, text):
        return self.automata_factory(text)

transparent = Transparent()

class TParser(object):

    def __init__(self, token_generator):
        self.tokens = list(token_generator)
        self.token_index = 0
        self.bind_site_index = 0

    def run(self):
        i = 0
        while self.bind_sites_remain(i):
            if self.compare_rule(i, ['right', 'recv', 'left']):
                #self.print_bind_sites()
                #print('Middle rule called, index = {0}.'.format(i))
                rbs = self.bind_site_idx(i)
                right = self.token_idx(i)
                recv = self.token_idx(i + 1)
                left = self.token_idx(i + 2)
                lbs = self.bind_site_idx(i + 2)
                if rbs.strength <= lbs.strength:
                    left.react('left', recv)
                elif rbs.strength > lbs.strength:
                    right.react('right', recv)
                i = 0
                continue
            elif self.compare_rule(i, ['recv', 'left']):
                #self.print_bind_sites()
                #print('Left rule calle, index = {0}.'.format(i))
                recv = self.token_idx(i)
                #print('recv:', recv)
                left = self.token_idx(i + 1)
                left.react('left', recv)
                i = 0
                continue
            elif self.compare_rule(i, ['right', 'recv']):
                #self.print_bind_sites()
                #print('Right rule calle, index = {0}.'.format(i))
                right = self.token_idx(i)
                recv = self.token_idx(i + 1)
                right.react('right', recv)
                i = 0
                continue
            elif self.compare_rule(i, ['take', 'left']):
                left = self.token_idx(i + 1)
                left.react('token')
                i = 0
                continue
            elif self.compare_rule(i, ['take', 'token']):
                take = self.token_idx(i)
                token = self.token_idx(i + 1)
                token.react('token')
                take.react('take', token)
                i = 0
                continue
                
            i += 1
    def print_bind_sites(self):
        print()
        j = 0
        while self.bind_sites_remain(j):
            print('bindsite[{0}]'.format(j), self.bind_site_idx(j))
            j += 1

    def bind_site_idx(self, index):
        bind_site_idx = 0
        for t in self.tokens:
            n_bind_sites = len(t.get_bind_sites())
            if bind_site_idx + n_bind_sites > index:
                # bind site is in this token
                return t.get_bind_sites()[index - bind_site_idx]
            else:
                bind_site_idx += n_bind_sites
        raise IndexError()

    def token_idx(self, bind_site_index):
        bind_site_idx = 0
        for t in self.tokens:
            n_bind_sites = len(t.get_bind_sites())
            if bind_site_idx + n_bind_sites > bind_site_index:
                # bind site is in this token
                return t
            else:
                bind_site_idx += n_bind_sites
        raise IndexError()

    def bind_sites_remain(self, index):
        try:
            self.bind_site_idx(index)
        except IndexError:
            return False
        else:
            return True

    def compare_rule(self, index, rule):
        try:
            for i in range(len(rule)):
                #print('Comparing', rule[i], self.bind_site_idx(index + i
                    #).direction)
                if rule[i] != self.bind_site_idx(index + i).direction:
                    return False
        except IndexError:
            return False
        return True

#class PrinterAutomaton(AutoToken):

    #def react(self, event_type=None, *args, **kwargs):
        #print(event_type, 'args =', args, 'kwargs =', kwargs)
        #return self.current_state.react(event_type, *args, **kwargs)

#@run
def test_TParser():
    pa = PrinterAutomaton()
    TParser([['right', 'recv', 'left'], ['right', 'left'], ['recv', 'left'],
        ['right', 'recv'], ['compound recv', 'left'], 
        ['right', 'compound recv'], ], [BindSite(pa, 'right', 9),
            BindSite(pa, 'recv'), BindSite(pa, 'left', 10)]).run()

#@run
def test_tokenator():
    t_gen = TokenGenerator([
        WhitespaceMatcher,
        SymbolTableMatcher(['[', ']', '(', ')', '{', '}', '`', '-',
            '+', '=', '|', '!', '~', '@', '$', '%', '^', '&', '*', '-(', ')->',
            '-->', '/', '<', ',', '.', '>',';', ':']),
        NumberMatcher,
        QuoteMatcher,
        SymbolMatcher,
        CommentMatcher
        ], 'x[] --> y -1_000e10 = "test"\n `k')
    print([token.text for token in t_gen])


@run
def test_tokenator():
    from functools import partial
    t_gen = TokenGenerator([
        WhitespaceMatcher,
        SymbolTableMatcher({
            '+' : Infix(4),
            '-' : Infix(4),
            '*' : Infix(7),
            '/' : Infix(7),
            '=' : Infix(3),
            }),
        NumberMatcher,
        QuoteMatcher,
        SymbolMatcher,
        CommentMatcher
        ], 'y = x + 10 / 2 * 15')
    results = list(t_gen)
    #print(results)
    #print([token.text for token in results])
    #print([token.get_bind_sites() for token in results])
    TParser(results).run()
    #print([t.__dict__ for t in results])
    print([str(t) for t in results])
    #print(list(t_gen))

def profile():
    import cProfile
    cProfile.run('for i in range(0, 100): test_tokenator()', 'parse_prof')

    import pstats
    p = pstats.Stats('parse_prof')
    p.strip_dirs().sort_stats('cumulative').print_stats(10)

def add_lookup(parent, name, node_table):
    n_name = node.ForeignData(name)
    node.Link(n_name, node_table["Inherit"], node_table["Lookup"])
    node.Link(parent, n_name, node_table[name])

def test_nodes():
    nodes = {"Global" : node.Node(), "Lookup" : node.Node(), "Inherit" :
            node.Node()}
    add_lookup(nodes["Global"], "Global", nodes)
    add_lookup(nodes["Global"], "Inherit", nodes)
    add_lookup(nodes["Global"], "Lookup", nodes)
    print(nodes["Global"].string(set(), set()))
