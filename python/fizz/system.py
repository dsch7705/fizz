from . import clibfizz
from .draw import Color
import ctypes

ID = ctypes.c_uint64

class System:
  def __init__(self):
    self._ptr = clibfizz.system_create()

  def __del__(self):
    clibfizz.system_destroy(self._ptr)

  def update(self, dt):
    clibfizz.system_update(self._ptr, dt)

  def draw(self, color):
    clibfizz.system_draw(self._ptr, color)

  def create_body(self, x, y, radius, isKinematic=False, mass=1.0):
    return clibfizz.system_create_body(self._ptr, x, y, radius, isKinematic, mass)

  def create_distance_constraint(self, b0, b1):
    return clibfizz.system_create_distance_constraint(self._ptr, b0, b1)

  def create_spring_constraint(self, b0, b1, k=150.0, damping=1.0):
    return clibfizz.system_create_spring_constraint(self._ptr, b0, b1, k, damping)

clibfizz.system_create.argtypes = None
clibfizz.system_create.restype = ctypes.c_void_p

clibfizz.system_destroy.argtypes = [ctypes.c_void_p]
clibfizz.system_destroy.restype = None

clibfizz.system_update.argtypes = [ctypes.c_void_p, ctypes.c_float]
clibfizz.system_update.restype = None

clibfizz.system_draw.argtypes = [ctypes.c_void_p, Color]
clibfizz.system_draw.restype = None

clibfizz.system_create_body.argtypes = [ctypes.c_void_p, ctypes.c_float, ctypes.c_float, ctypes.c_float, ctypes.c_bool, ctypes.c_float]
clibfizz.system_create_body.restype = ID

clibfizz.system_create_distance_constraint.argtypes = [ctypes.c_void_p, ID, ID]
clibfizz.system_create_distance_constraint.restype = ID

clibfizz.system_create_spring_constraint.argtypes = [ctypes.c_void_p, ID, ID, ctypes.c_float, ctypes.c_float]
clibfizz.system_create_spring_constraint.restype = ID