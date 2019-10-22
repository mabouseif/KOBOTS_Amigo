#include "ordering_function.h"

/*
    Things to remember:
    1- Many horizontal lines case.
    2- Receiving a line with the same point as vertices.
    3- Should fix permutations to include per-line-permutation in order
    to handle the case where a line is horizontal and could be cut in either
    direction. That would make the cost calculation more inclusive and the
    solution more optimal.
    4- Change the permutations from an array to a vector
*/


struct point p;

struct line l;

struct hash_fn h;



void print_map(std::unordered_map<point,int, hash_fn> const &m)
{
    for (auto const& pair: m) {
        std::cout << "(" << pair.first.x << ", " << pair.first.y << ")" <<" with value " << pair.second << std::endl;
    }
}


bool check_points_n_occurences(std::unordered_map<point,int, hash_fn> const &m)
{
    for (auto const& pair: m) {
        if (pair.second != 2)
        {
            std::cout << "Found a point with only one occurence. Not a polygon." << std::endl;
            return false;
        }
    }

    std::cout << "Line set are a polygon." << std::endl;

    return true;
}


bool is_polygon(std::vector<line> line_set)
{
    std::unordered_map<point, int, hash_fn> point_count;

    for(int i=0; i<line_set.size(); i++)
    {
        if (point_count.count(line_set[i].p1) == 0)
        {
            point_count.insert({line_set[i].p1, 1});
        }
        else
        {
            point_count[line_set[i].p1]++;
        }
        

        if (point_count.count(line_set[i].p2) == 0)
        {
            point_count.insert({line_set[i].p2, 1});
        }
        else
        {
            point_count[line_set[i].p2]++;
        }
        

    }

    print_map(point_count);


    // This check should be done early on when receiving the vector of lines
    // in the first place. But for now it is here.
    if (point_count.size() == 0)
    {
        std::cout << "There are no lines to check. Exiting.." << std::endl;
        exit (EXIT_FAILURE);
    }


    return check_points_n_occurences(point_count);
    
}


/////////////////////////////////////////////////////////////////////////////
////////////////////////////// Polygon-related end //////////////////////////
/////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////
////////////////////////////// Final Line-related start /////////////////////
/////////////////////////////////////////////////////////////////////////////


