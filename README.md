Tablescript
===========

Building
--------

```
cmake -S . -B build
cd build
make
```

Usage
-----

```
./tablescript file.tbl
```

or

```
cat file.tbl | ./tablescript
```


Syntax
------

### Cells
Cells are separated by the `|` operator.
Rows are separated by newlines.

```
Script               Result

A | B                ┌───┬───┐
C | C                │ A │ B │
                     │ C │ D │
                     └───┴───┘
```


### Row Separators
The `-` operator adds a thin row separator.
The `=` operator adds a thick row separator.
The separator can be any length.

```
Script               Result

A | B                ┌───┬───┐
-----                │ A │ B │
C | C                ├───┼───┤
                     │ C │ D │
                     └───┴───┘

A | B                ┌───┬───┐
=====                │ A │ B │
C | C                ╞═══╪═══╡
                     │ C │ D │
                     └───┴───┘
```

### Horizontal Merge
The `<` operator will merge a cell with the cell to its left.

```
Script               Result

A | <                ┌───────┐
C | D                │ A     │
                     ├───┬───┤
                     │ C │ D │
                     └───┴───┘
```

### Vertical Merge
The `^` operator will merge a cell with the cell above.

```
Script               Result

A | B                ┌───┬───┐
^ | C                │ A │ B │
                     │   ├───┤
                     │   │ D │
                     └───┴───┘
```

### Alignment
The `?` operator, followed by alignment operator(s) can align a cell.

```alignments.tbl
┌────────┬───────────────────┐
│ Symbol │ Alignment         │
├────────┼───────────────────┤
│ l      │ Horizontal Left   │
│ c      │ Horizontal Center │
│ r      │ Horizontal Right  │
├────────┼───────────────────┤
│ t      │ Vertical Top      │
│ m      │ Vertical Middle   │
│ b      │ Vertical Bottom   │
└────────┴───────────────────┘
```

```
Script               Result   
                      
?r A | <             ┌───────┐
C    | D             │     A │
                     ├───┬───┤
                     │ C │ D │
                     └───┴───┘

?rm A | B            ┌────┬───┐
^     | D            │    │ B │
                     │  A ├───┤
                     │    │ D │
                     └────┴───┘
```

### Code Snippets
Lua code can be inserted using curly braces (`{` and `}`).
Code will be treated as expressions if the code is a single line,
otherwise they are statements. You can escape code as expected with `\{}`.
For more information about scripting, look at the Scripting section.

```
Script               Result   

A | {1 + 1}          ┌───┬───┐
-----------          │ A │ 2 │
C | {3 * 3}          ├───┼───┤
                     │ C │ 9 │
                     └───┴───┘
```


Scripting
---------

### Functions
```functions.tbl
  Function           Description                                                
 ────────────────────────────────────────────────────────────────────────────── 
  width(int)         Set the table width. A value of zero will make the table   
                     stretch-to-fit.                                            
                                                                                
  maxwidth(int)      Set the maximum width the table can grow to. A value of    
                     zero means infinite max width. Default is 80.              
                                                                                
  widths(*int)       Set each column width individually. A value of zero will   
                     make the column stretch-to-fit.                            
                                                                                
  maxwidths(*int)    Set the maximum width for each column individually. A      
                     value of zero means infinite max width.                    
                                                                                
  border(STYLE)      Set the table border style. See the Table Style section    
                     for possible styles.                                       
                                                                                
  separator(STYLE)   Set the table thick separator style (the equals            
                     operator). See the Table Style section for possible        
                     styles.                                                    
                                                                                
  rowrange(int?)     Create a table containing all the numbers in the current   
                     row up to the current column. If a number is provided,     
                     then only cells within that distance to the current        
                     column are included. Only the first number found in each   
                     cell is used. The current cell is excluded.                
                                                                                
  rowsum(int?)       Sum all the numbers found using the rowrange.              
                                                                                
  rowprod(int?)      Mulitiply all the numbers found using rowrange.            
                                                                                
  rowavg(int?)       Average all the numbers found using rowrange.              
                                                                                
  rowmax(int?)       Find the largest number in the numbers found using         
                     rowrange.                                                  
                                                                                
  rowmin(int?)       Find the smallest number in the numbers found using        
                     rowrange.                                                  
                                                                                
  colrange(int?)     Create a table containing all the numbers in the current   
                     column up to the current row. If a number is provided,     
                     then only cells within that distance to the current row    
                     are included. Only the first number found in each cell is  
                     used. The current cell is excluded.                        
                                                                                
  colsum(int?)       Sum all the numbers found using the colrange.              
                                                                                
  colprod(int?)      Mulitiply all the numbers found using colrange.            
                                                                                
  colavg(int?)       Average all the numbers found using colrange.              
                                                                                
  colmax(int?)       Find the largest number in the numbers found using         
                     colrange.                                                  
                                                                                
  colmin(int?)       Find the smallest number in the numbers found using        
                     colrange.                                                  
```

