#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// i is current root node, and n is total number of el in arr
void	heapify(char **arr, int n, int i)
{
	int	largest;
	int	left;
	int	right;
	char	*temp;
	
	largest = i;
	left = 2*i + 1;
	right = 2*i + 2;
	if (left < n && strcmp(arr[left], arr[largest]) > 0)
		largest = left;
	if (right < n && strcmp(arr[right], arr[largest]) > 0)
		largest = right;
	if (largest != i)
	{
		temp = arr[i];
		arr[i] = arr[largest];
		arr[largest] = temp;
		heapify(arr, n, largest);
	}
}

void	heapsort_str(char **arr, int n)
{
	int	i;
	char *temp;

	i = n / 2 - 1;
	while (i >= 0)
		heapify(arr, n, i--);
	i = n - 1;
	while (i > 0)
	{
		temp = arr[0];
		arr[0] = arr[i];
		arr[i] = temp;
		heapify(arr, i--, 0);
	}
}

// Function to print an array
void print_array(char **arr, int n) {
	for (int i = 0; i < n; ++i)
		printf("%s ", arr[i]);
	printf("\n");
}

int main() {
	char *arr[] = {"Apple", "banana", "orange", "grape", "kiwi", "ape"};
	int n = sizeof(arr)/sizeof(arr[0]);

	printf("Original array: \n");
	print_array(arr, n);

	heapsort_str(arr, n);

	printf("\nSorted array: \n");
	print_array(arr, n);
	return 0;
}