std::vector<int> get_index_of_final_line_and_starting_vertex(std::vector<line> line_set)
{

    // This is the case where the number of lines is bigger than 1

    std::vector<int> line_and_pt_idx_vec;
    std::unordered_map<int, std::vector<int>> line_occurences_map; // line->{count, idx}
    int max_y = INT_MIN;
    std::vector<int> line_idx; // array of line indices corresponding to lines with the highest y-value
    std::vector<int> point_idx; // array of point indices corresponding to lines. 1 or 2 corresponding to p1 or p2
    for (int i = 0; i < line_set.size(); i++)
    {
        line_occurences_map.insert({i, {0, 0}});

        if (line_set[i].p1.y > max_y)
        {
            max_y = line_set[i].p1.y;
            line_idx.clear();
            point_idx.clear();
            line_idx.push_back(i);
            point_idx.push_back(1);
        }
        else if (line_set[i].p1.y == max_y)
        {
            line_idx.push_back(i);
            point_idx.push_back(1);
        }

        if (line_set[i].p2.y > max_y)
        {
            max_y = line_set[i].p2.y;
            line_idx.clear();
            point_idx.clear();
            line_idx.push_back(i);
            point_idx.push_back(2);
        }
        else if (line_set[i].p2.y == max_y)
        {
            line_idx.push_back(i);
            point_idx.push_back(2);
        }
    }

    std::cout << "line indices size corresponding to lines with the highest y-value : " << line_idx.size() << std::endl;

    // Counting the number of occurences of lines and keeping track of the index
    // in order to access the point later
    for (int i = 0; i < line_idx.size(); i++)
    {
        line_occurences_map[line_idx[i]] = {line_occurences_map[line_idx[i]][0]+1, i};
    }

    // By now, we should have the line indicies, their respective points, and the
    // number of line occurences
    // Check for one peak
    // Check for multi-peak
    // Check for flat peak
    // Check for no peak (no polygon) --> top corner (L shaped) case, and bottom corner case.
    // Top corners have their own case where only one line is returned, 
    // while bottom corners will be handled by the usual logic 

    if (line_idx.size() < 2) // or == 1 basically
    {
        // no peak/no polygon (Top corners L-shaped)
        line_and_pt_idx_vec.push_back(line_idx[0]);
        line_and_pt_idx_vec.push_back(point_idx[0]);

        std::cout << "No peak case" << std::endl;

        return line_and_pt_idx_vec; 

    }
    else if (line_idx.size() >= 2) // not sure if the equal should be removed
    {
        // Flat peak check

        // You may want to check which point
        // of the equal points you would like to return.
        // Also, right now, if there multiple flat lines exist, it returns 
        // a random one. It is good enough for now, unless we are optimizing for 
        // distance when multiple line sets exist.
        for (auto const& pair: line_occurences_map)
        {
            if (pair.second[0] > 1)
            {
                line_and_pt_idx_vec.push_back(pair.first);
                line_and_pt_idx_vec.push_back(point_idx[pair.second[1]]);

                std::cout << "Flat case" << std::endl;
                std::cout << "Note: Flat case is adding an extra element to the line_idx vector because one of the lines is repeated due to reccuring." << std::endl;

                return line_and_pt_idx_vec; 
            }
        }


        // one- and multi-peak check
        std::cout << "Peak and Multi-Peak case" << std::endl;

        int min_x = INT_MAX;
        std::vector<int> x_values_of_points;
        for (int i = 0; i < line_idx.size(); i++)
        {
            if (point_idx[i] == 1)
            {
                x_values_of_points.push_back(line_set[i].p1.x);
            }
            else if (point_idx[i] == 2)
            {
                x_values_of_points.push_back(line_set[i].p2.x);
            }
        }

        auto min_x_idx_1 = std::min_element(x_values_of_points.begin(), x_values_of_points.end()); //  - x_values_of_points.begin();
        int idx_l1 = line_idx[min_x_idx_1 - x_values_of_points.begin()];
        int idx_l1_pt = point_idx[min_x_idx_1 - x_values_of_points.begin()];

        auto min_x_idx_2 = std::min_element(min_x_idx_1 + 1 , x_values_of_points.end()); //  - x_values_of_points.begin();
        int idx_l2 = line_idx[min_x_idx_2 - x_values_of_points.begin()];
        int idx_l2_pt = point_idx[min_x_idx_2 - x_values_of_points.begin()];

        // Now we have two line indices
        // We choose whichever has the higher y-value for its second point
        // Note: the case where the lower points lie on the same y-value is
        // not explicitly accounted for. Needs to be done as well.
        std::cout << "The two lines for the peak are: " << idx_l1 << " and " << idx_l2 << std::endl;
        if (line_set[idx_l1].p1.y > line_set[idx_l1].p2.y)
        {
            if ((line_set[idx_l1].p2.y < line_set[idx_l2].p1.y) && (line_set[idx_l1].p2.y < line_set[idx_l2].p2.y))
            {
                // return idx_l2
                line_and_pt_idx_vec.push_back(idx_l2);
                line_and_pt_idx_vec.push_back(idx_l2_pt);
            }
            else
            {
                //return idx_l1
                line_and_pt_idx_vec.push_back(idx_l1);
                line_and_pt_idx_vec.push_back(idx_l1_pt);
            }
            
        }
        else if (line_set[idx_l1].p1.y < line_set[idx_l1].p2.y)
        {
            if ((line_set[idx_l1].p1.y < line_set[idx_l2].p1.y) && (line_set[idx_l1].p1.y < line_set[idx_l2].p2.y))
            {
                //return idx_l2
                line_and_pt_idx_vec.push_back(idx_l2);
                line_and_pt_idx_vec.push_back(idx_l2_pt);
            }
            else
            {
                // return idx_l1
                line_and_pt_idx_vec.push_back(idx_l1);
                line_and_pt_idx_vec.push_back(idx_l1_pt);
            }
            
        }
           
    }


    return line_and_pt_idx_vec; 
    
}


/////////////////////////////////////////////////////////////////////////////
////////////////////////////// Final Line-related end ///////////////////////
/////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////
////////////////////////////// Ordering Lines-related start /////////////////
/////////////////////////////////////////////////////////////////////////////

