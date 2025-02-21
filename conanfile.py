from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain
import pip

class MatrixRecipe(ConanFile):
  name = "matrix"
  version = "1.0"
  settings = "os", "compiler", "build_type", "arch"
  generators = "CMakeDeps"
  options = {"testing": [True, False]}
  default_options = {"testing": False}
  test_requires = "gtest/1.15.0"

  def requirements(self):
    if self.options.testing:
      pip.main(['install', 'numpy==2.2.3'])

  def generate(self):
    # Customize CMakeToolchain in the generate() method
    tc = CMakeToolchain(self)
    tc.variables["BUILD_TESTING"] = self.options.testing
    tc.generate()

  def build(self):
    cmake = CMake(self)
    cmake.configure()
    cmake.build()
    if self.options.testing:
      cmake.test()