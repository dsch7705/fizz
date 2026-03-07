import os
import ctypes

_package_dir = os.path.dirname(os.path.abspath(__file__))
_libfizz_path = os.path.join(_package_dir, "libfizz.so")

try:
  clibfizz = ctypes.CDLL(_libfizz_path)
except OSError as e:
  print("Failed to load libfizz: {e}")