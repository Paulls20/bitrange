#include <bitset>

/**
 * @author paul.varghese
 * @params bits std::bitset 
 * @params R,L the range [R, L) == [R, L - 1]
 * @return the result at compile time
 */
template<std::size_t R, std::size_t L, std::size_t N>
constexpr void set_range(std::bitset<N>& bits)
{
    static_assert(R <= L && L <= N, "invalid range");
    std::size_t numOnes = L - R; 
    std::bitset<N> mask;
    mask.set();
    mask <<= numOnes; 
    ~mask;
    mask <<= R ;
    bits |= mask;
}

// test set_range
int main()
{
    std::bitset<4> bar;
    set_range<0,4>(bar);
    return bar.to_ulong();
}
