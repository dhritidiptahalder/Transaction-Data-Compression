#include <stdio.h>
#include <stdlib.h>

#define MAX_TRANSACTIONS 1000
#define MAX_ITEMS 100
#define MAX_MAPPING 50

typedef struct
{
    int items[MAX_ITEMS];
    int itemCount;
} Transaction;

typedef struct
{
    int items[MAX_ITEMS];
    int itemCount;
    char label;
} Mapping;

// Function prototypes
int readDataset(const char *filename, Transaction transactions[]);
void compressDataset(Transaction transactions[], int transactionCount, Mapping mappings[], int mappingCount, Transaction compressedTransactions[]);
void decompressDataset(Transaction compressedTransactions[], int transactionCount, Mapping mappings[], int mappingCount, Transaction decompressedTransactions[]);
void printTransactions(Transaction transactions[], int transactionCount);
void printCompressedTransactions(Transaction compressedTransactions[], int transactionCount);
void saveCompressedDataset(const char *filename, Transaction compressedTransactions[], int transactionCount);
void saveMappingTable(const char *filename, Mapping mappings[], int mappingCount);
int mappingMatches(int items1[], int itemCount1, int items2[], int itemCount2);

int main()
{
    printf("Program started.\n");

    Transaction transactions[MAX_TRANSACTIONS];
    Transaction compressedTransactions[MAX_TRANSACTIONS];
    Transaction decompressedTransactions[MAX_TRANSACTIONS];

    // Define mappings (example)
    Mapping mappings[] = {
        {{1, 3, 5}, 3, 'X'},
        {{7, 9, 11}, 3, 'Y'}};
    int mappingCount = sizeof(mappings) / sizeof(mappings[0]);
    int transactionCount;

    // Read dataset from file
    printf("Reading D_small.dat...\n");
    transactionCount = readDataset("D_small.dat", transactions);
    if (transactionCount == -1)
        return 1;

    // Calculate size of original dataset
    int originalSize = 0;
    for (int i = 0; i < transactionCount; i++)
    {
        originalSize += transactions[i].itemCount;
    }

    // Compress dataset
    compressDataset(transactions, transactionCount, mappings, mappingCount, compressedTransactions);

    // Calculate size of compressed dataset
    int compressedSize = 0;
    for (int i = 0; i < transactionCount; i++)
    {
        compressedSize += compressedTransactions[i].itemCount;
    }
    int mappingTableSize = mappingCount * (sizeof(int) * MAX_ITEMS + sizeof(char)); // Size of mapping table
    int totalCompressedSize = compressedSize + mappingTableSize;

    // Calculate compression ratio
    float compressionRatio = (float)(originalSize - totalCompressedSize) / originalSize;
    float compressionRatioPercent = compressionRatio * 100;

    // Print results
    printf("D_small.dat - Original Size: %d\n", originalSize);
    printf("D_small.dat - Compressed Size (without mapping table): %d\n", compressedSize);
    printf("D_small.dat - Mapping Table Size: %d\n", mappingTableSize);
    printf("D_small.dat - Total Compressed Size: %d\n", totalCompressedSize);
    printf("D_small.dat - Compression Ratio: %.2f\n", compressionRatio);
    printf("D_small.dat - Compression Ratio (%%): %.2f%%\n", compressionRatioPercent);

    // Save compressed dataset and mapping table
    saveCompressedDataset("D_small_compressed.dat", compressedTransactions, transactionCount);
    saveMappingTable("D_small_mapping.dat", mappings, mappingCount);

    // Decompress dataset
    decompressDataset(compressedTransactions, transactionCount, mappings, mappingCount, decompressedTransactions);

    // Print decompressed transactions
    printf("Decompressed Transactions:\n");
    printTransactions(decompressedTransactions, transactionCount);

    // Repeat for D_medium.dat
    printf("Reading D_medium.dat...\n");
    transactionCount = readDataset("D_medium.dat", transactions);
    if (transactionCount == -1)
        return 1;

    // Calculate size of original dataset
    originalSize = 0;
    for (int i = 0; i < transactionCount; i++)
    {
        originalSize += transactions[i].itemCount;
    }

    // Compress dataset
    compressDataset(transactions, transactionCount, mappings, mappingCount, compressedTransactions);

    // Calculate size of compressed dataset
    compressedSize = 0;
    for (int i = 0; i < transactionCount; i++)
    {
        compressedSize += compressedTransactions[i].itemCount;
    }
    totalCompressedSize = compressedSize + mappingTableSize;

    // Calculate compression ratio
    compressionRatio = (float)(originalSize - totalCompressedSize) / originalSize;
    compressionRatioPercent = compressionRatio * 100;

    // Print results
    printf("D_medium.dat - Original Size: %d\n", originalSize);
    printf("D_medium.dat - Compressed Size (without mapping table): %d\n", compressedSize);
    printf("D_medium.dat - Mapping Table Size: %d\n", mappingTableSize);
    printf("D_medium.dat - Total Compressed Size: %d\n", totalCompressedSize);
    printf("D_medium.dat - Compression Ratio: %.2f\n", compressionRatio);
    printf("D_medium.dat - Compression Ratio (%%): %.2f%%\n", compressionRatioPercent);

    // Save compressed dataset and mapping table
    saveCompressedDataset("D_medium_compressed.dat", compressedTransactions, transactionCount);
    saveMappingTable("D_medium_mapping.dat", mappings, mappingCount);

    // Decompress dataset
    decompressDataset(compressedTransactions, transactionCount, mappings, mappingCount, decompressedTransactions);

    // Print decompressed transactions
    printf("Decompressed Transactions:\n");
    printTransactions(decompressedTransactions, transactionCount);

    return 0;
}

