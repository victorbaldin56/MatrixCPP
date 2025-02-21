from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain

import pip
import os

class MatrixRecipe(ConanFile):
  name = "matrix"
  version = "1.0"
  settings = "os", "compiler", "build_type", "arch"
  generators = "CMakeToolchain", "CMakeDeps"
  options = {"testing": [True, False]}
  default_options = {"testing": False}
  test_requires = "gtest/1.15.0"

  def requirements(self):
    if (self.options.testing):
      pip.main(['install', 'numpy==2.2.3'])

  def build(self):
    cmake = CMake(self)
    cmake.definitions["BUILD_TESTING"] = self.options.testing
    os.environ['ENABLE_TESTING'] = self.options.testing

    cmake.configure()
    cmake.build()
    if self.options.testing:
      cmake.test()
