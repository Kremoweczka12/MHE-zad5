#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <random>
#include "json.hpp"
#include <fstream>
#include <algorithm>
struct puzzle_t {
    int width;
    int height;
    int current_point;
    std::vector<int> board;
    std::vector<std::vector<int>> vertical_conditions;
    std::vector<std::vector<int>> horizontal_conditions;
    int &get(const int x, const int y){
        return board[y*width + x];
    }
    int get(const int x, const int y) const{
        return board.at(y*width + x);
    }
};


double evaluate(const puzzle_t & puzzle){
    return 0.0;
}

std::ostream &operator<<(std::ostream &o, const puzzle_t &puzzle) {
    for (int y = 0; y < puzzle.height; y++){

        o << "|";
        for (int x = 0; x < puzzle.width; x++){
            auto &val = puzzle.board[y*puzzle.width + x];
            o << "\t" << ((val>0)? "X":" ") << "|";
        }
        o << std::endl;
    }
    return o;
}

void print_nonogram(const puzzle_t table) {
    std::cout << "|";
    for(int i =0; i < table.width; i++){
        int left_spaces = 4 - table.horizontal_conditions[i].size();
        int right_spaces = 4 - table.horizontal_conditions[i].size();
        std::cout << std::string(left_spaces, ' ');
        for (int c =0; c<table.horizontal_conditions[i].size(); c++){
            std::cout <<table.horizontal_conditions[i][c] << ",";
        }
        std::cout << std::string(right_spaces, ' ') << "|";
    }
    std::cout << std::endl;
    for(int i = 0; i<table.height;i++){
        for(int c=0; c<table.vertical_conditions[i].size(); c++){
            std::cout <<table.vertical_conditions[i][c] << ", ";
        }
        for(int y=0; y<table.width;y++){
            auto &val = table.board[i*table.width + y];
            std::cout << "\t" << ((val>0)? "X":" ") << "|";
        }
        std::cout << std::endl;
        }


    }

int evaluate_puzzle(const puzzle_t puzzle){
    std::vector<std::vector<int>> errors = {};
    std::vector<int> vertical_errors = {};
    std::vector<int> horizontal_errors = {};
    for(int i = 0; i < puzzle.height; i++){

        std::vector<int> values = {};
        int value =0;
        for(int y = 0; y < puzzle.width; y++){

            int elem = puzzle.board[puzzle.width * i + y];
//            std::cout << value << std::endl;
            if(elem == 1){
                value += 1;
            }

            if(elem == 0 or y == puzzle.width - 1){

                if (value != 0){values.push_back(value); }
//                std::cout << value << std::endl;
                value = 0;
            }



        }
        if (values != puzzle.vertical_conditions[i]){
            vertical_errors.push_back(i);
        }
    }
    for(int i = 0; i < puzzle.width; i++){

        std::vector<int> values = {};
        int value =0;
        for(int y = i; y <= (puzzle.width*(puzzle.height- 1)) + i; y+=puzzle.width){
            if ( i == 1) {
                int z = 2;
            }
            int elem = puzzle.board[y];
//            std::cout << value << std::endl;
            if(elem == 1){
                value += 1;
            }

            if(elem == 0 or y == (puzzle.width*(puzzle.height - 1)) + i){

                if (value != 0){values.push_back(value); }
//                std::cout << value << std::endl;
                value = 0;
            }



        }

        if (values != puzzle.horizontal_conditions[i]){
            horizontal_errors.push_back(i);
        }
    }
    errors.push_back(vertical_errors);
    errors.push_back(horizontal_errors);
    int number_of_errors = errors.at(0).size() + errors.at(1).size();
    return number_of_errors;
}

bool get_table_comb(puzzle_t &puzzle) {
    int i = 0;
    for (; i < puzzle.board.size(); i++) {
        if (puzzle.board[i] == 0) {
            puzzle.board[i] = -1;
            break;
        } else if (puzzle.board[i] == -1) {
            puzzle.board[i] = 0;
        }
    }
    return (i != puzzle.board.size());
}

bool next_solution(puzzle_t &puzzle) {
    int i = 0;
    for (; i < puzzle.board.size(); i++) {
        if (puzzle.board[i] == 0) {
            puzzle.board[i] = 1;
            break;
        } else {
            puzzle.board[i] = 0;
        }
    }
    return (i != puzzle.board.size());
}

//int checkout_combinations(int h, int w, std::vector<int> solution){
//    for(int i = 0; i < h*w; i++){
//        std::vector<int> proposed_solution = get_table_comb(h, w);
//        if(solution == proposed_solution){
//            return 1;
//        }
//    }
//}

void full_view(puzzle_t tabela_2){
    int n = 0;
    while (next_solution(tabela_2)) {
        int errors = evaluate_puzzle(tabela_2);
        if ((n % 128) == 0) {
            std::cout << n << " : " << errors << std::endl << tabela_2 << std::endl;
        }
        if (errors == 0) {
            std::cout << tabela_2 << std::endl;
            break;
        }
        n++;
    }

}

