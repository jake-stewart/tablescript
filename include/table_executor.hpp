#ifndef TABLE_EXECUTOR_HPP
#define TABLE_EXECUTOR_HPP

extern "C" {
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

#include "border.hpp"
#include "range_functions.hpp"
#include "table.hpp"
#include <optional>
#include <string>
#include <vector>

struct TablescriptSharedData
{
    std::optional<std::vector<int>> widths;
    std::optional<std::vector<int>> maxWidths;
    std::optional<int> width;
    std::optional<int> maxWidth;
    int padding;
    BorderStyle borderStyle;
    BorderStyle separatorStyle;
};

static const char *FUNC_WIDTH = "width";
static const char *FUNC_MAXWIDTH = "maxwidth";
static const char *FUNC_WIDTHS = "widths";
static const char *FUNC_MAXWIDTHS = "maxwidths";
static const char *FUNC_BORDER = "border";
static const char *FUNC_SEPARATOR = "separator";
static const char *FUNC_PADDING = "padding";
static const char *FUNC_DECIMALS = "decimals";
static const char *FUNC_ROUND = "round";
static const char *FUNC_FLOOR = "floor";
static const char *FUNC_CEIL = "ceil";
static const char *FUNC_LIST = "list";
static const char *FUNC_ROWRANGE = "rowrange";
static const char *FUNC_ROWSUM = "rowsum";
static const char *FUNC_ROWPROD = "rowprod";
static const char *FUNC_ROWAVG = "rowavg";
static const char *FUNC_ROWMIN = "rowmin";
static const char *FUNC_ROWMAX = "rowmax";
static const char *FUNC_COLRANGE = "colrange";
static const char *FUNC_COLSUM = "colsum";
static const char *FUNC_COLPROD = "colprod";
static const char *FUNC_COLAVG = "colavg";
static const char *FUNC_COLMIN = "colmin";
static const char *FUNC_COLMAX = "colmax";

class TableExecutor
{
    static TableExecutor *_executor;
    TablescriptSharedData *_sharedData;
    bool _setSeparatorStyle = false;
    lua_State *_luaState;
    Table &_table;
    int _precision = 2;
    int _tenPowPrecision = 100;

    int _row;
    int _col;

public:
    TableExecutor(Table &table) : _table(table) {}
    void setSharedData(TablescriptSharedData *sharedData);
    void execute();

private:
    void _executeTable();
    void _exec(std::string code);
    std::string _eval(std::string code);
    void _addCallbacks();
    bool _validateIntArg(const char *funcName, int *value);
    bool _validateNumberArg(const char *funcName, lua_Number *value);
    bool _validateIntArgs(const char *funcName, std::vector<int> &value);
    bool _validateBorderArg(const char *funcName, int *value);
    void _pushNumber(lua_Number number);
    void _pushInteger(lua_Integer number);
    void _pushString(std::string str);

    std::vector<std::string> _findNumbersInRow(int n);
    std::vector<std::string> _findNumbersInCol(int n);

    static int _funcWidth(lua_State *L);
    static int _funcMaxWidth(lua_State *L);
    static int _funcWidths(lua_State *L);
    static int _funcMaxWidths(lua_State *L);
    static int _funcBorder(lua_State *L);
    static int _funcSeparator(lua_State *L);
    static int _funcPadding(lua_State *L);
    static int _funcList(lua_State *L);
    static int _funcDecimals(lua_State *L);

    static int _funcRound(lua_State *L);
    static int _funcFloor(lua_State *L);
    static int _funcCeil(lua_State *L);

    bool _performRangeFunc(
        const char *funcName,
        void (*func)(lua_State *, std::vector<std::string> &), bool rowWise
    );

    static int _funcRowrange(lua_State *L);
    static int _funcRowsum(lua_State *L);
    static int _funcRowprod(lua_State *L);
    static int _funcRowavg(lua_State *L);
    static int _funcRowmin(lua_State *L);
    static int _funcRowmax(lua_State *L);
    static int _funcColrange(lua_State *L);
    static int _funcColsum(lua_State *L);
    static int _funcColprod(lua_State *L);
    static int _funcColavg(lua_State *L);
    static int _funcColmin(lua_State *L);
    static int _funcColmax(lua_State *L);
};

#endif