Styles
------
### NONE
```
    Example Table
 ───────────────────
  Row One   Row One
  Row Two   Row Two
```

### SINGLE
```
┌───────────────────┐
│   Example Table   │
├─────────┬─────────┤
│ Row One │ Row One │
│ Row Two │ Row Two │
└─────────┴─────────┘
```

### DOUBLE
```
╔═══════════════════╗
║   Example Table   ║
╠═════════╦═════════╣
║ Row One ║ Row One ║
║ Row Two ║ Row Two ║
╚═════════╩═════════╝
```

### THICK
```
┏━━━━━━━━━━━━━━━━━━━┓
┃   Example Table   ┃
┣━━━━━━━━━┳━━━━━━━━━┫
┃ Row One ┃ Row One ┃
┃ Row Two ┃ Row Two ┃
┗━━━━━━━━━┻━━━━━━━━━┛
```

### ROUND
```
╭───────────────────╮
│   Example Table   │
├─────────┬─────────┤
│ Row One │ Row One │
│ Row Two │ Row Two │
╰─────────┴─────────╯
```

### ASCII
```
+-------------------+
|   Example Table   |
+---------+---------+
| Row One | Row One |
| Row Two | Row Two |
+---------+---------+
```

### THICK_ASCII
```
+===================+
|   Example Table   |
+=========+=========+
| Row One | Row One |
| Row Two | Row Two |
+=========+=========+
```

### DOTTED
```
.....................
:   Example Table   :
.....................
: Row One : Row One :
: Row Two : Row Two :
.....................
```

### DASHED
```
┌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌┐
┆   Example Table   ┆
├╌╌╌╌╌╌╌╌╌┬╌╌╌╌╌╌╌╌╌┤
┆ Row One ┆ Row One ┆
┆ Row Two ┆ Row Two ┆
└╌╌╌╌╌╌╌╌╌┴╌╌╌╌╌╌╌╌╌┘
```

### THICK_DASHED
```
┏╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍┓
┇   Example Table   ┇
┣╍╍╍╍╍╍╍╍╍┳╍╍╍╍╍╍╍╍╍┫
┇ Row One ┇ Row One ┇
┇ Row Two ┇ Row Two ┇
┗╍╍╍╍╍╍╍╍╍┻╍╍╍╍╍╍╍╍╍┛
```


### BLOCK
```
█▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀█
█   Example Table   █
█▀▀▀▀▀▀▀▀▀█▀▀▀▀▀▀▀▀▀█
█ Row One █ Row One █
█ Row Two █ Row Two █
▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
```

Example Tables
--------------

### Purchase Order

#### Script
```
Name     | Price | Quantity | Total
===
Tomato   | $1.99 | 8        | { rowprod() }
Potato   | $0.50 | 3        | { rowprod() }
Bread    | $3.00 | 1        | { rowprod() }
Lettuce  | $0.85 | 2        | { rowprod() }
Ham      | $3.00 | 5        | { rowprod() }
?r Total | <     | <        | { colsum() }
```

