#include "../include/table_executor.hpp"
#include "../include/range_functions.hpp"

#include <iomanip>
#include <regex>
#include <sstream>

TableExecutor *TableExecutor::_executor;

const std::regex NUMBER_REGEX("[0-9]+(\\.[0-9]+)?");

void TableExecutor::setSharedData(TablescriptSharedData *sharedData) {
    _sharedData = sharedData;
}

void TableExecutor::execute() {
    _executor = this;
    _luaState = luaL_newstate();
    luaL_openlibs(_luaState);
    _addCallbacks();
    _executeTable();
    lua_close(_luaState);
}

void TableExecutor::_executeTable() {
    if (_table.script.length()) {
        _exec(_table.script);
    }
    for (_row = 0; _row < _table.rows.size(); _row++) {
        TableRow &row = _table.rows[_row];
        for (_col = 0; _col < row.cells.size(); _col++) {
            Cell &cell = row.cells[_col];
            if (cell.merge) {
                continue;
            }
            cell.output = "";
            for (CellContent &content : cell.input) {
                if (content.script) {
                    cell.output += _eval(content.text);
                }
                else {
                    cell.output += content.text;
                }
            }
        }
    }
}

void TableExecutor::_exec(std::string code) {
    luaL_dostring(_luaState, code.c_str());
    int i = lua_gettop(_luaState);
    if (lua_isstring(_luaState, i)) {
        std::string output = lua_tostring(_luaState, i);
        printf("%s\n", output.c_str());
    }
}

std::string TableExecutor::_eval(std::string code) {
    code = "return (" + code + ")";
    luaL_dostring(_luaState, code.c_str());
    int i = lua_gettop(_luaState);
    int type = lua_type(_luaState, i);
    if (type == LUA_TNUMBER && lua_isinteger(_luaState, i)) {
        return std::to_string(lua_tointeger(_luaState, i));
    }
    else if (type == LUA_TNUMBER && lua_isnumber(_luaState, i)) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(_precision)
           << lua_tonumber(_luaState, i);
        return ss.str();
    }
    else if (lua_isstring(_luaState, i)) {
        return lua_tostring(_luaState, i);
    }
    return "";
}

void TableExecutor::_addCallbacks() {
    lua_pushinteger(_luaState, SINGLE);
    lua_setglobal(_luaState, "SINGLE");
    lua_pushinteger(_luaState, DOUBLE);
    lua_setglobal(_luaState, "DOUBLE");
    lua_pushinteger(_luaState, THICK);
    lua_setglobal(_luaState, "THICK");
    lua_pushinteger(_luaState, ASCII);
    lua_setglobal(_luaState, "ASCII");
    lua_pushinteger(_luaState, THICK_ASCII);
    lua_setglobal(_luaState, "THICK_ASCII");
    lua_pushinteger(_luaState, DOTTED);
    lua_setglobal(_luaState, "DOTTED");
    lua_pushinteger(_luaState, ROUND);
    lua_setglobal(_luaState, "ROUND");
    lua_pushinteger(_luaState, BLOCK);
    lua_setglobal(_luaState, "BLOCK");
    lua_pushinteger(_luaState, DASHED);
    lua_setglobal(_luaState, "DASHED");
    lua_pushinteger(_luaState, THICK_DASHED);
    lua_setglobal(_luaState, "THICK_DASHED");

    lua_pushcfunction(_luaState, TableExecutor::_funcWidth);
    lua_setglobal(_luaState, FUNC_WIDTH);
    lua_pushcfunction(_luaState, TableExecutor::_funcMaxWidth);
    lua_setglobal(_luaState, FUNC_MAXWIDTH);
    lua_pushcfunction(_luaState, TableExecutor::_funcWidths);
    lua_setglobal(_luaState, FUNC_WIDTHS);
    lua_pushcfunction(_luaState, TableExecutor::_funcMaxWidths);
    lua_setglobal(_luaState, FUNC_MAXWIDTHS);
    lua_pushcfunction(_luaState, TableExecutor::_funcBorder);
    lua_setglobal(_luaState, FUNC_BORDER);
    lua_pushcfunction(_luaState, TableExecutor::_funcSeparator);
    lua_setglobal(_luaState, FUNC_SEPARATOR);
    lua_pushcfunction(_luaState, TableExecutor::_funcPadding);
    lua_setglobal(_luaState, FUNC_PADDING);
    lua_pushcfunction(_luaState, TableExecutor::_funcDecimals);
    lua_setglobal(_luaState, FUNC_DECIMALS);
    lua_pushcfunction(_luaState, TableExecutor::_funcRound);
    lua_setglobal(_luaState, FUNC_ROUND);
    lua_pushcfunction(_luaState, TableExecutor::_funcFloor);
    lua_setglobal(_luaState, FUNC_FLOOR);
    lua_pushcfunction(_luaState, TableExecutor::_funcCeil);
    lua_setglobal(_luaState, FUNC_CEIL);
    lua_pushcfunction(_luaState, TableExecutor::_funcList);
    lua_setglobal(_luaState, FUNC_LIST);
    lua_pushcfunction(_luaState, TableExecutor::_funcRowrange);
    lua_setglobal(_luaState, FUNC_ROWRANGE);
    lua_pushcfunction(_luaState, TableExecutor::_funcRowsum);
    lua_setglobal(_luaState, FUNC_ROWSUM);
    lua_pushcfunction(_luaState, TableExecutor::_funcRowprod);
    lua_setglobal(_luaState, FUNC_ROWPROD);
    lua_pushcfunction(_luaState, TableExecutor::_funcRowavg);
    lua_setglobal(_luaState, FUNC_ROWAVG);
    lua_pushcfunction(_luaState, TableExecutor::_funcRowmin);
    lua_setglobal(_luaState, FUNC_ROWMIN);
    lua_pushcfunction(_luaState, TableExecutor::_funcRowmax);
    lua_setglobal(_luaState, FUNC_ROWMAX);
    lua_pushcfunction(_luaState, TableExecutor::_funcColrange);
    lua_setglobal(_luaState, FUNC_COLRANGE);
    lua_pushcfunction(_luaState, TableExecutor::_funcColsum);
    lua_setglobal(_luaState, FUNC_COLSUM);
    lua_pushcfunction(_luaState, TableExecutor::_funcColprod);
    lua_setglobal(_luaState, FUNC_COLPROD);
    lua_pushcfunction(_luaState, TableExecutor::_funcColavg);
    lua_setglobal(_luaState, FUNC_COLAVG);
    lua_pushcfunction(_luaState, TableExecutor::_funcColmin);
    lua_setglobal(_luaState, FUNC_COLMIN);
    lua_pushcfunction(_luaState, TableExecutor::_funcColmax);
    lua_setglobal(_luaState, FUNC_COLMAX);
}

