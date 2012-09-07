#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import print_function

import hashlib
import json

class APIError(Exception):
    pass

class _serialization_Ref(object):

    serialization_path = 'wick.ref'

    def __init__(self, proxied_obj=None, serializer=None):
        self.serializer = serializer
        try:
            self.proxied_hex_id = proxied_obj.hex_identifier
        except AttributeError:
            self.proxied_obj = _Wrapper(proxied_obj, serializer)
            self.proxied_hex_id = self.proxied_obj.hex_identifier
        else:
            self.proxied_obj = proxied_obj
        self.hex_identifier = (
        hashlib.sha256(str(self.proxied_hex_id).encode('ascii')).hexdigest())

    def serialize(self, serializer, ref):
        serializer.schedule(self.proxied_obj)
        return {'proxied_hex_id': self.proxied_hex_id}

class _Wrapper(object):

    serialization_path = 'wick.wrapper'

    def __init__(self, obj, serializer):
        self.proxied_obj = obj
        self.hex_identifier = serializer.get_hex_id(obj)
            
    def serialize(self, serializer, ref):
        return {'proxied_obj': self.proxied_obj}

    @classmethod
    def deserialize(cls, data, deserializer, hex_id):
        return data['proxied_obj']


class _deserialization_Ref(object):

    def __init__(self, proxied_hex_id=None):
        self.deserializer = None
        self.proxied_hex_id = proxied_hex_id

    @classmethod
    def deserialize(cls, data, deserializer, hex_id):
        result = cls(proxied_hex_id=data['proxied_hex_id'])
        result.deserializer = deserializer
        return result

    def resolve(self):
        return self.deserializer.get_obj(self.proxied_hex_id)

class _Ref_generator(object):
    def __init__(self, serializer, ref_type):
        self.serializer = serializer
        self.ref_type = ref_type

    def __call__(self, proxied_obj):
        return self.ref_type(proxied_obj, serializer=self.serializer) 


class Serializer(object):

    def __init__(self):
        self.scheduled_objs = [] 
        # Sets can't be iterated over as they're modified, so a list is
        # needed here.

        ref_gen = _Ref_generator(self, _serialization_Ref)

        self.ref = ref_gen

    def schedule(self, obj):
        if obj not in self.scheduled_objs:
            self.scheduled_objs.append(obj)
        return self

    def schedule_list(self, objs):
        for obj in objs:
            if obj not in self.scheduled_objs:
                self.scheduled_objs.append(obj)
        return self

    def get_data(self, obj):
        try:
            serialize_func = obj.serialize
        except AttributeError:
            return obj.__dict__
        else:
            return serialize_func(self, self.ref)

    def get_path(self, obj):
        try:
            return obj.serialization_path
        except AttributeError:
            return str(obj.__class__)

    def get_hex_id(self, obj):
        try:
            result = obj.hex_identifier
        except AttributeError:
            #result = hashlib.sha256(str(id(obj)).encode('ascii')).hexdigest()
            result = hashlib.sha256(str(obj).encode('ascii')).hexdigest()
        if result is None:
            raise APIError('Null hex_identifier')
        else:
            return result

    def to_serializable(self, obj):
        return {'wick.path' : self.get_path(obj),
                'wick.data' : self.get_data(obj),
                'wick.hex_id' : self.get_hex_id(obj) }

    def dumps(self):
        result = []
        for obj in self.scheduled_objs:
            result.append(json.dumps(obj, default=self.to_serializable,
                indent=2))
        return '[' + ',\n'.join(result) + ']'


