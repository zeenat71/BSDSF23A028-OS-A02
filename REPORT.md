# REPORT.md

## Feature 4: ls-v1.3.0 – Horizontal Column Display (-x Option)

### 1. Implementation Complexity: "Down Then Across" vs "Across" Printing Logic

**Down Then Across (Vertical) Printing Logic:**
- Isme files ko **row by row** print karte hain, matlab pehle har column ka first item, phir second row, etc.
- **Pre-calculation required:**
  - Total number of files
  - Maximum filename length
  - Number of rows aur columns
- Reason: Row-major printing nahi hoti, isliye pehle rows aur columns calculate karna zaroori hai taake alignment aur spacing sahi ho.
- Complexity: **High** – multiple iterations aur alignment logic chahiye.

**Across (Horizontal) Printing Logic:**
- Isme files ko **left to right** print karte hain aur line wrap tab hota hai jab terminal width cross ho.
- **Pre-calculation required:**
  - Maximum filename length (for column spacing)
  - Current cursor position (horizontal tracking)
- Reason: Simple row-major iteration, space padding aur newline management ke alawa zyada calculation nahi chahiye.
- Complexity: **Lower** – mostly sequential iteration aur line wrapping.

**Conclusion:**  
"Down then across" vertical printing requires more pre-calculation and planning, whereas horizontal "across" printing is simpler and more straightforward.

---

### 2. Strategy to Manage Different Display Modes

- **Display Modes Implemented:**
  - Default vertical (down then across)
  - Long-listing (-l)
  - Horizontal (-x)

- **How program decides which function to call:**
  1. **Command-line arguments parsed using `getopt()`**.
  2. A **display_mode flag** (integer or enum) is set based on the options:
     - `0` – default vertical
     - `1` – long-listing (-l)
     - `2` – horizontal (-x)
  3. After reading all filenames into an array, **conditional checks** call the appropriate function:
     - If `display_mode == 1` → call `do_ls_long()`
     - Else if `display_mode == 2` → call `do_ls_horizontal()`
     - Else → call `do_ls_vertical()`
- **Benefit:** Centralized control allows easy addition of future display modes without modifying main logic heavily.

---

### 3. Additional Notes

- Terminal width detected using `ioctl(TIOCGWINSZ)` for dynamic column calculation.
- Maximum filename length is used to determine spacing between columns.
- Horizontal mode ensures clean wrapping without breaking words or misalignment.
- Default vertical mode preserves compatibility with previous versions.

---
