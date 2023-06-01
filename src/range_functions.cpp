#include "../include/range_functions.hpp"

static void pushValue(lua_State *L, std::string &value) {
    if (value.find('.') != std::string::npos) {
        lua_pushnumber(L, std::stod(value));
    }
    else {
        lua_pushinteger(L, std::stoll(value));
    }
}

void range(lua_State *L, std::vector<std::string> &values) {
    lua_newtable(L);
    for (int i = 0; i < values.size(); i++) {
        pushValue(L, values[values.size() - i - 1]);
        lua_rawseti(L, -2, i + 1);
    }
}

void rangeSum(lua_State *L, std::vector<std::string> &values) {
    if (values.size() == 0) {
        lua_pushinteger(L, 0);
        return;
    }
    pushValue(L, values[0]);
    for (int i = 1; i < values.size(); i++) {
        pushValue(L, values[i]);
        lua_arith(L, LUA_OPADD);
    }
}

void rangeProd(lua_State *L, std::vector<std::string> &values) {
    if (values.size() == 0) {
        lua_pushinteger(L, 0);
        return;
    }
    pushValue(L, values[0]);
    for (int i = 1; i < values.size(); i++) {
        pushValue(L, values[i]);
        lua_arith(L, LUA_OPMUL);
    }
}

void rangeAvg(lua_State *L, std::vector<std::string> &values) {
    if (values.size() == 0) {
        lua_pushinteger(L, 0);
        return;
    }
    pushValue(L, values[0]);
    for (int i = 1; i < values.size(); i++) {
        pushValue(L, values[i]);
        lua_arith(L, LUA_OPADD);
    }
    lua_pushinteger(L, values.size());
    lua_arith(L, LUA_OPDIV);
}

void rangeMin(lua_State *L, std::vector<std::string> &values) {
    if (values.size() == 0) {
        lua_pushinteger(L, 0);
        return;
    }
    int minIdx = 0;
    double min = std::stod(values[0]);

    for (int i = 1; i < values.size(); i++) {
        double value = std::stod(values[i]);
        if (value < min) {
            min = value;
            minIdx = i;
        }
    }
    pushValue(L, values[minIdx]);
}

void rangeMax(lua_State *L, std::vector<std::string> &values) {
    if (values.size() == 0) {
        lua_pushinteger(L, 0);
        return;
    }
    int maxIdx = 0;
    double max = std::stod(values[0]);

    for (int i = 1; i < values.size(); i++) {
        double value = std::stod(values[i]);
        if (value > max) {
            max = value;
            maxIdx = i;
        }
    }
    pushValue(L, values[maxIdx]);
}
