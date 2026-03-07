from . import clibfizz
from .draw import Color
import ctypes

class System:
  def __init__(self):
    self._ptr = clibfizz.system_create()

  def __del__(self):
    clibfizz.system_destroy(self._ptr)

  def update(self, dT):
    clibfizz.system_update(self._ptr, dT)

  def draw(self, color):
    clibfizz.system_draw(self._ptr, color)

  def create_body(self, x, y, radius, isKinematic=False, mass=1.0):
    return clibfizz.system_create_body(self._ptr, x, y, radius, isKinematic, mass)


clibfizz.system_create.argtypes = None
clibfizz.system_create.restype = ctypes.c_void_p

clibfizz.system_destroy.argtypes = [ctypes.c_void_p]
clibfizz.system_destroy.restype = None

clibfizz.system_update.argtypes = [ctypes.c_void_p, ctypes.c_double]
clibfizz.system_update.restype = None

clibfizz.system_draw.argtypes = [ctypes.c_void_p, Color]
clibfizz.system_draw.restype = None

clibfizz.system_create_body.argtypes = [ctypes.c_void_p, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_bool, ctypes.c_double]
clibfizz.system_create_body.restype = ctypes.c_int