// Function to check if two sets of items match
int mappingMatches(int items1[], int itemCount1, int items2[], int itemCount2)
{
    if (itemCount1 != itemCount2)
        return 0; // Sizes must be the same

    for (int i = 0; i < itemCount1; i++)
    {
        int found = 0;
        for (int j = 0; j < itemCount2; j++)
        {
            if (items1[i] == items2[j])
            {
                found = 1;
                break;
            }
        }
        if (!found)
            return 0; // Item not found
    }
    return 1; // All items match
}

// Read dataset from file
int readDataset(const char *filename, Transaction transactions[])
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Error opening file");
        return -1;
    }

    int transactionIndex = 0;
    while (fscanf(file, "%d", &transactions[transactionIndex].items[transactions[transactionIndex].itemCount]) != EOF)
    {
        transactions[transactionIndex].itemCount++;
        if (fgetc(file) == '\n')
        {
            transactionIndex++;
            if (transactionIndex >= MAX_TRANSACTIONS)
                break;
        }
    }

    fclose(file);
    return transactionIndex;
}

void compressDataset(Transaction transactions[], int transactionCount, Mapping mappings[], int mappingCount, Transaction compressedTransactions[])
{
    for (int i = 0; i < transactionCount; i++)
    {
        int foundMapping = 0;
        for (int k = 0; k < mappingCount; k++)
        {
            if (mappingMatches(transactions[i].items, transactions[i].itemCount, mappings[k].items, mappings[k].itemCount))
            {
                compressedTransactions[i].items[0] = mappings[k].label; // Use the label of the mapping
                compressedTransactions[i].itemCount = 1;
                foundMapping = 1;
                break;
            }
        }
        if (!foundMapping)
        {
            // If no mapping matches, store items directly (could be improved)
            for (int j = 0; j < transactions[i].itemCount; j++)
            {
                compressedTransactions[i].items[j] = transactions[i].items[j];
            }
            compressedTransactions[i].itemCount = transactions[i].itemCount;
        }
    }
}

void decompressDataset(Transaction compressedTransactions[], int transactionCount, Mapping mappings[], int mappingCount, Transaction decompressedTransactions[])
{
    // Example implementation - this needs to be improved based on the mapping and compression logic
    for (int i = 0; i < transactionCount; i++)
    {
        int label = compressedTransactions[i].items[0];
        int foundMapping = 0;
        for (int k = 0; k < mappingCount; k++)
        {
            if (mappings[k].label == label)
            {
                for (int j = 0; j < mappings[k].itemCount; j++)
                {
                    decompressedTransactions[i].items[j] = mappings[k].items[j];
                }
                decompressedTransactions[i].itemCount = mappings[k].itemCount;
                foundMapping = 1;
                break;
            }
        }
        if (!foundMapping)
        {
            for (int j = 0; j < compressedTransactions[i].itemCount; j++)
            {
                decompressedTransactions[i].items[j] = compressedTransactions[i].items[j];
            }
            decompressedTransactions[i].itemCount = compressedTransactions[i].itemCount;
        }
    }
}

void printTransactions(Transaction transactions[], int transactionCount)
{
    for (int i = 0; i < transactionCount; i++)
    {
        for (int j = 0; j < transactions[i].itemCount; j++)
        {
            printf("%d ", transactions[i].items[j]);
        }
        printf("\n");
    }
}

void printCompressedTransactions(Transaction compressedTransactions[], int transactionCount)
{
    for (int i = 0; i < transactionCount; i++)
    {
        printf("%d\n", compressedTransactions[i].items[0]);
    }
}

void saveCompressedDataset(const char *filename, Transaction compressedTransactions[], int transactionCount)
{
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        perror("Error opening file for writing");
        return;
    }

    for (int i = 0; i < transactionCount; i++)
    {
        fprintf(file, "%d\n", compressedTransactions[i].items[0]);
    }

    fclose(file);
}

void saveMappingTable(const char *filename, Mapping mappings[], int mappingCount)
{
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        perror("Error opening file for writing");
        return;
    }

    for (int i = 0; i < mappingCount; i++)
    {
        fprintf(file, "%c ", mappings[i].label);
        for (int j = 0; j < mappings[i].itemCount; j++)
        {
            fprintf(file, "%d ", mappings[i].items[j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}