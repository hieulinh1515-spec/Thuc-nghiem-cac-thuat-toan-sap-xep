#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include <random>
#include <string>

using namespace std;

const int ARRAY_SIZE = 1000000;

float *vungNhoTam = nullptr;

void saveFile(const float arr[], int size, const string &filename)
{
    ofstream outFile(filename);
    for (int i = 0; i < size; ++i)
        outFile << arr[i] << '\n';
    outFile.close();
}

void loadFromFile(float arr[], int size, const string &filename)
{
    ifstream inFile(filename);
    for (int i = 0; i < size; ++i)
        inFile >> arr[i];
    inFile.close();
}

void generateAscending(float arr[], int size)
{
    for (int i = 0; i < size; ++i)
        arr[i] = static_cast<float>(i + 1);
}

void generateDescending(float arr[], int size)
{
    for (int i = 0; i < size; ++i)
        arr[i] = static_cast<float>(size - i);
}

void generateRandom(float arr[], int size)
{
    static mt19937 generator(1337);
    uniform_real_distribution<float> distribution(0.0f, 1e9f);
    for (int i = 0; i < size; ++i)
        arr[i] = distribution(generator);
}

void quickSort(float arr[], int left, int right)
{
    int i = left;
    int j = right;
    float pivot = arr[left + (right - left) / 2];

    while (i <= j)
    {
        while (arr[i] < pivot)
            i++;
        while (arr[j] > pivot)
            j--;

        if (i <= j)
        {
            swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }

    if (left < j)
        quickSort(arr, left, j);
    if (i < right)
        quickSort(arr, i, right);
}

void heapifyDown(float arr[], int size, int rootIdx)
{
    while (true)
    {
        int leftChild = 2 * rootIdx + 1;
        int rightChild = 2 * rootIdx + 2;
        int largest = rootIdx;

        if (leftChild < size && arr[leftChild] > arr[largest])
            largest = leftChild;
        if (rightChild < size && arr[rightChild] > arr[largest])
            largest = rightChild;

        if (largest != rootIdx)
        {
            swap(arr[rootIdx], arr[largest]);
            rootIdx = largest;
        }
        else
            break;
    }
}

void heapSort(float arr[], int size)
{
    for (int i = size / 2 - 1; i >= 0; --i)
        heapifyDown(arr, size, i);

    for (int i = size - 1; i > 0; --i)
    {
        swap(arr[0], arr[i]);
        heapifyDown(arr, i, 0);
    }
}

void merge(float arr[], int left, int mid, int right)
{
    int leftIdx = left;
    int rightIdx = mid + 1;
    int k = left;

    while (leftIdx <= mid && rightIdx <= right)
    {
        if (arr[leftIdx] <= arr[rightIdx])
            vungNhoTam[k++] = arr[leftIdx++];
        else
            vungNhoTam[k++] = arr[rightIdx++];
    }

    while (leftIdx <= mid)
        vungNhoTam[k++] = arr[leftIdx++];

    while (rightIdx <= right)
        vungNhoTam[k++] = arr[rightIdx++];

    for (int idx = left; idx <= right; ++idx)
        arr[idx] = vungNhoTam[idx];
}

void mergeSortInternal(float arr[], int left, int right)
{
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;
    mergeSortInternal(arr, left, mid);
    mergeSortInternal(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

void mergeSort(float arr[], int left, int right)
{
    bool isAllocated = false;
    if (vungNhoTam == nullptr)
    {
        vungNhoTam = new float[ARRAY_SIZE];
        isAllocated = true;
    }

    mergeSortInternal(arr, left, right);

    if (isAllocated)
    {
        delete[] vungNhoTam;
        vungNhoTam = nullptr;
    }
}

void stdSort(float arr[], int size)
{
    sort(arr, arr + size);
}

int main()
{
    srand(time(NULL));

    float *numbers = new float[ARRAY_SIZE];

    generateAscending(numbers, ARRAY_SIZE);
    saveFile(numbers, ARRAY_SIZE, "data1.txt");

    generateDescending(numbers, ARRAY_SIZE);
    saveFile(numbers, ARRAY_SIZE, "data2.txt");

    for (int fileIndex = 3; fileIndex <= 10; ++fileIndex)
    {
        generateRandom(numbers, ARRAY_SIZE);
        string filename = "data" + to_string(fileIndex) + ".txt";
        saveFile(numbers, ARRAY_SIZE, filename);
    }

    clock_t startTime, endTime;
    double elapsedTicks, elapsedSeconds;

    for (int fileId = 1; fileId <= 10; ++fileId)
    {
        string filename = "data" + to_string(fileId) + ".txt";

        cout << filename << endl;

        loadFromFile(numbers, ARRAY_SIZE, filename);
        startTime = clock();
        quickSort(numbers, 0, ARRAY_SIZE - 1);
        endTime = clock();

        elapsedTicks = (double)(endTime - startTime);
        elapsedSeconds = elapsedTicks / CLOCKS_PER_SEC;
        cout << "+ QuickSort: " << elapsedSeconds << "s" << endl;


        loadFromFile(numbers, ARRAY_SIZE, filename);
        startTime = clock();
        heapSort(numbers, ARRAY_SIZE);
        endTime = clock();

        elapsedTicks = (double)(endTime - startTime);
        elapsedSeconds = elapsedTicks / CLOCKS_PER_SEC;
        cout << "+ HeapSort: " << elapsedSeconds << "s" << endl;


        loadFromFile(numbers, ARRAY_SIZE, filename);
        startTime = clock();
        mergeSort(numbers, 0, ARRAY_SIZE - 1);
        endTime = clock();

        elapsedTicks = (double)(endTime - startTime);
        elapsedSeconds = elapsedTicks / CLOCKS_PER_SEC;
        cout << "+ MergeSort: " << elapsedSeconds << "s" << endl;


        loadFromFile(numbers, ARRAY_SIZE, filename);
        startTime = clock();
        stdSort(numbers, ARRAY_SIZE);
        endTime = clock();

        elapsedTicks = (double)(endTime - startTime);
        elapsedSeconds = elapsedTicks / CLOCKS_PER_SEC;
        cout << "+ C++ Sort: " << elapsedSeconds << "s" << endl;

        cout << endl;
    }

    delete[] numbers;
    return 0;
}
