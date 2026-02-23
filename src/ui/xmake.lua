
target("ui")

    add_deps("tbox")
    set_kind("binary")
    add_files("ui.c", "backlight.c")

    -- 添加LVGL支持
    add_options("lvgl")
    if has_config("lvgl") then
        add_packages("lvgl")
        add_defines("TB_CONFIG_PACKAGE_HAVE_LVGL")
    end

    after_build(function (target)
        import("lib.detect.find_tool")
        local ln = find_tool("ln")
        if ln then
            local outdir = path.join(os.projectdir(), "out")
            os.mkdir(outdir)
            local targetfile = path.absolute(target:targetfile())
            local linkfile = path.join(outdir, path.filename(targetfile))
            os.tryrm(linkfile)
            os.vrunv(ln.program, {"-sf", targetfile, linkfile})
            print("已软链接到: " .. linkfile)
        end
    end)

