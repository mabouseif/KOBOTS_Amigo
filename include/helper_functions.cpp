#include "helper_functions.h"

namespace plt = matplotlibcpp;

/*
    Things to remember:
    4- Change the permutations from an array to a vector.
    5- Memory leak check with valgrind for malloc usage.
*/



/////////////////////////////////////////////////////////////////////////////
////////////////////////////// Polygon-related START ////////////////////////
/////////////////////////////////////////////////////////////////////////////

struct point p;

struct line l;

struct hash_fn h;



void print_map(std::unordered_map<point,int, hash_fn> const &m)
{
    for (auto const& pair: m) {
        std::cout << "(" << pair.first.x << ", " << pair.first.y << ")" <<" with value " << pair.second << std::endl;
    }
}


// Counts vertices of user-provided shapes
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


// Valid polygon check
bool is_polygon(std::vector<line> line_set)
{
    std::unordered_map<point, int, hash_fn> point_count;

    for(int i=0; i<line_set.size(); i++)
    {
        if (point_count.count(line_set[i].p1) == 0)
            point_count.insert({line_set[i].p1, 1});
        else
            point_count[line_set[i].p1]++;
    

        if (point_count.count(line_set[i].p2) == 0)
            point_count.insert({line_set[i].p2, 1});
        else
            point_count[line_set[i].p2]++;
        
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
////////////////////////////// Polygon-related END //////////////////////////
/////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////
////////////////////////////// Final Line-related START /////////////////////
/////////////////////////////////////////////////////////////////////////////


// Return final line to be cut for the polygon, 
// along with the vertex to start cutting from.
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
////////////////////////////// Final Line-related END ///////////////////////
/////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////
////////////////////////// Stacked Horizontal lines-related START ///////////
/////////////////////////////////////////////////////////////////////////////


// Return order of the shapes to be cut
std::vector<std::vector<line>> reorder_board_wide_polys(std::map<int, custom_arr_type> board_wide_polygon_map, std::vector<std::vector<line>> poly_set_copy)
{
    std::vector<std::vector<line>> reordered_board_wide_polys;
    std::map<int, custom_arr_type> board_wide_polygon_map_copy;
    int y_val;
    int poly_idx, poly_idx_2;
    int line_idx, line_idx_2;
    std::vector<line> polygon, polygon_2;
    line board_wide_line;
    bool flag;

    for (std::map<int, custom_arr_type>::reverse_iterator rev_it = board_wide_polygon_map.rbegin(); rev_it != board_wide_polygon_map.rend(); )
    {
        poly_idx = (*rev_it).second[0]; line_idx = (*rev_it).second[1];
        polygon = poly_set_copy[poly_idx];
        flag = false;
        for (std::map<int, custom_arr_type>::reverse_iterator rev_it_2 = ++rev_it; rev_it_2 != board_wide_polygon_map.rend(); ++rev_it_2)
        {
            poly_idx_2 = (*rev_it_2).second[0]; line_idx_2 = (*rev_it_2).second[1];
            polygon_2 = poly_set_copy[poly_idx_2];
            if (poly_idx == poly_idx_2)
            {
                flag = true;
                if (polygon[line_idx].get_higher_point_y_wise().y >  polygon_2[line_idx_2].get_higher_point_y_wise().y)
                    board_wide_polygon_map_copy.insert(*rev_it_2);
                else
                    board_wide_polygon_map_copy.insert(*(rev_it.base()));
            }
        }

        if (!flag)
            board_wide_polygon_map_copy.insert(*(rev_it.base()));
    }

    std::cout << "Map size: " << board_wide_polygon_map.size() << std::endl;
    std::cout << "Map copy size: " << board_wide_polygon_map_copy.size() << std::endl;
    board_wide_polygon_map = board_wide_polygon_map_copy;

    for (std::map<int, custom_arr_type>::reverse_iterator rev_it = board_wide_polygon_map.rbegin(); rev_it != board_wide_polygon_map.rend(); ++rev_it)
    {
        poly_idx = (*rev_it).second[0]; line_idx = (*rev_it).second[1];
        polygon = poly_set_copy[poly_idx];

        if (polygon.size() > 1)
        {
            std::cout << "polygon.size() > 1" << std::endl;
            board_wide_line = polygon[line_idx];
            polygon = reorder_cuts(polygon, line_idx); 
        }

        reordered_board_wide_polys.push_back(polygon);   
    }

    return reordered_board_wide_polys;
}


// Reorder cuts, including the final line
std::map<int, custom_arr_type> reorder_final(std::vector<std::vector<line>>& poly_set, const int board_width)
{
    std::map<int, custom_arr_type> board_wide_polygon_map;
    std::pair<int, custom_arr_type> p;
    int poly_idx, line_idx, y_value;
    std::vector<line> polygon;
    std::vector<int> polygons_board_wide_idx;
    std::vector<int> polygons_board_wide_y_val_keys;
    custom_arr_type my_arr;
    for (int i = 0; i < poly_set.size(); i++)
    {
        polygon = poly_set[i];
        for (int j = 0; j < polygon.size(); j++)
            {
                if (std::abs(polygon[j].p1.x - polygon[j].p2.x) == board_width)
                {
                    poly_idx = i; line_idx = j; y_value = polygon[j].get_higher_point_y_wise().y;
                    polygons_board_wide_idx.push_back(poly_idx);
                    polygons_board_wide_y_val_keys.push_back(y_value);
                    my_arr = {poly_idx, line_idx};
                    board_wide_polygon_map.insert(std::make_pair(y_value, my_arr));
                    // break;
                }  
            }
    }
    std::vector<std::vector<line>> poly_set_copy = poly_set;
    std::cout << "poly_set_copy size is " << poly_set_copy.size() << std::endl;
    std::vector<std::vector<line>> reordered_board_wide_polys = reorder_board_wide_polys( board_wide_polygon_map, poly_set_copy);
    // board-wide polys indicies in poly_set sorted in ascending order
    std::sort(polygons_board_wide_idx.begin(), polygons_board_wide_idx.end());
    for (std::vector<int>::iterator it=polygons_board_wide_idx.begin(); it != polygons_board_wide_idx.end(); it++)
        std::cout << *it << std::endl;

    // By here you have (sorted in ascending order)
    // the y-val, {poly_idx, line_idx} map of the board-wide polys

    // Now remove those board-wide polys from poly_set, which is passed by REFERENCE!
    int n = polygons_board_wide_idx.size();
    int compare_idx;

    if (board_wide_polygon_map.size() > 0)
    {
        for (int i = 0; i < n; i++)
        {
            compare_idx = n-1-i;
            std::cout << "compare_idx: " << compare_idx << std::endl;
            std::cout << "polygons_board_wide_idx[compare_idx]: " << polygons_board_wide_idx[compare_idx] << std::endl;
            for (std::vector<std::vector<line>>::iterator it = poly_set.begin(); it != poly_set.end(); it++)
            {
                if ((*it) == poly_set_copy[polygons_board_wide_idx[compare_idx]])
                {   
                    // it->clear();
                    poly_set.erase(it);
                    break; // The break was necessary, because when you erase an element, the vector decreases in size,
                           // but you have the stop condition at vector.end(), which is already set before loop entry.
                }   
            }
        }
    }
    // Now that the poly set is normal, reorder cuts of the polyset
    std::cout << "poly_set.size() = " << poly_set.size() << std::endl;
    for (int i = 0; i < poly_set.size(); i++)
        poly_set[i] = reorder_cuts(poly_set[i], -1);


   for (int i =0; i < reordered_board_wide_polys.size(); i++)
        poly_set.push_back(reordered_board_wide_polys[i]);

    
    for (int i = 0; i < poly_set.size(); i++)
    {
        std::cout << "----------" << std::endl;
        for (int j = 0; j < poly_set[i].size(); j++)
            poly_set[i][j].print_points();
    }
        
    // Actually, returning the map is just for viewing purposes.
    // Since poly_set is passed by reference, it is modified accordingly.
    return board_wide_polygon_map;  
}


// Plotting
void plot_poly_set(std::vector<std::vector<line>> poly_set, int xlims[2], int ylims[2])
{
    line line_for_plot;
    double x1, y1, x2, y2;
    std::vector<std::string> line_texture_vec = {"r-", "m-", "g-", "y-", "k-", "b-", "r--", "m--", "g--", "y--", "k--", "b--"};
    std::string line_texture;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(line_texture_vec.begin(), line_texture_vec.end(), g);
    plt::figure_size(500, 500);

    if (line_texture_vec.size() < poly_set.size())
    {
        std::cout << "In plot_poly_set, not enough styles for all polys!" << std::endl;
        exit;
    }

    for (int i = 0; i < poly_set.size(); i++)
    {
        line_texture = line_texture_vec[i];

        for (int j = 0; j < poly_set[i].size(); j++)
        {
            line_for_plot = poly_set[i][j];
            x1 = line_for_plot.p1.x; y1 = line_for_plot.p1.y; x2 = line_for_plot.p2.x; y2= line_for_plot.p2.y;
            plt::plot({x1, x2}, {y1, y2}, line_texture);
            plt::text((x1+x2)/2, (y1+y2)/2, std::to_string(i)+ std::to_string(j));
        }
    }

    
    plt::xlim(xlims[0], xlims[1]);
    plt::ylim(ylims[0], ylims[1]);
    plt::show();
}


/////////////////////////////////////////////////////////////////////////////
////////////////////////////// Stacked Horizontal lines-related END /////////
/////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////
////////////////////////////// Ordering Lines-related START /////////////////
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
int** find_permutations(int a[], int n_elements, unsigned long long n_perms, int final_line_idx) 
{ 
    std::cout << "Number of permutations: " << n_perms << "\nNumber of elements: " << n_elements << std::endl;

    bool stop_condition = true;
    int count = 0;
    int** perms = (int**)malloc(n_perms * sizeof(int*));
    for (unsigned long long i = 0; i < n_perms; i++)
        perms[i] = (int *)malloc((n_elements+1) * sizeof(int));

    // Find all possible permutations 
    std::cout << "Possible permutations are:\n"; 
    do{
        // display(a, n_elements);
        for (int i = 0; i < n_elements; i++) 
        { 
            perms[count][i] = a[i];
        } 
        perms[count][n_elements] = final_line_idx;
        count++;
        stop_condition = std::next_permutation(a, a + n_elements);
    }while(stop_condition);


    return perms;
} 

// Printing permutations
void print_perms(int** perms, int count, int n_elements)
{
    std::cout << "Total count is " << count << std::endl;
    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < n_elements; j++)
        {
            std::cout << perms[i][j] << "  ";
        }
        std::cout << std::endl;
    }
}


