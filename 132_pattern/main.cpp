#include<vector>
#include<stack>
#include<limits.h>

using namespace std;


/*
* for i<j<k, reuqire that num[i]<num[k]<num[j], and we will call num[i],num[k],num[j] i,j,k for convenience.
* We will find the i<k<j in the vector nums. So if we find the max k that less than j, it will increase the chance
* to find the i. 
* We need a stack that Monotonic decreasing, and scaning the nums from right to left. We find the max k in the stack 
* that less than the current number. If we find the k, that means there is a number to the right of the current number
* that bigger than k, so if current number less k, we find the i(current number)<k<j.
*/
class Solution {
public:
    bool find132pattern(vector<int>& nums) {
        if(nums.size()<3)return false;
        stack<int>stk;
        int k = INT_MIN; //k init to INT_MIN 

        for(int i=nums.size()-1;i>=0;--i)
        {
            if(nums[i]<k)return true;// if nums[i]<k, that means find the k, and there is a j>k
            while(!stk.empty() && nums[i]>stk.top())//find the max k that less than j
            {
                k = max(k,stk.top());
                stk.pop();
            }
            stk.push(nums[i]);
        }
        return false;
    }
};