// Copyright (c) 2018-present The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <script/parsing.h>

#include <span.h>

#include <algorithm>
#include <cstddef>
#include <string>

namespace script {

bool Const(const std::string& str, std::span<const char>& sp, bool skip)
{
    if ((size_t)sp.size() >= str.size() && std::equal(str.begin(), str.end(), sp.begin())) {
        if (skip) sp = sp.subspan(str.size());
        return true;
    }
    return false;
}

bool Func(const std::string& str, std::span<const char>& sp)
{
    if ((size_t)sp.size() >= str.size() + 2 && sp[str.size()] == '(' && sp[sp.size() - 1] == ')' && std::equal(str.begin(), str.end(), sp.begin())) {
        sp = sp.subspan(str.size() + 1, sp.size() - str.size() - 2);
        return true;
    }
    return false;
}

std::span<const char> Expr(std::span<const char>& sp)
{
    int level = 0;
    auto it = sp.begin();
    while (it != sp.end()) {
        if (*it == '(' || *it == '{') {
            ++level;
        } else if (level && (*it == ')' || *it == '}')) {
            --level;
        } else if (level == 0 && (*it == ')' || *it == '}' || *it == ',')) {
            break;
        }
        ++it;
    }
    std::span<const char> ret = sp.first(it - sp.begin());
    sp = sp.subspan(it - sp.begin());
    return ret;
}

} // namespace script