#### Output
```purchase_order.tbl
┌─────────┬───────┬──────────┬────────┐
│ Name    │ Price │ Quantity │ Total  │
╞═════════╪═══════╪══════════╪════════╡
│ Tomato  │ $1.99 │        8 │ $15.92 │
│ Potato  │ $0.50 │        3 │  $1.50 │
│ Bread   │ $3.00 │        1 │  $3.00 │
│ Lettuce │ $0.85 │        2 │  $1.70 │
│ Ham     │ $3.00 │        5 │ $15.00 │
├─────────┴───────┴──────────┼────────┤
│                      Total │ $37.12 │
└────────────────────────────┴────────┘
```

#### Script
```
{
dog_desc = "Dogs are domesticated mammals known for their loyalty and companionship to humans. They come in various breeds, sizes, and colors, and are social animals that form strong bonds with their owners and families. Dogs are versatile and serve various roles, including working, service, and pet companions. They are better than cats."
dog_dot_points = [[
- Domesticated
- Loyal
- Cute
]]

cat_desc = "Cats are small, carnivorous mammals that have been domesticated for thousands of years. They are known for their independent and curious nature, often displaying agility and grace. Cats make affectionate pets and are valued for their companionship, hunting skills, and ability to adapt to different environments."
cat_dot_points = list("Feral", "Annoying", "Ugly")

rat_desc = "Rats are small rodents found worldwide, known for their quick movements and ability to squeeze through tiny openings. They have a compact body, a pointed snout, and large ears. Rats are often considered pests due to their ability to damage crops and spread diseases, but they also play a crucial role in various ecosystems as prey for predators and as experimental subjects in scientific research."
rat_dot_points = "- Better than cats"
}

===
Name | Description | Dot points
===
Dog  | { dog_desc }  | { dog_dot_points }
---
Cat  | { cat_desc }  | { cat_dot_points }
---
Rat  | { rat_desc }  | { rat_dot_points }
---
```

#### Output
```animals.tbl
╒══════╤══════════════════════════════════════════════════╤════════════════════╕
│ Name │ Description                                      │ Dot points         │
╞══════╪══════════════════════════════════════════════════╪════════════════════╡
│ Dog  │ Dogs are domesticated mammals known for their    │ - Domesticated     │
│      │ loyalty and companionship to humans. They come   │ - Loyal            │
│      │ in various breeds, sizes, and colors, and are    │ - Cute             │
│      │ social animals that form strong bonds with their │                    │
│      │ owners and families. Dogs are versatile and      │                    │
│      │ serve various roles, including working, service, │                    │
│      │ and pet companions. They are better than cats.   │                    │
├──────┼──────────────────────────────────────────────────┼────────────────────┤
│ Cat  │ Cats are small, carnivorous mammals that have    │ - Feral            │
│      │ been domesticated for thousands of years. They   │ - Annoying         │
│      │ are known for their independent and curious      │ - Ugly             │
│      │ nature, often displaying agility and grace. Cats │                    │
│      │ make affectionate pets and are valued for their  │                    │
│      │ companionship, hunting skills, and ability to    │                    │
│      │ adapt to different environments.                 │                    │
├──────┼──────────────────────────────────────────────────┼────────────────────┤
│ Rat  │ Rats are small rodents found worldwide, known    │ - Better than cats │
│      │ for their quick movements and ability to squeeze │                    │
│      │ through tiny openings. They have a compact body, │                    │
│      │ a pointed snout, and large ears. Rats are often  │                    │
│      │ considered pests due to their ability to damage  │                    │
│      │ crops and spread diseases, but they also play a  │                    │
│      │ crucial role in various ecosystems as prey for   │                    │
│      │ predators and as experimental subjects in        │                    │
│      │ scientific research.                             │                    │
└──────┴──────────────────────────────────────────────────┴────────────────────┘
```
