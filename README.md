# mf
A Matrix Factorization CLI tool

### Building

Use make to build the binary.

```
make build
```

The binary will be located at `bin/mf`

### Running

You can run the binary with the following command

```
./bin/mf -i input_matrix.csv -o output_file.txt
```

### CLI Options

`-i` (required)

Specifies the input matrix. For now this needs to be a CSV file but I have plans to make this configurable with a CLI option.

`-o` (optional)

Specifies an output file. If nothing is specified, the factorized matrix will be written to stdout.

`-v` (optional)

Specifies verbose output.