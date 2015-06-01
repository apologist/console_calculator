# console_calculator
Console calculator with the ability to set variables.
Using reverse polish notation for calculation.

Example :
```c
>var a = 10
>var b = 100
>a/b
0.100000
>b/a
10.000000
>var c = a*b
>vars
+-----------+-----------+
|       Name|      Value|
+-----------+-----------+
|          a|  10.000000|
|-----------|-----------|
|          b| 100.000000|
|-----------|-----------|
|          c|1000.000000|
+-----------+-----------+
>( 8 + 2 * 5 ) / ( 1 + 3 * 2 - 4 )
6.000000
```
