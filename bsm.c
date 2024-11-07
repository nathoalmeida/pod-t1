#include <stdio.h>
#include <stdlib.h>

void mergeSort(int arr[], int l, int r);
void merge(int arr[], int l, int m, int r);

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
            mergeSort(buckets[i].values, 0, buckets[i].count - 1);
            for (int j = 0; j < buckets[i].count; j++) {
                arr[index++] = buckets[i].values[j];
            }
        }
        freeBucket(&buckets[i]);
    }
    free(buckets);
}

void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++) {
        L[i] = arr[l + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = arr[m + 1 + j];
    }

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
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