int get_factorial(unsigned int n)
{ 
    unsigned long long factorial = 1;
    // unsigned int n;
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
        std::cout << a[i] << "  "; 
    } 
    std::cout << std::endl; 
} 
  
// Function to find the permutations of a given array
int** findPermutations(int a[], int n_elements, unsigned long long n_perms) 
{ 
    std::cout << "Number of permutations: " << n_perms << "\nNumber of elements: " << n_elements << std::endl;
    // Sort the given array 
    // sort(a, a + n); 
    bool stop_condition = true;
    int count = 0;
    // int perms [n_perms][n_elements];
    int** perms = (int**)malloc(n_perms * sizeof(int*));
    for (unsigned long long i = 0; i < n_perms; i++)
        perms[i] = (int *)malloc(n_elements * sizeof(int));

    // Find all possible permutations 
    std::cout << "Possible permutations are:\n"; 
    do{
        for (int i = 0; i < n_elements; i++) 
        { 
            perms[count][i] = a[i];
        } 
        count++;
        stop_condition = std::next_permutation(a, a + n_elements);
    }while(stop_condition);


    return perms;
} 


void print_perms(int** perms, int count, int n_elements)
{
    std::cout << "Total count is " << count << std::endl;
    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < n_elements; j++)
        {
            std::cout << perms[i][j] << " ";
        }
        std::cout << std::endl;
    }
}


double euc_dist(line l1, line l2)
{
    
    double x_diff = l1.p2.x - l2.p1.x;
    double y_diff = l1.p2.y - l2.p1.y;
    auto term1 = pow(x_diff, 2);
    auto term2 = pow(y_diff, 2);
    return std::sqrt(term1 + term2);
}

size_t get_cost_arr(int** perms, unsigned long long n_perms, int n_elements, std::vector<line> line_set)
{
    /*
        Should also append all indices of permutations that are resulting 
        in minimum cost and pick the one that would be more optimal when
        trying to optimize for multiple polygons at the same time.
    */

    float* cost_arr = (float*)malloc(n_perms * sizeof(float));
    float min_cost = FLT_MAX;
    size_t min_idx;
    for (unsigned long long i = 0; i < n_perms; i++)
    {
        for (int j = 0; j < n_elements-1; j++)
        {
            cost_arr[i] += euc_dist(line_set[perms[i][j]], line_set[perms[i][j+1]]);
        }
        // std::cout << "cost: " << cost_arr[i] << std::endl;
        
        if (cost_arr[i] < min_cost)
        {
            min_cost = cost_arr[i];
            min_idx = i;
        }
    }

    std::cout << "Minimum Cost is " << min_cost << std::endl;

    return min_idx;

}



void reorder_points(std::vector<line>& line_set)
{
    /*
        Reorders points of lines so that lines would have p1
        as the one with a higher y-value.
        If y-values are the same and assuming the line does not have
        both vertices as the same point, make the line have the first/entry
        point as the one with the bigger x-value
    */
    line temp_line;
    std::vector<line> new_lines;
    point temp_point;
    for (int i = 0; i < line_set.size(); i++)
    {
        int y1 = line_set[i].p1.y;
        int y2 = line_set[i].p2.y;

        if (y1 < y2)
        {
            temp_point = line_set[i].p1;
            line_set[i].p1 = line_set[i].p2;
            line_set[i].p2 = temp_point;
        }
        // else if (y1 == y2)
        // {
        //     // make sure you are not copying by REFERENCE
        //     temp_line = line_set[i];
        //     temp_point = temp_line.p1;
        //     temp_line.p1 = temp_line.p2;
        //     temp_line.p2 = temp_point;

        //     new_lines.push_back(temp_line);
        // }
        // line_set.insert(line_set.end(), new_lines.begin(), new_lines.end());

        else if (y1 == y2)
        {
            int x1 = line_set[i].p1.x;
            int x2 = line_set[i].p2.x;
            if (x1 < x2)
            {
                temp_point = line_set[i].p1;
                line_set[i].p1 = line_set[i].p2;
                line_set[i].p2 = temp_point;
            }    
        }

    }

    

}