puzzle_t get_random_tab(puzzle_t tabela){
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, 1); // define the range
    puzzle_t tab = tabela;
    tab.board = {};
    for(int i = 0; i < tab.height * tab.width;i++){
        int value = distr(gen);
        tab.board.push_back(value);
    }
    return tab;
}

puzzle_t stochastic_method(puzzle_t tabela, int iterations){
    puzzle_t temp_tab = get_random_tab(tabela);
    puzzle_t return_tab = temp_tab;
    int err = evaluate_puzzle(tabela);
    for(int i=0; i<iterations; i++){
        temp_tab = get_random_tab(tabela);
        int temp_err = evaluate_puzzle(temp_tab);
        if(temp_err < err){
            return_tab = temp_tab;
            err = temp_err;
        }
    }
    return return_tab;
}

std::vector<puzzle_t> get_neighbours(puzzle_t tabela){
    std::vector<puzzle_t> neighbours = {};
    std::vector<int> neighbours_ids = {};
    for(int i = 0; i < (tabela.width*tabela.height); i++){
//        if (i == tabela.current_point){
//            continue;
//        }
        puzzle_t temp_tab = tabela;
        temp_tab.board[i] = (temp_tab.board[i]+1)%2;
        temp_tab.current_point = i;
        neighbours.push_back(temp_tab);
    }

    return neighbours;
}

puzzle_t stochastic_hill_climbing(puzzle_t tabela, int iterations){
    puzzle_t result = tabela;
    int err = evaluate_puzzle(tabela);
    std::vector<puzzle_t> n;
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, (tabela.width*tabela.height)-1); // define the range
    puzzle_t great_tab = tabela;
    for(int i = 0; i < iterations; i++){
        n = get_neighbours(great_tab);
        puzzle_t temp_tab = n[distr(gen)];
        great_tab = temp_tab;
        int temp_err = evaluate_puzzle(temp_tab);
        if(temp_err<err){
            err = temp_err;
            result = temp_tab;
        }

    }
    return result;
}

void pop_front(std::vector<puzzle_t> &v)
{
    if (!v.empty()) {
        v.erase(v.begin());
    }
}


puzzle_t deterministic_hill_climbing(puzzle_t tabela){
    puzzle_t result = tabela;
    int err = evaluate_puzzle(tabela);
    std::vector<puzzle_t> n;
    puzzle_t great_tab = tabela;
    while (true){
        n = get_neighbours(great_tab);
        int pre_err = err;
        for(puzzle_t neighbour: n){
            int temp_err = evaluate_puzzle(neighbour);
            if(temp_err<err){
                err = temp_err;
                result = neighbour;
            }
        }
        great_tab = result;
        if(pre_err==err){
            return result;

        }
        pre_err = err;

    }
}

bool operator==(puzzle_t l, puzzle_t r) {
    if (l.width != r.width) return false;
    if (l.height!= r.height) return false;
    for (unsigned i = 0; i < r.board.size(); i++) {
        if (l.board.at(i) != r.board.at(i)) return false;
    }
    return true;
}

puzzle_t tabu_method(puzzle_t tabela, int iterations, int max_size){
    puzzle_t result = tabela;
    puzzle_t grand_result = tabela;
    int err = evaluate_puzzle(tabela);
    int grand_err = err;
    std::vector<puzzle_t> n;
    puzzle_t great_tab = tabela;
    std::vector<puzzle_t> tabu_tab = {};
//    tabu_tab.push_back(tabela);
    while (iterations > 0){
        n = get_neighbours(great_tab);
        int pre_err = err;

        bool c = false;
        int non_visited_n = 0;
        for(puzzle_t neighbour: n){
            if(std::find(tabu_tab.begin(), tabu_tab.end(), neighbour) == tabu_tab.end()) {
                non_visited_n += 1;
                if(!c){
                    c = true;
                    err = evaluate_puzzle(neighbour);
                    result = neighbour;
                    continue;
                }
                int temp_err = evaluate_puzzle(neighbour);
                if(temp_err<err){
                    err = temp_err;
                    result = neighbour;
                }


            }


        }
        std::cout << "non visited neighbours: " << non_visited_n << " per " << n.size() << " elements." << std::endl;
        if(std::find(tabu_tab.begin(), tabu_tab.end(), result) == tabu_tab.end()) {
            tabu_tab.push_back(result);}
        if (tabu_tab.size() == max_size){
            pop_front(tabu_tab);
            std::cout << " reduction! " << std::endl;
        }

        great_tab = result;
        if(non_visited_n==0){
            std::cout << "its stucked at: " << iterations << " iteration." << std::endl;
            return grand_result;

        }
        int temp_grand_err = evaluate_puzzle(result);
        if(temp_grand_err < grand_err){
            grand_result = result;
            grand_err = temp_grand_err;
        }
        pre_err = err;
        iterations -= 1;
        std::cout << iterations << std::endl;
    }
    return grand_result;
}


