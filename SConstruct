# vim: filetype=python: tw=100


Env = Environment()
Env.Append(CCFLAGS = [ '-Wall'
                     , '-Wextra'
                     #, '-Werror'
                     , '-g'
                     , '-O0'
                     , '-std=c++11' ])
Env.Append(CPPPATH   = ['include' ])
Env.Append(LINKFLAGS = ['-rdynamic', '-lrt' ])                            

lib = Env.Library('posix', source = Glob('src/*.cc'))

t0  = Env.Program('test-root', source = ['test/tst0.cc', lib])
t1  = Env.Program('test-user', source = ['test/tst1.cc', lib])
t2  = Env.Program('test-shmm', source = ['test/tst3.cc', lib])

#lib = Env.Program('test', source = Glob('src/*.cc'))