std::vector<line> reorder_cuts(std::vector<line> line_set)
{
    std::vector<line> reordered_lines;
    std::vector<int> final_idx_vec = get_index_of_final_line_and_starting_vertex(line_set);
    int final_line_idx = final_idx_vec[0];
    int final_line_starting_vertex_index = final_idx_vec[1];
    std::cout << "Final line index: " << final_line_idx << std::endl;

    line final_line = line_set[final_line_idx];
    line_set.erase(line_set.begin() + final_line_idx);

    reorder_points(line_set);

    int a[line_set.size()] = {};
    for (int i = 0; i < line_set.size(); i++)
        a[i] = i;

    int n_elements = sizeof(a) / sizeof(a[0]);

    unsigned long long n_perms = get_factorial(n_elements);

    clock_t begin = std::clock();
    int** perms = findPermutations(a, n_elements, n_perms); 
    clock_t end = std::clock();
    std::cout << "Elapsed time for finding permutations: " << double(end-begin)/ CLOCKS_PER_SEC << std::endl;

    begin = std::clock();
    size_t min_idx = get_cost_arr(perms, n_perms, n_elements, line_set);
    end = std::clock();
    std::cout << "Elapsed time for calculating cost of all permutations and finding the minimum value index: " << double(end-begin)/ CLOCKS_PER_SEC << std::endl;

    for (int i = 0; i< n_elements; i++)
        reordered_lines.push_back(line_set[perms[min_idx][i]]);

    reordered_lines.push_back(final_line);

    for (int i = 0; i < n_elements; i++)
        std::cout << perms[min_idx][i] << " ";
    std::cout << final_line_idx << std::endl;

    return reordered_lines;

}


/////////////////////////////////////////////////////////////////////////////
////////////////////////////// Ordering Lines-related end ///////////////////
/////////////////////////////////////////////////////////////////////////////







