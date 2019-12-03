#include "helper_functions.h"

namespace plt = matplotlibcpp;

/*
    Things to remember:
    1- Many horizontal lines case.
    2- Receiving a line with the same point as vertices.
    3- Should fix permutations to include per-line-permutation in order
    to handle the case where a line is horizontal and could be cut in either
    direction. That would make the cost calculation more inclusive and the
    solution more optimal, but it's way more costly.
    4- Change the permutations from an array to a vector.
    5- Memory leak check with valgrind for malloc usage.
*/




/////////////////////////////////////////////////////////////////////////////
////////////////////////////// Polygon-related START //////////////////////////
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
////////////////////////////// Polygon-related END //////////////////////////
/////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////
////////////////////////////// Final Line-related START /////////////////////
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
////////////////////////////// Final Line-related END ///////////////////////
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
////////////////////////////// Stacked Horizontal lines-related START /////////
/////////////////////////////////////////////////////////////////////////////

std::vector<std::vector<line>> reorder_board_wide_polys(std::map<int, custom_arr_type> board_wide_polygon_map, std::vector<std::vector<line>> poly_set_copy)
{
    std::vector<std::vector<line>> reordered_board_wide_polys;
    int y_val;
    int poly_idx;
    int line_idx;
    std::vector<line> polygon;
    line board_wide_line;
    for (std::map<int, custom_arr_type>::reverse_iterator rev_it = board_wide_polygon_map.rbegin(); rev_it != board_wide_polygon_map.rend(); ++rev_it)
    {
        poly_idx = (*rev_it).second[0]; line_idx = (*rev_it).second[1];
        polygon = poly_set_copy[poly_idx];
        if (polygon.size() > 1)
        {
            std::cout << "polygon.size() > 1" << std::endl;
            board_wide_line = polygon[line_idx];
            polygon.erase(polygon.begin()+line_idx);
            // You would need to add this line, as the final line in reordering,
            // so that it would be accounted for during permutations, in order
            // to get the best cost.
            polygon = reorder_cuts(polygon); 
            polygon.push_back(board_wide_line);
        }

        reordered_board_wide_polys.push_back(polygon);   
    }

    return reordered_board_wide_polys;
}

std::map<int, custom_arr_type> board_wide_poly(std::vector<std::vector<line>>& poly_set, const int board_width)
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
                }  
            }
    }
    // board-wide polys indicies in poly_set sorted in ascending order
    std::sort(polygons_board_wide_idx.begin(), polygons_board_wide_idx.end());
    for (std::vector<int>::iterator it=polygons_board_wide_idx.begin(); it != polygons_board_wide_idx.end(); it++)
        std::cout << *it << std::endl;

    // By here you have (sorted in ascending order)
    // the y-val, {poly_idx, line_idx} map of the board-wide polys

    // Now remove those board-wide polys from poly_set, which is passed by REFERENCE!
    int n = polygons_board_wide_idx.size();
    int compare_idx;
    std::vector<std::vector<line>> poly_set_copy = poly_set;
    std::cout << "poly_set_copy size is " << poly_set_copy.size() << std::endl;
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
        poly_set[i] = reorder_cuts(poly_set[i]);

    // Reorder board-wide polygons
    /* TODO: 1 - Normal case, where all lines are horizontal, only line per poly (done)
             2 - Polygon has multiple lines, and there's one which is board-wide
             3 - Multiple board-wide lines in the polygon
    */
   std::vector<std::vector<line>> reordered_board_wide_polys = reorder_board_wide_polys( board_wide_polygon_map, poly_set_copy);
   for (int i =0; i < reordered_board_wide_polys.size(); i++)
        poly_set.push_back(reordered_board_wide_polys[i]);

//    for (std::map<int, custom_arr_type>::reverse_iterator rev_it = board_wide_polygon_map.rbegin(); rev_it != board_wide_polygon_map.rend(); ++rev_it)
//         {
//             std::cout << "Index of poly during reappending (reverse iterator part): " << (*rev_it).second[0] << std::endl;
//             poly_set.push_back(poly_set_copy[(*rev_it).second[0]]);
//         }
    
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



