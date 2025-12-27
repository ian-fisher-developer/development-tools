#pragma once

namespace stats
{

/**
 * Returns the statistics undefined-value marker.
 */
const float& undefined();

/**
 * Returns true if the value is the statistics undefined-value marker.
 */
bool undefined(const float& value);

} // namespace stats