class Deserializer(object):

    def __init__(self, classes):
        self.object_paths = dict([(cls.serialization_path, cls) for cls in
            classes])
        self.object_paths['wick.ref'] = _deserialization_Ref
        self.object_paths['wick.wrapper'] = _Wrapper
        self.to_update = []
        self.objs_by_id = dict()

    def object_hook(self, obj):
        if 'wick.path' not in obj:
            return obj
        obj_path = obj['wick.path']
        obj_data = obj['wick.data']
        obj_id = int(obj['wick.hex_id'], base=16)
        result = self.object_paths[obj_path].deserialize(obj_data, self,
                obj_id)
        self.objs_by_id[obj['wick.hex_id']] = result
        return result

    def schedule_update(self, obj):
        if obj not in self.to_update:
            self.to_update.append(obj)

    def loads(self, encoded_data):
        result = json.loads(encoded_data, object_hook=self.object_hook)[0]
        for obj in self.to_update:
            obj.deserialization_update()
        return result

    def get_obj(self, hex_id):
        return self.objs_by_id[hex_id]

def run(func):
    if __name__ == '__main__':
        func()
    return func

@run
def test_serialization_process():
    class DummyObject1(object):

        serialization_path = 'wick.DummyObject1'

        def __init__(self, a):
            self.a = a

        def serialize(self, serializer, ref):
            return {'a' : self.a}

        @classmethod
        def deserialize(cls, data, deserializer, hex_id):
            return cls(data['a'])

    obj = DummyObject1('b')
    json_repr = Serializer().schedule(obj).dumps()
    print('JSON repr:\n', json_repr)
    obj_again = Deserializer([DummyObject1]).loads(json_repr)
    assert(obj_again.a == 'b')

@run
def test_serialization():
    class DummyObject1(object):

        serialization_path = 'wick.DummyObject1'

        hex_identifier = 'a'

        def __init__(self, a):
            self.a = a

        def serialize(self, serializer, ref):
            return {'a' : self.a}

        @classmethod
        def deserialize(cls, data, deserializer, hex_id):
            return cls(data['a'])

    obj = DummyObject1('b')
    json_repr = Serializer().schedule(obj).dumps()
    print('JSON repr:\n', json_repr)
    predicted_result = (
            '[{\n'\
            '  "wick.path": "wick.DummyObject1", \n'\
            '  "wick.data": {\n'\
            '    "a": "b"\n'\
            '  }, \n'\
            '  "wick.hex_id": "a"\n'\
            '}]')
    print(repr(json_repr))
    print(repr(predicted_result))
    assert(json_repr == predicted_result)

@run
def test_deserialization():
    class DummyObject1(object):

        serialization_path = 'wick.DummyObject1'

        def __init__(self, a):
            self.a = a

        def serialize(self, serializer, ref):
            return {'a' : self.a}

        @classmethod
        def deserialize(cls, data, deserializer, hex_id):
            return cls(data['a'])

    json_repr = (
            '[{\n'\
            '  "wick.path": "wick.DummyObject1", \n'\
            '  "wick.data": {\n'\
            '    "a": "b"\n'\
            '  }, \n'\
            '  "wick.hex_id": "a"\n'\
            '}]')
    print('JSON repr:\n', json_repr)
    obj = Deserializer([DummyObject1]).loads(json_repr)
    assert(obj.a == 'b')

@run
def test_ref_serialization():
    class DummyObject1(object):

        serialization_path = 'wick.DummyObject1'

        def __init__(self, a):
            self.a = a

        def serialize(self, serializer, ref):
            return {'a' : ref(self.a)}

        @classmethod
        def deserialize(cls, data, deserializer, hex_id):
            result = cls(data['a'])
            deserializer.schedule_update(result)
            return result

        def deserialization_update(self):
            self.a = self.a.resolve()

    from random import getrandbits

    class DummyObject2(object):

        serialization_path = 'wick.DummyObject2'

        def __init__(self, identifier=None):
            self.identifier = identifier if identifier else self.gen_identifier()

        def gen_identifier(self):
            return getrandbits(256)

        @property
        def hex_identifier(self):
            return hex(self.identifier)[2:].split('L')[0]

        @classmethod
        def deserialize(cls, data, deserializer, hex_id):
            return cls(data['identifier'])

    obj = DummyObject1(DummyObject2())
    json_repr = Serializer().schedule(obj).dumps()
    print('JSON repr:\n', json_repr)
    obj_again = Deserializer([DummyObject1, DummyObject2]).loads(json_repr)
    assert(obj.a.identifier == obj_again.a.identifier)

