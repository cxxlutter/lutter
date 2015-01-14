env=Environment(CPPDEFINES=[],
                LIBS=[],
                CPPPATH=["../Lunch-Time-Tracer/Include/"],
                # CXXFLAGS="-Wall -Wextra -std=c++11 -g -ggdb3",
                CXXFLAGS="-Ofast -DNDEBUG -flto -Wall -Wextra -std=c++11",
                LINKFLAGS="-flto"
                )

env.Program('lutter', Glob('*.cpp'))

scene = Command( target = "scene.txt",
                source = "scenegen.py",
                action = "python scenegen.py >scene.txt" )

test = Command( target = "scene.bmp",
                source = ["./lutter", "scene.txt"],
                action = "time ./lutter -f bmp < scene.txt > $TARGET" )

AlwaysBuild( test )
