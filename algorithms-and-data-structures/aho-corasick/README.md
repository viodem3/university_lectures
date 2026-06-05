# Aho-Corasick Automaton with DFA Optimization

This repository contains a clean, modern **C++20** object-oriented implementation of the **Aho-Corasick algorithm**.

The implementation transitions from a standard trie with failure links to a fully flat **Deterministic Finite Automaton (DFA)**, ensuring strictly **O(1)** time complexity per character during text streaming.

Additionally, the project includes built-in support for exporting the automaton structure into Graphviz `.dot` files for easy visualization.

---

## Features

- **Strict O(1) Processing per Character**
  
  Eliminates runtime fallback loops (link climbing) by flattening transitions using Dynamic Programming (`upgrade_failed_links`).

- **Clean Object-Oriented Design**
  
  Avoids global matrices or hardcoded state limits common in competitive programming. Each `TrieNode` dynamically encapsulates its own routing table.

- **Memory Safe (RAII)**
  
  Uses `std::unique_ptr` for automated and safe tree memory management.

- **Graphviz Integration**
  
  Generates `.dot` visualization graphs showing trie edges, failure links, and dictionary links.

---

## File Structure

| File | Description |
|--------|-------------|
| `aho-corasick.h` | Core header file containing definitions for `TrieNode` and the `Trie` class. |
| `source.cpp` | Example entry point initializing the trie, building links, and generating a `.dot` export file. |
| `CMakeLists.txt` | Modern CMake configuration script (requires `{fmt}` library). |
| `Aho-Corasick Automaton Optimization.pdf` | Accompanying technical presentation slides. |

---

## Technical Overview

### 1. Classic Structure vs. Optimized DFA

In a standard Aho-Corasick implementation, handling a character mismatch forces the state machine to iteratively traverse back via `fail_link` paths. In the worst case, this results in multiple random memory lookups per symbol.

By calling `upgrade_failed_links()`, the lookup paths are precalculated and flattened into the `go` transition table for each node:

```cpp
// Fully optimized O(1) state transition
cur = cur->go[c];
```

---

### 2. Time & Space Complexity

| Operation | Complexity |
|------------|------------|
| Automaton preprocessing | **O(m × Σ)** |
| Memory usage | **O(m × Σ)** |
| Stream text search | **O(n)** |

Where:

- `m` = total length of all patterns
- `Σ` = alphabet size (`256`)
- `n` = length of the searched text

This makes the implementation particularly suitable for live, non-backtracking network data streams.

---

## Getting Started

### Prerequisites

- A **C++20** compatible compiler (GCC, Clang, or MSVC)
- **CMake** version 3.15 or newer
- **{fmt}** library

### Building the Project

#### Clone the repository

```bash
git clone https://github.com/your-username/your-repo-name.git
cd your-repo-name
```

#### Create a build directory and compile

```bash
mkdir build
cd build

cmake ..
make
```

#### Run the executable

```bash
./aho_corasick
```

---

## Visualization

The method:

```cpp
make_dot_file(std::string file_name);
```

writes a Graphviz `.dot` representation of the automaton.

To render the graph into a PNG image:

```bash
dot -Tpng test1.dot -o automaton.png
```

### Color Codes in Exported Graphs

| Color / Style | Meaning |
|---------------|----------|
| 🔵 Blue solid arrows | Standard trie prefix edges (`children`) |
| 🔴 Red dotted arrows | Suffix failure links (`fail_link`) |
| 🟣 Purple dotted arrows | Compressed dictionary paths (`dict_link`) |
| 🟢 Green double circles | Valid end-of-word matching states |

---

## License

This project is open-source and available under the **MIT License**.