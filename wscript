#! /usr/bin/env python3
# encoding: utf-8
APPNAME = 'wick'
VERSION = '0.0'
top = '.'
out = 'wbuild'

from waf_extensions import declare_variants

declare_variants('debug', 'release', 'micro')

def options(opt):
    opt.add_option('--cc',
                   action='store',
                   default='clang',
                   help='The C Compiler to use.')
    opt.add_option('--cflags',
                   action='store',
                   default='',
                   help='Extra cflags.')
    opt.add_option('--strip-micro',
                   action='store',
                   default='yes',
                   choices=['yes', 'no', 'try'],
                   help='Strip micro library.')
    opt.add_option('--strip',
                   action='store',
                   default='strip',
                   help='Program to use to strip micro library.')
    opt.add_option('--computed-gotos',
                   '--use-computed-gotos',
                   action='store',
                   default='try',
                   choices=['yes', 'no', 'try'],
                   help='Whether to use computed gotos or not.')
    opt.load('compiler_c')
    opt.recurse('src')
    opt.recurse('tests')

profiling = False
build_tests = True

def configure_mingw(conf):
    conf.env['cprogram_PATTERN'] = '%s.exe'

def has_cflag(conf, flag):
    try:
        conf.check(feature='cc cprogram cstdlib', cflags=flag)
        return True
    except Exception as e:
        return False

def add_cflags(conf, flags, alternatives=tuple()):
    try:
        conf.check(feature='cc cprogram cstdlib', cflags=flags)
    except Exception as e:
        if alternatives:
            add_cflags(conf, alternatives[0], alternatives[1:])
    else:
        conf.env.append_value('CFLAGS', flags)

def common_configure(conf):
    if conf.find_program(conf.options.cc):
        conf.env['CC'] = conf.options.cc
        conf.env['CC_NAME'] = conf.options.cc
        conf.env['COMPILER_CC'] = conf.options.cc
        conf.env['LINK_CC'] = conf.options.cc
    conf.load('compiler_c')
    if conf.find_program(conf.options.strip):
        conf.env['strip'] = conf.options.strip
        conf.env['strip_micro'] = conf.options.strip_micro
    elif (conf.options.strip_micro == 'yes' or
          conf.options.strip != 'strip'):
        conf.fatal('Could not find {}.'.format(conf.options.strip))
    else:
        conf.env['strip_micro'] = 'no'
    conf.env['core_cflags'] = ''
    if has_cflag(conf, '-fno-gcse'):
        conf.env.append_value('core_cflags', ['-fno-gcse'])
    if 'mingw' in conf.env['CC']:
        configure_mingw(conf)
    else:
        add_cflags(conf, '-fPIC')
    for flag in conf.options.cflags.split(' '):
        add_cflags(conf, flag)
    version = '-std=gnu99'
    warning_flags = [
        '-Wall',
    ]
    add_cflags(conf, '-std=gnu99', ['-std=c99', '-std=gnu89', '-std=c89'])
    add_cflags(conf, warning_flags)
    add_cflags(conf, '-Wno-unused-function')

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
    add_cflags(conf, '-DWICK_DEBUG')
    add_cflags(conf, '-g')

def configure_release(conf):
    conf.setenv('release')
    common_configure(conf)
    add_cflags(conf, '-DNDEBUG')
    add_cflags(conf, '-DWICK_RELEASE')
    add_cflags(conf, '-O4', ['-O3'])

def configure_micro(conf):
    conf.setenv('micro')
    profiling = False
    common_configure(conf)
    add_cflags(conf, '-DWICK_RELEASE')
    add_cflags(conf, '-Os')
    if has_cflag(conf, '-O4'):
        conf.env.append_value('core_cflags', ['-O4'])
    elif has_cflag(conf, '-O3'):
        conf.env.append_value('core_cflags', ['-O3'])

def configure(conf):
    configure_debug(conf)
    configure_release(conf)
    configure_micro(conf)
    add_cflags(conf, '-DWICK_MICRO')
    conf.recurse('src')
    conf.recurse('tests')

def build(bld):
    if not bld.variant:
        print('Building all variants.')
        from waflib.Scripting import run_command
        run_command('build_debug')
        run_command('build_release')
        run_command('build_micro')
    else:
        bld.recurse('src')
        if build_tests:
            bld.recurse('tests')

def test(context):
    build_tests = True
    context.recurse('tests')
