# JsonManager

A small C++17 console application for managing a list of workers persisted in a JSON file. The program loads a JSON array, lets you add/edit/remove/search/sort/filter workers and computes simple statistics — every mutation is flushed to disk immediately, so the file is always the source of truth.

![screen](https://github.com/user-attachments/assets/e2067800-d9eb-47ca-815a-3cafe30299fb)

## JSON schema

The file must contain a JSON **array** whose elements have these five fields:

```json
[
  {
    "name":       "John",
    "surname":    "Doe",
    "tax_code":   "ABCDEFGHIJKLMNOP",
    "age":        30,
    "assignment": "php developer"
  }
]
```

Field names are centralized in `utils/JsonParamFeatures.h`. Renaming a field on disk is a one-line change there — no other source file hard-codes these strings.

### Validation rules

- `name`, `surname`, `tax_code`, `assignment` are non-empty after trimming
- `tax_code` is exactly 16 characters long and unique across the file
- `age` is in the `[18, 65)` range

Failing any rule when adding a worker raises an error and leaves the file untouched.

## Features

| Key | Action                                                                      |
|-----|-----------------------------------------------------------------------------|
| `p` | Print every worker                                                          |
| `n` | Add a new worker (validated end-to-end)                                     |
| `m` | Change the assignment (role) of a worker                                    |
| `a` | Update a worker's age                                                       |
| `r` | Remove a worker                                                             |
| `c` | Search workers by name/surname/tax code (case-insensitive substring match)  |
| `f` | Filter workers by assignment                                                |
| `s` | Sort by `name`, `surname`, `age` or `assignment`                            |
| `i` | Statistics (count, average/min/max age, distribution by role)               |
| `g` | Print a greeting line from every worker                                     |
| `k` | Clear the whole JSON (in memory and on disk)                                |
| `?` | Re-print the menu                                                           |
| `e` | Exit                                                                        |

Searches are *case-insensitive substring* matches, so typing `doe`, `Doe` or even `OE` finds *John Doe*.

## Architecture

```
employee/
├── main.cpp                  # Entry point + interactive REPL
├── model/
│   ├── Person.h              # Abstract base: identity fields + validation
│   ├── Worker.h              # Concrete worker with an "assignment"
│   └── Worker.cpp
├── parser/
│   └── JsonParser.h          # Pure I/O layer (load / write / clear / normalize path)
├── storage/
│   ├── Storage.h             # In-memory list + business operations
│   └── Storage.cpp
├── utils/
│   ├── Function.h            # trim, toLower, icontains
│   └── JsonParamFeatures.h   # Centralized JSON field names
└── libs/
    └── json.hpp              # nlohmann::json (single-header library)
```

The layers are strictly stacked:

```
 main.cpp   ──▶   Storage   ──▶   JsonParser   ──▶   filesystem
   (UI)         (business)         (I/O)
                    │
                    ▼
                  Worker  ◀──  Person   ◀── Function (helpers)
                            (domain types + invariants)
```

- `main.cpp` owns the user-facing REPL. It never touches the JSON or the filesystem directly — it only talks to `Storage`.
- `Storage` is the only mutator of the in-memory `vector<Worker>`. Every successful mutation is followed by a single full-file rewrite via `JsonParser`.
- `JsonParser` knows nothing about business rules; it only marshals between `Worker` objects and `nlohmann::json` and handles file I/O.
- `Worker` and `Person` enforce all domain invariants in their constructors and validated setters, so any `Worker` instance the rest of the code sees is by construction valid.

Adding a feature like *"list workers older than X"* only touches `Storage` (a new method) and the menu in `main.cpp` (one new case). Renaming a JSON field only touches `utils/JsonParamFeatures.h`.

## Build

Requires CMake ≥ 3.28 and a C++17 compiler.

```bash
cmake -B build
cmake --build build
./build/JsonManager
```

## Run

On startup you are asked for the path to the JSON file. If the file is missing, malformed, or not a top-level array you get an error and you can retry — the program does not exit on a bad path. From there the menu drives every operation; type `?` to re-print it at any time.

## License

Released under the GPL-3.0 — see [`LICENSE`](LICENSE).
