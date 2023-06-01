#ifndef RANGE_FUNCTIONS_HPP
#define RANGE_FUNCTIONS_HPP

#include <string>
#include <vector>

extern "C" {
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

static void pushValue(lua_State *L, std::string &value);
void range(lua_State *L, std::vector<std::string> &values);
void rangeSum(lua_State *L, std::vector<std::string> &values);
void rangeProd(lua_State *L, std::vector<std::string> &values);
void rangeAvg(lua_State *L, std::vector<std::string> &values);
void rangeMin(lua_State *L, std::vector<std::string> &values);
void rangeMax(lua_State *L, std::vector<std::string> &values);

#endif
