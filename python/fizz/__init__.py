import os
import platform
import ctypes

if platform.system() == "Windows":
  _libfizz_filename = "fizz.dll"
else:
  _libfizz_filename = "libfizz.so"

_package_dir = os.path.dirname(os.path.abspath(__file__))
_libfizz_path = os.path.join(_package_dir, _libfizz_filename)

try:
  clibfizz = ctypes.CDLL(_libfizz_path)
except OSError as e:
  print(f"Failed to load libfizz: {e}")