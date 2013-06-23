#! /usr/bin/env python3
# encoding: utf-8
APPNAME = 'wick'
VERSION = '0.0'
top = '.'
out = 'wbuild'

from waf_extensions import declare_variants

declare_variants('debug', 'release')

def options(opt):
    opt.add_option('--cc',
                   action='store',
                   default='clang',
                   help='The C Compiler to use.')
    opt.add_option('--computed-gotos',
                   '--use-computed-gotos',
                   action='store',
                   default='try',
                   choices=['yes', 'no', 'try'],
                   help='Whether to use computed gotos or not.')
    opt.load('compiler_c')
    opt.recurse('deps')
    opt.recurse('src')
    opt.recurse('tests')

profiling = False
build_tests = True

def common_configure(conf):
    if conf.find_program(conf.options.cc):
        conf.env['CC'] = conf.options.cc
    conf.load('compiler_c')
    version = '-std=c99'
    version = '-std=gnu99'
    warning_flags = ['-Wall',
                     # '-Wextra'
    ]
    conf.check(feature='cc cprogram cstdlib', cflags=warning_flags + [version])
    conf.env.append_value('CFLAGS', warning_flags + [version])
    if conf.options.computed_gotos in ['yes', 'try']:
        need_computed_gotos = conf.options.computed_gotos == 'yes'
        have_computed_gotos = conf.check_cc(
            msg = 'Checking for computed gotos',
            fragment = 'int main() {'
            'void * target_val;'
            'target: target_val = &&target;'
            'goto *target_val;'
            'return 0;}',
            define_name = 'HAVE_COMPUTED_GOTOS',
            mandatory = need_computed_gotos
        )
        if have_computed_gotos:
            conf.define('USE_COMPUTED_GOTOS', '')
    if profiling:
        conf.check(feature='cc cprogram cstdlib', cflags=['-pg'],
            linkflags=['-pg'])
        conf.env.append_value('LINKFLAGS', ['-pg'])
        conf.env.append_value('CFLAGS', ['-pg'])

def configure_debug(conf):
    conf.setenv('debug')
    common_configure(conf)
    conf.check(cflags=['-g'])
    conf.env.append_value('CFLAGS', ['-DWICK_DEBUG'])
    conf.env.append_value('CFLAGS', ['-g'])

def configure_release(conf):
    conf.setenv('release')
    common_configure(conf)
    conf.env.append_value('CFLAGS', ['-DNDEBUG'])
    conf.env.append_value('CFLAGS', ['-DWICK_RELEASE'])
    try:
        conf.check(cflags=['-O4'])
    except Exception as e:
        conf.check(cflags=['-O3'])
        conf.env.append_value('CFLAGS', ['-O3'])
    else:
        conf.env.append_value('CFLAGS', ['-O4'])

def configure(conf):
    configure_debug(conf)
    configure_release(conf)
    conf.recurse('deps')
    conf.recurse('src')
    conf.recurse('tests')

def build(bld):
    if not bld.variant:
        print('Building all variants.')
        from waflib.Scripting import run_command
        run_command('build_debug')
        run_command('build_release')
    else:
        bld.recurse('deps')
        bld.recurse('src')
        if build_tests:
            bld.recurse('tests')

def test(context):
    build_tests = True
    context.recurse('tests')
