#include "scopeDepth.hpp"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

string printDepth(vector<int> &depthStack)
{
    string result = "";
    for (size_t i = 0; i < depthStack.size(); ++i)
    {
        result += to_string(depthStack[i]);
        if (i < depthStack.size() - 1)
        {
            result += ".";
        }
    }
    return result;
}

// if (ch == '{')
// {
//     if (depthStack.empty())
//     {
//         depthStack.push_back(1);
//     }
//     else
//     {
//         ++depthStack.back();
//     }
//     cout << printDepth(depthStack) << endl;
//     depthStack.push_back(0);
// }
// else if (ch == '}')
// {
//     if (!depthStack.empty())
//     {
//         depthStack.pop_back();
//     }
// }
