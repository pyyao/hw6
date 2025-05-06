#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // Add your code here
        const int GROUP_SIZE = 6; //  At most 6 characters in each group
        const int MAX_GROUPS = 5; // Max string length is 28, so at most 5 groups of 6 characters
        HASH_INDEX_T w[MAX_GROUPS] = {0};
        // Convert string to base-36 values using helper function
        std::vector<HASH_INDEX_T> base36_vals;
        for (std::size_t i = 0; i < k.size(); ++i)
        {
            base36_vals.push_back(letterDigitToNumber(k[i]));
        }
        // Process string in reverse, loop through string backwards
        int group_index = 4;
        int len = static_cast<int>(base36_vals.size());
        while (len > 0 && group_index >= 0)
        {
            HASH_INDEX_T val = 0;
            HASH_INDEX_T power = 1;
            int start = len - GROUP_SIZE;
            if (start < 0) {
                start = 0;
            }
            // Compute hash value for current window, at most 6 characters
            for (int j = len - 1; j >= start; --j)
            {
                // Calculate according to formula
                val += base36_vals[j] * power;
                power *= 36;
            }
            w[group_index] = val; 
            len = start; // Move window back to the next group
            group_index -= 1; // Decrement group index
        }
        // Final hash computation
        HASH_INDEX_T h = 0;
        for (int i = 0; i < MAX_GROUPS; ++i)
        {
            h += rValues[i] * w[i];
        }

        return h;

    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it
        if (letter >= '0' && letter <= '9') {
            return static_cast<HASH_INDEX_T>(letter - '0' + 26); // Convert 0-9 to integral value
        }
        else {
            if (letter >= 'A' && letter <= 'Z') {
                letter = static_cast<char>(letter + 32); // Convert upper case char to lower case
            }
            return static_cast<HASH_INDEX_T>(letter - 'a'); // Convert a-z to integral value
        }
    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
