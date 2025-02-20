# Guide

### Basics

> For a basic video tutorial, see this [thread by Failcat](https://twitter.com/Faiilcat/status/1891523246878503360)

This tool places several copies of the selected objects, varying various properties such as position, rotation or HSV.

Each box allows you to enter a mathematical formula, for instance `1`, `3 + t` or `5 * sin(t + 90) + 2`
The "Amount" specifies the number of copies the tool should place.

"Start t" and "End t" sets bounds for `t`. Formulas can be parameterized by `t` to change the properties for each copy.
For instance, if you set "Amount" to `5`, "Start t" to `10.0` and "End t" to `20.0`, the first copy will have `t = 10.0`, the second copy will have `t = 12.0`, and so on. Note that this range is exclusive, the "End t" value will never be reached: the last copy will have `t = 18.0` instead of `t = 20.0`.

> For some example presets, check the folder at the top right. You can also save your own presets too (see Buttons).

Join the [Discord server](https://discord.gg/cWng6AamgY) for additional help and feedback.

### Fields

- Position x(t):
    - Moves copy horizontally.
    - Ex: `1` moves copies 1 block to the right
    - See also: use robtop unit

- Position y(t):
    - Moves copy vertically.
    - Ex: `1` moves copies 1 block up.
    - See also: use robtop unit

- Rotation(t):
    - Rotates copy by some angle.
    - Angles are in degrees, unless the "Use radians" option is enabled, in which case angles will be in radians.

- Scale X(t):
    - Scales copy horizontally.
    - Examples: 
        - `1` will not stretch the copy horizontally.
        - `2` will double the copy's width
        - `-1` will flip the copy horizontally.

- Scale Y(t):
    - Scales copy vertically.

> Note that because of how GD handles scaling, scaling rotated or warped objects on only one axis may not give the expected result.

- HSV:
    - Will change base and detail HSV values of the copy.
    - Changes are relative to each object's own values, for instance putting `0.2` in base hue will *add* 0.2 to the existing hue of each object instead of setting every object's base hue to 0.2.

### Supported operations
- `+`, `-`, `*`, `/`, `**`
    - Basic operations
    - Division is float division: `5 / 2` will evaluate to `2.5`.
    - `**` is exponentiation: `3 ** 2` will evaluate to `9`. Non-integer exponents are supported.
- `random()`
    - Outputs a random number between `0.0` and `1.0`.
- `cos(...)`, `sin(...)`, `tan(...)`:
    - Trigonometric functions
    - Arguments are in degrees if the "Use Radians" options is disabled, otherwise in radians.
        - With "Use Radians": `sin(pi / 2)` evaluates to `1.0`
        - Without "Use Radians": `sin(90)` evaluates to `1.0`
- `acos(...)`, `asin(...)`, `atan(...)`
    - Inverse trigonometric functions
    - Results are in degrees if the "Use Radians" options is disabled, otherwise in radians
- `sqrt(...)`, `cbrt(...)`
    - Square root and cube root
- `exp(...)`, `ln(...)`, `log10(...)`, `log2(...)`
    - Exponential function, natural logarithm, base 10 logarithm base 2 logarithm.
- `cosh(...)`, `sinh(...)`, `tanh(...)`
    - Hyperbolic trigonometric functions
- `abs(...)`, `sgn(...)`
    - Absolute value: `-3` will evaluate to `3`.
    - Sign: will return `1` if argument is positive, `-1` if argument is negative or `0` if argument is zero.
- `round(...)`, `floor(...)`, `ceil(...)`
    - `round(4.7)` evaluates to `5`.
    - `floor(4.2)` evaluates to `4`.
    - `ceil(4.2)` evaluates to `5`.
- `min(..., ...)`, `max(..., ...)`
    - Returns the minimum/maximum of both arguments.
- `pi`
    - The mathematical constant
    - Will evaluate to approximately `3.14`.

### Buttons

The folder button (top right) allows you to browse presets. The copy button of a preset copies the preset string to your clipboard. This allows you to paste it somewhere else and share it with someone else!

The paste button pastes a preset string from your clipboard.

The `+` button allows you to save new presets.

The lock button (bottom right) allows you to keep your configuration when you exit the tool instead of clearing it.

### Options

- Absolute Scaling
    - Scales objects without moving their position.
- Absolute Rotation
    - Rotates objects without moving their position.
- Use Robtop Unit
    - By default in position fields, `1` means one block to the right/to the top.
    - Enabling this option makes the tool the game's internal units, in which 1 block is 30 units.
- Use radians
    - See rotation(t) in Basics and trigonometric functions in Supported operations.
- Delete original
    - By default the tool will not delete the original objects you selected before entering the tool.
    - Enabling this option will make the tool delete those objects.
    - This is useful when the original objects are replaced or when they are only used as a reference position.
