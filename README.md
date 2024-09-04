# Transaction Data Compression

## Overview
This project focuses on compressing large transactional datasets using frequent itemset mining. The primary goal is to reduce storage costs while ensuring that the compression is lossless, meaning the original dataset can be fully reconstructed from the compressed version.

## Features
- **Lossless Compression:** Ensures accurate reconstruction of the original dataset from the compressed version.
- **Efficient Storage:** Reduces the size of transactional datasets, optimizing storage costs.
- **Customizable Mapping:** Allows flexible itemset grouping to maximize compression based on dataset characteristics.

## Project Structure
- **main.c:** Contains the core implementation of the dataset compression and decompression logic.
- **D_small.dat & D_medium.dat:** Example datasets used to test the compression algorithm.
- **D_small_compressed.dat & D_medium_compressed.dat:** Compressed versions of the example datasets.
- **D_small_mapping.dat & D_medium_mapping.dat:** Mapping tables used to decode the compressed datasets.

## How to Run
1. **Compile the Program:**
   ```bash
   gcc main.c -o transaction_compression
   ```

2. **Run the Program:**
   ```bash
   ./transaction_compression
   ```

   This will read the datasets, compress them, and print the compression ratios. The compressed datasets and mapping tables will be saved to files.

## Example
Given a dataset like:
```
T1 - A, B, C, D, E
T2 - A, B, C, D, F
T3 - A, B, C, D, E, G
T4 - A, B, C, D, E, F, G
```

A possible compression might result in:
```
T1 - X, E
T2 - X, F
T3 - X, Y
T4 - X, Y, F
```

Where:
```
X = {A, B, C, D}
Y = {E, G}
```

## Results
The program calculates and outputs the compression ratio for the given datasets, showing the efficiency of the compression method.

## Contributing
Feel free to fork this repository and submit pull requests for improvements or new features.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
