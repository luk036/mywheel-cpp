add_rules("mode.debug", "mode.release", "mode.coverage")
add_requires("doctest", "fmt")

set_languages("c++17")
add_includedirs("include", "../py2cpp/include")
set_optimize("fastest")

if is_mode("coverage") then
    add_cxflags("-ftest-coverage", "-fprofile-arcs", {force = true})
end

if is_plat("linux") then
    set_warnings("all", "error")
    add_cxflags("-Wconversion", {force = true})
    -- Termux/Android ships its libc++ headers under $PREFIX
    local prefix = os.getenv("PREFIX")
    if prefix then
        add_cxflags("-Wno-unused-command-line-argument", {force = true})
        add_sysincludedirs(prefix .. "/include/c++/v1", prefix .. "/include", {public = true})
    end
elseif is_plat("windows") then
    add_cxflags("/EHsc /utf-8 /W4 /WX /wd4702", {force = true})
end

target("test_mywheel")
    add_files("test/source/*.cpp")
    add_packages("doctest", "fmt")
    add_tests("default")