bool TableExecutor::_validateIntArg(const char *funcName, int *value) {
    if (lua_gettop(_luaState) != 1) {
        printf("%s() takes one argument\n", funcName);
        return false;
    }
    if (!lua_isinteger(_luaState, 1)) {
        printf("%s() requires an integer argument\n", funcName);
        return false;
    }
    *value = lua_tointeger(_luaState, 1);
    return true;
}

bool TableExecutor::_validateNumberArg(
    const char *funcName, lua_Number *value
) {
    if (lua_gettop(_luaState) != 1) {
        printf("%s() takes one argument\n", funcName);
        return false;
    }
    if (!lua_isnumber(_luaState, 1)) {
        printf("%s() requires a numeric argument\n", funcName);
        return false;
    }
    *value = lua_tonumber(_luaState, 1);
    return true;
}

bool TableExecutor::_validateIntArgs(
    const char *funcName, std::vector<int> &value
) {
    int numArgs = lua_gettop(_luaState);
    for (int i = 0; i < numArgs; i++) {
        if (!lua_isinteger(_luaState, i + 1)) {
            printf("%s() requires integer arguments\n", funcName);
            return false;
        }
        int d = lua_tointeger(_luaState, i + 1);
        value.push_back(d);
    }
    return true;
}

bool TableExecutor::_validateBorderArg(const char *funcName, int *value) {
    if (_validateIntArg(funcName, value)) {
        if (*value >= NUM_BORDER_STYLES) {
            printf("Invalid border type provided to %s()\n", funcName);
            return false;
        }
        return true;
    }
    return false;
}

void TableExecutor::_pushNumber(lua_Number number) {
    if (_precision == 0) {
        _pushInteger(number);
    }
    else {
        lua_pushnumber(_luaState, number);
    }
}

void TableExecutor::_pushInteger(lua_Integer number) {
    lua_pushinteger(_luaState, number);
}

void TableExecutor::_pushString(std::string str) {
    lua_pushstring(_luaState, str.c_str());
}

