#include <stdio.h>
#include <stdlib.h>

void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

typedef struct {
    int *values;
    int count;
    int capacity;
} Bucket;

void initBucket(Bucket *bucket, int capacity) {
    bucket->values = (int *)malloc(capacity * sizeof(int));
    bucket->count = 0;
    bucket->capacity = capacity;
}

void addValueToBucket(Bucket *bucket, int value) {
    if (bucket->count == bucket->capacity) {
        bucket->capacity *= 2;
        bucket->values = (int *)realloc(bucket->values, bucket->capacity * sizeof(int));
    }
    bucket->values[bucket->count++] = value;
}

void freeBucket(Bucket *bucket) {
    free(bucket->values);
    bucket->count = 0;
    bucket->capacity = 0;
}

void bucketSort(int arr[], int n, int numBuckets) {
    if (n <= 0) return;

    Bucket *buckets = (Bucket *)malloc(numBuckets * sizeof(Bucket));
    for (int i = 0; i < numBuckets; i++) {
        initBucket(&buckets[i], 5);
    }

    int maxValue = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > maxValue) {
            maxValue = arr[i];
        }
    }

    for (int i = 0; i < n; i++) {
        int bucketIndex = (arr[i] * numBuckets) / (maxValue + 1);
        addValueToBucket(&buckets[bucketIndex], arr[i]);
    }

    int index = 0;
    for (int i = 0; i < numBuckets; i++) {
        if (buckets[i].count > 0) {
            insertionSort(buckets[i].values, buckets[i].count);
            for (int j = 0; j < buckets[i].count; j++) {
                arr[index++] = buckets[i].values[j];
            }
        }
        freeBucket(&buckets[i]);
    }
    free(buckets);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Digite: %s <nome_do_arquivo> <tamanho_do_array>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[2]);
    if (n <= 0) {
        printf("Erro: O tamanho do array deve ser um número positivo.\n");
        return 1;
    }

    int *arr = (int *)malloc(n * sizeof(int));
    if (!arr) {
        perror("Erro ao alocar memória para o array\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Erro ao abrir o arquivo para leitura\n");
        free(arr);
        return 1;
    }

    int count = 0;
    while (count < n && fscanf(file, "%d", &arr[count]) == 1) {
        count++;
    }
    fclose(file);

    int numBuckets = 5;
    bucketSort(arr, count, numBuckets);

    file = fopen(argv[1], "w"); 
    if (!file) {
        perror("Erro ao abrir o arquivo para escrita\n");
        free(arr);
        return 1;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%d\n", arr[i]);
    }

    fclose(file);
    free(arr);

   
    return 0;
}
