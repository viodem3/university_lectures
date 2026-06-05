# Aho-Corasick Automaton with DFA Optimization

A modern **C++20** implementation of the **Aho-Corasick multi-pattern string matching algorithm**.

The project builds a classical Aho-Corasick automaton and transforms it into a fully deterministic finite automaton (DFA) by precomputing all failure transitions. This allows pattern matching to be performed using a single state transition per input character, making the implementation well-suited for high-throughput text processing and streaming applications.

The project also includes Graphviz export functionality for visualizing the automaton structure, failure links, and dictionary links.

---

## Features

- Classical Aho-Corasick trie construction
- Failure-link and dictionary-link generation
- DFA optimization through precomputed transitions
- Constant-time state transitions during text processing
- Object-oriented C++20 design
- Automatic memory management using `std::unique_ptr`
- Graphviz `.dot` export for automaton visualization

---

## File Structure

| File | Description |
|--------|-------------|
| `aho-corasick.h` | Core implementation of the trie, automaton construction, and search algorithms |
| `source.cpp` | Example program demonstrating automaton construction and visualization |
| `CMakeLists.txt` | CMake build configuration |
| `Aho-Corasick Automaton Optimization.pdf` | Technical presentation describing the optimization process |

---

## Technical Overview

### DFA Optimization

A standard Aho-Corasick automaton resolves missing transitions by repeatedly following failure links until a valid transition is found.

After calling:

```cpp
upgrade_failed_links();
```

all fallback transitions are precomputed and stored directly inside the transition table of every state.

As a result, state updates during pattern matching become:

```cpp
cur = cur->go[c];
```

eliminating runtime failure-link traversal.

### Complexity

Let:

- `m` be the total length of all patterns,
- `Σ = 256` be the alphabet size,
- `n` be the length of the processed text.

| Operation | Complexity |
|------------|------------|
| Automaton construction | O(m) |
| DFA transition completion | O(m × Σ) |
| Memory usage | O(m × Σ) |
| Pattern matching | O(n) |

After preprocessing, every input character requires exactly one transition lookup.

---

## Building

### Requirements

- C++20 compatible compiler
- CMake 3.15 or newer
- `{fmt}` library

### Compilation

```bash
git clone https://github.com/your-username/your-repo-name.git
cd your-repo-name

mkdir build
cd build

cmake ..
make
```

### Running

```bash
./aho_corasick
```

---

## Visualization

The method

```cpp
make_dot_file(std::string file_name);
```

exports the automaton into a Graphviz `.dot` file.

To generate an image:

```bash
dot -Tpng automaton.dot -o automaton.png
```

### Graph Legend

| Element | Description |
|----------|-------------|
| Blue edges | Trie transitions |
| Red dotted edges | Failure links |
| Purple dotted edges | Dictionary links |
| Green double circles | Terminal states |

