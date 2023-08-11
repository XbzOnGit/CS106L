#include <iostream>     // for cout, cin
#include <fstream>      // for ifstream
#include <sstream>      // for stringstream
#include <filesystem>   	// making inputting files easier
#include <stdexcept>
#include <unordered_set>
#include <vector>
#include <queue>
#include <unordered_map>
#include "wikiscraper.h"

using std::cout;            using std::endl;
using std::ifstream;        using std::stringstream;
using std::string;          using std::vector;
using std::priority_queue;  using std::unordered_map;
using std::unordered_set;   using std::cin;
int numCommonLinks(const unordered_set<string>& curr_set, const unordered_set<string>& target_set);
struct wikipath
{
    vector<string>path;
    int numcommon;
    wikipath(const vector<string>& init_path,const unordered_set<string>& curr_set ,const unordered_set<string>& target_set)
    {
        path = init_path;
        numcommon = numCommonLinks(curr_set,target_set);
    }
};


/*
 * This is the function you will be implementing parts of. It takes
 * two string representing the names of a start_page and
 * end_page and is supposed to return a ladder, represented
 * as a vector<string>, of links that can be followed from
 * start_page to get to the end_page.
 *
 * For the purposes of this algorithm, the "name" of a Wikipedia
 * page is what shows at the end of the URL when you visit that page
 * in your web browser. For ex. the name of the Stanford University
 * Wikipedia page is "Stanford_University" since the URL that shows
 * in your browser when you visit this page is:
 *
 *       https://en.wikipedia.org/wiki/Stanford_University
 */

// TODO: ASSIGNMENT 2 TASK 5:
// Please implement the following function, which should take in two sets of strings
// and returns the number of common strings between the two sets. You should use 
// lambdas and std::count_if.
// Estimated length: <4 lines

///////////////////////////////////////////////////////////////////////////////////////////////////
// BEGIN STUDENT CODE HERE
int numCommonLinks(const unordered_set<string>& curr_set, const unordered_set<string>& target_set) {
    return std::count_if(curr_set.begin(),curr_set.end(),[&](const string& a){ return (target_set.find(a) != target_set.end());});
}
// END STUDENT CODE HERE
///////////////////////////////////////////////////////////////////////////////////////////////////

vector<string> findWikiLadder(const string& start_page, const string& end_page) {
    if(start_page == end_page)
    {
        return {start_page};
    }
    WikiScraper w;

    unordered_set<string> target_set = w.getLinkSet(end_page);

    // TODO: ASSIGNMENT 2 TASK 6:
    // Please implement the comparator function that will be used in the priority queue.
    // You'll need to consider what variables this lambda will need to capture, as well as
    // what parameters it'll take in. Be sure to use the function you implemented in Task 1!
    // Estimated length: <3 lines
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // BEGIN STUDENT CODE HERE
    auto cmp_fn = [&w, &target_set](const wikipath& left, const wikipath& right) {
        return left.numcommon < right.numcommon;
    };
    // END STUDENT CODE HERE
    ///////////////////////////////////////////////////////////////////////////////////////////////////

    // TODO: ASSIGNMENT 2 TASK 7:
    // Last exercise! please instantiate the priority queue for this algorithm, called "queue". Be sure 
    // to use your work from Task 2, cmp_fn, to instantiate our queue. 
    // Estimated length: 1 line
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // BEGIN STUDENT CODE HERE
    // something like priority_queue<...> queue(...);
    // please delete ALL 4 of these lines! they are here just for the code to compile.
    std::priority_queue<wikipath,vector<wikipath>,decltype(cmp_fn)> queue(cmp_fn);

    // END STUDENT CODE HERE
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    auto first_extend = w.getLinkSet(start_page);
    if(first_extend.find(end_page) != first_extend.end())
    {
        return vector<string>{start_page,end_page};
    }
    wikipath first_one({start_page},first_extend,target_set);
    queue.push(first_one);
    unordered_set<string> visited;//Is in queue or has been out of queue.
    visited.insert(start_page);
    while(!queue.empty()) {
        wikipath curr_pack = queue.top();
        vector<string>& curr_path = curr_pack.path;
        queue.pop();
        string& curr = curr_path.back();
        //guarantee that 1. curr != end_page  
        //2. Cannot reach end_page directly from curr.  


        //cout << "curr_path:  " << endl;
        //for(const auto& tstr : curr_path)
        //{
        //    cout << tstr << endl;
        //}
        //cout << "get set: " << curr << endl;
        auto link_set = w.getLinkSet(curr);
        //cout << "NOW look for neighbor" << endl;
        for(const string& neighbour : link_set) {
            if(visited.find(neighbour) == visited.end()) {
                //cout << neighbour << endl;
                visited.insert(neighbour);
                vector<string> new_path = curr_path;
                new_path.push_back(neighbour);
                auto tempuset = w.getLinkSet(neighbour);
                //Early check.  
                if(tempuset.find(end_page)!=tempuset.end())
                {
                    new_path.push_back(end_page);
                    //cout << endl << "found: " << end_page << endl;
                    //cout << endl << "DONE!!" << endl;
                    return new_path;
                }
                wikipath nextpath(new_path,tempuset,target_set);
                queue.push(nextpath);
            }
        }
    }
    return {};
}

int main() {
    // a quick working directory fix to allow for easier filename inputs
    auto path = std::filesystem::current_path() / "res/";
    std::filesystem::current_path(path);
    std::string filenames = "Available input files: ";

    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        std::string filename = entry.path().string();
        filename = filename.substr(filename.rfind("/") + 1);
        filenames += filename + ", ";
    }
    // omit last ", ".
    cout << filenames.substr(0, filenames.size() - 2) << "." << endl;

    /* Container to store the found ladders in */
    vector<vector<string>> outputLadders;

    cout << "Enter a file name: ";
    string filename;
    getline(cin, filename);

    ifstream in(filename);
    int numPairs;
    // parse the first line as the number of tokens
    in >> numPairs;

    // loop through each line, parsing out page names and calling findWikiLadder
    string startPage, endPage;
    for (int i = 0; i < numPairs; i++) {
        // parse the start and end page from each line
        in >> startPage >> endPage;
        outputLadders.push_back(findWikiLadder(startPage, endPage));
    }

    /*
     * Print out all ladders in outputLadders.
     * We've already implemented this for you!
     */
    for (auto& ladder : outputLadders) {
        if(ladder.empty()) {
            cout << "No ladder found!" << endl;
        } else {
            cout << "Ladder found:" << endl;
            cout << "\t" << "{";

            std::copy(ladder.begin(), ladder.end() - 1,
                      std::ostream_iterator<string>(cout, ", "));
            /*
             * The above is an alternate way to print to cout using the
             * STL algorithms library and iterators. This is equivalent to:
             *    for (size_t i = 0; i < ladder.size() - 1; ++i) {
             *        cout << ladder[i] << ", ";
             *    }
             */
            cout << ladder.back() << "}" << endl;
        }
    }
    return 0;
}




