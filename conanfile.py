from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain
from conan.tools.files import copy


class Connix(ConanFile):
    name = "connix"
    version = "0.1.0"

    settings = "os", "compiler", "build_type", "arch"

    def is_native_build(self):
        return self.settings.os == "Linux"

    def requirements(self):
        if self.is_native_build():
            self.requires("gtest/1.16.0")  # type: ignore

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.cache_variables["CONAN_BUILD"] = "ON"
        tc.generate()

    def export_sources(self):
        copy(self, "CMakeLists.txt", self.recipe_folder, self.export_sources_folder)
        copy(self, "connix/*", self.recipe_folder, self.export_sources_folder)

    def build(self):
        cmake = CMake(self)
        cmake._generator = "Ninja"
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["connix"]
