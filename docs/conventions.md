# tinyDFA Coding and Documentation Conventions

## General Rules
- tiny is always `tiny`, DFA is `DFA`, and tinyDFA is `tinyDFA`
- Public symbols live in `tiny::DFA`
- Initialisms are uppercase when practical
- Camel Case
  > ex: `FirstSecondThird`
  - Class methods
  - Class names
  - Type names
- Lower Snake Case
  > ex: `first_second_third`
  - Functions
- Capital Snake Case
  > ex: `FIRST_SECOND_THIRD`
  - Compile time configuration
- Literal Snake Case
  > ex: `Folder/Name/file_name.ext`
  - Files and Folder References
- Leading Underscore
  > ex: `_FirstSecondThird` `_first_second_third` `_FIRST_SECOND_THIRD`
  - Internal Use / Build Dependant Symbols

## File and Folder Naming
- Spaces should always be replaced by underscore
- File Names
  - Initial capital letter for items that define **Classes**
  - Fully lowercase for other items is preferred
  - Exceptional items
    - README.md
    - LICENSE
- Folder Names
  - Folders that contain logical portions of a source file **must** preserve the case of the source
  - Fully lowercase for other items is preferred
- Documentation
  - Should preserve the path and file case that the document references
- Existing Structure
  - `release/`
    - Contains the historic and current builds of tinyDFA in point-release folders
    - Unstable releases are marked with a 'development_release' file
  - `docs/`
    - Contains documentation pertaining to tinyDFA as a whole, with specialized documentation being located in a subfolder
  - `docs/src/`
    - Documentation for source files to augment the internal doxygen comments
  - `docs/reference/`
    - General explanations and concepts for implementing tinyDFA
  - `examples/`
    - Example projects for the Arduino Studio
  - `tinyDFA/`
    - Resources for tinyDFA
  - `tinyDFA/keywords/`
    - Macro definitions that define keywords
  - `tinyDFA/structure/`
    - Macro definitions that simplify program structure
  - `tinyDFA/State/`
    - Default state definitions

## Macro Naming
- Internal symbols (macro supporting macros)
  > `_tinyDFA_[Path]_[File]_[Name]{_[Parameters]}`  
  - Path: Path to this header from library root
  - File: The literal file name without extension
  - Name: The macro symbol name
  - _Parameters_: How many parameters does this macro expect
  - `_tinyDFA_structure_process_Process_2` 
- Header guard
  > `_tinyDFA_[Path]_[File]_[Name]_h`  
  - Path: Path to this header from library root
  - File: The literal file name without extension
  - Name: The macro symbol name
- Configuration option
  - In upper snake case  `OUTPUT_DEVICE`
  - Meaning should be logically clear
  - Option Flags (disable)
    - Template: `OMIT_tinyDFA_[OPTION]
    - Option name should be a tinyDFA feature
    - Value agnostic (Internally #ifndef is used)
- Structures
  - Should reflect the classes the scaffold
    - `State(S)` expands to `class S : State`
    - `Device(S,T)` expands to `class S : Device<T>`
- Keywords
  - Should be lower camel case
  - Symbols should be treated as const 
    
## Source Code
- Open braces on line `if (...) {`
- Break on 80 (or 132)
- No object (documentation excepted) should exceed an A4 page
- No repeated code
- Reuse before reinvention