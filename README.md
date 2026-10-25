# LCPang

LCPang constructs a pangenome graph (variation graph) in GFA format directly from a multi-sequence FASTA file, without requiring sequence alignment. Based on [AlfaPang](https://github.com/AdamCicherski/AlfaPang), LCPang constructs the same graph using a faster algorithm.

Like de Bruijn graph construction, the approach relies on k-mers, but instead of representing distinct k-mers as vertices, it identifies input positions corresponding to the same offsets in matching k-mers and takes the transitive closure of these identifications.

## Dependencies

- `libdivsufsort`
- OpenMP

## Building

```bash
git clone git@github.com:AdamCicherski/LCPang.git
cd LCPang
mkdir build
cd build
cmake ..
cmake --build .
```

## Usage

```bash
./LCPang -i input.fasta -o output.gfa -k 31 -t 8
```

### Options

| Option | Description |
|---|---|
| `-i` | Input multi-sequence FASTA file. |
| `-o` | Output graph file in GFA format. |
| `-k` | k-mer size; must be a positive odd integer. |
| `-t` | Number of threads to use. |
| `-e` | Enable the external-memory index. |

To use the external-memory index, add the `-e` flag:

```bash
./LCPang -i input.fasta -o output.gfa -k 31 -t 8 -e
```