// Calculate Euclidian distance
double euc_dist(line l1, line l2)
{
    
    double x_diff = l1.p2.x - l2.p1.x;
    double y_diff = l1.p2.y - l2.p1.y;
    double term1 = pow(x_diff, 2);
    double term2 = pow(y_diff, 2);
    return double(std::sqrt(term1 + term2));
}


// Reorder points for minimal energy waste
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


void print_lines(std::vector<line> line_set)
{
    for (int i = 0; i < line_set.size(); i++)
        line_set[i].print_points();
}


std::vector<line> reorder_lines_into_chain(std::vector<line> poly)
{
    std::vector<line> reordered_lines;
    std::vector<int> final_idx_vec = get_index_of_final_line_and_starting_vertex(poly);
    reorder_points(poly);
    point p_compare;
    line final_line = poly[final_idx_vec[0]];
    p_compare = poly[final_idx_vec[0]].get_higher_point_x_wise();

    poly.erase(poly.begin()+final_idx_vec[0]);

    int j = 0;
    while (poly.size() > 0)
    {
        if (p_compare == poly[j].p1)
        {
            reordered_lines.push_back(poly[j]);
            p_compare = poly[j].p2;
            poly.erase(poly.begin()+j);
            j = 0;
        }
        else if (p_compare == poly[j].p2)
        {
            reordered_lines.push_back(poly[j]);
            p_compare = poly[j].p1;
            poly.erase(poly.begin()+j);
            j = 0;
        }
        else j++;

    }

    reordered_lines.push_back(final_line);

    return reordered_lines;
}


