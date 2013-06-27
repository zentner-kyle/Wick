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

def configure_gcc(conf):
    conf.env['use_no_gcse_for_core'] = True

def configure_non_gcc(conf):
    conf.env['use_no_gcse_for_core'] = False

def configure_mingw(conf):
    conf.env['cprogram_PATTERN'] = '%s.exe'

def add_cflags(conf, flags, alternatives=tuple()):
    try:
        conf.check(feature='cc cprogram cstdlib', cflags=flags)
    except Exception as e:
        if alternatives:
            add_cflags(conf, alternatives[0], alternatives[1:])
    else:
        conf.env.append_value('CFLAGS', flags)

def common_configure(conf):
    conf.load('compiler_c')
    if conf.find_program(conf.options.cc):
        conf.env['CC'] = conf.options.cc
        conf.env['CC_NAME'] = conf.options.cc
        conf.env['COMPILER_CC'] = conf.options.cc
        conf.env['LINK_CC'] = conf.options.cc
    if conf.env['CC'] == 'gcc':
        configure_gcc(conf)
    else:
        configure_non_gcc(conf)
    if conf.env['CC'].find('mingw') != -1:
        configure_mingw(conf)
    else:
        add_cflags(conf, '-fPIC')
    version = '-std=gnu99'
    warning_flags = [
        '-Wall',
        #'-Wextra'
    ]
    add_cflags(conf, '-std=gnu99', ['-std=c99', '-std=gnu89', '-std=c89'])
    add_cflags(conf, warning_flags)

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
        try:
            conf.check(feature='cc cprogram cstdlib', cflags=['-pg'],
                linkflags=['-pg'])
        except Exception:
            pass
        else:
            conf.env.append_value('LINKFLAGS', ['-pg'])
            conf.env.append_value('CFLAGS', ['-pg'])

def configure_debug(conf):
    conf.setenv('debug')
    common_configure(conf)
    add_cflags(conf, '-g')
    add_cflags(conf, '-DWICK_DEBUG')

def configure_release(conf):
    conf.setenv('release')
    common_configure(conf)
    add_cflags(conf, '-DNDEBUG')
    add_cflags(conf, '-DWICK_RELEASE')
    add_cflags(conf, '-O4', ['-O3'])

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
