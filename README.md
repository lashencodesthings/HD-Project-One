# HD-Project-One
High Distinction Project for FIT1045

How to run:
clang++ -std=c++20 (Get-ChildItem -Recurse -Filter *.cpp | ForEach-Object { $_.FullName }) -I src -l splashkit -o test -Wall