int TableExecutor::_funcWidth(lua_State *L) {
    int width;
    if (_executor->_validateIntArg(FUNC_WIDTH, &width)) {
        if (width <= 0) {
            printf("width() requires a positive integer argument\n");
            return 0;
        }
        _executor->_sharedData->maxWidth.reset();
        _executor->_sharedData->width = width;
    }
    return 0;
}

int TableExecutor::_funcMaxWidth(lua_State *L) {
    int maxWidth;
    if (_executor->_validateIntArg(FUNC_WIDTH, &maxWidth)) {
        if (maxWidth <= 0) {
            maxWidth = INT_MAX;
        }
        _executor->_sharedData->width.reset();
        _executor->_sharedData->maxWidth = maxWidth;
    }
    return 0;
}

int TableExecutor::_funcWidths(lua_State *L) {
    std::vector<int> ints;
    if (_executor->_validateIntArgs(FUNC_WIDTHS, ints)) {
        for (int i = 0; i < ints.size(); i++) {
            if (ints[i] < 0) {
                printf("widths() cannot accept negative arguments\n");
                return 0;
            }
        }
        _executor->_sharedData->maxWidths.reset();
        _executor->_sharedData->widths = ints;
    }
    return 0;
}

int TableExecutor::_funcMaxWidths(lua_State *L) {
    std::vector<int> ints;
    if (_executor->_validateIntArgs(FUNC_MAXWIDTHS, ints)) {
        for (int i = 0; i < ints.size(); i++) {
            if (ints[i] <= 0) {
                ints[i] = INT_MAX;
            }
        }
        _executor->_sharedData->widths.reset();
        _executor->_sharedData->maxWidths = ints;
    }
    return 0;
}

int TableExecutor::_funcBorder(lua_State *L) {
    int value;
    if (_executor->_validateBorderArg(FUNC_BORDER, &value)) {
        _executor->_sharedData->borderStyle = (BorderStyle)value;
        if (!_executor->_setSeparatorStyle) {
            if (value == ASCII || value == DOTTED || value == THICK_ASCII) {
                _executor->_sharedData->separatorStyle = THICK_ASCII;
            }
            else {
                _executor->_sharedData->separatorStyle = DOUBLE;
            }
        }
    }
    return 0;
}

int TableExecutor::_funcSeparator(lua_State *L) {
    int value;
    if (_executor->_validateBorderArg(FUNC_SEPARATOR, &value)) {
        _executor->_sharedData->separatorStyle = (BorderStyle)value;
        _executor->_setSeparatorStyle = true;
    }
    return 0;
}

int TableExecutor::_funcPadding(lua_State *L) {
    int value;
    if (_executor->_validateIntArg(FUNC_PADDING, &value)) {
        if (value < 0) {
            printf("padding() cannot accept a negative argument\n");
            return 0;
        }
        _executor->_sharedData->padding = value;
    }
    return 0;
}

int TableExecutor::_funcDecimals(lua_State *L) {
    int value;
    if (_executor->_validateIntArg(FUNC_DECIMALS, &value)) {
        if (value < 0) {
            printf("%s() cannot accept a negative argument\n", FUNC_DECIMALS);
            return 0;
        }
        _executor->_precision = value;
        _executor->_tenPowPrecision = pow(10, value);
    }
    return 0;
}

int TableExecutor::_funcRound(lua_State *L) {
    lua_Number value;
    if (_executor->_validateNumberArg(FUNC_ROUND, &value)) {
        lua_Number rounded = floor(
                                 value * (lua_Number)_executor->_tenPowPrecision
                             ) /
            (lua_Number)_executor->_tenPowPrecision;
        _executor->_pushNumber(rounded);
    }
    else {
        lua_pushnil(L);
    }
    return 1;
}

int TableExecutor::_funcFloor(lua_State *L) {
    lua_Number value;
    if (_executor->_validateNumberArg(FUNC_FLOOR, &value)) {
        _executor->_pushNumber(floor(value));
    }
    else {
        lua_pushnil(L);
    }
    return 1;
}

int TableExecutor::_funcCeil(lua_State *L) {
    lua_Number value;
    if (_executor->_validateNumberArg(FUNC_CEIL, &value)) {
        _executor->_pushNumber(ceil(value));
    }
    else {
        lua_pushnil(L);
    }
    return 1;
}

int TableExecutor::_funcList(lua_State *L) {
    int numArgs = lua_gettop(L);
    if (!numArgs) {
        lua_pushstring(L, "");
        return 1;
    }
    std::vector<std::string> args;
    for (int i = 0; i < numArgs; i++) {
        if (lua_isstring(L, i + 1)) {
            args.push_back(lua_tostring(L, i + 1));
        }
    }
    lua_pushstring(L, ("- " + join(args, "\n- ")).c_str());
    return 1;
}

