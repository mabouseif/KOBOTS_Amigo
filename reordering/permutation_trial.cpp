#include <bits/stdc++.h> 
#include <ctime>
using namespace std;



int get_factorial(unsigned int n)
{ 
    unsigned long long factorial = 1;
    // unsigned int n;
    // cout << "Enter a positive integer: ";
    // cin >> n;
    for(int i = 1; i <=n; ++i)
    {
        factorial *= i;
    }

    return factorial;
}


// Function to display the array 
void display(int a[], int n) 
{ 
    for (int i = 0; i < n; i++) { 
        cout << a[i] << "  "; 
    } 
    cout << endl; 
} 

// void display(std::vector<int> v) 
// { 
//     for (int i = 0; i < v.size(); i++) { 
//         cout << v[i] << "  "; 
//     } 
//     cout << endl; 
// } 
  
// Function to find the permutations 
void findPermutations(int a[], int n_elements, unsigned long long n_perms) 
{ 
    std::cout << n_perms << " and " << n_elements << std::endl;
    // Sort the given array 
    // sort(a, a + n); 
    bool stop_condition = true;
    int count = 0;
    // int perms [n_perms][n_elements];
    int** perms = (int**)malloc(n_perms * sizeof(int*));
    for (int i = 0; i < n_perms; i++)
        perms[i] = (int *)malloc(n_elements * sizeof(int));

    // Find all possible permutations 
    cout << "Possible permutations are:\n"; 
    do{
        for (int i = 0; i < n_elements; i++) 
        { 
            perms[count][i] = a[i];
        } 
        count++;
        stop_condition = next_permutation(a, a + n_elements);
    }while(stop_condition);


    std::cout << "Total count is " << count << std::endl;
    // for (int i = 0; i < count; i++)
    // {
    //     for (int j = 0; j < n_elements; j++)
    //     {
    //         std::cout << perms[i][j] << " ";
    //     }
    //     std::cout << std::endl;
    // }
} 



  
// Driver code 
int main() 
{ 
    int a[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9}; 
    int n_elements = sizeof(a) / sizeof(a[0]);
    // std::vector<int> v = { 1, 2, 3, 4, 5};
    // int n_elements = v.size();

    unsigned long long n_perms = get_factorial(n_elements);
  
 
    std::cout << "Number of elements in array is " << n_elements << std::endl;
    std::cout << "Number of permutations is " << n_perms << std::endl;
    std::cout << "Don't forget to change the hard-coded array" << std::endl;
  
    clock_t begin = std::clock();
    findPermutations(a, n_elements, n_perms); 
    // findPermutations(v, n_elements, n_perms); 
    clock_t end = std::clock();
    std::cout << double(end-begin)/ CLOCKS_PER_SEC << std::endl;
  
    return 0; 
} 