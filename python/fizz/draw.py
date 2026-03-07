from . import clibfizz
import ctypes

class Color(ctypes.Structure):
  _fields_ = [
    ("r", ctypes.c_uint8),
    ("g", ctypes.c_uint8),
    ("b", ctypes.c_uint8),
    ("a", ctypes.c_uint8),
  ]

# draw_set_circle_callback
FIZZ_CIRCLE_CALLBACK_TYPE = ctypes.CFUNCTYPE(None, ctypes.c_double, ctypes.c_double, ctypes.c_float, Color)
clibfizz.draw_set_circle_callback.argtypes = [FIZZ_CIRCLE_CALLBACK_TYPE]
clibfizz.draw_set_circle_callback.restype = None

# draw_set_line_callback
FIZZ_LINE_CALLBACK_TYPE = ctypes.CFUNCTYPE(None, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_float, Color)
clibfizz.draw_set_line_callback.argtypes = [FIZZ_LINE_CALLBACK_TYPE]
clibfizz.draw_set_line_callback.restype = None


_circle_cb_handle = None
def set_circle_callback(callback):
  global _circle_cb_handle
  _circle_cb_handle = FIZZ_CIRCLE_CALLBACK_TYPE(callback)
  clibfizz.draw_set_circle_callback(_circle_cb_handle)

_line_cb_handle = None
def set_line_callback(callback):
  global _line_cb_handle
  _line_cb_handle = FIZZ_LINE_CALLBACK_TYPE(callback)
  clibfizz.draw_set_line_callback(_line_cb_handle)