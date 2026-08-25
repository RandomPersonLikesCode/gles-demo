local bin = os.getenv("ANDROID_NDK_BIN")

vim.lsp.config("clangd", {
    cmd = {bin .. "/clangd"},
})
