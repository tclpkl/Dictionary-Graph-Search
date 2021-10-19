#include "MinHeap.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

// Creating struct to hold node information
struct Node {
    // Holds node's word
    std::string word_string;
    // Holds word's index in words_list
    int index;
    // Holds vector of connectd nodes
    std::vector<Node> connected_nodes;
    // Holds g_value
    int g_value = 0;
    // Holds h_value which is initialized to -1
    int h_value = -1;
    // Function to find fvalue
    int get_fvalue() { return g_value + h_value; };
    // Function to find priority
    int get_priority() { return get_fvalue() * (int(word_string.size()) + 1) + h_value; };
    // Operator overloading to determine node a < node b by string and alphabet
    bool operator<(const Node& b) const { return (word_string < b.word_string); }
};

int main(int argc, char** argv) {

    // Storing command line arguments
    std::string start_word = argv[1];
    // Capitalizing start_word
    for (int i = 0; i < int(start_word.length()); i += 1) {
        if (int(start_word[i]) >= 97 && int(start_word[i]) <= 122) {
            start_word[i] -= 32;
        }
    }
    std::string end_word = argv[2];
    // Capitalizing end word
    for (int i = 0; i < int(end_word.length()); i += 1) {
        if (int(end_word[i]) >= 97 && int(end_word[i]) <= 122) {
            end_word[i] -= 32;
        }
    }

    // Opening file of words
    std::string words_file = argv[3];
    std::ifstream file(words_file);
    // Number of words in file
    int num_words;
    file >> num_words;

    // CREATING VECTOR OF NODES OF WORDS
    // Pushing all words into vector of nodes of the words
    std::vector<Node> words_list;
    int counter = 0;
    while (!file.eof()) {
        std::string word;
        file >> word;
        // Capitalizing word
        for (int i = 0; i < int(word.length()); i += 1) {
            if (int(word[i]) >= 97 && int(word[i]) <= 122) {
                word[i] -= 32;
            }
        }
        // Creating word node with its word, index, and initialize other data
        Node word_node;
        word_node.word_string = word;
        word_node.index = counter;
        // Calculating h_value
        int letters_different = 0;
        if (word.length() == end_word.length()) {
            for (int i = 0; i < int(word.length()); i += 1) {
                if (word[i] != end_word[i]) {
                    letters_different += 1;
                }
            }
        }
        word_node.h_value = letters_different;
        // Pushing this node into the vector
        words_list.push_back(word_node);
        counter += 1;
    }

    // CREATING MAP OF WORDS
    std::map<std::string, int> map_words_index;
    for (int i = 0; i < int(words_list.size()); i += 1) {
        map_words_index[words_list[i].word_string] = i;
    }

    // Creating edges and adding them to node info (creating graph)
    for (int i = 0; i < int(words_list.size()); i += 1) {
        std::string word_being_permuted = words_list[i].word_string;
        // Iterating through each character of node's word
        for (int j = 0; j < int(word_being_permuted.size()); j += 1) {
            // Storing character
            char letter = word_being_permuted[j];
            // Making alphabet
            std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
            // Changing character to all possible letters
            for (int k = 0; k < 27; k += 1) {
                word_being_permuted[j] = alphabet[k];
                // Checking if this one letter permutation exists in another node
                // by checking if it exists in the map of words to their index
                // Ensures word permutation is not original word and also same length
                if (map_words_index.find(word_being_permuted)->first == word_being_permuted
                    && word_being_permuted != words_list[i].word_string) {
                    // If exists, use index in map to add corresponding node into this node's connected nodes' vector
                    // data member
                    if (word_being_permuted.size() == words_list[i].word_string.size()) {
                        words_list[i].connected_nodes.push_back(
                                words_list[map_words_index.find(word_being_permuted)->second]);
                    }
                }
            }
            // Changing character back to original letter
            word_being_permuted[j] = letter;
        }
    }
    
    //------------------
    // IMPLEMENTING A STAR SEARCH
    MinHeap<Node> heap(2);
    int steps = 0;
    int expansions = 0;
    // Finding starting word and adding to heap
    Node starting_node = words_list[map_words_index.find(start_word)->second];
    heap.add(starting_node, starting_node.get_priority());
    // Set of explored words

    // While heap isn't empty
    Node current_node;
    while (!heap.isEmpty()) {
        Node current_node = heap.peek();
        // Checking if node is outdated
        if (current_node.g_value > words_list[current_node.index].g_value) {
            if (heap.isEmpty()) {
                break;
            }
            heap.remove();
            heap.peek();
        }
        // If last node is found, steps is its g_value
        if (current_node.word_string == end_word) {
            steps = current_node.g_value;
            break;
        }
        heap.remove();
        expansions += 1;
        // Checking all of connected nodes of current nodes
        for (int i = 0; i < int(current_node.connected_nodes.size()); i += 1) {
            // If node unvisited or if new priority would be less than previously calculated priority
            int new_priority = ((current_node.g_value + 1) + current_node.connected_nodes[i].h_value)
                                       * (current_node.word_string.size() + 1)
                               + current_node.connected_nodes[i].h_value;
            // Checking if starting node
            if (current_node.connected_nodes[i].word_string != start_word) {
                if (words_list[current_node.connected_nodes[i].index].g_value == 0
                    || new_priority < words_list[current_node.connected_nodes[i].index].get_priority()) {
                    // Updating connected node's g_value to current node's g_value + 1 in both heap and words_list
                    current_node.connected_nodes[i].g_value = current_node.g_value + 1;
                    words_list[current_node.connected_nodes[i].index].g_value = current_node.g_value + 1;
                    // Pushing node into heap
                    heap.add(
                            words_list[current_node.connected_nodes[i].index],
                            words_list[current_node.connected_nodes[i].index].get_priority());
                }
            }
        }
    }

    // If no answer
    if (steps == 0 && start_word != end_word) {
        std::cout << "No transformation" << std::endl << expansions << std::endl;
    } else {
        // Printing out result
        std::cout << steps << std::endl << expansions << std::endl;
    }
}