/////////////////////////////////////////////////////////////////////////////
////////////////////////////// Main /////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/*
int main(int argc, char** argv)
{
    // This is here because "true" and "false" are printed out as 1 and 0, whereas this fixes it.
    std::cout.setf(std::ios::boolalpha);

    std::vector<line> square;
    std::vector<line> triangle;
    std::vector<line> kings_crown;
    std::vector<line> upper_left_corner_mirrored_L_shape;
    std::vector<line> lower_left_corner_inverted_L_shape;
    std::vector<line> lower_left_corner_inverted_wide_angle_L_shape;

    point p1 = {2, 2};
    point p2 = {2, 4};
    point p3 = {4, 4};
    point p4 = {4, 2};
    // point p5 = {7, 8};

    line l1 = {p1, p2};
    line l2 = {p2, p3};
    line l3 = {p3, p4};
    line l4 = {p4, p1};
    line l5;
    line l6;
    line l7;
    line l8;
    line l9;
    line l10;
    line l11;
    line l12;

    square.push_back(l1);
    square.push_back(l2);
    square.push_back(l3);
    square.push_back(l4);

    l1 = {p1, p3};
    l2 = {p3, p4};
    l3 = {p4, p1};

    triangle.push_back(l1);
    triangle.push_back(l2);
    triangle.push_back(l3);

    point p11 = {0, 0};
    point p12 = {1, 5};
    point p13 = {2, 3};
    point p14 = {3, 5};
    point p15 = {4, 3};
    point p16 = {5, 5};
    point p17 = {6, 3};
    point p18 = {7, 5};
    point p19 = {8, 3};
    point p20 = {9, 5};
    point p21 = {10, 0};
    point p22 = {11, 5};
    point p23 = {12, 0};

    l1 = {p11, p12};
    l2 = {p12, p13};
    l3 = {p13, p14};
    l4 = {p14, p15};
    l5 = {p15, p16};
    l6 = {p16, p17};
    l7 = {p17, p18};
    l8 = {p18, p19};
    l9 = {p19, p20};
    l10 = {p20, p21};
    l11 = {p21, p11};
    l12 = {p22, p23};

    kings_crown.push_back(l1);
    kings_crown.push_back(l2);
    kings_crown.push_back(l3);
    kings_crown.push_back(l4);
    kings_crown.push_back(l5);
    kings_crown.push_back(l6);
    kings_crown.push_back(l7);
    kings_crown.push_back(l8);
    kings_crown.push_back(l9);
    kings_crown.push_back(l10);
    kings_crown.push_back(l11);
    // kings_crown.push_back(l12);

    p1 = {0, 5};
    p2 = {2, 5};
    p3 = {2, 8};

    l1 = {p1, p2};
    l2 = {p2, p3};

    upper_left_corner_mirrored_L_shape.push_back(l1);
    upper_left_corner_mirrored_L_shape.push_back(l2);

    p1 = {2, 0};
    p2 = {2, 4};
    p3 = {0, 4};

    l1 = {p1, p2};
    l2 = {p2, p3};
    
    lower_left_corner_inverted_L_shape.push_back(l1);
    lower_left_corner_inverted_L_shape.push_back(l2);


    p1 = {2, 0};
    p2 = {2, 4};
    p3 = {0, 5};

    l1 = {p1, p2};
    l2 = {p2, p3};
    
    lower_left_corner_inverted_wide_angle_L_shape.push_back(l1);
    lower_left_corner_inverted_wide_angle_L_shape.push_back(l2);


    std::cout << "Running.." << std::endl;
    std::cout << std::endl;
    // std::cout << is_polygon(square) << std::endl;


    // std::vector<int> final_line;

    // final_line = get_index_of_final_line_and_starting_vertex(square);
    // std::cout << "Final line for square is: " << final_line[0] << std::endl;
    // std::cout << std::endl;
    // final_line = get_index_of_final_line_and_starting_vertex(triangle);
    // std::cout << "Final line for triangle is: " << final_line[0] << std::endl;
    // std::cout << std::endl;
    // final_line = get_index_of_final_line_and_starting_vertex(kings_crown);
    // std::cout << "Final line for king's crown is: " << final_line[0] << std::endl;
    // std::cout << std::endl;
    // final_line = get_index_of_final_line_and_starting_vertex(upper_left_corner_mirrored_L_shape);
    // std::cout << "Final line for upper_left_corner_mirrored_L_shape is: " << final_line[0] << std::endl;
    // std::cout << std::endl;
    // final_line = get_index_of_final_line_and_starting_vertex(lower_left_corner_inverted_L_shape);
    // std::cout << "Final line for lower_left_corner_inverted_L_shape is: " << final_line[0] << std::endl;
    // std::cout << std::endl;
    // final_line = get_index_of_final_line_and_starting_vertex(lower_left_corner_inverted_wide_angle_L_shape);
    // std::cout << "Final line for lower_left_corner_inverted_wide_angle_L_shape is: " << final_line[0] << std::endl;


    ///////////////////////////////////////////////////////////

    std::vector<line> shape = kings_crown;
    std::cout << "Old size of shape: " << shape.size() << std::endl;
    reorder_points(shape);
    std::cout << "New size of shape: " << shape.size() << std::endl;
    
    // Let's say we have a vector of lines (n_elements)
    int a[shape.size()] = {};
    for (int i = 0; i < shape.size(); i++)
        a[i] = i;

    // int a[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9}; 
    int n_elements = sizeof(a) / sizeof(a[0]);

    unsigned long long n_perms = get_factorial(n_elements);
  
 
    std::cout << "Number of elements in array is " << n_elements << std::endl;
    std::cout << "Number of permutations is " << n_perms << std::endl;
    std::cout << "Don't forget to change the hard-coded array" << std::endl;
  
    clock_t begin = std::clock();
    int** perms = findPermutations(a, n_elements, n_perms); 
    clock_t end = std::clock();
    std::cout << "Elapsed time for finding permutations: " << double(end-begin)/ CLOCKS_PER_SEC << std::endl;

    // print_perms(perms, n_perms, n_elements);


    // reorder_points(shape);
    // // std::cout << "New size of shape: " << shape.size() << std::endl;

    begin = std::clock();

    size_t min_idx = get_cost_arr(perms, n_perms, n_elements, shape);
    std::cout << "Minimum Permutation index: " << min_idx << std::endl;

    end = std::clock();
    std::cout << "Elapsed time for calculating cost of all permutations and finding the minimum value index: " << double(end-begin)/ CLOCKS_PER_SEC << std::endl;

    for (int i = 0; i < n_elements; i++)
        std::cout << perms[min_idx][i] << " ";

    std::cout << std::endl;

    return 0;
}
*/
