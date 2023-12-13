import pyurx as urx
import numpy as np

a = urx.A()
a_2 = urx.A()
a_3 = urx.A()

a

a_2.x = 1
a_2.y = 2
a_2

a_3.x = 3
a_3.y = 4
a_3

b = urx.B()
b_2 = urx.B()
b_3 = urx.B()

b_2.z = 1.23
b_2.a = a_2

b_3.z = 4.56
b_3.a = a_3

toto = [b, b_2]
print("len(toto) == 2 ", len(toto) == 2)

toto.append(b_3)
print("len(toto) == 3 ", len(toto) == 3)

toto[0] = b_3
print("toto[0] == b_3 ", toto[0] == b_3)

b_3.z = 7.41
print("toto[0] == b_3 ", toto[0] == b_3)
print("toto[2] == b_3 ", toto[2] == b_3)

toto[0].z = 9.63
print("b_3.z == 9.63 ", b_3.z == 9.63)
print("toto[2].z == 9.63 ", toto[2].z == 9.63)

b_3.z = 4.56

c = urx.C()

c.b = urx.VecB([b, b_2])
print("len(c.b) == 2 ", len(c.b) == 2)

c.b.append(b_3)
print("len(c.b) == 3 ", len(c.b) == 3)

c.b[0] = b_2
print("c.b[0] == b_2 ", c.b[0] == b_2)