void plot_poly_set(std::vector<std::vector<line>> poly_set, int xlims[2], int ylims[2])
{
    line line_for_plot;
    double x1, y1, x2, y2;
    std::vector<std::string> line_texture_vec = {"r-", "m-", "g-", "y-", "k-", "b-"};
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


double euc_dist(line l1, line l2)
{
    
    double x_diff = l1.p2.x - l2.p1.x;
    double y_diff = l1.p2.y - l2.p1.y;
    double term1 = pow(x_diff, 2);
    double term2 = pow(y_diff, 2);
    return double(std::sqrt(term1 + term2));
}

size_t get_cost_arr(int** perms, unsigned long long n_perms, int n_elements, std::vector<line> line_set)
{
    /*
        Should also append all indices of permutations that are resulting 
        in minimum cost and pick the one that would be more optimal when
        trying to optimize for multiple polygons at the same time.
    */

    double* cost_arr = (double*)malloc(n_perms * sizeof(double));

    double min_cost = DBL_MAX;
    size_t min_idx;
    int final_line_idx = perms[0][n_elements-1];
    std::cout << "Final line index printed from get_cost function is: " << final_line_idx << std::endl;
    std::cout << "n_elements: " << n_elements << std::endl;
    for (unsigned long long i = 0; i < n_perms; i++)
    {
        cost_arr[i] = 0;
        for (int j = 0; j < n_elements-1; j++)
        {   
            // std::cout << perms[i][j] << " to " << perms[i][j+1] << std::endl;
            // line_set[perms[i][j]].print_points();
            // line_set[perms[i][j+1]].print_points();
            // std::cout << euc_dist(line_set[perms[i][j]], line_set[perms[i][j+1]]) << std::endl;

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

    free(cost_arr);
    std::cout << "Cost array freed (supposedly)" << std::endl;

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


std::vector<line> reorder_cuts(std::vector<line> line_set)
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
        std::vector<int> final_idx_vec = get_index_of_final_line_and_starting_vertex(line_set);
        int final_line_idx = final_idx_vec[0];
        int final_line_starting_vertex_index = final_idx_vec[1];
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


size_t get_cost_arr_new(int** perms, unsigned long long n_perms, int n_elements, std::vector<line>& line_set)
{
    /*
        Should also append all indices of permutations that are resulting 
        in minimum cost and pick the one that would be more optimal when
        trying to optimize for multiple polygons at the same time.
    */

    double* cost_arr = (double*)malloc(n_perms * sizeof(double));
    memset(cost_arr, 999999, sizeof(cost_arr));

    int* truth_table_min_cost_idicies = (int*)malloc(n_perms * sizeof(int));

    double min_cost = DBL_MAX;
    size_t min_idx;
    int final_line_idx = perms[0][n_elements-1];
    std::cout << "Final line index printed from get_cost function is: " << final_line_idx << std::endl;
    std::cout << "n_elements: " << n_elements << std::endl;

    // Get flat line indices
    std::vector<int> flat_line_indices;
    for (int i = 0; i < line_set.size(); i++)
        if (line_set[i].is_flat())
            flat_line_indices.push_back(i);
    
    // Create truth_table
    std::vector<std::vector<int>> truth_table = create_truth_table(flat_line_indices.size());

    

    std::vector<line> line_set_copy;
    double cost;
    int min_cost_perm_idx;
    for (unsigned long long i = 0; i < n_perms; i++)
    {
        for (int j = 0; j < truth_table.size(); j++)
        {
            line_set_copy = line_set;
            switch_line_points(line_set_copy, flat_line_indices, truth_table[j]);

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

    switch_line_points(line_set, flat_line_indices, truth_table[truth_table_min_cost_idicies[min_cost_perm_idx]]);
    

    free(truth_table_min_cost_idicies);
    free(cost_arr);

    return min_cost_perm_idx;
}


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

    // // This loop just print out the results, nothing more.
    // for(unsigned x = 0; x < (1 << n); ++x)
    // {
    //     for(unsigned y = 0; y < n; ++y)
    //     {
    //         std::cout << output[y][x] << " ";
    //     }
    //     std::cout << std::endl;
    // }
}

void switch_line_points(std::vector<line>& line_set, std::vector<int> line_indicies, std::vector<int> truth_table_input)
{
    std::cout << line_set.size() << std::endl;
    std::cout << line_indicies.size() << std::endl;
    std::cout << truth_table_input.size() << std::endl;

    for (int i = 0; i < line_indicies.size(); i++)
    {
        if (truth_table_input[i] == 1)
        {
            line_set[line_indicies[i]].switch_points();
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
////////////////////////////// Ordering Lines-related END ///////////////////
/////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////
/////////////////// Tool Diamater Compensation-related START ////////////////
/////////////////////////////////////////////////////////////////////////////


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
    int** perms = find_permutations(a, n_elements, n_perms); 
    clock_t end = strrd::clock();
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
