set_policy("package.precompiled", false)
add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", { outputdir = ".", lsp = "clangd" })

set_project("MouseSim")
set_version("1.0.4")

-- GLOBAL COMPILER FLAGS --
set_encodings("utf-8")
add_cxxflags("clang::-fexperimental-library")

-- WARNINGS --
set_warnings("allextra", "pedantic")
add_cxxflags(
    "-Wshadow",
    "-Wno-missing-braces",
    "-Wno-unused-parameter",
    "-Wno-unused-variable",
    { tools = { "clang", "clangxx", "gcc" } })
add_cxxflags(
    "-Wshadow-all",
    "-Wno-gnu-line-marker",
    "-Wno-gnu-anonymous-struct",
    "-Wno-gnu-zero-variadic-macro-arguments",
    { tools = { "clang", "clangxx" } })

target("MouseSim")
    set_kind("binary")
    set_languages("cxx23")

    add_includedirs("./src/**.hpp", { public = true })
    add_files("./src/**.cpp")
    remove_files("./src/socket*")

    add_links("./lib/vJoyInterface.lib", "user32")

    -- Copy vJoyInterface.dll to the output directory
    after_build(function (target)
        os.cp("./lib/vJoyInterface.dll", target:targetdir())
    end)

    -- Package the executable and DLL
    on_package(function (target)
        local package_dir = "./packaged"
        os.mkdir(package_dir)
        os.cp(target:targetfile(), package_dir)
        os.cp("./lib/vJoyInterface.dll", package_dir)
        print("Package created at: " .. package_dir)
    end)
target_end()