int main(int argc, char** argv) {
    std::string method = argv[1];
    int iter = std::stoi(argv[2]);
    std::string file_name = argv[3];
    int max_size = std::stoi(argv[4]);
    std::ifstream file_input(file_name, std::ifstream::binary);
    nlohmann::json data = nlohmann::json::parse(file_input);


    puzzle_t tabela;
    tabela.width = 10;
    tabela.height = 10;
    tabela.vertical_conditions = {
            {3, 5}, {1,5}, {1, 6}, {5}, {2,4,1}, {2, 1}, {3}, {5, 1}, {1}, {2, 1, 1}
    };

    tabela.horizontal_conditions = {
            {1, 4, 1}, {3, 4, 1}, {1, 3}, {1, 1}, {3, 1}, {5}, {5,1}, {4, 1, 1}, {5, 1}, {3}
    };
    tabela.board = {
            1,1,1,0,0,1,1,1,1,1,
             0,1,0,0,0,1,1,1,1,1,
             0,1,0,0,1,1,1,1,1,1,
             0,0,0,0,1,1,1,1,1,0,
             1,1,0,1,1,1,1,0,1,0,
             1,1,0,0,0,0,0,1,0,0,
             1,1,1,0,0,0,0,0,0,0,
             1,1,1,1,1,0,0,1,0,0,
             0,0,1,0,0,0,0,0,0,0,
             1,1,0,0,0,0,1,0,1,0
    };


//
//    print_nonogram(tabela);

//    int err = evaluate_puzzle(tabela);
//    std::cout << "error" << std::endl;
//    std::cout << err << std::endl;

    // zad 2

    puzzle_t tabela_2;
    tabela_2.width = 3;
    tabela_2.height = 3;
    tabela_2.vertical_conditions = {
            {3}, {}, {1}
    };

    tabela_2.horizontal_conditions = {
            {1}, {1 , 1}, {1}
    };
    tabela_2.board = {
            0,0,0,
            0, 0, 0,
            0, 0, 0,
    };

    tabela_2.current_point = 0;

    puzzle_t tabela_3;
    tabela_3.width = 5;
    tabela_3.height = 5;
    tabela_3.vertical_conditions = {
            {1,2}, {2}, {3}, {3}, {1,1}
    };

    tabela_3.horizontal_conditions = {
            {1}, {1}, {3}, {4}, {4}
    };
    tabela_3.board = {
            0,0,0,0,0,
            0, 0, 0,0,0,
            0, 0, 0, 0, 0,
            0,0,0,0,0,
            0,0,0,0,0,
    };
    tabela_3.horizontal_conditions = data["h"];
    tabela_3.vertical_conditions = data["v"];

//    puzzle_t tabela_4;
//    tabela_4.width = 3;
//    tabela_4.height = 3;
//
//    tabela_4.board = {
//            0,0,0,
//            0,0,0,
//            0,0,0
//    };
//    tabela_4.horizontal_conditions = data["h"];
//    tabela_4.vertical_conditions = data["v"];



    if(method=="d"){
        puzzle_t tab = get_random_tab(tabela_3);
        puzzle_t tab_3 = deterministic_hill_climbing(tab);
        std::cout << evaluate_puzzle(tab_3) << std::endl;
        std::cout << tab_3 << std::endl;
    }
    if(method=="sh"){
        puzzle_t tab = get_random_tab(tabela_3);
        puzzle_t tab_3 = stochastic_hill_climbing(tab, iter);
        std::cout << evaluate_puzzle(tab_3) << std::endl;
        std::cout << tab_3 << std::endl;
    }
    if(method=="p"){
        full_view(tabela_3);
    }
    if(method=="s"){
        puzzle_t tab = get_random_tab(tabela_3);
        puzzle_t tab_3 = stochastic_method(tab, iter);
        std::cout << evaluate_puzzle(tab_3) << std::endl;
        std::cout << tab_3 << std::endl;
    }
    if(method=="t"){
        puzzle_t tab = get_random_tab(tabela_3);
        puzzle_t tab_3 = tabu_method(tab, iter, max_size);
        std::cout << evaluate_puzzle(tab_3) << std::endl;
        std::cout << tab_3 << std::endl;
    }

//    std::cout << tab << std::endl;
//    puzzle_t tab_3 = deterministic_hill_climbing(tab);
//    std::cout << evaluate_puzzle(tab_3) << std::endl;
//    std::cout << tab_3 << std::endl;

//    puzzle_t tab_1 = stochastic_hill_climbing(tabela_2, 1000);
//    std::cout << tab_1 << std::endl;
//    puzzle_t tab_2 = stochastic_method(tabela_2, 1000);
//    std::cout << tab_2 << std::endl;
    return 0;
}