std::vector<line> reorder_cuts(std::vector<line> line_set, int final_line_idx)
{
    std::vector<line> reordered_lines;

    if (line_set.size() > 11)
    {
        std::cout << "Reordering sequentially.." << std::endl;
        reordered_lines = reorder_lines_into_chain(line_set);
    }
    else
    {
        std::cout << "Reordering cost-wise.." << std::endl;

        if (final_line_idx == -1)
        {
            std::vector<int> final_idx_vec = get_index_of_final_line_and_starting_vertex(line_set);
            final_line_idx = final_idx_vec[0];
            int final_line_starting_vertex_index = final_idx_vec[1];
        }
        
        std::cout << "Final line index: " << final_line_idx << std::endl;

        // print_lines(line_set);
        std::cout << std::endl;
        
        reorder_points(line_set);

        print_lines(line_set);
        std::cout << "---------------------" << std::endl;

        line final_line = line_set[final_line_idx];
        

        int a[line_set.size()-1] = {};
        int j;
        for (int i = 0; i < line_set.size(); i++)
        {
            j = i;
            if (i >= final_line_idx)
                j++;
            a[i] = j;
        }       

        int n_elements = sizeof(a) / sizeof(a[0]);

        unsigned long long n_perms = get_factorial(n_elements);

        clock_t begin = std::clock();
        int** perms = find_permutations(a, n_elements, n_perms, final_line_idx); 
        clock_t end = std::clock();
        std::cout << "Elapsed time for finding permutations: " << double(end-begin)/ CLOCKS_PER_SEC << std::endl;

        n_elements++;

        // print_perms(perms, n_perms, n_elements);

        print_lines(line_set);
        std::cout << std::endl;

        begin = std::clock();
        // get_cost_arr
        size_t min_idx = get_cost_arr_new(perms, n_perms, n_elements, line_set);
        end = std::clock();
        std::cout << "Elapsed time for calculating cost of all permutations and finding the minimum value index: " << double(end-begin)/ CLOCKS_PER_SEC << std::endl;

        for (int i = 0; i< n_elements; i++)
            reordered_lines.push_back(line_set[perms[min_idx][i]]);

        std::cout << "Best permutation: ";
        for (int i = 0; i < n_elements; i++)
            std::cout << perms[min_idx][i] << " ";
        // std::cout << final_line_idx << std::endl;
        std::cout << std::endl;

        for (size_t i = 0; i < n_perms; i++)
            free(perms[i]);
        free(perms);
    }


    return reordered_lines;

}

