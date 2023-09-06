local M = {}

--- 从tb获取 键值为key的子表，如果不存在就以default或者{}构建子表并返回
function M.subtb(tb, key, default)
    local sub = tb[key]
    if sub then
        return sub    
    end

    sub = default or {}
    tb[key] = sub
    return sub
end

return M