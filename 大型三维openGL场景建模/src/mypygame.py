from time import time


class FrameManager:
    def __init__(self, flagPrintFPS:bool=False):
        self.__frameCount_i = 0
        self.__lastCountedTime_f = 0.0
        self.__frameDelta_f = 0.0
        self.__lastCountFPS_f = time()
        self.__fpsCounter_i = 0
        self.__lastFPS_i = 0
        self.__flagPrintFPS = flagPrintFPS
        self.__isFreshFps = False

    #### getters ####

    def getFrameCount(self):
        return self.__frameCount_i

    def getFrameDelta(self):
        return 0.0 if self.__frameDelta_f > 100.0 else self.__frameDelta_f

    def getFPS(self):
        if not self.__isFreshFps:
            return self.__lastFPS_i, False
        self.__isFreshFps = False
        return self.__lastFPS_i, True

    #### setter ####

    def setPrintFPS(self, opt:bool):
        self.__flagPrintFPS = opt

    ####  ####

    def update(self):
        self.__frameCount_i += 1
        self.__frameDelta_f = time() - self.__lastCountedTime_f
        self.__lastCountedTime_f = time()
        if self.__frameCount_i > 10000:
            self.__frameCount_i = 0

        self.__fpsCounter_i += 1
        if time() - self.__lastCountFPS_f > 1.0:
            self.__lastFPS_i = self.__fpsCounter_i
            self.__fpsCounter_i = 0
            self.__lastCountFPS_f = time()
            self.__isFreshFps = True
            if self.__flagPrintFPS:
                print("FPS:", self.__lastFPS_i)
