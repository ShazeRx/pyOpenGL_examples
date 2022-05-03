# common.py
import math
from OpenGL.GL import *
from OpenGL.arrays import vbo
from OpenGL.GLU import *
from OpenGL.GLUT import *
# import OpenGL.GLUT as glut
import numpy as ny


# Python Imaging Library (PIL)
class common:
    bCreate = False


# 球的实现
class sphere(common):
    def __init__(self, rigns, segments, radius):
        self.rigns = rigns
        self.segments = segments
        self.radius = radius

    def createVAO(self):
        vdata = []
        vindex = []
        for y in range(self.rigns):
            phi = float(y) / (self.rigns - 1) * math.pi
            for x in range(self.segments):
                theta = float(x) / float(self.segments - 1) * 2 * math.pi
                vdata.extend(
                    (
                        self.radius * math.sin(phi) * math.cos(theta),
                        self.radius * math.cos(phi),
                        self.radius * math.sin(phi) * math.sin(theta),
                        math.sin(phi) * math.cos(theta),
                        math.cos(phi),
                        math.sin(phi) * math.sin(theta),
                    )
                )

        for y in range(self.rigns - 1):
            for x in range(self.segments - 1):
                vindex.extend(
                    (
                        (y + 0) * self.segments + x,
                        (y + 1) * self.segments + x,
                        (y + 1) * self.segments + x + 1,
                        (y + 1) * self.segments + x + 1,
                        (y + 0) * self.segments + x + 1,
                        (y + 0) * self.segments + x,
                    )
                )

        # this.vboID = glGenBuffers(1)
        # glBindBuffer(GL_ARRAY_BUFFER,this.vboID)
        # glBufferData (GL_ARRAY_BUFFER, len(vdata)*4, vdata, GL_STATIC_DRAW)
        # this.eboID = glGenBuffers(1)
        # glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this.eboID)
        # glBufferData (GL_ELEMENT_ARRAY_BUFFER, len(vIndex)*4, vIndex,
        # GL_STATIC_DRAW)
        self.vbo = vbo.VBO(ny.array(vdata, 'f'))
        self.ebo = vbo.VBO(ny.array(vindex, 'H'), target=GL_ELEMENT_ARRAY_BUFFER)
        self.vboLength = self.segments * self.rigns
        self.eboLength = len(vindex)
        self.bCreate = True

    def drawShader(self, vi, ni, ei):
        if not self.bCreate:
            self.createVAO()
        # glBindBuffer(GL_ARRAY_BUFFER,this.vboID)
        # glVertexAttribPointer(vi,3,GL_FLOAT,False,24,0)
        # glEnableVertexAttribArray(vi)
        # glVertexAttribPointer(ni,3,GL_FLOAT,False,24,12)
        # glEnableVertexAttribArray(ni)
        # glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this.eboID)
        # glDrawElements(GL_TRIANGLES,this.eboLength,GL_UNSIGNED_INT,0)
        self.vbo.bind()

    def draw(self):
        if not self.bCreate:
            self.createVAO()
        # glBindBuffer(GL_ARRAY_BUFFER,this.vboID)
        # glInterleavedArrays(GL_N3F_V3F,0,None)
        # glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this.eboID)
        # glDrawElements(GL_TRIANGLES,this.eboLength,GL_UNSIGNED_INT,None)
        self.vbo.bind()
        glInterleavedArrays(GL_N3F_V3F, 0, None)
        self.ebo.bind()
        glDrawElements(GL_TRIANGLES, self.eboLength, GL_UNSIGNED_SHORT, None)


