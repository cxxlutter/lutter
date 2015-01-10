#!/usr/bin/python
from math import sin, cos, pi
print("""global {
  width 16000
  height 9000
}
camera {
  location <0,10,-30>
  direction <0,-.45,1>
  right <1.12,0,0>
  up <0,.63,0>
  sky_emission <.8,.8,1.>
  ground_emission <.4,.3,.2>
}
""")
for r in range(1,11):
    print(r"sphere {{ <{},{},{}>, 3 color <{},{},{}> emission <{},{},{}> }}".format(
        10*cos(r*pi/5), (r*10007)%107/107*6-3, 10*sin(r*pi/5),
        r/10.,r%5/5.,2*r%5/5.,
        0, 0, 0))