std::vector<std::string> TableExecutor::_findNumbersInRow(int n) {
    std::vector<std::string> matches;
    if (n <= 0) {
        n = _col;
    }
    TableRow &row = _table[_row];
    int end = std::max(0, _col - n);
    for (int col = _col - 1; col >= end; col--) {
        Cell &cell = row[col];
        if (cell.merge) {
            continue;
        }
        std::smatch match;
        if (std::regex_search(cell.output, match, NUMBER_REGEX)) {
            matches.push_back(match.str());
            continue;
        }
        for (CellContent &content : cell.input) {
            std::smatch match;
            if (content.script) {
                continue;
            }
            if (std::regex_search(content.text, match, NUMBER_REGEX)) {
                matches.push_back(match.str());
                break;
            }
        }
    }
    return matches;
}

std::vector<std::string> TableExecutor::_findNumbersInCol(int n) {
    std::vector<std::string> matches;
    if (n <= 0) {
        n = _row;
    }
    int end = std::max(0, _row - n);
    for (int row = _row - 1; row >= end; row--) {
        Cell &cell = _table[row][_col];
        if (cell.merge) {
            continue;
        }
        std::smatch match;
        if (std::regex_search(cell.output, match, NUMBER_REGEX)) {
            matches.push_back(match.str());
            continue;
        }
        for (CellContent &content : cell.input) {
            std::smatch match;
            if (content.script) {
                continue;
            }
            if (std::regex_search(content.text, match, NUMBER_REGEX)) {
                matches.push_back(match.str());
                break;
            }
        }
    }
    return matches;
}

bool TableExecutor::_performRangeFunc(
    const char *funcName, void (*func)(lua_State *, std::vector<std::string> &),
    bool rowWise
) {
    int numArgs = lua_gettop(_luaState);
    std::vector<std::string> numberStrings;

    if (numArgs == 0) {
        numberStrings = rowWise ? _findNumbersInRow(0) : _findNumbersInCol(0);
    }
    else {
        int number;
        if (!_executor->_validateIntArg(funcName, &number)) {
            _pushInteger(0);
            return false;
        }
        numberStrings = rowWise ? _findNumbersInRow(number)
                                : _findNumbersInCol(number);
    }

    try {
        func(_luaState, numberStrings);
    }
    catch (std::out_of_range) {
        _pushString("Out of range");
        return false;
    }
    return true;
}

int TableExecutor::_funcRowrange(lua_State *L) {
    _executor->_performRangeFunc(FUNC_ROWRANGE, range, true);
    return 1;
}

int TableExecutor::_funcRowsum(lua_State *L) {
    _executor->_performRangeFunc(FUNC_ROWSUM, rangeSum, true);
    return 1;
}

int TableExecutor::_funcRowprod(lua_State *L) {
    _executor->_performRangeFunc(FUNC_ROWPROD, rangeProd, true);
    return 1;
}

int TableExecutor::_funcRowavg(lua_State *L) {
    _executor->_performRangeFunc(FUNC_ROWAVG, rangeAvg, true);
    return 1;
}

int TableExecutor::_funcRowmin(lua_State *L) {
    _executor->_performRangeFunc(FUNC_ROWMIN, rangeMin, true);
    return 1;
}

int TableExecutor::_funcRowmax(lua_State *L) {
    _executor->_performRangeFunc(FUNC_ROWMAX, rangeMax, true);
    return 1;
}

int TableExecutor::_funcColrange(lua_State *L) {
    _executor->_performRangeFunc(FUNC_COLRANGE, range, false);
    return 1;
}

int TableExecutor::_funcColsum(lua_State *L) {
    _executor->_performRangeFunc(FUNC_COLSUM, rangeSum, false);
    return 1;
}

int TableExecutor::_funcColprod(lua_State *L) {
    _executor->_performRangeFunc(FUNC_COLPROD, rangeProd, false);
    return 1;
}

int TableExecutor::_funcColavg(lua_State *L) {
    _executor->_performRangeFunc(FUNC_COLAVG, rangeAvg, false);
    return 1;
}

int TableExecutor::_funcColmin(lua_State *L) {
    _executor->_performRangeFunc(FUNC_COLMIN, rangeMin, false);
    return 1;
}

int TableExecutor::_funcColmax(lua_State *L) {
    _executor->_performRangeFunc(FUNC_COLMAX, rangeMax, false);
    return 1;
}
