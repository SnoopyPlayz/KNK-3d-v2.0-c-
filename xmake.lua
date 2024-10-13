add_rules("mode.debug", "mode.release")
add_requires("glfw")
add_requires("glew")
add_requires("glm")
add_requires("joltphysics")
--add_requires("meshoptimizer")
--add_requires("glfw", {configs = {vs_runtime = "MT"}})
target("hello")
    set_kind("shared")
    set_arch("x86_64")
    add_files("src/*.cpp")
    add_files("src/physics/*.cpp")
    add_files("src/fileLoading/*.cpp")
    add_files("src/render/*.cpp")
    add_files("src/physics/*.cpp")
    --add_files("src/networking/*.cpp")
    add_includedirs("include")
    add_includedirs("include/physics")
    add_includedirs("include/fileLoading")
    add_includedirs("include/render")
    add_includedirs("include/physics")
    add_includedirs("include/networking")
    add_packages("glfw", {force = true})
    add_packages("glm", {force = true})
    add_packages("glew", {force = true})
    add_packages("joltphysics", {force = true})
    --add_links("gdi32", "opengl32")
    --add_packages("meshoptimizer", {force = true})

--
-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
--@code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro definition
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
-- add_cxflags("-stdnolib", "-fno-strict-aliasing")
--
--release
--add_cxflags("-Ofast", "-flto", "-s", "-static")
add_cxflags("-Ofast", "-flto", "-s", "-static", "-mwindows")
--debug
--add_cxflags("-Wall", "-s","-g", "-Wextra", "-fsanitize=address", {force = true})
--
--add_cxflags("-Wall", "-s","-g", "-Wextra", "-L/usr/x86_64-w64-mingw32/lib")
--
add_ldflags("-L/usr/x86_64-w64-mingw32/lib", {force = true})
--add_ldflags("-fsanitize=address", {force = true})
--
--no gcc on windows
set_toolchains("gcc")

--
--
--add_ldflags("-static")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
--@endcode
--

