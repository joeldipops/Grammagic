#include "jobClass.h"
#include "../core.h"
using namespace Play;

//{ Lifecycle
/**
 * Constructor
 */
JobClass::JobClass(const Templates::JobTemplate& tmpl)
{
    _name = tmpl.Name;
}

/**
 * Destructor
 */
JobClass::~JobClass(void) {}

//}

//{ Properties
const std::string& JobClass::name(void) const { return _name; }
//}
