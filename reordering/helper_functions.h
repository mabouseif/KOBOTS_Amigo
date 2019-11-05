#include <stdio.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <bits/stdc++.h> 
#include <algorithm>
#include <cmath>
#include <math.h>


/* The point struct has an extra operator == function because it is required 
 when creating unordered maps with custom classes/structs
 Also the function hash_fn is part of it. Check the links for more info */
struct point
{
    int x;
    int y;

    // https://www.techiedelight.com/use-struct-key-std-unordered_map-cpp/
    // https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
    bool operator==(const point &other) const
    { 
        return (x == other.x && y == other.y);
    }
    

};


struct line
{
    point p1;
    point p2;

    void print_points()
    {
        std::cout << "(" << p1.x << ", " << p1.y << ")" << " and " << "(" << p2.x << ", " << p2.y << ")" << std::endl;
    }

    point get_higher_point_y_wise()
    {
        if (p1.y > p2.y)
            return p1;
        else return p2;
    }

    bool operator==(const line &other) const
    {
        if (p1== other.p1 && p2==other.p2)
            return true;
        if (p1== other.p2 && p2==other.p1)
        {
            std::cout << "lines equal but reversed" << std::endl;
            return true;
        }
        
        return false;
    }

};


// specialized hash function for unordered_map keys, for the point struct
struct hash_fn
{
	// https://www.techiedelight.com/use-struct-key-std-unordered_map-cpp/
    // https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
    
	std::size_t operator()(const point &p) const
	{
		std::size_t h1 = std::hash<int>()(p.x);
		std::size_t h2 = std::hash<int>()(p.y);

		return h1 ^ h2;
	}
};



typedef std::array<int, 2> custom_arr_type;


void print_map(std::unordered_map<point,int, hash_fn> const &m);

bool check_points_n_occurences(std::unordered_map<point,int, hash_fn> const &m);

bool is_polygon(std::vector<line> line_set);

std::vector<int> get_index_of_final_line_and_starting_vertex(std::vector<line> line_set);

int get_factorial(unsigned int n);

void display(int a[], int n);

int** find_permutations(int a[], int n_elements, unsigned long long n_perms);

void print_perms(int** perms, int count, int n_elements);

void print_lines(std::vector<line> line_set);

double euc_dist(line l1, line l2);

size_t get_cost_arr(int** perms, unsigned long long n_perms, int n_elements, std::vector<line> line_set);

void reorder_points(std::vector<line>& line_set);

std::vector<line> reorder_cuts(std::vector<line> line_set);

std::map<int, custom_arr_type> board_wide_poly(std::vector<std::vector<line>>& poly_set, const int board_width);