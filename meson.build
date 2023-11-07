project('loader', 'cpp',
    default_options : ['cpp_std=c++17'])

# Configuration for C++ compiler
cpp = meson.get_compiler('cpp')

# Include directories
include_directories = include_directories('./include')

# Define sources
sources = [
  './testes/idea.cpp',
  'src/loader/loader.cpp',
]

# Create a shared library
plugin = shared_library(
  'plugin',
  'testes/plugin.cpp',
  include_directories: include_directories,
)

# Custom target to build loader library
loader = shared_library(
  'loader',
  'src/loader/loader.cpp',
  include_directories: include_directories,
  dependencies: [dependency('dl')],
)

# Create an executable
idea = executable(
  'idea',
  sources,
  include_directories: include_directories,
  link_with : loader,
  dependencies: [dependency('dl')],
)

