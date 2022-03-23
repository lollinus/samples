project('samples', 'cpp',
        default_options : [
          'warning_level=3',
          'cpp_std=c++17'])

CPP_ARGS=['-Wall'] # '-Werror'

progs = [['time', 'time.cpp'],
         ['mi', 'multiple_interfaces.cpp'],
         ['endian', 'endian.cpp'],
         ['ctad', 'ctad.cpp'],
         ['vec_sample', 'vec_sample.cpp'],
         ['containers', 'containers.cpp'],
         ['fib', 'fib.cpp'],
         ['bitmask', 'bitmask.cpp'],
         ['conversions', 'conversions.cpp']]

foreach p : progs
  e = executable(p[0], p[1], cpp_args: CPP_ARGS)
  test(p[0], e)
endforeach
