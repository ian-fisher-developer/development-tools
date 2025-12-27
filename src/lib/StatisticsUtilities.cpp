#include "stats/StatisticsUtilities.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>

namespace // unnamed namspace
{

// Make a special value that shows as FEFEFEFE in a debugger hex dump.
// The value is chosen specifically NOT to fall in the NaN space, so equality
// comparisons work as expected.
float make_fefefefe()
{
    std::uint32_t bits = 0xFEFEFEFE;
    float fefefefe;
    memcpy(&fefefefe, &bits, sizeof(fefefefe));
    return fefefefe;
}
const float kUndefined(make_fefefefe());

} // unnamed namespace

namespace stats
{

const float& undefined()
{
    return kUndefined;
}

bool undefined(const float& value)
{
    return value == kUndefined;
}

} // namespace stats