class plane(common):
    def __init__(self, xres, yres, xscale, yscale):
        self.xr, self.yr, self.xc, self.yc = xres - 1, yres - 1, xscale, yscale

    def createVAO(self):
        helfx = self.xr * self.xc * 0.5
        helfy = self.yr * self.yc * 0.5
        vdata = []
        vindex = []
        for y in range(self.yr):
            for x in range(self.xr):
                vdata.extend((self.xc * float(x) - helfx, 0., self.yc * float(y) - helfy))
        for y in range(self.yr - 1):
            for x in range(self.xr - 1):
                vindex.extend(
                    (
                        (y + 0) * self.xr + x,
                        (y + 1) * self.xr + x,
                        (y + 0) * self.xr + x + 1,
                        (y + 0) * self.xr + x + 1,
                        (y + 1) * self.xr + x,
                        (y + 1) * self.xr + x + 1,
                    )
                )

        print(len(vdata), len(vindex))
        self.vbo = vbo.VBO(ny.array(vdata, 'f'))
        self.ebo = vbo.VBO(ny.array(vindex, 'H'), target=GL_ELEMENT_ARRAY_BUFFER)
        self.eboLength = len(vindex)
        self.bCreate = True

    def draw(self):
        if not self.bCreate:
            self.createVAO()
        self.vbo.bind()
        glInterleavedArrays(GL_V3F, 0, None)
        self.ebo.bind()
        glDrawElements(GL_TRIANGLES, self.eboLength, GL_UNSIGNED_SHORT, None)


class camera:
    origin = [0.0, 0.0, 0.0]
    length = 1.
    yangle = 0.
    zangle = 0.
    __bthree = False

    def __init__(self):
        self.mouselocation = [0.0, 0.0]
        self.offest = 0.01
        self.zangle = math.pi if self.__bthree else 0.

    def setthree(self, value):
        self.__bthree = value
        self.zangle = self.zangle + math.pi
        self.yangle = -self.yangle

    def eye(self):
        return self.direction() if self.__bthree else self.origin

    def target(self):
        return self.origin if self.__bthree else self.direction()

    def direction(self):
        if self.zangle > math.pi * 2.0:
            self.zangle < -self.zangle - math.pi * 2.0
        elif self.zangle < 0.0:
            self.zangle < -self.zangle + math.pi * 2.0
        len = (self.length if 0. else 1.) if self.__bthree else 1.
        xy = math.cos(self.yangle) * len
        x = self.origin[0] + xy * math.sin(self.zangle)
        y = self.origin[1] + len * math.sin(self.yangle)
        z = self.origin[2] + xy * math.cos(self.zangle)
        return [x, y, z]

    def move(self, x, y, z):
        sinz, cosz = math.sin(self.zangle), math.cos(self.zangle)
        xstep, zstep = x * cosz + z * sinz, z * cosz - x * sinz
        if self.__bthree:
            xstep = -xstep
            zstep = -zstep
        self.origin = [
            self.origin[0] + xstep,
            self.origin[1] + y,
            self.origin[2] + zstep,
        ]

    def rotate(self, z, y):
        self.zangle, self.yangle = (
            self.zangle - z,
            -y if self.__bthree else self.yangle + y,
        )

    def setLookat(self):
        ve, vt = self.eye(), self.target()
        # print ve,vt
        glLoadIdentity()
        gluLookAt(ve[0], ve[1], ve[2], vt[0], vt[1], vt[2], 0.0, 1.0, 0.0)

    def keypress(self, key, x, y):
        print(key)
        if key in (b'e', b'E'):
            self.move(0., 0., 5 * self.offest)
        if key in (b'f', b'F'):
            self.move(5 * self.offest, 0., 0.)
        if key in (b's', b'S'):
            self.move(-5 * self.offest, 0., 0.)
        if key in (b'd', b'D'):
            self.move(0., 0., -5 * self.offest)
        if key in (b'w', b'W'):
            self.move(0., 5 * self.offest, 0.)
        if key in (b'r', b'R'):
            self.move(0., -5 * self.offest, 0.)
        if key in (b'v', b'V'):
            # this.__bthree = not this.__bthree
            self.setthree(not self.__bthree)
        if key == GLUT_KEY_UP:
            self.offest = self.offest + 1
        if key == GLUT_KEY_DOWN:
            self.offest = self.offest - 1

    def mouse(self, x, y):
        rx = (x - self.mouselocation[0]) * self.offest * 0.1
        ry = (y - self.mouselocation[1]) * self.offest * -0.1
        self.rotate(rx, ry)
        print(x, y)
        self.mouselocation = [x, y]