// Cost calculation of permutation
size_t get_cost_arr_new(int** perms, unsigned long long n_perms, int n_elements, std::vector<line>& line_set)
{
    /*
        Should also append all indices of permutations that are resulting 
        in minimum cost and pick the one that would be more optimal when
        trying to optimize for multiple polygons at the same time.
    */

    double* cost_arr = (double*)malloc(n_perms * sizeof(double));
    memset(cost_arr, 999999, sizeof(cost_arr));

    double min_cost = DBL_MAX;
    int final_line_idx = perms[0][n_elements-1];
    int min_cost_perm_idx;
    double cost;

    std::cout << "Final line index printed from get_cost function is: " << final_line_idx << std::endl;
    std::cout << "n_elements: " << n_elements << std::endl;


    // Get flat line indices
    std::vector<int> flat_line_indices;
    for (int i = 0; i < line_set.size(); i++)
        if (line_set[i].is_flat())
            flat_line_indices.push_back(i);
    
    // Calculate cost (if poly without flat lines case, else poly with flat lines)
    if (flat_line_indices.size() == 0)
    {
        for (unsigned long long i = 0; i < n_perms; i++)
        {
            cost = 0;
            for (int j = 0; j < n_elements-1; j++)
            {   
                cost += euc_dist(line_set[perms[i][j]], line_set[perms[i][j+1]]);
                if (cost > min_cost)
                    break;
            }
            
            if (cost < min_cost)
            {
                min_cost = cost;
                min_cost_perm_idx = i;
            }
        }
    }
    else 
    {
        
        int* truth_table_min_cost_idicies = (int*)malloc(n_perms * sizeof(int));

        // Create truth_table
        std::vector<std::vector<int>> truth_table = create_truth_table(flat_line_indices.size());

        std::vector<line> line_set_copy;
        
        for (unsigned long long i = 0; i < n_perms; i++)
        {
            for (int j = 0; j < truth_table.size(); j++)
            {
                line_set_copy = line_set;
                switch_flat_line_points(line_set_copy, flat_line_indices, truth_table[j]);

                cost = 0;
                for (int k = 0; k < n_elements-1; k++)
                {
                    cost += euc_dist(line_set_copy[perms[i][k]], line_set_copy[perms[i][k+1]]);
                    if (cost > min_cost)
                        break;
                }
                // need to consider/treat the equal case. Either here or on
                // the previous cost check
                if (cost < min_cost)
                {
                    min_cost = cost;
                    truth_table_min_cost_idicies[i] = j;
                    min_cost_perm_idx = i; // should we have a container for this as well?
                }
            }
        }

        switch_flat_line_points(line_set, flat_line_indices, truth_table[truth_table_min_cost_idicies[min_cost_perm_idx]]);
        
        free(truth_table_min_cost_idicies);

    }

    free(cost_arr);

    return min_cost_perm_idx;
}

