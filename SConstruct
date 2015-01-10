env=Environment(CPPDEFINES=[],
                LIBS=[],
                # CXXFLAGS="-Wall -Wextra -std=c++11 -g -ggdb3",
                CXXFLAGS="-Ofast -DNDEBUG -flto -Wall -Wextra -std=c++11",
                LINKFLAGS="-flto"
                )

env.Program('lutter', Glob('*.cpp'))

scene = Command( target = "scene.txt",
                source = "scenegen.py",
                action = "python scenegen.py >scene.txt" )

test = Command( target = "scene.ppm",
                source = ["./lutter", "scene.txt"],
                action = "time ./lutter < scene.txt > $TARGET" )

AlwaysBuild( test )
