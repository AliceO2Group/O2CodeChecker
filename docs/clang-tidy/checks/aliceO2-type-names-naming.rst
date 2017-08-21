.. title:: clang-tidy - aliceO2-type-names-naming

aliceO2-type-names-naming
=========================

Type names follow camel case convention and start with an upper case letter: MyClass, MyEnum.

Description:

The names of all types — classes, structs, typedefs, and enums — have the same naming convention. For example:

// classes and structs
class UrlTable 
{ ...
class UrlTableTester 
{ ...
struct UrlTableProperties 
{ ...

// typedefs
typedef HashMap<UrlTableProperties *, string> PropertiesMap;

// enums
enum UrlTableErrors { ...
