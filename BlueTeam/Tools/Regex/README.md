# Regular Expressions (Regex)

## Overview
Regular Expressions (Regex) are powerful patterns for searching and matching text in logs, enabling you to find specific strings, patterns, or anomalies (e.g., words starting with "A" or IPs ending in numbers). This module teaches regex basics—charsets, wildcards, metacharacters, repetitions, anchors, and groups—for log analysis. Designed for beginners and intermediate learners, it includes a hands-on activity using `egrep` on a Unix system or online tools like [Regex101](https://regex101.com/) or [CyberChef](https://gchq.github.io/CyberChef/).

## Why Regex?
Regex lets you search logs beyond exact strings, matching patterns like:
- Lines starting with "error".
- IPs with specific ranges.
- Usernames followed by digits.

**Analogy**: Regex is like a customizable filter, sifting through logs to find exactly what you need.

## Regex Basics
Regex matches patterns in text using special characters and constructs.

### Charsets
Match specific characters or ranges using `[square brackets]`.
- `[abc]`: Matches `a`, `b`, or `c`.
- `[a-c]`: Matches `a`, `b`, or `c` (range).
- `[a-cx-z]zz`: Matches `azz`, `bzz`, `czz`, `xzz`, `yzz`, `zzz`.
- `[a-zA-Z]`: Any letter (lowercase or uppercase).
- `file[1-3]`: Matches `file1`, `file2`, `file3`.
- `[^k]ing`: Matches `ring`, `sing`, `$ing`, but not `king`.
- `[^a-c]at`: Matches `fat`, `hat`, but not `bat`, `cat`.

### Wildcard
- `.`: Matches any single character (except newline).
  - `a.c`: Matches `abc`, `a@c`, `a1c`.
  - `a\.c`: Matches literal `a.c` (escapes the dot).

### Optional Characters
- `?`: Makes the preceding character optional.
  - `abc?`: Matches `ab` or `abc`.

### Metacharacters
Shorthand for common character types:
- `\d`: Any digit (`0-9`).
- `\D`: Non-digit (e.g., `A`, `@`).
- `\w`: Alphanumeric or underscore (e.g., `a`, `3`, `_`).
- `\W`: Non-alphanumeric (e.g., `!`, `#`).
- `\s`: Whitespace (space, tab, newline).
- `\S`: Non-whitespace.

### Repetitions
Match multiple occurrences of a pattern:
- `{n}`: Exactly `n` times (e.g., `z{2}` matches `zz`).
- `{n,m}`: `n` to `m` times (e.g., `\d{1,3}` matches `7`, `42`, `123`).
- `{n,}`: `n` or more times (e.g., `\d{2,}` matches `12`, `1234`).
- `*`: 0 or more times (e.g., `\w*` matches `""`, `abc`).
- `+`: 1 or more times (e.g., `\d+` matches `123`, `45678`).

### Anchors
Specify position in a line:
- `^`: Start of line (e.g., `^error` matches lines starting with "error").
- `$`: End of line (e.g., `failed$` matches lines ending with "failed").

**Note**: `^` inside `[]` excludes characters (e.g., `[^a-c]`); outside, it’s an anchor.

### Groups
Use `(parentheses)` for grouping or alternation:
- `(day|night)`: Matches `day` or `night` (e.g., `during the (day|night)` matches `during the day` or `during the night`).
- `(no){5}`: Matches `nonononono`.



**Alternative: Test Online**:
   - Go to [Regex101](https://regex101.com/) or [CyberChef](https://gchq.github.io/CyberChef/) (use "Regular expression" operation).
   - Paste the `test.log` content into the text field.
   - Try patterns like `^error`, `\w+\d+`, or `(logged in|logged out)$`.
   - View matches and explanations in the UI.

## Best Practices
- **Be Specific**: Use precise patterns (e.g., `\d{1,3}` for 1-3 digits, not `\d*`).
- **Escape Special Characters**: Use `\` for literals (e.g., `a\.c` for `a.c`).
- **Test Patterns**: Validate with `egrep` or online tools to avoid over-matching.
- **Use Premade Regex**: For complex patterns (e.g., emails), search for validated expressions online.
- **Combine Constructs**: Use groups and anchors for targeted matches (e.g., `^(\w+\d+)$`).

## Troubleshooting
- **No Matches**: Check pattern syntax (e.g., `^error` vs `error`).
- **Too Many Matches**: Narrow with anchors (`^`, `$`) or specific charsets.
- **Special Characters**: Escape dots (`.`) or slashes (`\/`) with `\`.
- **Case Sensitivity**: Use `egrep -i` for case-insensitive matching.

## Learning Tips
- **Practice with `egrep`**: Run `egrep 'pattern' test.log` on sample logs.
- **Use Online Tools**: Test patterns on [Regex101](https://regex101.com/) for instant feedback.
- **Cheat Sheet**: Save patterns like `\d+`, `^error`, `[a-zA-Z]+`.
- **Start Simple**: Begin with `[a-z]`, then add complexity (e.g., `\w+\d{2}`).
- **Study Logs**: Apply regex to real logs (e.g., `/var/log/syslog`).

## Resources
- [Regex101](https://regex101.com/): Interactive regex tester.
- [CyberChef](https://gchq.github.io/CyberChef/): Multi-tool for regex and log analysis.
- [Regular-Expressions.info](https://www.regular-expressions.info/): Detailed regex guide.
- [Linux man egrep](https://man7.org/linux/man-pages/man1/grep.1.html): `egrep` documentation.

