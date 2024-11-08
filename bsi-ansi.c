#include <stdio.h>
#include <stdlib.h>

void insertionSort(int arr[], int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
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
    if (!bucket->values) {
        printf("Erro ao alocar memória para o bucket\n");
        exit(1);
    }
    bucket->count = 0;
    bucket->capacity = capacity;
}

void addValueToBucket(Bucket *bucket, int value) {
    if (bucket->count == bucket->capacity) {
        bucket->capacity *= 2;
        bucket->values = (int *)realloc(bucket->values, bucket->capacity * sizeof(int));
        if (!bucket->values) {
            printf("Erro ao realocar memória para o bucket\n");
            exit(1);
        }
    }
    bucket->values[bucket->count++] = value;
}

void freeBucket(Bucket *bucket) {
    free(bucket->values);
    bucket->count = 0;
    bucket->capacity = 0;
}

void bucketSort(int arr[], int n, int numBuckets) {
    int i, maxValue, bucketIndex, index;
    Bucket *buckets;

    if (n <= 0) return;

    buckets = (Bucket *)malloc(numBuckets * sizeof(Bucket));
    if (!buckets) {
        printf("Erro ao alocar memória para os buckets\n");
        exit(1);
    }

    for (i = 0; i < numBuckets; i++) {
        initBucket(&buckets[i], 5);
    }

    maxValue = arr[0];
    for (i = 1; i < n; i++) {
        if (arr[i] > maxValue) {
            maxValue = arr[i];
        }
    }

    for (i = 0; i < n; i++) {
        bucketIndex = (arr[i] * numBuckets) / (maxValue + 1);
        addValueToBucket(&buckets[bucketIndex], arr[i]);
    }

    index = 0;
    for (i = 0; i < numBuckets; i++) {
        if (buckets[i].count > 0) {
            insertionSort(buckets[i].values, buckets[i].count);
            int j;
            for (j = 0; j < buckets[i].count; j++) {
                arr[index++] = buckets[i].values[j];
            }
        }
        freeBucket(&buckets[i]);
    }
    free(buckets);
}

int main(int argc, char *argv[]) {
    int n, count, numBuckets, i;
    int *arr;
    FILE *file;

    if (argc != 3) {
        printf("Digite: %s <nome_do_arquivo> <tamanho_do_array>\n", argv[0]);
        return 1;
    }

    n = atoi(argv[2]);
    if (n <= 0) {
        printf("Erro: O tamanho do array deve ser um número positivo.\n");
        return 1;
    }

    arr = (int *)malloc(n * sizeof(int));
    if (!arr) {
        printf("Erro ao alocar memória para o array\n");
        return 1;
    }

    file = fopen(argv[1], "r");
    if (!file) {
        printf("Erro ao abrir o arquivo para leitura\n");
        free(arr);
        return 1;
    }

    count = 0;
    while (count < n && fscanf(file, "%d", &arr[count]) == 1) {
        count++;
    }
    fclose(file);

    numBuckets = 100;
    bucketSort(arr, count, numBuckets);

    file = fopen(argv[1], "w");
    if (!file) {
        printf("Erro ao abrir o arquivo para escrita\n");
        free(arr);
        return 1;
    }

    for (i = 0; i < count; i++) {
        fprintf(file, "%d\n", arr[i]);
    }

    fclose(file);
    free(arr);

    return 0;
}
