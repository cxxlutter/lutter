from os import path

#get the mode flag from the command line
#default to 'debug' if the user didn't specify
mode = ARGUMENTS.get('mode', 'debug')   #holds current mode

#check if the user has been naughty: only 'debug' or 'release' allowed
if not (mode in ['debug', 'release']):
   print "Error: expected 'debug' or 'release', found: " + mode
   Exit(1)

#tell the user what we're doing
print '**** Compiling in ' + mode + ' mode...'

cflags = ['-std=c++11','-Wall', '-Wextra']
lflags = []

if mode == 'debug':
    cflags.extend(['-g', '-ggdb3'])
else:
    cflags.extend(['-O3', '-march=native', '-DNDEBUG', '-flto'])
    lflags.extend(['-flto'])

buildroot = path.join('..', 'build', mode)

env = Environment()
env.Replace(AR = "gcc-ar")
env.Replace(RANLIB = "gcc-ranlib")
env.Append(CXXFLAGS=cflags)
env.Append(LINKFLAGS=lflags)

#make sure the sconscripts can get to the variables
Export('env', 'buildroot')

#put all .sconsign files in one place
env.SConsignFile()

project = 'lib'
SConscript('lib/SConscript', exports=['project'])

project = 'lutter'
SConscript('src/SConscript', exports=['project'])


scene = Command( target = "scene.txt",
                source = "scenegen.py",
                action = "python scenegen.py >scene.txt" )

lutterbinary = path.join('build', mode, 'lutter', 'lutter')

test = Command( target = "scene.bmp",
                source = [lutterbinary, "scene.txt"],
                action = "time ./" + lutterbinary + " -f bmp < scene.txt > $TARGET" )
