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

### Example

The example matrix.csv file

```
$ cat matrix.csv
5,3,0,1
4,0,0,1
1,1,0,5
1,0,0,4
0,1,5,4
```

Factorized Matrix
```
$ ./bin/mf -i matrix.csv
5.097594,2.616170,5.274430,1.007833
3.903666,2.003899,4.142148,1.004289
1.126201,0.587651,3.273581,4.978885
0.945446,0.492933,2.660717,3.982480
3.210711,1.654635,4.814942,4.002566
```

Note: Your results may vary. The factorization uses a random number generator to find starting values for the factoried matrix. 