// Helper function for cost calculations
std::vector<std::vector<int> > create_truth_table(const unsigned n)
{
    // Copied from StackOverflow
    std::vector<std::vector<int> > output(n, std::vector<int>(1 << n));

    unsigned num_to_fill = 1U << (n - 1);
    for (unsigned col = 0; col < n; ++col, num_to_fill >>= 1U)
    {
        for (unsigned row = num_to_fill; row < (1U << n); row += (num_to_fill * 2))
        {
            std::fill_n(&output[col][row], num_to_fill, 1);
        }
    }

    return output;
}



void switch_flat_line_points(std::vector<line>& line_set, std::vector<int> line_indicies, std::vector<int> truth_table_input)
{
    for (int i = 0; i < line_indicies.size(); i++)
        if (truth_table_input[i] == 1)
            line_set[line_indicies[i]].switch_points();
}

/////////////////////////////////////////////////////////////////////////////
////////////////////////////// Ordering Lines-related END ///////////////////
/////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////
/////////////////// Tool Diamater Compensation-related START ////////////////
/////////////////////////////////////////////////////////////////////////////

// Offset points by tool size
std::vector<std::vector<line>> offset_total_polys(std::vector<std::vector<std::vector<line>>> total_polys, int mode)
{
    std::vector<line> outer_poly = total_polys[0][0];
    std::vector<std::vector<line>> hole_polys = total_polys[1];
    int offset_val;


    switch(mode)
    {
        case(0): 
            offset_val = -3;
            std::cout << "Shrinking polygon by offset " << offset_val << std::endl;
            break;
        
        case(1):
            offset_val = 3;
            std::cout << "Expanding polygon by offset " << offset_val << std::endl;
            break;

        default:
            std::cout << "Invalid offset mode!" << std::endl;
            exit (EXIT_FAILURE);
    }

    ClipperLib::Path subj_outer;
    ClipperLib::Path subj_inner;
    ClipperLib::Paths subj_inner_all;
    ClipperLib::Paths original_outer;
    ClipperLib::Paths solution;

    // Convert enclosing polygon to ClipperLib format
    for (int i = 0; i < outer_poly.size(); i++)
    {
        subj_outer << ClipperLib::IntPoint(outer_poly[i].p1.x, outer_poly[i].p1.y);
        subj_outer << ClipperLib::IntPoint(outer_poly[i].p2.x, outer_poly[i].p2.y);
    }
    
    // Clean enclosing polygon
    ClipperLib::CleanPolygon(subj_outer);

    // Convert hole polygons to ClipperLib format
    for (int i = 0; i < hole_polys.size(); i++)
    {
        for (int j = 0; j < hole_polys[i].size(); j++)
        {
            subj_inner << ClipperLib::IntPoint(hole_polys[i][j].p1.x, hole_polys[i][j].p1.y);
            subj_inner << ClipperLib::IntPoint(hole_polys[i][j].p2.x, hole_polys[i][j].p2.y);
        }
        // Clean hole polygons
        ClipperLib::CleanPolygon(subj_inner);
        subj_inner_all << subj_inner;
        subj_inner.clear();
    }

    // Reverse orientation of holy polygons (opposite of enclosing polygon) in order to define them as holes
    ClipperLib::ReversePaths(subj_inner_all);

    // Solve
    double miter_limit = 2.0; // Minimum is 2.0. Used during expansion.
    ClipperLib::ClipperOffset co;
    co.MiterLimit = miter_limit;
    co.AddPath(subj_outer, ClipperLib::jtMiter, ClipperLib::etClosedPolygon);
    co.AddPaths(subj_inner_all, ClipperLib::jtMiter, ClipperLib::etClosedPolygon);
    co.Execute(solution, offset_val);

    std::cout << "Miter limit = " << miter_limit << std::endl;
    std::cout << "Solution size: " << solution.size() << std::endl;
    


    // Convert solution back to normal format
    std::vector<std::vector<line>> modified_poly_set;
    std::vector<line> temp_poly;
    line temp_line; 
    point p1;
    point p2;     
    int n; 

    for (int i = 0; i < solution.size(); i++)
    {
        temp_poly.clear();
        n = solution[i].size();

        for (int j = 0; j < solution[i].size(); j++)
        {
            p1.x = solution[i][j].X; p1.y = solution[i][j].Y;
            p2.x = solution[i][(j+1)%n].X; p2.y = solution[i][(j+1)%n].Y;
            temp_line.p1 = p1; temp_line.p2 = p2;
            temp_poly.push_back(temp_line);
        }

        modified_poly_set.push_back(temp_poly);
    }


    return modified_poly_set;


    // // "Drawing" the result
    // original_outer << subj_outer;

    // SVGBuilder svg;
    // svg.style.penWidth = 1.5;
    // svg.style.brushClr = 0x1200009C;
    // svg.style.penClr = 0xFF000000;
    // svg.style.pft = pftNonZero;
    // svg.AddPaths(original_outer);

    // svg.style.brushClr = 0x1200009C;
    // svg.style.penClr = 0xFF000000;
    // svg.style.pft = pftNonZero;
    // svg.AddPaths(subj_inner_all);
    // svg.SaveToFile("./clipping/solution_main_pre.svg");//, svg_scale);

    // SVGBuilder svg_2;
    // svg_2.style.brushClr = 0x3000FF00;
    // svg_2.style.penClr = 0xFF006600;
    // svg_2.style.pft = pftEvenOdd;
    // svg_2.AddPaths(solution);
    // svg_2.SaveToFile("./clipping/solution_main_post.svg");//, svg_scale);

    //finally, show the svg image in the default viewing application (not working)
    // system("./solution_main.svg"); 

}


/////////////////////////////////////////////////////////////////////////////
/////////////////// Tool Diamater Compensation-related END ////////////////
/////////////////////////////////////////////////////////////////////////////