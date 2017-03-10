//============================================================================
// Name        : Project2.cpp
// Author      : Trenton Fleming
// Version     :
// Copyright   : 
// Description : Super secret classified algorithm in C++, Ansi-style
//============================================================================

#include <iostream>
#include <iostream>
#include <vector>
#include <stdlib.h>


using namespace std;

int* find_occurrences(vector<string> text, string hint, int N);
void find_suffix(string hint, int hint_size, int *lps);
static int max(int a, int b);
vector<int> find_sequence(int* main, int* secondary, int N);

int main() {

	string hint;													//Fetch hint from input
	cin >> hint;

	int N;															//Fetch string size from input
	cin >> N;

	string input;

	vector<string> rows;											//Represents rows of the N*N matrix
	vector<string> columns;											//Represent

	int z = 0;

	while(z < N){													//Fetch rows of strings for size N O(N)

		cin >> input;
		rows.push_back(input);
		z++;

	}

	z=0;

	while(z < N){													//Generate columns of matrix O(N^2)

		string temp = "";

		for(int i = 0; i<N; i++){
			temp += rows.at(i)[z];
		}

		columns.push_back(temp);

		z++;
	}

	int* row_combo = find_occurrences(rows, hint, N);						//Find number of occurrences in each row	worst case O(N^2)
	int* column_combo = find_occurrences(columns, hint, N);					//Find number of occurrences in each column


	vector<int> solution_A = find_sequence(row_combo, column_combo, N);		//Find the longest common sub sequence of both lock "combinations"
	vector<int> solution_B = find_sequence(column_combo, row_combo, N);		//The other solution

	 for(unsigned int i = 0; i < solution_A.size(); i++)					//Print out the reverse longest common subsequence in reverse
			   cout << solution_A.at(solution_A.size() -1 - i) << " ";

	return 0;
}

int* find_occurrences(vector<string> text, string hint, int N){				//Find occurrences in each string. O(hint_size) for pre-processing and O(N) for KMP (overall O(N + hint_size).
																			//There are N strings, over all function runs in O(N(N+hint_size)). Overall O(N^2) at the worst case
	int* occurence_count = new int[N];

	int hint_size = hint.length();

	for(int x = 0; x < N; x++){							//Find # of occurrences in each row

		int count = 0;

		int suffix[hint_size];							//Generate the new comparison location in the hint after a failure for each particular character in the hint
		find_suffix(hint, hint_size, suffix);

		int i = 0;  									//Location of character in string
		int j  = 0;  									//Location of character in hint

		while (i < N){									//Find	# of occurrences of hint in each string

			if (hint[j] == text.at(x)[i]){				//If the characters match, check to see if the next characters match

				j++;
				i++;

			}

			if (j == hint_size){						//There is an full occurrence of the hint within the string

				count++;								//Increment count
				j = suffix[j-1];						//Adjust the hint index to only have to make one comparison on the next iteration

			}

			else if (i < N && hint[j] != text.at(x)[i]){	//Mismatch found, if i is outside of bounds, don't continue

				if (j != 0)									//Fetch the new number of comparisons needed
					j = suffix[j-1];

				else										//Have to compare the whole hint, just move on
					i = i+1;

			}
		}

			occurence_count[x] = count;						//Record count

		}

	return occurence_count;									//Return the number of occurrences in each array

}

void find_suffix(string hint, int hint_size, int *lps)				//Generate the number of shifts needed per failure of each character in hint string
{

    int length = 0;													//The number of characters that don't need to be re-compared

    lps[0] = 0; 													//Starting out, need to compare the whole hint

    int i = 1;

    while (i < hint_size){											//Pre-process the whole hint O(hint_size)

        if (hint[i] == hint[length]){									//If characters match, record that the whole hint does not have to be compared

            length++;
            lps[i] = length;
            i++;

        }
        else {

            if (length != 0){										//The character or the pattern is precedented

                length = lps[length-1];								//Fetch last longest suffix

            }
            else{													//The character or the pattern is unprecedented

                lps[i] = 0;
                i++;

            }
        }
    }
}

static int max(int a, int b) {									//Returns the maximum of two numbers
	return a >= b ? a : b;
}

vector<int> find_sequence(int* main, int* secondary, int N){	//Find the longest common subsequence, dynamic algorithm, O(N*N)

	vector<int> solution;										//Solutions

	   int L[N+1][N+1];											//Generate empty  N*N matrix
	   int i;													//Row tracker
	   int j;													//Column tracker

	   for (i=0; i<=N; i++)
	   {
	     for (j=0; j<=N; j++)
	     {
	       if (i == 0 || j == 0)								//The "border" of the matrix needs to be 0's
	         L[i][j] = 0;

	       else if (main[i-1] == secondary[j-1])				//If a character matches the other string, increment the length of the longest sub sequence
	         L[i][j] = L[i-1][j-1] + 1;

	       else													//Continue with the max of either the length before or above the current index
	         L[i][j] = max(L[i-1][j], L[i][j-1]);
	     }
	   }

	   i = N;													//Reset
	   j = N;

	      while (i > 0 && j > 0){								//back-track through the matrix from the last element in the matrix


	         if (main[i-1] == secondary[j-1]){					//If there is a match between the two sequences

	             solution.push_back(main[i-1]); 				//Put the current occurrence # in result

	             i--;											//Back track to the next element to the upper left of the matrix
	             j--;
	         }

	         else if (L[i-1][j] > L[i][j-1])					//otherwise if the next longest length is in the same column, change rows
	            i--;
	         else
	            j--;											//Otherwise otherwise, if the next longest length is in the same row, then change columns
	      }

	   return solution;											//The reverse order of the Longest common subsequence

}