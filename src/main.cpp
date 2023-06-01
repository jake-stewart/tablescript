#include "../include/table_auto_formatter.hpp"
#include "../include/table_executor.hpp"
#include "../include/table_height_sizer.hpp"
#include "../include/table_lexer.hpp"
#include "../include/table_parser.hpp"
#include "../include/table_printer.hpp"
#include "../include/table_width_sizer.hpp"
#include "../include/table_word_wrapper.hpp"
#include "../include/util.hpp"
#include <cstdio>
#include <exception>
#include <string>

std::string handleArgs(int argc, const char **argv) {
    if (argc == 1) {
        try {
            return catStdin();
        }
        catch (std::exception) {
            printf("\n");
            exit(1);
        }
    }
    else if (argc == 2) {
        try {
            return catFile(argv[1]);
        }
        catch (std::exception ex) {
            printf("Could not read file '%s'\n", argv[1]);
            exit(1);
        }
    }
    else {
        printf("Usage: %s [file.tbl]\n", argv[0]);
        exit(1);
    }
}

int main(int argc, const char **argv) {
    Table table;
    TableLexer lexer;
    TableBuilder builder(table);
    TableParser parser(builder);
    TableExecutor executor(table);
    TableWordWrapper wrapper(table);
    TableWidthSizer widthSizer(table);
    TableHeightSizer heightSizer(table);
    TableAutoFormatter formatter(table);
    TablePrinter printer(table);

    TablescriptSharedData sharedData;
    sharedData.borderStyle = SINGLE;
    sharedData.separatorStyle = DOUBLE;
    sharedData.padding = 1;
    sharedData.maxWidth = 80;

    std::string script = handleArgs(argc, argv);

    lexer.lex(script);
    parser.parse(&lexer.getTokens());

    builder.fixFullMerges();

    if (table.rows.size() == 0) {
        return 0;
    }

    executor.setSharedData(&sharedData);
    executor.execute();
    table.padding = sharedData.padding;
    table.border = sharedData.borderStyle;
    table.separator = sharedData.separatorStyle;

    builder.deleteEmptyRows();
    builder.deleteEmptyColumns();

    TableWidthConfig widthConfig;
    widthConfig.width = sharedData.width;
    widthConfig.maxWidth = sharedData.maxWidth;
    widthConfig.columnWidths = sharedData.widths;
    widthConfig.maxColumnWidths = sharedData.maxWidths;
    widthSizer.setWidthConfig(widthConfig);

    widthSizer.applySizing();
    heightSizer.applySizing();
    formatter.format();
    printer.print();
}
