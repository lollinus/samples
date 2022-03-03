project('samples', 'cpp',
        default_options : [
          'warning_level=3',
          'cpp_std=c++17'])

CPP_ARGS=['-Wall', '-Werror']


t = executable('time', 'time.cpp')
mi = executable('multiple_interfaces', 'multiple_interfaces.cpp')
e = executable('endian', 'endian.cpp')
c = executable('ctad', 'ctad.cpp')
v = executable('vec_sample', 'vec_sample.cpp')
containers = executable('containers', 'containers.cpp')
fib = executable('fib', 'fib.cpp', cpp_args: CPP_ARGS)
bitmask = executable('bitmask', 'bitmask.cpp', cpp_args: CPP_ARGS)

test('time test', t)
test('multiple interfaces test', mi)
test('endian test', e)
test('ctad test', c)
test('vector sample test', v)
test('containers sample test', containers)
test('fib sample test', fib)
test('bitmast sample test', bitmask)
